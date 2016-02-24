/*
 * Program:     Period98
 *
 * File:        xggraph.h
 * Purpose:     header-file for
 *              ?
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */

#ifndef __myframe_h__
#define __myframe_h__

#include <wx.h>
#include "display.h"

class CMyFrame : public wxFrame, public CMyDisplay
{
 public:
  CMyFrame(CProject * project, int type,wxFrame *parent, char *title,
           int x=-1, int y=-1, int w=-1, int h=-1,
           long style = wxSDI | wxDEFAULT_FRAME,
           char *name = "myframe")
    :wxFrame(parent,title,x,y,w,h,style,name),
    CMyDisplay(project,type)
    {}
  ~CMyFrame() {}
  
  virtual void UpdateDisplay(int flag)=0;
};

#endif
