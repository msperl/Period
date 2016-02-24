/*
 * Program:     Period98
 *
 * File:        xggraph.cpp
 * Purpose:     implementation-file for
 *              ?
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */

#include <math.h>
#include <wx.h>
#include <wx_print.h>

#include "xggraph.h"
#include "xgmenu.h"

#include "xgeneral.h"
#include "xgprojec.h"

#include "xgsize.h"
#include <string.h>
#include <math.h>

#define XDISTANCE (XLabelWidth/2)
#define YDISTANCE (LabelHeight/2)

#define STATUSBAR_COUNT 1

CMyGraph::CMyGraph(CProject *project,int type,
		   wxFrame *parent, char *title,
		   int x, int y, int width, int height,
		   long style, char *name,int flag,int usetitle)
  :CMyFrame(project,type,parent,title,x,y,width,height,style,name),
   myPlotArea(0),
   DisplayMinX(0.0f),DisplayMinY(0.0f),Ticksize(6),downcoords(-999.0f,-999.0f)
{
  // sets the Statusline
  CreateStatusLine(STATUSBAR_COUNT);
  // Set Extrems
  CalcExtrems();
  // now set the visible range
  SetVisible(ExtXMin,ExtXMax,ExtYMin,ExtYMax);
  // create canvas and show frame 
  // if we are are called without the standalone-flag
  // this is only to keep from redrawing many times
  if (flag)
    {
      // create the canvas
      CreateCanvas();
      // and show the frame
      Show(TRUE);
    }
  UseTitle=usetitle;
}

void CMyGraph::CreateCanvas()
{
  // creates the Child-frame
  int w,h;
  GetClientSize(&w,&h);
  myPlotArea= new CMyPlotArea(this,0,0,w,h);
}

CMyGraph::~CMyGraph()
{
}

void CMyGraph::UpdateDisplay(int )
{
  // redraw plot
  myPlotArea->OnPaint();
}

void CMyGraph::UpdateData()
{
  // nothing to do for graphs
}


Bool CMyGraph::OnClose()
{
  return TRUE;
}

void CMyGraph::CalcExtrems()
{
  ExtXMin=(float)0.0;
  ExtXMax=(float)1.0;
  ExtYMin=(float)0.0;
  ExtYMax=(float)1.0;
}

void CMyGraph::SetVisible(double xmi,double xma,double ymi,double yma)
{
  XMin=(float)((xmi<ExtXMin)?ExtXMin:xmi);
  XMax=(float)((xma>ExtXMax)?ExtXMax:xma);
  YMin=(float)((ymi<ExtYMin)?ExtYMin:ymi);
  YMax=(float)((yma>ExtYMax)?ExtYMax:yma);
}

myString CMyGraph::GetDocTitle()
{
  return MYGRAPH_TITLE;
}

void CMyGraph::NoData(wxDC & DC)
{
  char *text=NO_POINTS;
  float w,h;
  DC.GetTextExtent(text,&w,&h);
  wxPoint min=TransformToScreen(ExtXMin,ExtYMin);
  wxPoint max=TransformToScreen(ExtXMin,ExtYMin);
  wxPoint pnt;
  pnt.x=(max.x-min.x)/2-w;
  pnt.y=(max.y-min.y)/2-h;

  DC.DrawText(text,pnt.x,pnt.y);
}

class CMyGraphPrintout : public wxPrintout
{
public:
  CMyGraphPrintout(char *title,CMyGraph* plot)
    :wxPrintout(title),myPlotData(plot)
  {}

  void GetPageInfo(int *min,int *max, int *from, int *to)
  {
    *min=1;*max=1;*from=1;*to=1;
  }

