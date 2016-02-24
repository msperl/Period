/*
 * Program:     Period98
 *
 * File:        xgprojec.cpp
 * Purpose:     implementation-file for
 *              ?
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */

#include "xgeneral.h"
#include "xgprojec.h"
#include "xgmainfr.h"

#include "xpspecia.h"

#include "xfgraph.h"
#include "xftable.h"
#include "xtgraph.h"
#include "xttable.h"
#include "xpgraph.h"

#include "xgmainfr.h"
#include "ldialtxt.h"
#include "px.h"
#include "error.h"

wxPeriProject myProject;

#include <signal.h>

#ifdef wx_msw
volatile double MSVC_FLOAT_TRAP=0.0f;
#endif

#define USERMESSAGE(x) MYERROR(x);myProject.InformUser( x );

// signal handlers
void Signal_Handler(int sig)
{
  // restore signal handler - needed for coredump
  (void) signal(SIGFPE , SIG_DFL);  
  (void) signal(SIGSEGV, SIG_DFL);  
  // inform user
  switch (sig)
    {
    case SIGFPE:
      USERMESSAGE(SIGNAL_FPE);
      break;
    case SIGSEGV:
      USERMESSAGE(SIGNAL_SEGV);
      break;
    default:
      USERMESSAGE(SIGNAL_UKN);
    }
  // write project
  myProject.SaveProjectAs();
  // abort program - forcing core-dump, if possible
  abort();
  // exit forcefully from program
  exit(255);
}

wxPeriProject::wxPeriProject()
  :mProtocolWindow(NULL)
{
  // install help-instance
  HelpInstance= new wxHelpInstance(TRUE);
  HelpInstance->Initialize(APPNAME);
  // install signal handler
//#ifndef wx_msw
#ifndef NO_USE_SIGNAL_HANDLER
  (void) signal(SIGFPE , Signal_Handler);  
  (void) signal(SIGSEGV, Signal_Handler);
#endif
//#endif
}

wxPeriProject::~wxPeriProject()
{
  delete Protocol;
  // Remove Help
  HelpInstance->Quit();
  delete HelpInstance;
}

void wxPeriProject::SetProtocol(wxTextWindow* protocol)
{
  if (mProtocolWindow!=NULL)
    {
      cerr<<"Set Protocol has allready been called once before..."<<endl;
      exit(2);
    }
  // make it the default-window
  mProtocolWindow=protocol;
  // create the important stream...
  Protocol= new ostream(mProtocolWindow);
}
void wxPeriProject::AppendAtEndOfProtocol()
{
  mProtocolWindow->SetInsertionPointEnd();
}

void wxPeriProject::CleanProtocol()
{
  if (mProtocolWindow!=NULL)
    {
      mProtocolWindow->Clear();
    }
}

void wxPeriProject::AddaptProtocolSize()
{
#ifdef wx_msw
  // this is only relevant for some versions of MS-Windows,
  // which do not allow log-files longer than 32k or so...
  int Major,Minor;
  int OSVersion=::wxGetOsVersion(&Major,&Minor);
  if (
      (OSVersion==wxWIN32S)
      || (OSVersion==wxWINDOWS)
      || (OSVersion==wxWIN95)
      )
    {
      // check for the size of the Logfile
      if (mProtocolWindow->GetLastPosition()>28000)
	{
	  // ask the user if he wants half his log killed...
	  if (Confirm(PROT_LOGFULL))
	    {
	      // kill lines...
	      mProtocolWindow->Remove(0,14000);
	      // and add at the head a new message...
	      mProtocolWindow->SetInsertionPoint(0);
	      *Protocol<<PROT_LOGFULL_MSG<<endl;
	    }
	}
    }
#endif
}

void wxPeriProject::WriteSpecificProtocol(ostream &ost)
{
  // get the Text
  char *ptr=mProtocolWindow->GetContents();
  // output text
  ost<<ptr<<endl;
  // delete text
  // this may produce some memory-leaks with mswin, but 
  // to be on the safe side, as Xt does hickup on this,
  // it is best to leave it like this...
  // delete ptr;
}

void wxPeriProject::InformUser(char *text)
{
  (void)wxMessageBox(text,WXPROJ_WARNING,wxOK|wxCENTRE,MainFrame);
}

int wxPeriProject::Confirm(char *text)
{
  int answer=wxMessageBox(text,"",wxYES_NO|wxCENTRE,MainFrame);
  if (answer==wxYES)
    { return 1; }
  return 0;
}

myString wxPeriProject::GetFileName(int Load,char* title,myString filename,
				   char*ext,char* pat)
{
  char *result;
  //
  result=wxFileSelector(title,NULL,filename.chars(),
			ext,pat,
			(Load!=0)? wxOPEN : wxSAVE|wxOVERWRITE_PROMPT,
			MainFrame);
  //
  if (result)
    {
      if (result[0]==0)
	{
	  result=NULL;
	}
    }
  //
  return result;
}

int wxPeriProject::GetPeriodSelection()
{
  return SpecialDialog(MainFrame);
}

int *wxPeriProject::GetAmpVarData(int *freqs, int* what, CalcMode *mode)
{
  return AmplitudeVariation(MainFrame,freqs,what,mode);
}

void wxPeriProject::DisplayAmpVarData(char *txt)
{ 
  AmplitudeVariationDisplay(MainFrame,txt); 
}

void wxPeriProject::Waiting(int on,int cancel)
{
  MainFrame->SetCalculating(on,cancel);
  wxYield();
}

void wxPeriProject::HelpContents()
{
  HelpInstance->LoadFile();
  HelpInstance->DisplayContents();
}

void wxPeriProject::HelpKeyword(char *s)
{
  char *s1=s;
  if (s==NULL)
    {
      s1=wxGetTextFromUser(WXPROJ_KEYWORD);
    }
  if (s1)
    {
      HelpInstance->LoadFile();
      HelpInstance->KeywordSearch(s);
    }
}

// Display creation

void wxPeriProject::DisplayTSGraph()
{
  (void) new t_Graph_Frame(this,DISPLAY_TIMESTRING,
                           NULL,TIME_PLOTNAME,
                           -1,-1,
                           500,400
                           );

}

void wxPeriProject::DisplayTSTable()
{
  (void) new t_Table_Frame(this,DISPLAY_TIMETABLE,
			   NULL,TIME_TABLENAME,
			   -1,-1,
			   500,400
			   );		
}

void wxPeriProject::DisplayPeriodGraph()
{
  (void) new p_Graph_Frame(this,DISPLAY_PHASEPLOT,
                           NULL,PER_PLOTNAME,
                           -1,-1,
                           500,400
                           );

}

void wxPeriProject::DisplayFourierGraph()
{
  if (GetFourierEntries()==0)
    { return; }
  (void) new f_Graph_Frame(this,GetFourierActive()->GetFourierID(),
			   NULL,FOU_PLOTNAME,
			   -1,-1,
			   500,400
			   );
}

void wxPeriProject::DisplayFourierTable()
{
  if (GetFourierEntries()==0)
    { return; }
  
  (void) new f_Table_Frame(this,GetFourierActive()->GetFourierID(),
			   NULL,FOU_TABLENAME,
			   -1,-1,
			   500,400
			   );
}



