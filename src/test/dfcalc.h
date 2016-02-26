/*
 * Program:     Period98
 *
 * File:        dfcalc.h
 * Purpose:     header-file for
 *              ?
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */
#ifndef __fourcalc__
#define __fourcalc__

#include "complex.h"

#include "dfourier.h"
#include "constant.h"

///
class CFourCalcData
{
private:
  ///
  double mFreq;
  ///
  mycomplex mComp;
  ///
  double mPow;
  ///
  double mPhase;
public:
  ///
  CFourCalcData(double F=-1,mycomplex c=0):mFreq(F),mComp(c),mPow(-1) {;}
  ///
  double &Freq() {return mFreq;}
  ///
  mycomplex &Comp() {return mComp;}
  ///
  double Pow() const {return mPow;}
  ///
  double Phase() const { return mPhase; }
  ///
  void CalcPow()
  {
    mPow=norm(mComp); 
    mPhase=atan2(mComp.real(),mComp.imag())/(MY2PI);
    if (mPhase<0)
      { mPhase+=1.0; }
  }
};

///
class CFourCalc
{
private:
  ///
  CFourCalcData * Array;
  ///
  int wind;
public:
  ///
  CFourCalc(int size);
  ///
  ~CFourCalc();
  ///
  int IsPeak(int i, CompactMode mode)
  {
    CFourCalcData *tmp=&Array[i];
    
    if (tmp->Freq()<0)
      return 0;
    if (mode==AllData)
      return 1;

    // first one is a peak
    if (i==0)
      { return 1; }
    if (i==wind-1)
      { return 1; }
    
    double t_m=tmp[-1].Pow();
    double t_0=tmp->Pow();
    double t_p=tmp[+1].Pow();
    return (
	    ( (t_m<t_0) && (t_0>=t_p) )
	    ||
	    ( (t_m>t_0) && (t_0<=t_p) )
	    );
  }
  ///
  inline CFourCalcData &operator[](int i) const
  {
    return Array[i];
  }
  ///
  void Power();
};

#endif