  Bool OnPrintPage(int)
  {
    wxDC *dc=GetDC();
    // prepare printout
    // Get the block to be repainted
    int x,y,w,h;
    x=0;
    y=0;
    GetPageSizePixels(&w,&h);
    // get the scaling
    float scale;
    int pw,ph,sw,sh;
    GetPPIPrinter(&pw,&ph);
    GetPPIScreen(&sw,&sh);
#ifdef wx_xt    
    scale=1;
#else
    scale=(pw/sw+ph/sh)/2.0f;
#endif
    // now for the orientation
    x=x+GRAPHBORDERS;
    y=y+GRAPHBORDERS;
    w=w-x-GRAPHBORDERS;
    h=h-y-GRAPHBORDERS;
    // now plot the stuff
    myPlotData->CMyGraph::PaintPlot(*dc,x,y,w,h,scale);
  
    return TRUE;
  }
  
private:
  CMyGraph *myPlotData;
};

void CMyGraph::OnPrint()
{
  wxPrinter printer;
  CMyGraphPrintout printout("MyGraphPrintout",this);
  printer.Print(this,&printout,TRUE);
}

int CMyGraph::ReverseScaleX()
{ return 0;}

int CMyGraph::ReverseScaleY()
{ return 0;}

void CMyGraph::PaintPlot(wxDC & DC, int x0,int y0,int w0, int h0,float scale )
{
  // Set Clipping
  DC.SetClippingRegion((float)x0,(float)y0,(float)w0,(float)h0);
  // Create the font to use
  wxFont *myFont=wxTheFontList->FindOrCreateFont(int(14*scale),wxMODERN,
						 wxNORMAL,
						 wxNORMAL,
						 FALSE);
  // Set Font for DC (Device Context)
  DC.SetFont(myFont);
  // GetLabelsize
  CalculateLabelSize(DC);
  // Set the Origin of top Left
  int x,y,w,h;
  x=x0+YLabelWidth;
  y=y0+YDISTANCE;
  // remove borders for Labels from width and height
  h=h0-LabelHeight-Ticksize-YDISTANCE;
  w=w0-YLabelWidth-XDISTANCE;
  //
  if (UseTitle)
    {
      h=h-LabelHeight;
      y=y+LabelHeight;
    }
  // Clean Area
  DC.SetPen(wxWHITE_PEN);
  DC.SetBrush(wxWHITE_BRUSH);
  DC.DrawRectangle((float)x0,(float)y0,(float)w0,(float)h0);
  DC.SetPen(wxBLACK_PEN);
  // Calculate Scales
  CalculateScales(x,y,w,h);
  // Draw-X-Coordinates
  DrawXCoordinates(DC,int(Ticksize*scale),TRUE,scale);
  // Draw-Y-Coordinates
  DrawYCoordinates(DC,int(Ticksize*scale),TRUE,scale);
  // Draw Title
  if (UseTitle)
    {
      myString Title=GetDocTitle();
      float tw,th;
      DC.GetTextExtent(Title.chars(),&tw,&th);
      DC.DrawText(Title.chars(),x+(w-tw)/2.0f,y-(float)LabelHeight);
    }
  // now Draw Plot...
  DrawPlot(DC,scale);
  // remove Clipping
  DC.DestroyClippingRegion();
}

void CMyGraph::CalculateLabelSize(wxDC &DC)
{
  // get X-Scales
  {
    double xmin,step;
    float xsize,ysize;
    // use find out about last label
    CalcScaleDivisions(300,10,XMin,XMax,&xmin,&step);
    // Calculate Size
    char tmp1[32],tmp2[32];
    sprintf(tmp1,"  %g",xmin+step);
    sprintf(tmp2,"  %g",xmin+10*step);
    // Now get the size of the text
    if (strlen(tmp1)<strlen(tmp2))
      {
	DC.GetTextExtent(tmp2,&xsize,&ysize);
      }
    else
      {
	DC.GetTextExtent(tmp1,&xsize,&ysize);
      }
    // Assign values...
    XLabelWidth=(int)xsize;
  }
  // get Y-Scales
  {
    double xmin,step;
    float xsize,ysize;
    // use find out about last label
    CalcScaleDivisions(300,10,YMin,YMax,&xmin,&step);
    // Calculate Size
    char tmp1[32],tmp2[32];
    sprintf(tmp1,"  %g",xmin+step);
    sprintf(tmp2,"  %g",xmin+10*step);
    // Now get the size of the text
    if (strlen(tmp1)<strlen(tmp2))
      {
	DC.GetTextExtent(tmp2,&xsize,&ysize);
      }
    else
      {
	DC.GetTextExtent(tmp1,&xsize,&ysize);
      }
    // Assign values...
    YLabelWidth=(int)xsize;
    LabelHeight=(int)ysize;
  }
}

