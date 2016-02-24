/*
 * Program:     Period98
 *
 * File:        xfdialog.h
 * Purpose:     header-file for
 *              ?
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */

#ifndef __f_file_h_
#define __f_file_h_
#include "xgdialog.h"
#include "dfourier.h"

#define BTN_CALC 100
#define BTN_DELETE 101
#define BTN_RENAME 102

///
void FourierListHandler(wxListBox &but,wxCommandEvent& event);
///
class f_File: public myFile
{
public:
  //
  f_File(CProject* project,wxWindow* parent,int x, int y, int w, int height);
  //
  ~f_File();
  //
  void SetSize(int x, int y, int w, int h, int flags=wxSIZE_AUTO);
  //
  void OnUknButton(int i);
  //
  void OnDelete();
  //
  void OnRename();
  //
  void OnCalc();
  //
  void UpdateStepping(CFourier::StepQuality stepq,double step);
  //
  virtual void OnSave();
  //
  virtual void OnLoad();

  //
  Bool Show(Bool i);
  //
  void UpdateDisplay(int i=0);
  //
  void UpdateData();
  //
  void UpdateList();
  //
  const CFourier *GetDisplayData() const;
  //
  myString GetName(int i) const;
  //
  int GetNames() const;
  //
  void OnDispData();
  //
  void OnDispGraph();

  //
  friend void FourierSteppingHandler(wxChoice &choice, wxCommandEvent& event);

protected:
  //
  wxText *Title;
  //
  wxText *From;
  //
  wxText *To;
  //
  wxMessage *Nyquist;
  //
  wxText *Stepping;
  //
  wxChoice *StepQuality;
  //
  wxMessage *PeakFreq;
  //
  wxMessage *PeakAmp;
  //
  wxRadioBox *Use;
  //
  wxRadioBox *Compact;
  //
  wxCheckBox *Weight;
  //
  wxButton* CalcBtn;
  //
  wxButton* DeleteBtn;
  //
  wxButton* RenameBtn;
  //
  wxListBox *List;
};

#endif






