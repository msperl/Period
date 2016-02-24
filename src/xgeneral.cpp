/*
 * Program:     Period98
 *
 * File:        xgprojec.cpp
 * Purpose:     implementation-file for
 *              general stuff
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */

#include "xgeneral.h"

void myGeneralButtonHandler(wxButton& but,wxEvent &)
{
  but.GetParent()->Show(FALSE);
  but.SetClientData((char*)1);
}

void myGeneralButtonHandlerNoHide(wxButton& but,wxEvent &)
{
  but.SetClientData((char*)1);
}

void FitListBox(wxListBox* listbox)
{
    float w,h,wmax,hmax;
    wmax=-1.0f;
    hmax=0.0f;
    for (int i=0;i<listbox->Number();++i)
      {
          listbox->GetTextExtent(listbox->GetString(i),&w,&h);
          if (w>wmax) { wmax=w; }
      }
    if (wmax!=-1)
      {
#ifdef wx_msw
		wmax=wmax*3.0f/2.0f;
#endif
        // 40 is to cope with the optional Scrollbar?
        // I have no idea how to check the real size out
        // Markus?
		int wl,hl;
		listbox->GetSize(&wl,&hl);
        listbox->SetSize(-1,-1,40+(int)wmax,hl,wxSIZE_USE_EXISTING);
      }
}
