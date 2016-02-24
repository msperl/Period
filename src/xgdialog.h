/*
 * Program:     Period98
 *
 * File:        xgdialog.h
 * Purpose:     header-file for
 *              ?
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */

#ifndef __xgdialog_h_
#define __xgdialog_h_
#include <wx.h>
#include "project.h"

#define MY_BTN_DATA 1
#define MY_BTN_SAVE 2
#define MY_BTN_LOAD 3
#define MY_BTN_GRAPH 4

///
void myFile_ButtonHandler(wxButton &but, wxCommandEvent& event);

///
class myFile: public wxPanel, public CMyDisplay
{
public:
  ///
  myFile(CProject *pro,int type,wxWindow* parent,int x, int y, int w, int h);
  ///
  myFile(CProject *pro,int type,wxWindow* parent,int x, int y, int w, int h, int i);
  ///
  ~myFile();
  ///
  virtual void SetSize(int x,int y,int w, int h,int flags=wxSIZE_AUTO);
  ///
  void CreateButtons();
  ///
  void ResizeButtons(int x,int size);
  ///
  virtual void OnSave();
  ///
  virtual void OnLoad();
  ///
  virtual void OnDispGraph();
  ///
  virtual void OnDispData();
  ///
  virtual void OnUknButton(int button);
  ///
  virtual void UpdateDisplay(int i)=0;
  ///
  virtual void UpdateData()=0;

  ///
  wxFunction DefaultButtonHandler() const
    { return(wxFunction) myFile_ButtonHandler;}
protected:
  ///
  wxButton *DispData;
  ///
  wxButton *Save;
  ///
  wxButton *Load;
  ///
  wxButton *DispGraph;
};


#endif




