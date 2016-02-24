/*
 * Program:     Period98
 *
 * File:        xgprojec.h
 * Purpose:     header-file for
 *              ?
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */

#ifndef __wxproject_h_
#define __wxproject_h_

#include <wx.h>

#include <wx_help.h>

#include "project.h"
#include "xpspecia.h"
//@ManMemo: include the file-data

class wxPeriProject: public CProject
{
public:
  ///
  wxPeriProject();
  ///
  virtual ~wxPeriProject();
  ///
  void SetProtocol(wxTextWindow* protocol);
  ///
  void WriteSpecificProtocol(ostream &ost);

  //@ManMemo: general virtual functions
  void InformUser(char *text);
  ///
  int Confirm(char *text);
  ///
  myString GetFileName(int Load,char* title,myString name="",
		       char *ext=NULL,char* pat="*.*");

  ///
  void Waiting(int on=FALSE, int cancel=FALSE);
  
  //@ManMemo: TimeString virtual functions
  myString FileFormat(myString format,int Phase=0);
  myString FileFormat(myString format,myString filename);

  
  //@ManMemo: Period virtual functions
  int GetPeriodSelection();
  ///
  int *GetAmpVarData(int *freqs, int* what, CalcMode *mode);
  ///
  void DisplayAmpVarData(char *txt);

  //@ManMemo: Protocol stuff
  void CleanProtocol();
  ///
  void AppendAtEndOfProtocol();
  ///
  void AddaptProtocolSize();

  //@ManMemo: Help stuff
  void HelpContents();
  ///
  void HelpKeyword(char *s=NULL);

  ///
  void DisplayTSGraph();
  ///
  void DisplayTSTable();
  ///
  void DisplayFourierGraph();
  ///
  void DisplayFourierTable();
  ///
  void DisplayPeriodGraph();
  
protected:
  ///
  wxTextWindow *mProtocolWindow;
  ///
  wxHelpInstance *HelpInstance;
};

///
extern wxPeriProject myProject;

#endif


