//
// period
// ======
//
// written by: Martin Sperl 1995 (c)
//
/////////////////////////////////////////////////
//
// fourrout.cc
// ===========
//
// Purpose: fast fourier analysis
//
// written by Martin Sperl (c) 1995
//
/////////////////////////////////////////////////


#include "dfourier.h"
#include "dfcalc.h"
#include "dtimestr.h"
#include "dgtimein.h"
#include <math.h>

#define END -2

#define EXECUTE_CODE if (calc.IsPeak(loc,compact)) {add(CFourierPoint(calc[loc].Freq(),calc[loc].Pow()));}


void CFourier::Transform(CTimeString const &time,double zero)
{
  // common variable-definitions
  double PercentageStart=0;
  double PercentageStep=100;
  int UseWeight=GetUseWeight();
  double from=GetFrom();
  double to=GetTo();
  double step=GetStepping();
  CompactMode compact=GetCompact();
  DataMode mode=GetMode();

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
  // make access more efficient
  MakeStatic();
}










