/*
 * Program:     Period98
 *
 * File:        xtgraph.cpp
 * Purpose:     implementation-file for
 *              ?
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */

#include "xpgraph.h"
#include "xgmenu.h"
#include "str.h"
#include "ldialtxt.h"
#include "xgmainfr.h"
#include "xtrename.h"
#include "stdio.h"
#include "fgeneral.h"

// include the icon for X
#ifdef wx_x
#if USE_XPM
#include "bitmaps/phaseplot.xpm"
#else
#include "bitmaps/phaseplot.xbm"
#endif
#endif

#define OBJECTSIZE 3
#define MAXDIST 100

p_Graph_Frame::p_Graph_Frame(CProject *data,int type,
			     wxFrame* parent, char*Name,
			     int x, int y,
			     int width, int height)
  :CMyGraph(data,type,parent,Name,x,y,width,height,
	    wxSDI|wxDEFAULT_FRAME,PER_PLOTNAME,0,1),
   Bins(10),Frequency(1.0),
   binsize(0),binampl(NULL),bincount(NULL),
   DrawError(0)
{
  mData=&data->GetTimeString();
  mPeri=&data->GetPeriod();
  // set the icon
#ifdef wx_msw
  SetIcon(new wxIcon("phaseplot"));
#endif
#ifdef wx_x
#if USE_XPM
  SetIcon(new wxIcon(phaseplot_xpm));
#else
  SetIcon(new wxIcon(phaseplot_bits,phaseplot_width,phaseplot_height));
#endif
#endif
  // Set MenuBar
  SetMenuBar(CreateMenuBar_p_Graph());
  // Set the Names used
  SetUseName(0);
  // Set the Data to display
  SetUseData(2);
  // Turn off binning
  SetUseBinning(0);
  FindFirstFrequency();
  FillStatusBarWithFrequency();
  // Create Canvas
  CreateCanvas();
  // Now set extrems
  CalcExtrems();
  // And the Visible
  SetVisible(ExtXMin,ExtXMax,ExtYMin,ExtYMax);
  // Show the Frame
  Show(TRUE);
}

p_Graph_Frame::~p_Graph_Frame()
{
  // remove Binning
  SetUseBinning(0);
}

int p_Graph_Frame::ReverseScaleY()
{ 
  return mData->GetReverseScale();
}


void p_Graph_Frame::CalcExtrems()
{
  int SelectedPoints=mData->GetSelectedPoints();
  if (SelectedPoints==0)
    {
      ExtXMax=1.0f;
      ExtYMax=1.0f;
      ExtXMin=0.0f;
      ExtYMin=0.0f;
      return;
    }

  // Get extrems in x
  ExtXMax=float((*mData)[SelectedPoints-1].GetTime());
  ExtXMin=float((*mData)[0].GetTime());

  // Get Extrems in Y
  
  // use Start-values
  ExtYMin=float(GetAmplitude(0));
  ExtYMax=ExtYMin;

  for (int i=1;i<SelectedPoints;i++)
    {
      double y=GetAmplitude(i);
      ExtYMin=float((ExtYMin<y)?ExtYMin:y);
      ExtYMax=float((ExtYMax>y)?ExtYMax:y);
    }

  ExtXMax=1.0f;
  ExtXMin=0.0f;
}

double p_Graph_Frame::GetAmplitude(int i)
{
  switch (UseData)
    {
    case 0: // Observed
      return (*mData)[i].GetObserved();break;
    case 1: // Adjusted
      return (*mData)[i].GetAdjusted();break;
    case 2: // Residuals to Observed
      return (*mData)[i].GetDataResidual();break;
    case 3: // Residuals to Observed
      return (*mData)[i].GetAdjustedResidual();break;
    }
  return 0;
}

void p_Graph_Frame::SetColor(int i, wxDC& DC)
{
  int id=(*mData)[i].GetIDName(ColorWhat);
  char *color=mData->GetIDName(ColorWhat,id).GetTrueColor();
  wxBrush *brush=wxTheBrushList->FindOrCreateBrush(color,wxSOLID);
  DC.SetBrush(brush);
}

myString p_Graph_Frame::GetDocTitle()
{
  char text[256];
  sprintf(text,FOUT_FRE"="FORMAT_FREQUENCY,Frequency);
  return text;
}

