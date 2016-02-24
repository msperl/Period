/*
 * Program:     Period98
 *
 * File:        xgdialog.cpp
 * Purpose:     implementation-file for
 *              ?
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */

#include "xgdialog.h"
#include "ldialtxt.h"

myFile::myFile(CProject * pro, int typ,wxWindow* parent,int x,int y,int w, int h)
  :wxPanel(parent,x,y,w,h),CMyDisplay(pro,typ)
{
  CreateButtons();
}

myFile::myFile(CProject * pro, int typ,wxWindow* parent,int x,int y,int w, int h,int)
  :wxPanel(parent,x,y,w,h),CMyDisplay(pro,typ)
{
}

void myFile::CreateButtons()
{
  // create Buttons Load,Save
  DispData=  new wxButton(this,DefaultButtonHandler(),
			  MYFILE_DATA);
  DispData->SetClientData((char*)MY_BTN_DATA);
  Load=      new wxButton(this,DefaultButtonHandler(),
			  MYFILE_LOAD);
  Load->SetClientData((char*)MY_BTN_LOAD);
  Save=      new wxButton(this,DefaultButtonHandler(),
			  MYFILE_SAVE);
  Save->SetClientData((char*)MY_BTN_SAVE);
  DispGraph= new wxButton(this,DefaultButtonHandler(),
			  MYFILE_GRAPH);
  DispGraph->SetClientData((char*)MY_BTN_GRAPH);
  // now Fit Panel to Fit
  Fit();
}

void myFile::SetSize(int x,int y,int w, int h, int flags)
{
  // sets the size
  wxPanel::SetSize(x,y,w,h,flags);
  // sets the coordinates for the buttons/etc...
  int bw,bh;
  DispData->GetSize(&bw,&bh);
  y=h-bh-5;
  w=w-20;
  x=10;
  bw=(w-30)/4;
  // rescales Buttons
  DispData ->SetSize(x        ,y,bw,bh);
  Load     ->SetSize(x+bw+10  ,y,bw,bh);
  Save     ->SetSize(x+2*bw+20,y,bw,bh);
  DispGraph->SetSize(x+3*bw+30,y,bw,bh);
}


myFile::~myFile()
{
  // nothing to destruct yet !!!
}

void myFile_ButtonHandler(wxButton &but, wxCommandEvent&)
{
  myFile* tmp=(myFile*)(but.GetParent());
  int which=(int)but.GetClientData();
  switch (which)
    {
    case MY_BTN_LOAD:  tmp->OnLoad();           break;
    case MY_BTN_SAVE:  tmp->OnSave();           break;
    case MY_BTN_GRAPH: tmp->OnDispGraph();      break;
    case MY_BTN_DATA:  tmp->OnDispData();       break;
    default:           tmp->OnUknButton(which); break;
    }
}

void myFile::OnUknButton(int)
{
  (void)wxMessageBox(MYFILE_UKNBUT,WARNING,wxOK|wxCENTRE);
}

void myFile::OnLoad()
{
  (void)wxMessageBox(MYFILE_NOLOAD,WARNING,wxOK|wxCENTRE);
}

void myFile::OnSave()
{
  (void)wxMessageBox(MYFILE_NOSAVE,WARNING,wxOK|wxCENTRE);
}

void myFile::OnDispGraph()
{
  (void)wxMessageBox(MYFILE_NOGRAP,WARNING,wxOK|wxCENTRE);
}
void myFile::OnDispData()
{
  (void)wxMessageBox(MYFILE_NODATA,WARNING,wxOK|wxCENTRE);
}








