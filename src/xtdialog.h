/*
 * Program:     Period98
 *
 * File:        xtdialog.h
 * Purpose:     header-file for
 *              ?
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */
#ifndef __d_file_h_
#define __d_file_h_

#include "xgdialog.h"
#include "xgprojec.h"

#define BTN_EDIT 1000
#define BTN_LABEL 2000
#define BTN_SELECT 77

///
class t_File: public myFile
{
public:
  ///
  t_File(CProject *project,wxWindow* parent,int x, int y, int w, int height);
  ///
  ~t_File();
  ///
  void SetSize(int x, int y, int w, int h, int flags=wxSIZE_AUTO);
  ///
  void UpdateDisplay(int i=0);
  ///
  void UpdateData();
  ///
  virtual void OnLoad();
  ///
  virtual void OnSave();
  ///
  void SetSelection();
  ///
  void SetReverse();

protected:
  ///
  void myAllign(wxListBox* lb, wxButton *bu, int id);
  ///
  void FillList(wxListBox * lbox, int i);
  ///
  void OnUknButton(int i);
  ///
  void UpdateSelected();
  ///
  void EditSettings(int i);
  ///
  void EditLabel(int i);
  ///
  void OnDispGraph();
  ///
  void OnDispData();
  ///
  wxMultiText *DataFile;
  ///
  wxMessage *Selected;
  ///
  wxMessage *Total;
  ///
  wxMessage *Start;
  ///
  wxMessage *End;
  ///
  wxCheckBox *ReverseScale;

  ///
  wxButton  *Label[4];
  ///
  wxListBox *Lists[4];
  ///
  wxButton  *Edit[4];
  
  ///
  wxButton * Select;
};

//@ManMemo: event-handler for the Change-Dialog...
void ChangeDialogCancel(wxButton&,wxEvent &);
///
void ChangeDialogOK(wxButton&,wxEvent &);
///
void ListBoxClick(wxListBox&,wxEvent &);
///
void ReverseClick(wxCheckBox&,wxEvent &);
#endif