void p_Graph_Frame::DrawPlot(wxDC &DC,float scale)
{
  int i;
  int leftmostPoint=0;
  // Display Points
  {
    int first=0;
    int last=mData->GetSelectedPoints();
    // now loop
    for (i=first;i<last;i++)
      {
	// Get coordinates
	double t,a,e;
	t=GetTime(i);
	// only plot if data is within range
	// Time is higher?
	if (t>XMax)
	  {
	    break;
	  }
	// Time is lower?
	if (t>XMin)
	  {
	    a=GetAmplitude(i);
	    // Get additional graphics-objects
	    float w=OBJECTSIZE*scale;
	    if (DrawError)
              {
                e=GetError(i);
                wxPoint tmp1=TransformToScreen(t,a+e);
                wxPoint tmp2=TransformToScreen(t,a-e);
                DC.DrawLine(tmp1.x-w,tmp1.y,tmp1.x+w,tmp1.y);
                DC.DrawLine(tmp1.x,tmp1.y,tmp2.x,tmp2.y);
                DC.DrawLine(tmp2.x-w,tmp2.y,tmp2.x+w,tmp2.y);
              }
	    // plot circle
	    SetColor(i,DC);
	    // convert coordinates to screen
	    wxPoint tmp=TransformToScreen(t,a);
	    // plot Data
	    DC.DrawEllipse(tmp.x-w,tmp.y-w,2*w,2*w);
	  }
	else
	  {
	    leftmostPoint=i;
	  }
      }
  }

  // store old pen
  wxPen * old=DC.GetPen();
  // create a new Pen if needed and attatch it
  wxPen *newpen=
    wxThePenList->FindOrCreatePen(
				 "BLACK",
				 OBJECTSIZE,
				 wxSOLID);
  DC.SetPen(newpen);
  // Draw binned data using splines
  if  (UseBinning)
    {
      wxPoint * list=new wxPoint[binsize+2];
      int i;
      int pos=1;
      for (i=0;i<binsize;i++)
	{
	  if (bincount[i]!=0)
	    {
	      list[pos]=TransformToScreen(
					  binphase[i],
					  binampl[i]);
	      pos++;
	    }
	  wxPoint left=TransformToScreen(
					 binphase[i]-BinValue()/2,
					 binampl[i]);
	  wxPoint right=TransformToScreen(
					 binphase[i]+BinValue()/2,
					 binampl[i]+binampl2[i]);
	  DC.DrawLine(left.x,left.y,right.x,left.y);
	  float dist=right.y-left.y;
	  DC.DrawLine(list[pos-1].x,list[pos-1].y-dist,
		      list[pos-1].x,list[pos-1].y+dist);
	}
      list[0]=TransformToScreen(0,0);
      list[0].y=(list[1].y+list[pos-1].y)/2;
      list[pos]=TransformToScreen(1.0,0);
      list[pos].y=list[0].y;
      pos++;

      DC.DrawSpline(pos,list);
      delete [] list;
    }
  // restore old pen
  DC.SetPen(old);
}

void p_Graph_Frame::SetUseName(int id)
{
  // Check if id is allowed
  mData->CheckBounds(id);
  // Set the flag
  ColorWhat=id;
  // activate apropriate menu...
  wxMenuBar* tmp=GetMenuBar();
  // No menu set, so don't do it...
  if (tmp==NULL)
    { return; }
  // Delete all ticks in the menu...
  for (int i=0;i<4;i++)
    {
      tmp->Check(401+i,FALSE);
    }
  // Add the tick at the correct position
  tmp->Check(401+id,TRUE);
  // recalculate Binning and redraw 
  SetUseBinning(UseBinning);
}

void p_Graph_Frame::SetUseData(int id)
{
  // Set the flag
  UseData=id;
  // activate apropriate menu...
  wxMenuBar* tmp=GetMenuBar();
  // No menu set, so don't do it...
  if (tmp==NULL)
    { return; }
  // Delete all ticks in the menu...
  for (int i=0;i<4;i++)
    {
      tmp->Check(501+i,FALSE);
    }
  // Add the tick at the correct position
  tmp->Check(501+id,TRUE);
  // Calc extremes...
  CalcExtrems();
  // Set Values anew if necessary
  SetVisible(XMin,XMax,ExtYMin,ExtYMax);
  // recalculate Binning and redraw 
  SetUseBinning(UseBinning);
}

void p_Graph_Frame::FillStatusBarWithFrequency()
{
  // is not needed !!!
  // char text[256];
  // sprintf(text,FOUT_FRE"="FORMAT_FREQUENCY,Frequency);
  // SetTitle(text);
}

void p_Graph_Frame::FindFirstFrequency()
{
  int i;
  for (i=0;i<mPeri->GetFrequencies();i++)
    {
      if (!(*mPeri)[i].GetActive())
	if (!(*mPeri)[i].Empty())
	  {
	    Frequency=(*mPeri)[i].GetFrequency();
	    return;
	  }
    }

}

