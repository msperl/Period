/*
 * Program:     Period98
 *
 * File:        xttable.cpp
 * Purpose:     implementation-file for
 *              ?
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */

#include "ldialtxt.h"
#include "xgprojec.h"
#include "xgmainfr.h"
#include "xgeneral.h"
#include "xtrename.h"
#include "fgeneral.h"
#include "xgdial.h"

void DialogDeletePoint(int row)
{
  // delete the point
  myProject.DeletePoint(row);
}

void DialogRelabelPoint(int row)
{
  // create a dialog and ask user
  myDialogBox dialog(NULL,DIAL_RELABEL_TIT,TRUE);
  // write the message
  {
    char msgtxt[256];
    sprintf(msgtxt,DIAL_RELABEL_TIME
	    FORMAT_TIME,
	    myProject.GetTimeString()[row].GetTime());
    (void) new wxMessage(&dialog,msgtxt);
    dialog.NewLine();
  }
  // create entries
  wxText *names[4];
  int i;
  myString tmp;
  int x,y,w,h,xmax=0,wmax=0;
  for (i=0;i<4;i++)
    {
      // first write the message
      tmp=myProject.GetTimeString().NameSet(i);
      (void) new wxMessage(&dialog,tmp.chars(),10);
      // then fill the text
      int index=myProject.GetTimeString()[row].GetIDName(i);
      tmp=myProject.GetIDNameStr(i,index);
      names[i]=new wxText(&dialog,NULL,NULL,tmp.chars());
      // find out about the size
      names[i]->GetPosition(&x,&y);
      names[i]->GetSize(&w,&h);
      if (x>xmax) { xmax=x; }
      if (w>wmax) { wmax=w; }
      // and now do newline
      dialog.NewLine();
    }
  // now allign all
  for (i=0;i<4;i++)
    {
      names[i]->SetSize(xmax,-1,wmax,-1);
    }
  // now create Button OK/cancel
  wxButton *OK=new wxButton(&dialog,GENERAL_BUTTON_HANDLER,OKAY);
  OK->SetClientData(0);
  OK->SetDefault();
  wxButton *Cancel= new wxButton(&dialog,GENERAL_BUTTON_HANDLER,CANCEL);
  Cancel->SetClientData(0);
  
  // fit the dialog
  dialog.Fit();
  
  // now show dialog
  dialog.Show(TRUE);
  
  // has OK been pressed?
  if (OK->GetClientData()!=0)
    {
      // relabel data-point
      myProject.RelabelPoint(row,
			     names[0]->GetValue(),
			     names[1]->GetValue(),
			     names[2]->GetValue(),
			     names[3]->GetValue());
    }
}

void DialogDeletePointInfo(wxWindow *Frame)
{
  // create dialog
  myDialogBox dialog(Frame,DIAL_DELPNT_TIT,TRUE);
  // get relevant values
  int what;
  myString name;
  myProject.GetDeletePointInfo(&what,&name);
  // create entries
  myString StrOptions[4];
  char *options[4];
  for (int i=0;i<4;i++)
    {
      StrOptions[i]=myProject.NameSet(i);
      options[i]=StrOptions[i].chars();
    }
  (void) new wxMessage(&dialog,DIAL_DELPNT_3);
  dialog.NewLine();
  wxChoice *Column= new wxChoice(&dialog,NULL,
                                 NULL,
                                 -1,-1,-1,-1,
                                 4,options);
  Column->SetSelection(what);
  dialog.NewLine();
  // now the label 
  (void) new wxMessage(&dialog,DIAL_DELPNT_2);
  dialog.NewLine();
  wxText * Label= new wxText(&dialog,NULL,NULL,name.chars());
  wxButton *OK, *Cancel;
  dialog.NewLine();
  // now create Button OK/cancel
  OK=new wxButton(&dialog,GENERAL_BUTTON_HANDLER,OKAY);
  OK->SetClientData(0);
  OK->SetDefault();
  Cancel=new wxButton(&dialog,GENERAL_BUTTON_HANDLER,CANCEL);
  // Fit the Dialog
  dialog.Fit();
  // resize all stuff to fit...
  {
    // dialog size
    int dw,dh;
    dialog.GetSize(&dw,&dh);
    // calculate stuff
    int xOk,yOk,wOk,hOk;
    OK->GetSize(&wOk,&hOk);
    OK->GetPosition(&xOk,&yOk);
    int xCancel,yCancel;
    Cancel->GetPosition(&xCancel,&yCancel);
    int spacing=xCancel-xOk-wOk;
    int width=(dw-xOk-2*spacing)/2;
    // resize Buttons
    OK->SetSize(xOk,-1,width,-1);
    Cancel->SetSize(xOk+width+spacing,-1,width,-1);
    // resize other elements
    Label->SetSize(xOk,-1,dw-xOk-spacing,-1);
    Column->SetSize(xOk,-1,dw-xOk-spacing,-1);
  }
  // fit the dialog
  dialog.Fit();
  dialog.Centre();
  // now show dialog
  dialog.Show(TRUE);  
  // has OK been pressed?
  if (OK->GetClientData()!=0)
    {
      // get names
      what=Column->GetSelection();
      name=Label->GetValue();
      // relabel data-point
      myProject.SetDeletePointInfo(what,name);
    }
}



