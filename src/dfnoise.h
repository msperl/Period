/*
 * Program:     Period98
 *
 * File:        dfnoise.h
 * Purpose:     header-file for
 *              ?
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */
//
//
//
//

#ifndef __noise_h_
#define __noise_h_
#include "dtimestr.h"

///
double NoiseCalc(CTimeString const &time,double zero,
		 double from, double to, double stepping,
		 int UseWeight, DataMode mode,
		 double PercentageStart=0, double PercentageStep=100);

#endif
