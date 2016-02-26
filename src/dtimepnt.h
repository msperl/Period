/*
 * Program:     Period98
 *
 * File:        dtimepnt.h
 * Purpose:     header-file for
 *              time-string-data-point
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */

#ifndef __timepnt_h__
#define __timepnt_h__

#include <iostream.h>
#include <math.h>

#define NO_ADJUST -99934.2134234325423

///
class CTimePoint
{
public:
  ///constructor
  CTimePoint(int i=0);
  ///
  CTimePoint(double t, double o,double a=NO_ADJUST, 
	     double c=0,double pw=1.0,
	     int n1=0, int n2=0, int n3=0, int n4=0,int index=0);
  ///destructor
  ~CTimePoint();
  /// returns the running index of the data
  int GetRunningIndex() const { return mRunningIndex; }
  /// returns the time of the data
  double GetTime() const { return mTime; }
  double GetPhasedTime(double Frequency) const
    { double tmp; return modf(mTime*Frequency,&tmp); }
  
  /// returns the observed value of the data
  double GetObserved() const { return mObserved; }
  
  /// returns the adjusted value of the data
  double GetAdjusted() const { return mAdjusted; }
  /// sets the adjusted value of the data 
  void SetAdjusted(double t) { mAdjusted=t; }
  
  /// returns the calculated value for the data
  double GetCalculated() const { return mCalculated; }
  /// sets the calculated value for the data
  void SetCalculated(double t) { mCalculated=t ; }
  
  /// returns the residuals in dependance to the observed values
  double GetDataResidual() const { return mObserved-mCalculated; }
  /// returns the residuals in dependance to the adjusted values
  double GetAdjustedResidual() const { return mAdjusted-mCalculated; }

  /// sets the weight for the data
  void SetWeight(double w) { mWeight=w; }
  /// returns the weight for the data
  double GetWeight() const { return mWeight; }

  /// returns the weight for the data
  double GetPointWeight() const { return mPointWeight; }

  
  /// returns the ID of the name of the data for a column
  int GetIDName(int column) const {return mNames[column];}
  /// sets the ID of the name of the data for a column
  void SetIDName(int column, int ID) { mNames[column]=ID; }
  
  //@ManMemo: data
private:
  ///Time
  double const mTime;
  ///Observed
  double const mObserved;
  ///Adjusted
  double mAdjusted;
  ///Calculated
  double mCalculated;
  ///Weight
  double mWeight;
  ///Weight for point
  double mPointWeight;
  ///Array of names
  int mNames[4];
  /// the index to use for the next file
  static int mNextIndex;
  /// the global running index
  int mRunningIndex;
};

//@ManMemo: compare
int operator<(CTimePoint const &t1, CTimePoint const &t2);
int operator<=(CTimePoint const &t1, CTimePoint const &t2);
//@ManMemo: output
ostream& operator<<(ostream &str, CTimePoint const &t);

#endif
