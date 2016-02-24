/*
 * Program:     Period98
 *
 * File:        xgsize.h
 * Purpose:     header-file for
 *              ?
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */
//
// here go the relevant Frame-sizes
//

#ifndef __framsiz_h__
#define __framsiz_h__

///
const int fdialog_x=2;
///
const int fdialog_y=30;
///
const int Fourier_ListWidth=200;
///
const int Fourier_ListHeight=40;
// for the screen
#define GRAPHBORDER_X 50
#define GRAPHBORDER_Y 50
// printing Borders
#define GRAPHBORDERS 50

#ifdef wx_motif
///
const int Timestring_ListHeight=300;
///
const int Period_FreqShown=6;
#endif // wx_motif

#ifdef wx_xt
///
const int Timestring_ListHeight=300;
///
const int Period_FreqShown=6;
#endif // wx_xt

#ifdef wx_msw
const int Timestring_ListHeight=200;
const int Period_FreqShown=5;
#endif // wx_msw

#endif // __framsiz_h__











