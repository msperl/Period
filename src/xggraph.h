/*
 * Program:     Period98
 *
 * File:        xggraph.h
 * Purpose:     header-file for
 *              ?
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */

#ifndef __mygraph_h__
#define __mygraph_h__

#include <wx.h>
#include "str.h"
#include "ldialtxt.h"
#include "xgframe.h"

class CMyPlotArea;

///
class CMyGraph :  public CMyFrame
{
public:
  ///
  CMyGraph(CProject *project,int type,
	   wxFrame *parent, char *title,
	   int x=-1, int y=-1, int width=-1, int height=-1,
	   long style = wxSDI | wxDEFAULT_FRAME,
	   char *name = MYGRAPH_NAME,int flag=1,int Title=0);
  ///
  ~CMyGraph();

  ///
  void CreateCanvas();
  ///
  void UpdateDisplay(int flag);
  ///
  void UpdateData();

  ///
  Bool OnClose();
  ///
  void OnPrint();
  ///
  void OnMenuCommand(int id);
  ///
  void PaintPlot(wxDC & dc, int x,int y, int width, int height,float scale=1);
  ///
  void NoData(wxDC& dc);
  ///
  void SetVisible(double xmi,double xma,double ymi,double yma);

  ///
  wxPoint TransformToScreen(double x, double y);
  ///
  wxPoint TransformFromScreen(float x, float y);

  ///
  void SetCoordinates(float x, float y);
  ///
  void ButtonDown(float x, float y);
  ///
  void ButtonUp(float x, float y);
  ///
  virtual void RightButtonPressed(wxMouseEvent &event);
  ///
  void RemoveBox();
  ///
  void DrawBox(float x, float y);
  
protected:
  ///
  virtual myString GetDocTitle();
  ///
  virtual void CalcExtrems();
  ///
  virtual void DrawPlot(wxDC &DC, float scale=1);
  ///
  virtual int ReverseScaleX();
  ///
  virtual int ReverseScaleY();

  ///
  void CalculateLabelSize(wxDC &DC);

  ///
  void CalculateScales(int x, int y, int w, int h);

  ///
  void DrawXCoordinates(wxDC &DC,int Ticksize=5, int Label=1,float scale=1.);
  ///
  void DrawYCoordinates(wxDC &DC,int Ticksize=5, int Label=1,float scale=1.);
  ///
  void CalcScaleDivisions(int Pixel,int TxtPixel,double min,double max,
			   double *start, double *step);
  ///
  virtual char* GetXLabel() { return "X"; }
  ///
  virtual char* GetYLabel() { return "Y"; }
  
protected:
  //@ManMemo: Maximum-Data-Size
  float ExtXMax,ExtXMin;
  ///
  float ExtYMax,ExtYMin;
  //@ManMemo: Displayed Data-Size
  float XMin,XMax;
  ///
  float YMin,YMax;
  ///
  CMyPlotArea *myPlotArea;

private:
  ///
  int XLabelWidth,LabelHeight;
  ///
  int YLabelWidth;

  ///
  float DisplayScaleX,DisplayScaleY;
  ///
  float DisplayMinX,DisplayMinY;
  ///
  int   DisplayWidth,DisplayHeight;
  ///
  int   Ticksize;
  ///
  wxPoint downcoords;
  ///
  wxPoint lastcoords;
  ///
  int UseTitle;
};

///
class CMyPlotArea : public wxCanvas
{
public:
  ///
  CMyPlotArea(CMyGraph* Parent,
	      int x=-1, int y=-1,
	      int w=-1, int h=-1);
  ///
  ~CMyPlotArea();

  ///
  void OnEvent(wxMouseEvent& event);
  ///
  void OnPaint();
  
};


#endif
