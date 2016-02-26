/*
 * Program:     Period98
 *
 * File:        dfourier.h
 * Purpose:     header-file for
 *              ?
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */

#ifndef __fourier_h__
#define __fourier_h__

#include <iostream.h>

#include "lists.h"
#include "str.h"

#include "dtimestr.h"
#include "dperipnt.h"
#include "error.h"

///
enum CompactMode { AllData, PeaksOnly};

///
class CFourierPoint
{
  /// Data
private:
  double mFrequency;
  ///
  double mAmplitude;
public:
  /// constructor
  CFourierPoint();
  ///
  CFourierPoint(double f,double a);
  ///
  double GetFrequency() const { return mFrequency; }
  ///
  double GetAmplitude() const { return mAmplitude; }
  ///
  int ReadIn(char *ptr);
};

///
istream &operator>>(istream &str,CFourierPoint &f1);

/// comperator needed for sorted list...
int operator<(CFourierPoint const &f1, CFourierPoint const &f2);
int operator<=(CFourierPoint const &f1, CFourierPoint const &f2);
/// output
ostream &operator<<(ostream &str,CFourierPoint const &f1);

///
class CFourier : public TSortUpList<CFourierPoint>
{
public:
  ///
  enum StepQuality {High=20,Medium=15,Low=10,Custom=0};
  ///
  CFourier(myString title,double from=0,double to=0, 
           StepQuality qual=High,double step=0.1,
	   DataMode Mode=Observed,
	   CompactMode Compact=PeaksOnly,int Weight=0);
  ///
  ~CFourier();
  //
    int GetFourierID() { return mFourierID;}

  ///
  void SetTitle(myString const &title);
  ///
  myString GetTitle() const { return mTitle; }

  ///
  void SetFrom(double from);
  ///
  double GetFrom() const { return mFrom; }
  
  ///
  void SetTo(double from);
  ///
  double GetTo() const { return mTo; }

  ///
  void SetStepping(StepQuality type=High,double step=0.1) 
    { mStepQuality=type; mStepping=step; }
  ///
  double GetStepping() const { return mStepping; }
  ///
  StepQuality GetStepQuality() const { return mStepQuality; }

  ///
  void SetMode(DataMode Mode) { mMode=Mode; }
  ///
  DataMode GetMode() const { return mMode; }

  ///
  void SetCompact(CompactMode Compact) { mCompact=Compact; }
  ///
  CompactMode GetCompact() const { return mCompact; }

  ///
  CPeriPoint const &GetPeak() const { return mPeak; }

  ///
  int GetDone() const { return mDone; }

  /// gives a Point-estimation
  int Points() const
    {
      if (mData==NULL)
	{
	  return TSortUpList<CFourierPoint>::entries();
	}
      else
	{
	  return mPoints;
	}
    }
  ///
  int PointEstimate(double Baseline);
  /// Transformation
  void Transform(CTimeString const &Times, double zero);
  ///
  ostream &WriteHeader(ostream & stream) const;
  ///
  istream &ReadHeader(istream & stream);
  ///
  ostream &WriteData(ostream &stream) const;
  ///
  istream &ReadData(istream &istream);
  ///
  void DeallocateData();
  
  /// add overload
  void add (CFourierPoint const &point);
  ///
  CFourierPoint & operator[] (int i) const
    {
      if (mData==NULL)
	{
	  return TSortUpList<CFourierPoint>::operator[](i);
	}
      else
	{
	  return mData[i];
	}
    }
    
  ///
  void MakeStatic();  
  ///
  void SetUseWeight(int flag) { mUseWeight=flag; }
  ///
  int GetUseWeight() const { return mUseWeight; }
protected:
  ///
  int SetPeak(double const &Fre, double const& Amp, double const &Pha)
  {
    if (Amp>mPeak.GetAmplitude(0))
      {
	mPeak=CPeriPoint(Fre,Amp,Pha);
	// returns !=0 if this is a Peak
	return 1;
      }
    return 0;
  }
  
private:
  ///
  int mFourierID;
  ///
  static int mFourierIDCounter;
  ///
  myString mTitle;
  ///
  double mFrom;
  ///
  double mTo;
  ///
  double mStepping;
  ///
  StepQuality mStepQuality;
  ///
  DataMode mMode;
  ///
  CompactMode mCompact;
  ///
  CPeriPoint mPeak;
  ///
  int mDone;
  ///
  void * mGraphDisplay;
  ///
  void * mDataDisplay;
  ///
  int mUseWeight;
  ///
  int mPoints;
  ///
  CFourierPoint * mData;

};

///
void SetDone(int Done);

#endif


