/*
 * Program:     Period98
 *
 * File:        xgmainfr.cpp
 * Purpose:     implementation-file for
 *              ?
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */

#include <wx.h>
#include <wxtab.h>

#include "wxtxtprint.h"

#include "xgsize.h"
#include "xgmainfr.h"
#include "xgmenu.h"
#include "project.h"
#include "str.h"

#include "xppredic.h"
#include "xfnoise.h"
#include "xgdialog.h"
// now include definitions for all labels
#include "ldialtxt.h"
#include "xtrename.h"

#include "fproject.h"
#include "flog.h"
#include "xgeneral.h"

// include the icon for X
#ifdef wx_x
#if USE_XPM
#include "bitmaps/programm.xpm"
#else
#include "bitmaps/programm.xbm"
#endif
#endif

#define TAB_PROTOCOL   900
#define TAB_FOURIER    901
#define TAB_PERIOD     902
#define TAB_TIMESTRING 903

CMainFrame * MainFrame;

CMainFrame::CMainFrame()
  //  :wxFrame(NULL,APPNAME,-1,-1,mainframe_w,mainframe_h)
  :wxFrame(NULL,APPNAME,-1,-1,900,900,
	   wxSDI|
  	   wxMINIMIZE_BOX|
#ifndef wx_msw
           wxTHICK_FRAME|
#endif
           wxSYSTEM_MENU|
           wxCAPTION)
{
  // set the variable;
  MainFrame=this;
  // define the icon for the main-application-window
#ifdef wx_msw
  SetIcon(new wxIcon("programm"));
#endif

#ifdef wx_x
#if USE_XPM
  SetIcon(new wxIcon(programm_xpm));
#else
  SetIcon(new wxIcon(programm_bits,programm_width,programm_height));
#endif
#endif
  
  // create a status-line
  CreateStatusLine(1);
  
  // create the menu
  MenuBar=CreateMenuBar();
  SetMenuBar(MenuBar);
  
  // create a tabbed panel
  
  panel= new wxTabbedPanel(this,0,0,800,800);
  view= new wxPanelTabView(panel,wxTAB_STYLE_DRAW_BOX);
    
  // now create the subframes
  wxFont *myFont=wxTheFontList->FindOrCreateFont(12,wxMODERN,
						 wxNORMAL,wxNORMAL);
  TextDisp= new wxTextWindow(panel,0,0,800,800,wxNATIVE_IMPL);
  TextDisp->SetFont(myFont);
  TextDisp->Show(FALSE);
  // and make it the default-protocol
  myProject.SetProtocol(TextDisp);

  // the Timestring
  DataDisp= new t_File(&myProject,panel,0,0,800,800);
  // now the Period
  PeriDisp= new p_File(&myProject,panel,0,0,800,800);
  // now the Fourier
  FourDisp= new f_File(&myProject,panel,0,0,800,800);

  // Now find largest Size
  int xm,ym,xt,yt;
  DataDisp->Fit();
  DataDisp->GetSize(&xm,&ym);
  PeriDisp->Fit();
  PeriDisp->GetSize(&xt,&yt);
  xm=(xm>xt)?xm:xt;
  ym=(ym>yt)?ym:yt;
  FourDisp->Fit();
  FourDisp->GetSize(&xt,&yt);
  xm=(xm>xt)?xm:xt;
  ym=(ym>yt)?ym:yt;
  
  
  // Now set the size of all panels
  TextDisp->SetSize(fdialog_x+5,fdialog_y+5,xm,ym);
  DataDisp->SetSize(fdialog_x+5,fdialog_y+5,xm,ym);
  PeriDisp->SetSize(fdialog_x+5,fdialog_y+5,xm,ym);
  FourDisp->SetSize(fdialog_x+5,fdialog_y+5,xm,ym);
  
  // now define Size
  wxRectangle rect;
  rect.x=fdialog_x;
  rect.y=fdialog_y;
  rect.width=xm+10;
  rect.height=ym+10;

  // now set the view-area
  view->SetViewRect(rect);

  // And now calculate tab width
  view->CalculateTabWidth(4,TRUE);
  
  // now add the labels to the tabbed panel
  view->AddTab(TAB_TIMESTRING,wxString(TIMESTRING));
  view->AddTab(TAB_PERIOD,wxString(PERIODS));
  view->AddTab(TAB_FOURIER,wxString(FOURIER));
  view->AddTab(TAB_PROTOCOL,wxString(PROTOCOL));

  // now add the Windows
  view->AddTabWindow(TAB_PROTOCOL,TextDisp);
  view->AddTabWindow(TAB_TIMESTRING,DataDisp);
  view->AddTabWindow(TAB_PERIOD,PeriDisp);
  view->AddTabWindow(TAB_FOURIER,FourDisp);

  // now set the selection
  view->SetTabSelection(TAB_TIMESTRING);

  // now resize us to fit...
  panel->wxWindow::SetSize(xm+15,ym+50);

  // now resize Window
  Fit();

  // now create the Calculating-panel
  Calculating= new wxPanel(this,0,0,xm+15,ym+50);
  // write Text
  wxMessage *tmp= new wxMessage(Calculating,CALCULATING,
				100,ym/2);
  Calculating->NewLine();
  CalcInfo= new wxMessage(Calculating,"....",100,-1);
  Calculating->NewLine();
  Calculating->NewLine();
  tmp->GetSize(&xm,&ym);
  CalcCancel= new wxButton(Calculating,GENERAL_BUTTON_HANDLER_NOHIDE,CANCEL,
			   100,-1,xm,-1);
  CalcCancel->SetClientData(0);
  CalcCancel->Show(FALSE);
  // Set the wait-cursor
  wxCursor *cursor= new wxCursor(wxCURSOR_WAIT);
  Calculating->SetCursor(cursor);
  // and hide it
  Calculating->Show(FALSE);
  // We are not calculating
  IsCalculating=0;
  // Show the frame
  Show(TRUE);
}

void CMainFrame::OnSize(int w, int h)
{
  /*
    if ((w!=mainframe_w)||(h!=mainframe_h))
    {
      w=mainframe_w;
      h=mainframe_h;
      wxWindow::SetSize(w,h);
    }
  */
  wxFrame::OnSize(w,h);
}

Bool CMainFrame::OnClose()
{
  return NeedToSave();
  //return wxFrame::OnClose();
  //return TRUE;
}

void CMainFrame::SetInfo(char * text)
{
  // Set the label
  CalcInfo->SetLabel(text);
}

int CMainFrame::CancelCalculation() const
{
  return (int)CalcCancel->GetClientData();
}

void CMainFrame::SetCalculating(int flag,int Cancel)
{
  if (flag)
    {
      // clear Calculate info
      SetInfo("");
      // Hide TabbedPannel
      panel->Show(FALSE);
      // Show Calulate Pannel
      Calculating->Show(TRUE);
      // shall we show the CANCEL-button?
      CalcCancel->Show(Cancel);
      CalcCancel->SetClientData(0);
      // Set Is Calculating flag
      IsCalculating=TRUE;
    }
  else
    {
      // Hide Calulate Pannel
      Calculating->Show(FALSE);
      // Show TabbedPannel
      panel->Show(TRUE);
      // clear Is Calculating flag
      IsCalculating=FALSE;
    }
}

void InformUserOfProgress(char *text)
{
  MainFrame->SetInfo(text);
}

int AskUserToContinue(char * text)
{
  return (wxMessageBox(text,"",wxYES_NO|wxCENTRE,MainFrame)==wxYES);
}








