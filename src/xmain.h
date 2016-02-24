/*
 * Program:     Period98
 *
 * File:        xmain.h
 * Purpose:     header-file for
 *              ?
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */

#ifndef __wxperiod_h
#define __wxperiod_h

#include <wx.h>

//
// Here go the main-window-definition
//@ManMemo:

class wxPeriod: public wxApp
{
public:
  //@ManMemo: constructor
  wxPeriod(void);
  //@ManMemo: main-application-initialisation
  wxFrame *OnInit();
  //
  int FrequencyRows;
};

extern wxPeriod myApp;

#endif
























