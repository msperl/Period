/*
 * Program:     Period98
 *
 * File:        xgtable.h
 * Purpose:     header-file for
 *              ?
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */

#ifndef __myTable_h__
#define __myTable_h__

#include <wx.h>
#include "str.h"
#include "ldialtxt.h"
#include "xgframe.h"

class CMyTableArea;

///
class CMyTable :  public CMyFrame
{
public:
  ///
  CMyTable(CProject *data, int type,
	   wxFrame *parent, char *title,
	   int x=-1, int y=-1, int width=-1, int height=-1,
	   long style = wxSDI | wxDEFAULT_FRAME,
	   char *name = MYTABLE_NAME,int flag=0);
  ///
  ~CMyTable();

  ///
  void CreateCanvas();
  ///
  Bool OnClose();
  ///
  void OnPrint();
  ///
  void OnMenuCommand(int id);
  ///
  int PaintPlot(wxDC & dc, int x,int y, int width, int height,int first);
  ///
  void UpdateDisplay(int flag=0);
  ///
  void DrawText(wxDC &DC, myString const & text,
		int x0, int y0,
		int w0, int h0,
		int where);  
  ///
  void OnMyMouseEvent(wxMouseEvent& event);
  ///
  virtual void OnMouseEvent(wxMouseEvent& event, int x, int y);

protected:
  ///
  virtual myString GetDocTitle();

  ///
  virtual myString GetHeader(int column) const =0;
  ///
  virtual int NumberOfLines() const =0;
  ///
  virtual myString GetData(int line,int column) const =0;

  ///
  virtual int NumberOfTabs() const =0;
  ///
  virtual int TabWidth(int i) const =0;
  
protected:
  ///
  CMyTableArea *myPlotArea;
  ///
  int LineHeight;
private:
};


///
class CMyTableArea : public wxCanvas
{
public:
  ///
  CMyTableArea(CMyTable* Parent,
	      int x=-1, int y=-1,
	      int w=-1, int h=-1);
  ///
  ~CMyTableArea();

  ///
  void OnPaint();
  ///
  void OnSize(int w,int h);
  ///
  void OnScroll(wxCommandEvent & event);
  ///
  void SetLineHeight(int h) { LineHeight=h; }
  /// Eventhandler for Mouse-events
  void OnEvent(wxMouseEvent& event);
protected:
  ///
  int LineHeight;
};


#endif





