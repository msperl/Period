/*
 * Program:     Period98
 *
 * File:        xtgraph.h
 * Purpose:     header-file for
 *              ?
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */
#ifndef __t_graph_h_
#define __t_graph_h_
#include <wx.h>
#include "project.h"
#include "dtimestr.h"
#include "dperiod.h"
#include "xggraph.h"
#include <math.h>
#include "display.h"
#include "ldialtxt.h"

///
class t_Graph_Frame : public CMyGraph
{
public:
  ///
  t_Graph_Frame(CProject *data,int type,
		wxFrame* parent, char*Name,
		int x=-1, int y=-1,int width=-1, int height=-1);
  ///
  ~t_Graph_Frame();
  ///
  wxMenuBar *CreateMenu() const;

  ///
  virtual int ReverseScaleY();

  ///
  myString GetDocTitle();
  ///
  virtual char* GetXLabel() { return TIME; }
  ///
  virtual char* GetYLabel() { return SIGNAL; }
  ///
  void CalcExtrems();
  ///
  void DrawPlot(wxDC &DC, float scale=1);

  ///
  double GetAmplitude(int i);
  ///
  double GetError(int i)
    { return Project()->GetError(i);}
  ///
  double GetTime(int i);
  ///
  void SetColor(int i, wxDC& DC);
  ///
  void OnMenuCommand(int id);

  ///
  void SetUseName(int i);
  ///
  void SetUseData(int i);

  ///
  void RightButtonPressed(wxMouseEvent &event);
  
private:
  ///
  CTimeString *mData;
  ///
  CPeriod *mPeri;
  ///
  int ColorWhat;
  ///
  int UseData;
  ///
  int DrawError;
};

#endif









