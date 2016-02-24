/*
 * Program:     Period98
 *
 * File:        xpgraph.h
 * Purpose:     header-file for
 *              phase diagram
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */
#ifndef __p_graph_h_
#define __p_graph_h_
#include <wx.h>
#include "project.h"
#include "dtimestr.h"
#include "dperiod.h"
#include "xggraph.h"
#include <math.h>
#include "display.h"
#include "ldialtxt.h"

///
class p_Graph_Frame : public CMyGraph
{
public:
  ///
  p_Graph_Frame(CProject *data,int type,
		wxFrame* parent, char*Name,
		int x=-1, int y=-1,int width=-1, int height=-1);
  ///
  ~p_Graph_Frame();
  ///
  wxMenuBar *CreateMenu() const;

  ///
  myString GetDocTitle();
  ///
  virtual int ReverseScaleY();
  ///
  virtual char* GetXLabel() { return PHASE; }
  ///
  virtual char* GetYLabel() { return SIGNAL; }
  ///
  void CalcExtrems();
  ///
  void DrawPlot(wxDC &DC, float scale=1);

  ///
  double GetError(int i)
    { return Project()->GetError(i);}
  ///
  double GetAmplitude(int i);
  ///
  double GetTime(int i)
  { return (*mData)[i].GetPhasedTime(Frequency); }
  ///
  double BinValue() const 
  { return 1.0/Bins;}
  ///
  void SetColor(int i, wxDC& DC);
  ///
  void OnMenuCommand(int id);

  ///
  void SetUseName(int i);
  ///
  void SetUseData(int i);
  ///
  void SetUseBinning(int i);
  ///
  void ChangeFrequency();
  ///
  void ChangeBinSpacing();
  ///
  void FindFirstFrequency();
  ///
  void FillStatusBarWithFrequency();
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
  int UseBinning;
  ///
  int Bins;
  ///
  double Frequency;
  ///
  int binsize;
  double *binampl;
  double *binampl2;
  double *binphase;
  int *bincount;
  ///
  int DrawError;
};

#endif









