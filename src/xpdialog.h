/*
 * Program:     Period98
 *
 * File:        xpdialog.h
 * Purpose:     header-file for
 *              ?
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */

#ifndef __p_file_h_
#define __p_file_h_
#include "xgdialog.h"
#include "xgprojec.h"
#include "xpfreq.h"

#define BTN_CALC_ALL 200
#define BTN_CALC_FIXED 201
#define BTN_CALC_SELECT 202

///
class p_File: public myFile
{
public:
  ///
  p_File(CProject* project,wxWindow* parent,int x, int y, int w, int h);
  ///
  ~p_File();
  ///
  void SetSize(int x, int y, int w, int h, int flags=wxSIZE_AUTO);
  ///
  void UpdateDisplay(int i=0);
  ///
  void UpdateData() { Freqs->GetData(); }
  ///
  Bool Show(Bool show);

  ///
  void OnLoad();
  ///
  void OnSave();
  ///
  void OnDispGraph();
  ///
  void OnUknButton(int button);

  
protected:
  ///
  wxMessage *Active;
  ///
  wxMessage *Zero;
  ///
  wxMessage *Residuals;
  ///
  wxMessage *Loops;
  ///
  wxRadioBox *Based;
  ///
  wxCheckBox *Weight;
  
  ///
  DispFreqs *Freqs;
  
  ///
  wxButton *CalcFixed;
  ///
  wxButton *CalcAll;
  ///
  wxButton *CalcSelect;
};


#endif






