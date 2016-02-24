/*
 * Program:     Period98
 *
 * File:        xgdial.h
 * Purpose:     header-file for
 *              ?
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */

#ifndef __mydial_h__
#define __mydial_h__

#include <wx.h>

class myDialogBox : public wxDialogBox
{
 public:
  myDialogBox(void)
    :wxDialogBox()
    {}

  myDialogBox(wxWindow *parent, Const char *title, Bool modal=FALSE,
	      int x=-1, int y=-1, int width=500, int height=500,
	      long style=wxDEFAULT_DIALOG_STYLE, 
	      Constdata char *name=wxDialogNameStr)
    :wxDialogBox(parent,title,modal,
	      x, y, width, height,
	      style,name)
    {}

  Bool OnClose(void);

};

#endif /*__mydial_h__*/