void CMyGraph::DrawPlot(wxDC &DC,float)
{
  // Get Coordinates
  wxPoint TopLeft=     TransformToScreen(XMin,YMax);
  wxPoint TopRight=    TransformToScreen(XMax,YMax);
  wxPoint BottomLeft=  TransformToScreen(XMin,YMin);
  wxPoint BottomRight= TransformToScreen(XMax,YMin);
  // Draw a cross
  DC.DrawLine(TopLeft.x,TopLeft.y,BottomRight.x,BottomRight.y);
  DC.DrawLine(TopRight.x,TopRight.y,BottomLeft.x,BottomLeft.y);
}

wxPoint CMyGraph::TransformToScreen(double x, double y)
{
  wxPoint tmp;
  double dx=x-XMin;
  if (ReverseScaleX()) {dx=XMax-x;}
  double dy=y-YMin;
  if (ReverseScaleY()) {dy=YMax-y;}

  tmp.x=(float)(DisplayMinX+DisplayScaleX*dx);
  tmp.y=(float)(DisplayMinY+DisplayScaleY*dy);
  return tmp;
}

wxPoint CMyGraph::TransformFromScreen(float x, float y)
  // reverse does not work
{
  wxPoint tmp;
  tmp.x=XMin+(x-DisplayMinX)/DisplayScaleX;
  tmp.y=YMin+(y-DisplayMinY)/DisplayScaleY;
  // switch if necessary
  if (ReverseScaleX()) 
    { tmp.x=XMax-(tmp.x-XMin);}
  if (ReverseScaleY()) 
    { tmp.y=YMax-(tmp.y-YMin);}
  // check for other stuff
  if (tmp.x<XMin) { tmp.x=XMin; }
  if (tmp.x>XMax) { tmp.x=XMax; }
  if (tmp.y<YMin) { tmp.y=YMin; }
  if (tmp.y>YMax) { tmp.y=YMax; }
  
  return tmp;
}

void CMyGraph::SetCoordinates(float x, float y)
{ 
  // Get true Values
  wxPoint tmp=TransformFromScreen(x,y);
  // create a string
  char tmp1[256];
  sprintf(tmp1,"(x,y)= (%g,%g)",tmp.x,tmp.y);
  // Write this string
  SetStatusText(tmp1,STATUSBAR_COUNT-1);
}

void CMyGraph::DrawBox(float x, float y)
{
  // Getting Device-Context
  wxDC *dc=myPlotArea->GetDC();
  dc->SetPen(wxBLACK_PEN);
  dc->SetBrush(wxTRANSPARENT_BRUSH);
  dc->SetLogicalFunction(wxXOR);
  // Calculate points
  dc->DrawLine(x,y,downcoords.x,y);
  dc->DrawLine(x,y,x,downcoords.y);
  dc->DrawLine(downcoords.x,downcoords.y,x,downcoords.y);
  dc->DrawLine(downcoords.x,downcoords.y,downcoords.x,y);
  // Reset all
  dc->SetLogicalFunction(wxCOPY);
  // set this point as last
  lastcoords=wxPoint(x,y);
}

void CMyGraph::RemoveBox()
{
  DrawBox(lastcoords.x,lastcoords.y);
}

void CMyGraph::ButtonDown(float x, float y)
{
  downcoords=wxPoint(x,y);
  lastcoords=downcoords;
  // Now draw the Box
  DrawBox(x,y);
}

