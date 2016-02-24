/*
 * Program:     Period98
 *
 * File:        xfgraph.cpp
 * Purpose:     implementation-file for
 *              ?
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */

#include "xfgraph.h"
#include "xgmenu.h"
#include "str.h"
#include "ldialtxt.h"

// include the icon for X
#ifdef wx_x
#if USE_XPM
#include "bitmaps/fouplot.xpm"
#else
#include "bitmaps/fouplot.xbm"
#endif
#endif

f_Graph_Frame::f_Graph_Frame(CProject *data,int type,
			     wxFrame* parent, char*Name,
			     int x, int y,
			     int width, int height)
  :CMyGraph(data,type,parent,Name,x,y,width,height,
	    wxSDI|wxDEFAULT_FRAME,FOUG_NAME,0,1)
{
   mData=data->GetFourierActive();
  // set the icon
#ifdef wx_msw
  SetIcon(new wxIcon("fouplot"));
#endif
#ifdef wx_x
#if USE_XPM
  SetIcon(new wxIcon(fouplot_xpm));
#else
  SetIcon(new wxIcon(fouplot_bits,fouplot_width,fouplot_height));
#endif
#endif
  // Set MenuBar
  SetMenuBar(CreateMenuBar_f_Graph());
  // Don't use Power
  SetUsePower(0);
  // rename frame
  char txt[256];
  sprintf(txt,FOUG_NAME": %s",mData->GetTitle().chars());
  SetTitle(txt);
  // Create Canvas
  CreateCanvas();
  // Now set extrems
  CalcExtrems();
  // And the Visible
  SetVisible(ExtXMin,ExtXMax,ExtYMin,ExtYMax);
  // Show the Frame
  Show(TRUE);
}

f_Graph_Frame::~f_Graph_Frame()
{}

void f_Graph_Frame::CalcExtrems()
{
  int SelectedPoints=mData->Points();
  // Get extrems in x
  ExtXMin=(float)mData->GetFrom();
  ExtXMax=(float)mData->GetTo();

  // Get Extrems in Y
  double f,a;
  
  // use Start-values
  ExtYMin=(float)0.0;
  ExtYMax=ExtYMin;

  for (int i=0;i<SelectedPoints;i++)
    {
      GetData(i,&f,&a);
      ExtYMin=float((ExtYMin<a)?ExtYMin:a);
      ExtYMax=float((ExtYMax>a)?ExtYMax:a);
    }
}

myString f_Graph_Frame::GetDocTitle()
{
  return mData->GetTitle();
}

void f_Graph_Frame::DrawPlot(wxDC &DC, float)
{
  // Set Start-values
  wxPoint lastAmpl =TransformToScreen(XMin,YMin);
  double lastf=XMin;
  // now plot all points
  for (int i=0;i<mData->Points();i++)
    {
      // Get the relevant data
      double f,a;
      GetData(i,&f,&a);
      // see if we are in range
      if (f>=XMin)
	{
	  wxPoint tmp=TransformToScreen(f,a);
	  DC.DrawLine(lastAmpl.x,lastAmpl.y,tmp.x,tmp.y);
	  lastf=f;
	  lastAmpl=tmp;
	  // check if we are allready outside !!!
	  if (f>XMax)
	    {
	      break;
	    }
	}
    }
}

void f_Graph_Frame::SetUsePower(int id)
{
  UsePower=id;
  CalcExtrems();
  SetVisible(XMin,XMax,ExtYMin,ExtYMax);
  GetMenuBar()->Check(M_DISPLAY_POWER,UsePower);
  // Refresh Displays
  if (myPlotArea)
    {
      myPlotArea->Refresh();
    }
}

void f_Graph_Frame::GetData(int i, double *f,double *a)
{
  *f=(*mData)[i].GetFrequency();
  *a=(*mData)[i].GetAmplitude();
  if (UsePower)
    {
      *a=(*a)*(*a);
    }
}



