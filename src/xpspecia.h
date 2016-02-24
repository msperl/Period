/*
 * Program:     Period98
 *
 * File:        xpspecia.h
 * Purpose:     header-file for
 *              ?
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */

#ifndef __special_h__
#define __special_h__

#include <wx.h>
#include "dperipnt.h"

///
int SpecialDialog(wxWindow *Frame);
///
void OnSpecialDialogButton(wxButton& but, wxEvent& event);

///
void OnAmpVarButton(wxButton& cancel, wxEvent& event);

///
int *AmplitudeVariation(wxWindow *Frame,
			int *freqs, int* what, CalcMode *mode);
///
void AmplitudeVariationDisplay(wxWindow *Frame,char* txt);

///
void EpochDialog(wxWindow *Frame);

#endif