void CMyGraph::ButtonUp(float x, float y)
{
  // Remove the Box
  RemoveBox();
  // check if the size is not to small
  float dx,dy;
  dx=x-downcoords.x;
  dy=y-downcoords.y;
  // mirror if necessary
  if (dx<0) { dx=-dx; }
  if (dy<0) { dy=-dy; }
  
  if ((dx<2)||(dy<2)||(dx*dx+dy*dy<50))
    {
      // to small, so let's return
      return;
    }
  //Calculate true position
  wxPoint tmplast=TransformFromScreen(x,y);
  wxPoint tmpfirst=TransformFromScreen(downcoords.x,downcoords.y);
  // calculate x
  XMin=tmplast.x;
  XMax=tmpfirst.x;
  if (XMin>XMax)
    {
      float tmp=XMax;
      XMax=XMin;
      XMin=tmp;
    }
  //calculate y
  YMin=tmpfirst.y;
  YMax=tmplast.y;
  if (YMin>YMax)
    {
      float tmp=YMax;
      YMax=YMin;
      YMin=tmp;
    }
  // Plot page anew
  myPlotArea->Refresh();
}

void CMyGraph::CalculateScales(int x, int y, int w, int h)
{
  // Calculate Scale for x
  DisplayScaleX=w/(XMax-XMin);
  DisplayMinX=(float)x;
  // Calculate Scale for y
  DisplayScaleY=-h/(YMax-YMin);
  DisplayMinY=(float)(y+h);
  // Store Size
  DisplayWidth=w;
  DisplayHeight=h;
}

void CMyGraph::DrawXCoordinates(wxDC &DC,
				int ticksize,int Label,float)
{
  // Draw Axis
  {
    wxPoint start,end,max;
    start=TransformToScreen(XMin,YMin);
    end=  TransformToScreen(XMax,YMin);
    max=  TransformToScreen(XMin,YMax);
    // draw bottom line
    DC.DrawLine(start.x,start.y,end.x,end.y);
    // draw top-line
    DC.DrawLine(start.x,max.y,end.x,max.y);
  }
  // Calculate first and ticks
  double start,step;
  CalcScaleDivisions(DisplayWidth,XLabelWidth,XMin,XMax,&start,&step);
  // Now plot ticks and Labels
  while (start<XMax)
    {
      wxPoint tmp;
      // draw top-ticks
      {
	// Calculate coordinates
	tmp=TransformToScreen(start,YMax);
	// create major tick
	DC.DrawLine(tmp.x,tmp.y+ticksize,tmp.x,tmp.y);
      }
      // draw bottom-ticks
      {
	// Calculate coordinates
	tmp=TransformToScreen(start,YMin);
	// create major tick
	DC.DrawLine(tmp.x,tmp.y,tmp.x,tmp.y-ticksize);
      }
      // shall we also write out the labels?
      if (Label)
	{
	  if (ReverseScaleY())
	    { 
	      tmp=TransformToScreen(start,YMax);
	    }
	  // see if value is close to 0
	  if (
	      ((start<0)?-start:start)
	      *10<step)
	    {
	      start=0;
	    }
	  // make String
	  char string[32];
	  sprintf(string,"%g",start);
	  // get Size of the String
	  float w,h;
	  DC.GetTextExtent(string,&w,&h);
	  // plot string
	  DC.DrawText(string,tmp.x-(w/2),tmp.y+ticksize);
	}
      // go to next label
      start+=step;
    }
}

