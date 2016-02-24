/*
 * Program:     Period98
 *
 * File:        xgmainfr.h
 * Purpose:     header-file for
 *              ?
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */

#ifndef __mainfram_h
#define __mainfram_h

#include <wx.h>
// include the file-data
#include "xtdialog.h"
#include "xpdialog.h"
#include "xfdialog.h"
#include <wxtab.h>

//@ManMemo: main-frame-class

class CMainFrame: public wxFrame
{
public:
  /// constructor
  CMainFrame();
  ///
  void OnSize(int w,int h);
  ///
  Bool OnClose();
  ///
  void OnMenuCommand(int id);
  ///
  int FileMenu(int id);
  ///
  int TimestringMenu(int id);
  ///
  int FitMenu(int id);
  ///
  int FourierMenu(int id);
  ///
  int LogMenu(int id);

  ///
  void SetCalculating(int flag,int Cancel=FALSE);
  ///
  void SetInfo(char* text);
  ///
  int CancelCalculation() const;
  
private:
  ///
  wxTabbedPanel *panel;
  ///
  wxPanelTabView *view;

  ///
  wxPanel *Calculating;
  ///
  wxMessage *CalcInfo;
  ///
  wxButton *CalcCancel;
  ///
  wxMenuBar *MenuBar;
  
  ///
  int IsCalculating;
  
  ///
  t_File *DataDisp;
  ///
  p_File *PeriDisp;
  ///
  f_File *FourDisp;
  ///
  wxTextWindow *TextDisp;
};

///
extern CMainFrame * MainFrame;

#endif