void p_Graph_Frame::ChangeFrequency()
{
  double value=Frequency;
  // Listbox
  
  // count number of unused Frequencies
  int i;
  int points=0;
  for (i=0;i<mPeri->GetFrequencies();i++)
    {
      if (!(*mPeri)[i].GetActive())
	if (!(*mPeri)[i].Empty())
	  { points++; }
    }
  // add an additional entry
  points++;
  // create array and fill it
  typedef char* charp;
  char **choices= new charp[points];
  int *FreqID=new int[points];
  int pos=0;
  for (i=0;i<mPeri->GetFrequencies();i++)
    {
      if (!(*mPeri)[i].GetActive())
	if (!(*mPeri)[i].Empty())
	  { 
	    FreqID[pos]=i;
	    choices[pos]=new char[256];
	    if ((*mPeri)[i].IsComposition())
	      {
		sprintf(choices[pos],
			"F%i: %s",
			(*mPeri)[i].GetNumber()+1,
			(*mPeri)[i].GetCompositeString()
			);
	      }
	    else
	      {
		sprintf(choices[pos],
			"F%i: "FORMAT_FREQUENCY,
			(*mPeri)[i].GetNumber()+1,
			(*mPeri)[i].GetFrequency()
			);
	      }
	    pos++; 
	  }
    }
  choices[pos]=PERG_OTHER_VALUE;
  FreqID[pos]=-5;
  int selection=::wxGetSingleChoiceIndex(PERG_FREQUENCY,
					 PERG_CHOOSE_FREQ,
					 points,choices,
					 this);
  // destroy allocated data
  for (i=0;i<pos;i++)
    {
      delete [] choices[i];
    }
  delete [] choices;

  if (selection==-1) { return; }

  if (FreqID[selection]==-5)
    {
      // Simple Question
      char text[256];
      sprintf(text,"%g",Frequency);
      char *result=::wxGetTextFromUser(PERG_FREQUENCY,
				       PERG_NEW_FREQ,
				       text,
				       this);
      if (result != NULL)
	{
	  sscanf(result,"%lf",&value);
	}
    }
  else
    {
      int freq=FreqID[selection];
      value=(*mPeri)[freq].GetFrequency();
    }

  if (value >0)
    {
      Frequency=value;
      FillStatusBarWithFrequency();
      SetUseBinning(UseBinning);
    }
}

void p_Graph_Frame::ChangeBinSpacing()
{
  char text[256];
  sprintf(text,"%i",Bins);
  char* result=::wxGetTextFromUser(PERG_BINNING,
				   PERG_BINS,
				   text,
				   this);
  if (result != NULL)
    {
      int value;
      sscanf(result,"%i",&value);
      if (value >0)
	{
	  Bins=value;
	  SetUseBinning(UseBinning);
	}
    }
}

void p_Graph_Frame::SetUseBinning(int id)
{
  UseBinning=id;
  if (binsize !=0 )
    {
      if (binampl  !=0 ) { delete [] binampl; }
      if (binampl2 !=0 ) { delete [] binampl2; }
      if (binphase !=0 ) { delete [] binphase; }
      if (bincount !=0 ) { delete [] bincount; }
      binsize=0;
      binampl =NULL;
      binampl2=NULL;
      bincount=NULL;
      binphase=NULL;
    }
  if (id)
    {
      // prepare data
      binsize=1+(int)(1.0/BinValue());
      binampl=new double[binsize];
      binampl2=new double[binsize];
      binphase=new double[binsize];
      bincount=new int[binsize];
      int i;
      // filling in data
      for (i=0;i<binsize;i++)
	{
	  binampl [i]=0;
	  binampl2[i]=0;
	  binphase[i]=i*BinValue()+(BinValue()/2);
	  bincount[i]=0;
	}
      // fill in data
      for (i=0;i<mData->GetSelectedPoints();i++)
	{
	  //get cordinates
	  double t,a;
	  t=GetTime(i);
	  a=GetAmplitude(i);
	  int bin=(int)(t/BinValue());
	  if (bin<binsize)
	    {
	      bincount[bin]++;
	      binampl[bin]+=a;
	      binampl2[bin]+=a*a;
	    }
	  else
	    {
	      MYERROR("Something wrong with binning!!!");
	    }
	}
      // normalize data
      for (i=0;i<binsize;i++)
	{
	  int n=bincount[i];
	  if (n!=0)
	    { 
	      binampl[i]=binampl[i]/n;
	      if (n!=1)
		{
		  binampl2[i]=sqrt(
				   (binampl2[i]-n*binampl[i]*binampl[i])/
				   ((n-1)*n)
				   );
		}
	      else 
		{
		  binampl2[i]=0.0;
		}
	    }
	}
    }
  // Update Menus
  (this->GetMenuBar())->Enable(M_FILE_SAVEPHABIN,id);
  // Refresh Displays
  if (myPlotArea)
    {
      myPlotArea->Refresh();
    }
}




