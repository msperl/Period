/*
 * Program:     Period98
 *
 * File:        dperiod.h
 * Purpose:     header-file for
 *              ?
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */

#ifndef __period_h__
#define __period_h__

#include <iostream.h>
#include <math.h>
#include "matrix.h"
#include "dtimestr.h"
#include "lists.h"
#include "str.h"

#include "dperipnt.h"

///
struct derive_data
{
  ///
  double Derived;
  ///
  double Weighted;
  ///
  double Start;
  ///
  double *Result;
  ///
  int freq;
  ///
  Parts what;
  ///
  int ID;
};

///
class CPeriod
{
public:
  ///
  CPeriod(int size=256);
  ///
  ~CPeriod();

  ///
  CPeriPoint &operator[](int i) const;
  ///
  int FindEmpty() const;
  
  ///
  void Add();
  ///
  void Remove();
  ///
  void ResizeLeast(int i);

  ///
  void SetUseWeight(int flag) { mUseWeight=flag; }
  ///
  int GetUseWeight() const { return mUseWeight; }

  /// 
  void SetUseData(DataMode mode) { mUseData=mode; }
  ///
  DataMode GetUseData() const { return mUseData;}
  
  ///
  int GetFrequencies() const { return mFrequencies; }
  ///
  int GetActiveFrequencies() const;
  
  ///
  void SetFrequency(int i, double f)
  {(*this)[i].SetFrequency(f);Recalc();};
  ///
  void SetCompositeString(int i, const char * f)
  {(*this)[i].SetCompositeString(f);Recalc();}
  
  ///
  double GetZeropoint() const { return mZeropoint; }
  ///
  void SetZeropoint(double z) { mZeropoint=z; }
  ///
  double GetResiduals() const { return sqrt(mChisqr); }
  ///
  void SetResiduals(double z) { mChisqr=z*z; }
  ///
  int GetIterations() const { return mIterations; }
  ///
  int MaxIterations() const { return mMaxIterations;}

  //@ManMemo: use what as ID
  int GetUseID() const { return mWhat; }
  ///
  void SetUseID(int i) { mWhat=i; }
  
  //@ManMemo: what parts to calculate
  
  void SelectDefault();
  ///
  void SelectAll();
  ///
  void StoreSelection();
  ///
  void RestoreSelection();
  
  //@ManMemo: Calculation-Functions

  void SetPointers(int i=-1);

  ///
  int Calc(CTimeString &times);
  ///
  int CalcLoop(CTimeString &times);
  ///
  void SetResiduals(CTimeString &times);

  ///
  inline double Predict(double t, int ID)
  {
    double val=GetZeropoint();
    for(int i=0;i<GetFrequencies();i++)
      {
	if (mData[i].GetActive())
	  { val+=mData[i].Calc(t,ID); }
      }
    return val;
  }

  ///
  void SetActiveIndex(); 
  ///
  void CheckActive() 
    { 
      for (int i=0;i<GetFrequencies();i++) 
	{
	  mData[i].CheckActive();
	}
    }

  ///
  double ActivePredict(double t, int ID)
    {
      double val=GetZeropoint();
      for(int i=0;i<mActive;i++)
	{
	  val+=mData[mActiveIndex[i]].Calc(t,ID);
	}
      return val;
    }


  //@ManMemo: Output-Functions

  void CreateArtificialData(myString file,double from,double to,
			    double *step,
			    double leading,
			    int append);
  ///
  void Load(myString const & name);
  ///
  void Save(myString const & name);
  ///
  inline myString GetFileName() const { return mFileName; }
  
  ///
  friend istream& operator>>(istream& s,CPeriod &t);
  ///
  friend ostream& operator<<(ostream& s,const CPeriod &t);
  ///
  ostream &AdditionalOut(ostream &s);
  ///
  ostream &WriteSelected(ostream &s);
  //@ManMemo: Private routines
  
private:
  void DeconvoluteMatrix();
  ///
  void PrepareWorkMatrix();
  ///
  void PrepareDerive();
  ///
  void PrepareMatrices();
  
  ///
  int CountDegreesOfFreedome(int ActiveFreqs);
  ///
  void Recalc();
  ///
  void CheckPointers();
  ///
  void Prepare(CTimeString &times);
  ///
  void Terminate();
  ///
  int CurFit();
  ///
  void Derive(double t, int id);
  ///
  double ChiSqr(CTimeString & tmp);
  ///
  void Adapt();

private:
  //@ManMemo: variable Definition  
  CMatrix mBaseMatrix;
  ///
  CMatrix mWorkMatrix;
  ///
  derive_data *mWork;
  ///
  int mVariables;
  ///
  int mIterations;
  ///
  double mFlamda;
  ///
  double mFlamdadef;
  ///
  double mZeropoint;
  ///
  double mChisqr;
  ///
  CTimeString *mTimeString;
  ///
  CPeriPoint *mData;
  ///
  int mFrequencies;
  ///
  int mMaximumFrequencies;
  ///
  int mMaxIterations;
  ///
  int mActive;
  ///
  int *mActiveIndex;

  ///
  myString mFileName;

  ///
  int mUseWeight;
  ///
  DataMode mUseData;
  ///
  int mWhat;
  
};

#endif







