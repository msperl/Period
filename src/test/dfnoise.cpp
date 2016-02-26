/*
 * Program:     Period98
 *
 * File:        dfnoise.cpp
 * Purpose:     implementation-file for
 *              ?
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */

#include "dfcalc.h"
#include "dfnoise.h"
#include "dfourier.h"
#include "dtimestr.h"
#include "dgtimein.h"
#include <math.h>

#define END -2

#define EXECUTE_CODE 	   points++;Sum+=calc[loc].Pow();


double NoiseCalc(CTimeString const &time,double zero,
		 double from, double to, double step,
		 int UseWeight, DataMode mode,
		 double PercentageStart, double PercentageStep)
{

  if (from<0) {from=0;}
  if (to<from) {to=from+1;}

  double Sum=0;
  int points=0;

#define HEADER
#include "dfroutin.h"
  
  if (mode==SpectralWindow)
    {
      if (UseWeight)
	{
#define SPECTRALWINDOW
#define USEWEIGHTS
#define MAINCODE
#undef  HEADER
#include "dfroutin.h"
	}
      else
	{
#define SPECTRALWINDOW
#undef  USEWEIGHTS
#define MAINCODE
#undef  HEADER
#include "dfroutin.h"
	}
    }
  else
    {
      if (UseWeight)
	{
#undef  SPECTRALWINDOW
#define USEWEIGHTS
#define MAINCODE
#undef  HEADER
#include "dfroutin.h"
	}
      else
	{
#undef  SPECTRALWINDOW
#undef  USEWEIGHTS
#define MAINCODE
#undef  HEADER
#include "dfroutin.h"
	}
    }
  return Sum/(points);
}













