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

#include "xtgraph.h"
#include "xgmenu.h"
#include "str.h"
#include "ldialtxt.h"
#include "xgmainfr.h"
#include "xtrename.h"

// include the icon for X
#ifdef wx_x
#if USE_XPM
#include "bitmaps/timeplot.xpm"
#else
#include "bitmaps/timeplot.xbm"
#endif
#endif

#define OBJECTSIZE 3
#define MAXDIST 100
#define PERCENTAGE 0.05f

#define OFFSET ((XMax-XMin)/100.0)

t_Graph_Frame::t_Graph_Frame(CProject *data,int type,
			     wxFrame* parent, char*Name,
			     int x, int y,
			     int width, int height)
  :CMyGraph(data, type,
	    parent,Name,x,y,width,height,
	    wxSDI|wxDEFAULT_FRAME,TIMEG_NAME,0),
		DrawError(0)
{
  mData=&(data->GetTimeString());
  mPeri=&(data->GetPeriod());
  // set the icon
#ifdef wx_msw
  SetIcon(new wxIcon("timeplot"));
#endif
#ifdef wx_x
#if USE_XPM
  SetIcon(new wxIcon(timeplot_xpm));
#else
  SetIcon(new wxIcon(timeplot_bits,timeplot_width,timeplot_height));
#endif
#endif
  // Set MenuBar
  SetMenuBar(CreateMenuBar_t_Graph());
  // Set the Names used
  SetUseName(0);
  // Set the Data to display
  SetUseData(0);
  // Create Canvas
  CreateCanvas();
  // Now set extrems
  CalcExtrems();
  // And the Visible
  SetVisible(ExtXMin,ExtXMax,ExtYMin,ExtYMax);
  // Show the Frame
  Show(TRUE);
}

t_Graph_Frame::~t_Graph_Frame()
{
}

void t_Graph_Frame::CalcExtrems()
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

  for (int i=0;i<SelectedPoints;i++)
    {
      double y=GetAmplitude(i);
      ExtYMin=float((ExtYMin<y)?ExtYMin:y);
      ExtYMax=float((ExtYMax>y)?ExtYMax:y);
    }
  if (ExtXMin==ExtXMax)
    {
      ExtXMin-=0.5f;
      ExtXMax+=0.5f;
    }
  if (ExtYMin==ExtYMax)
    {
      ExtYMin-=0.5f;
      ExtYMax+=0.5f;
    }
  else
    {
      float range=(ExtYMax-ExtYMin)*PERCENTAGE;
      ExtYMax+=range;
      ExtYMin-=range;
    }
}

double t_Graph_Frame::GetTime(int i)
{ 
  return (*mData)[i].GetTime(); 
}

int t_Graph_Frame::ReverseScaleY()
{ 
  return mData->GetReverseScale();
}

double t_Graph_Frame::GetAmplitude(int i)
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

void t_Graph_Frame::SetColor(int i, wxDC& DC)
{
  int id=(*mData)[i].GetIDName(ColorWhat);
  char *color=mData->GetIDName(ColorWhat,id).GetTrueColor();
  wxBrush *brush=wxTheBrushList->FindOrCreateBrush(color,wxSOLID);
  DC.SetBrush(brush);
}

myString t_Graph_Frame::GetDocTitle()
{
  return TIMEG_NAME;
}

void t_Graph_Frame::DrawPlot(wxDC &DC,float scale)
{
  if (mData->GetSelectedPoints()==0)
    {
      NoData(DC);return;
    }
  int i;
  int plotted=0;
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
	if (t>XMax+OFFSET)
	  {
	    break;
	  }
	// Time is lower?
	if (t>=XMin-OFFSET)
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
            // add plotted;
            plotted++;
	  }
      }
  }
  // find out if predicted should be plotted:
  // Get coordinates
  wxPoint xmin=TransformToScreen(XMin,YMin);
  wxPoint xmax=TransformToScreen(XMax,YMin);
  double sampling=(XMax-XMin)/(xmax.x-xmin.x);
  // find out maximum frequency
  double maxfreq=0.0;
  for (i=0;i<mPeri->GetFrequencies();i++)
    {
      if ( (*mPeri)[i].GetActive() )
	{
	  double tmp=(*mPeri)[i].GetFrequency();
	  if (tmp>maxfreq)
	    { maxfreq=tmp; }
	}
    }
  if (maxfreq!=0.0) { maxfreq=1/maxfreq; }
  // now plot predicted
  if ((UseData<2) && (8*sampling<maxfreq))
    {
      // find out about first and last
      int first=(int)floor(xmin.x);
      int last=(int)ceil(xmax.x);
      // now loop
      wxPoint lastpnt, currentpnt;
      lastpnt=TransformToScreen(XMin,YMin);
      for (i=first;i<last;i++)
	{
	  // convert these coordinates into usable ones
	  wxPoint mytmp=TransformFromScreen(float(i),xmin.y);
	  // this is the time
	  double t=mytmp.x;
	  // Calculate Amplitude
	  double a=mPeri->Predict(t,-1);
	  // Recalculate Screen-position
	  currentpnt=TransformToScreen(t,a);
	  // draw a line
	  DC.DrawLine(lastpnt.x,lastpnt.y,currentpnt.x,currentpnt.y);
	  // make current last
	  lastpnt=currentpnt;
	}
    }
}


void t_Graph_Frame::SetUseName(int id)
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
  // Refresh Displays
  if (myPlotArea)
    {
      myPlotArea->Refresh();
    }
}

void t_Graph_Frame::SetUseData(int id)
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
  // Refresh Displays
  // Refresh Displays
  if (myPlotArea)
    {
      myPlotArea->Refresh();
    }
}

void t_Graph_Frame::RightButtonPressed(wxMouseEvent &event)
{
  // transform to PLOT-coordinates
  wxPoint OnScreen(event.x,event.y);
  // find all possible points within range
  int PointFound=-1;
  // loop over the number of points
  int last=mData->GetSelectedPoints();
  // now loop
  int i;
  for (i=0;i<last;i++)
    {
      // Get coordinates
      double t,a;
      t=GetTime(i);
      // only plot if data is within range
      // Time is higher?
      if (t>XMax+OFFSET)
	{
	  break;
	}
      // Time is on screen
      if (t>XMin-OFFSET)
	{
	  // so get Amplitude
	  a=GetAmplitude(i);
	  // transform to screen-coordinates
	  wxPoint screenpnt=TransformToScreen(t,a);
	  // and calculate the difference
	  screenpnt.x=(screenpnt.x-OnScreen.x);
	  screenpnt.y=(screenpnt.y-OnScreen.y);
	  // and now the distance
	  float dist=
	    +screenpnt.x*screenpnt.x
	    +screenpnt.y*screenpnt.y;
	  // now check if in range
	  if (dist<MAXDIST)
	    {
	      // is there another Point in range?
	      if (PointFound!=-1)
		{
		  // yes, let's exit without changes...
		  return;
		}
	      // this is the Point found
	      PointFound=i;
	    }
	}
    }
  // find out if a Point was in range
  if (PointFound==-1)
    {
      // no, let's exit without changes...
      return;
    }
  // now relabel Point
  if (event.ControlDown())
    {
      // Relabel as deleted...
      DialogDeletePoint(PointFound);
      // ring the bell
      ::wxBell();
      wxYield();
    }
  else
    {
      // Interact with user
      DialogRelabelPoint(PointFound); 
    }
}