void CMyGraph::DrawYCoordinates(wxDC &DC,
				int ticksize ,int Label,float)
{
  // Draw Axis
  {
    wxPoint start,end,max;
    start=TransformToScreen(XMin,YMin);
    end=  TransformToScreen(XMin,YMax);
    max=  TransformToScreen(XMax,YMax);
    // draw left line
    DC.DrawLine(start.x,start.y,end.x,end.y);
    // draw right line
    DC.DrawLine(max.x,start.y,max.x,end.y);
  }
  // Calculate first and ticks
  double start,step;
  CalcScaleDivisions(DisplayHeight,LabelHeight,YMin,YMax,&start,&step);
  // Now plot ticks and Labels
  while (start<YMax)
    {
      wxPoint tmp;
      // draw right line
      {
	// Calculate coordinates
	tmp=TransformToScreen(XMax,start);
	// create major tick
	DC.DrawLine(tmp.x-ticksize,tmp.y,tmp.x,tmp.y);
      }
      // draw left line
      {
	// Calculate coordinates
	tmp=TransformToScreen(XMin,start);
	// create major tick
	DC.DrawLine(tmp.x,tmp.y,tmp.x+ticksize,tmp.y);
      }
      
      // shall we also write out the labels?
      if (Label)
	{
	  if (ReverseScaleX())
	    { 
	      tmp=TransformToScreen(XMax,start);
	    }
	  // see if value is close to 0
	  if (
	      ((start<0)?-start:start)
	      *10<step)
	    {
	      start=0;
	    }
	  // make String
	  char string[256];
	  sprintf(string,"%g",start);
	  // get Size of the String
	  float w,h;

	  DC.GetTextExtent(string,&w,&h);
	  // plot string
	  DC.DrawText(string,tmp.x-w-ticksize,tmp.y-h/2);
	}
      // go to next label
      start+=step;
    }
}

void CMyGraph::CalcScaleDivisions(int Pixel,int TxtPixel,double min,double max,
				   double *start, double *step)
{
  double dX=max-min;
  int nMax=(int)floor(1.0*Pixel/TxtPixel);
  
  if (nMax>12)
    {
      nMax=12;
    }
  (*step)=dX/nMax;
  
  double StepInt=floor(log10(*step));
  double StepRem=pow(10.0,(log10(*step)-StepInt));

  if (StepRem>5)
    { StepRem=10; }
  else if (StepRem>2)
    { StepRem=5; }
  else if (StepRem>1)
    { StepRem=2; }
  else
    { StepRem=1; }
  
  (*step)=StepRem*pow(10.0,StepInt);

  // Calculate start
  *start=int(min/(*step))*(*step);
  if (*start<min)
    {
      (*start)+=(*step);
    }
}

void CMyGraph::RightButtonPressed(wxMouseEvent &)
{
  // nothing to do...
}

//////////////////////////////////////////////////////////////////////

CMyPlotArea::CMyPlotArea(CMyGraph* Parent,
			 int x, int y,
			 int w, int h)
  :wxCanvas(Parent,x,y,w,h,0)
{
  SetBackground(wxWHITE_BRUSH);
}

CMyPlotArea::~CMyPlotArea()
{
}

void CMyPlotArea::OnPaint()
{
  // Get the device-context
  wxDC &dc=*GetDC();
  // Get the block to be repainted
  int w,h;
  GetClientSize(&w,&h);
  // Now tell parent to paint the Stuff
  ((CMyGraph*)GetParent())->PaintPlot(dc,0,0,w,h);
  //That's all folks
}

void CMyPlotArea::OnEvent(wxMouseEvent& event)
{
  // Get coordinates
  float x,y;
  event.Position(&x,&y);
  // is the cursor moving?
  if (event.Moving())
    {
      ((CMyGraph*)GetParent())->SetCoordinates(x,y);
    }
  if (event.Dragging()&&event.LeftIsDown())
    {
      ((CMyGraph*)GetParent())->SetCoordinates(x,y);
      ((CMyGraph*)GetParent())->RemoveBox();
      ((CMyGraph*)GetParent())->DrawBox(x,y);
    }
  if (event.LeftDown())
    {
      ((CMyGraph*)GetParent())->ButtonDown(x,y);
    }
  if (event.LeftUp())
    {
      ((CMyGraph*)GetParent())->ButtonUp(x,y);
    }
  if (event.LeftIsDown()&&event.Leaving())
    {
      ((CMyGraph*)GetParent())->ButtonUp(x,y);
    }
  if (event.RightDown())
    {
      ((CMyGraph*)GetParent())->RightButtonPressed(event);
    }
}









