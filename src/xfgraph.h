/*
 * Program:     Period98
 *
 * File:        xfgraph.h
 * Purpose:     header-file for
 *              ?
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */

#ifndef __f_graph_h_
#define __f_graph_h_
#include <wx.h>
#include "project.h"
#include "xggraph.h"
#include <math.h>
#include "ldialtxt.h"

///
class f_Graph_Frame : public CMyGraph
{
public:
  ///
  f_Graph_Frame(CProject *data,int type,
		wxFrame* parent, char*Name,
		int x=-1, int y=-1,int width=-1, int height=-1);
  ///
  ~f_Graph_Frame();

  ///
  myString GetDocTitle();
  ///
  virtual char* GetXLabel() { return FREQUENCY; }
  ///
  virtual char* GetYLabel() { return (UsePower)?POWER:AMPLITUDE; }
  ///
  void CalcExtrems();
  ///
  void DrawPlot(wxDC &DC, float scale=1);

  ///
  void GetData(int i,double *f,double *a);
  
  ///
  void OnMenuCommand(int id);

  ///
  void SetUsePower(int i);
  
private:
  ///
  CFourier *mData;
  ///
  int UsePower;
  ///
  double MinAmp;
  ///
  double MaxAmp;
};

#endif

