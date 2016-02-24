/*
 * Program:     Period98
 *
 * File:        xpfreq.h
 * Purpose:     header-file for
 *              ?
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */

#ifndef __periods_h_
#define __periods_h_

#include <wx.h>
#include "xgprojec.h"
#include "str.h"

class DispFreqs;

///
class DispFreqLine
{
friend class DispFreqs;
public:
  ///
  DispFreqLine();
  ///
  ~DispFreqLine();
  ///
  void CreateDialogs(wxPanel *parent);
  ///
  void SetData(int i,int active,myString num,
	       myString fre, myString amp, 
	       myString pha, myString com="");
  ///
  void GetData(int &i, int *active,
	       myString *fre, myString * amp, 
	       myString * pha);
protected:
  ///
  wxCheckBox *mActive;
  ///
  wxText     *mFrequency;
  ///
  wxText     *mAmplitude;
  ///
  wxText     *mPhase;
  ///
  wxMessage  *mComposit;
  ///
  int         mLine;
};

///
class DispFreqs
{
public:
  ///
  DispFreqs(wxPanel * parent, int Lines);
  ///
  ~DispFreqs();
  ///
  void ShiftBy(int i);
  ///
  void UpdateDisplay()
  { GetData(); SetData(); }
  ///
  int GetLines() const
  { return mLines; }
  ///
  void SetData();
  ///
  void GetData();
  ///
  void ResizeButtons(int x, int w);
  
protected:
  ///
  DispFreqLine *mLinesStruct;
  ///
  int           mLines;
  ///
  int           mTop;
  ///
  wxButton *btmp1,*btmp2,*btmp3,*btmp4; 
};

///
void DispFreq_PrevNum(wxButton &but,wxCommandEvent& event);
///
void DispFreq_Prev(wxButton &but,wxCommandEvent& event);
///
void DispFreq_Next(wxButton &but,wxCommandEvent& event);
///
void DispFreq_NextNum(wxButton &but,wxCommandEvent& event);
///
void FrequencyEntriesChanged(wxText& text,wxCommandEvent& tmp);

#endif






