/*
 * Program:     Period98
 *
 * File:        xgeneral.h
 * Purpose:     header-file for
 *              general stuff lik Button Handler
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */

#ifndef __general_h_
#define __general_h_

#include <wx.h>

#define GENERAL_BUTTON_HANDLER (wxFunction)&myGeneralButtonHandler
void myGeneralButtonHandler(wxButton& but,wxEvent &);

#define GENERAL_BUTTON_HANDLER_NOHIDE (wxFunction)&myGeneralButtonHandlerNoHide
void myGeneralButtonHandlerNoHide(wxButton& but,wxEvent &);

void FitListBox(wxListBox* listbox);

#endif

