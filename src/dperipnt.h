/*
 * Program:     Period98
 *
 * File:        dperipnt.h
 * Purpose:     header-file for
 *              peripoint-data-structure
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */
#ifndef __peripoint_h__
#define __peripoint_h__

#include <iostream.h>
#include <math.h>
#include <stdio.h>
#include "dtimestr.h"
#include "str.h"
#include "error.h"

///
enum Parts {Fre,Amp,Pha};
///
enum CalcMode {NoVar,AllVar,AmpVar,PhaseVar};

class CSimplePeriPoint;
class CAmpVarPeriPoint;
class CFreqCompPeriPoint;

// if the following is active there may still be problems with 
// Amplitude-variations and Frequency-compositions
//typedef CAmpVarPeriPoint CPeriPoint;
typedef CFreqCompPeriPoint CPeriPoint;

#define ZEROFREQ 0
#define ZEROAMP  0
#define ZEROPHASE 0

///
class CSimplePeriPoint
{
public:
  ///
  CSimplePeriPoint(double f=ZEROFREQ, double a=ZEROAMP, double p=ZEROPHASE)
    :mNumber(0),mFrequency(f),mPhase(p),
     mSelection(0),mActive(1)
  { SetAmplitude(a,0); }

  ///
  CSimplePeriPoint & operator =(CSimplePeriPoint const & tmp);
  
  ///
  void SetNumber(int i) { mNumber=i; }
  ///
  int GetNumber() const { return mNumber; }

  ///
  void SetFrequency(double f) { mFrequency=f; mActive=1; }
  ///
  virtual double GetFrequency() const { return mFrequency; }
  
  ///
  void SetAmplitude(double a,int) { mAmplitude=(a<0.0)?-a:a; }
  ///
  virtual double GetAmplitude(int) const { return mAmplitude; }

  ///
  void SetPhase(double p,int) { mPhase=fmod(p,1); }
  ///
  virtual double GetPhase(int) const { return mPhase; }

  //@ManMemo: is this Frequency allready in use
  
  virtual int Empty() const;
  ///
  virtual void GetDepending(int,int *f) { *f=-1; };
  ///
  virtual void SetDepending(int,int, CSimplePeriPoint*) {;}

  //@ManMemo: is for Amplitude Variations
  
  virtual CalcMode GetAmpVariation() const { return NoVar; }
  ///
  virtual void SetAmpVar(CalcMode) {/* nothing to be done */}

  ///
  virtual void CreateAmplitudeVariationData(int) { /* nothing to be done */ }
  
  ///
  virtual int GetDegreesOfFreedome(int)
  { return
      (IsSelected(Fre)?1:0)
      +(IsSelected(Amp)?1:0)
      +(IsSelected(Pha)?1:0);
  }
  
  //@ManMemo: is this Frequency a Composition
  
  virtual int IsComposition() const { return 0; }
  ///
  virtual const char * GetCompositeString() const { return ""; }
  ///
  virtual void SetCompositeString(const char *) { /* nothing to be done */ }
  ///
  virtual void CleanComposition() { /* nothing to be done */ }
  
  //@ManMemo: Stuff to do with selections
  
  virtual void SetSelection(Parts p)
  { mSelection|=1<<p; }
  ///
  void UnsetSelection(Parts p)
  { mSelection&=~(1<<p); }
  ///
  int IsSelected(Parts p) const {return (mActive)?(mSelection&(1<<p)):0;}

  ///
  void StoreSelection()
  { int tmp=mSelection&7;mSelection=tmp|(tmp<<3); }
  ///
  void RestoreSelection()
  { int tmp=(mSelection>>3)&7;mSelection=tmp|(tmp<<3); }

  ///Activate/deactivate Frequency
  void SetActiveNoCheck(int i) 
    { mActive=i;}

  void SetActive(int i) 
    { 
      SetActiveNoCheck(i);
      if (mFrequency==0.0) 
	{ SetActiveNoCheck(0); }
    }
  ///Is this Frequency activated
  int GetActive() const { return mActive; }

  //@ManMemo: For Calculations
  void RecalcFrequencies() {/*nothing to be done here*/}
  ///
  double *GetFrequencyRef() { return &mFrequency; }
  ///
  double *GetAmplitudeRef(int) { return &mAmplitude; }
  ///
  double *GetPhaseRef(int) { return &mPhase; }

  ///
  double Calc(double t, int ID) const { return GetAmplitude(ID)*sin(PhaseCalc(t,ID)); }
  ///
  double PhaseCalc(double t,int ID) const
  {
    double pha=t*GetFrequency()+GetPhase(ID);
    double tmp;
    return MY2PI*modf(pha,&tmp);
  }

  ///
  double CalcEpoch(double t, int Intensity) const;

  ///
  double Derive(Parts what, double t, int ID) const
  {
    if (mActive)
      {
	switch(what)
	  {
	  case Fre:
	    return GetAmplitude(ID)*cos(PhaseCalc(t,ID))*MY2PI*t;
	  case Amp:
	    return (sin(PhaseCalc(t,ID)));
	  case Pha:
	    return (GetAmplitude(ID)*MY2PI*cos(PhaseCalc(t,ID)));
	  }
      }
    return 0;
  }

  ///
  void Adapt();
  
  //@ManMemo: Input/Output

  virtual ostream& WriteOut(ostream &s, int Selected) const;
  ///
  istream& ReadIn(istream &s)
  {
    char buffer[1024];
    s.getline(buffer,1023);
    if (ReadIn(buffer))
      {
	// there has been an error, so let us set a flag
	s.clear(ios::badbit);
      }
    return s;
  }
  ///
  virtual int ReadIn(char* ptr);

protected:
  ///
  int mNumber;
  ///
  double mFrequency;
  ///
  double mAmplitude;
  ///
  double mPhase;
  ///
  int mSelection;
  ///
  int mActive;


};

///
class CAmpVarPeriPoint : public CSimplePeriPoint
{
public:
  ///
  CAmpVarPeriPoint(double f=ZEROFREQ, double a=ZEROAMP, double p=ZEROPHASE)
    :CSimplePeriPoint(f,a,p),mIsAmpVar(NoVar),mPhaseVar(0),mAmpVar(0),mMaxID(0)
  {}

  ///
  virtual ~CAmpVarPeriPoint()
  { SetAmpVar(NoVar); }
  
  ///
  virtual CAmpVarPeriPoint & operator =(CAmpVarPeriPoint const & tmp);    

  ///
  void SetAmplitude(double a,int ID)
  {
    a=(a==0.0)?ZEROAMP:a;
    // have we really got Amplitude variations ?
    if (mMaxID<=ID)
      { ID= -1; }
    // Check which one to set
    if (ID<0)
      {
	mAmplitude=a;
      }
    else
      {
	mAmpVar[ID]=a;
      }
  }
  
  ///
  double GetAmplitude(int ID) const
  {
    if (mMaxID<=ID)
      { ID= -1; }
    // Check which one to set
    if (ID<0)
      { return mAmplitude; }
    return mAmpVar[ID];
  }

  ///
  void SetPhase(double p,int ID)
  {
    p=fmod(p,1);
    // have we really got Amplitude variations ?
    if (mMaxID<=ID)
      { ID= -1; }
    // Check which one to set
    if (ID<0)
      { mPhase=p; }
    else
      { mPhaseVar[ID]=p; }
  }
  
  ///
  double GetPhase(int ID) const
  {
    if (mMaxID<=ID)
      { ID= -1; }
    // Check which one to set
    if (ID<0)
      { return mPhase; }
    return mPhaseVar[ID];
  }  

  //@ManMemo: is for Amplitude Variations

  CalcMode GetAmpVariation() const { return mIsAmpVar; }
  ///
  void SetAmpVar(CalcMode i);

  ///
  void CreateAmplitudeVariationData(int MaxID);
  
  ///
  virtual int GetDegreesOfFreedome(int names)
  {
    switch (mIsAmpVar)
      {
      case NoVar:
	return (IsSelected(Fre)?1:0)
	      +(IsSelected(Amp)?1:0)
	      +(IsSelected(Pha)?1:0);
      case AllVar:
	return (IsSelected(Fre)?1:0)
	      +(IsSelected(Amp)?names:0)
	      +(IsSelected(Pha)?names:0);
      case AmpVar:
	return (IsSelected(Fre)?1:0)
	      +(IsSelected(Amp)?names:0)
 	      +(IsSelected(Pha)?1:0);
      case PhaseVar:
	return (IsSelected(Fre)?1:0)
	      +(IsSelected(Amp)?1:0)
	      +(IsSelected(Pha)?names:0);
      }
    return 0;
  }

  ///
  double *GetAmplitudeRef(int ID)
  {
    if (mMaxID<=ID)
      { ID= -1; }
    // Check which one to set
    if (ID<0)
      { return &mAmplitude; }
    return &mAmpVar[ID];
  }
  
  ///
  double *GetPhaseRef(int ID)
  {
    if (mMaxID<=ID)
      { ID= -1; }
    // Check which one to set
    if (ID<0)
      { return &mPhase; }
    return &mPhaseVar[ID];
  }  

  ///
  double Calc(double t, int ID) const
  {
    switch (GetAmpVariation())
      {
      case NoVar:
	return CSimplePeriPoint::GetAmplitude(ID)
	  *sin(CSimplePeriPoint::PhaseCalc(t,ID));
      case AllVar:
	return GetAmplitude(ID)*sin(PhaseCalc(t,ID));
      case AmpVar:
	return GetAmplitude(ID)*sin(PhaseCalc(t,-1));
      case PhaseVar:
	return GetAmplitude(-1)*sin(PhaseCalc(t,ID));
      }
    return 0;
  }
  
  ///
  double PhaseCalc(double t,int ID) const
  {
    double pha=t*GetFrequency()+GetPhase(ID);
    double tmp;
    return MY2PI*modf(pha,&tmp);
  }

  ///
  double Derive(Parts what, double t, int ID) const
  {
    if (mActive)
      {
	switch(what)
	  {
	  case Fre:
	    return GetAmplitude(ID)*cos(PhaseCalc(t,ID))*MY2PI*t;
	  case Amp:
	    if ((mIsAmpVar==AllVar)||(mIsAmpVar==AmpVar))
	      {
		return (sin(PhaseCalc(t,ID)));
	      }
	    else
	      {
		return (sin(PhaseCalc(t,-1)));
	      }
	  case Pha:
	    switch (mIsAmpVar)
	      {
	      case NoVar:
		return (GetAmplitude(-1)*MY2PI*cos(PhaseCalc(t,-1)));
	      case AllVar:
		return (GetAmplitude(ID)*MY2PI*cos(PhaseCalc(t,ID)));
	      case AmpVar:
		return (GetAmplitude(ID)*MY2PI*cos(PhaseCalc(t,-1)));
	      case PhaseVar:
		return (GetAmplitude(-1)*MY2PI*cos(PhaseCalc(t,ID)));
	      }
	    break;
	  }
      }
    return 0;
  }
  
protected:
  ///
  CalcMode mIsAmpVar;

  //@ManMemo: normal data
  
  double *mPhaseVar;
  ///
  double *mAmpVar;
  ///
  int mMaxID;
};

//
// Frequency Composition
//@ManMemo:

class FreqCompo
{
public:
  ///
  FreqCompo()
    : Freq(-1),Factor(0),pnt(0)
    {}
  ///
  double GetOtherFactor() { return Factor; }
  ///
  void SetOtherFactor(double fac) { Factor=fac; }
  ///
  int GetOtherFrequency() { return Freq; }
  ///
  void SetOtherFrequency(int freq) { Freq=freq; pnt=0; } 
  //@ManMemo: should only be called from CleanComposition
  void SetPointer(int fr, CFreqCompPeriPoint* pt);
  ///
  void GetPointer(int *fr, CFreqCompPeriPoint **pt) {*fr=Freq; *pt=pnt;};
  ///
  double GetFrequency() 
    {
      if (pnt!=0)
	{
	  if (Freq!=((CAmpVarPeriPoint*)pnt)->GetNumber())
	    {
	      MYERROR("something about the pointers is wrong !!!\n"
		    "Should be:"<<Freq<<" is:"<<
		    ((CAmpVarPeriPoint*)pnt)->GetNumber());
	    }
	  return ((CAmpVarPeriPoint*)pnt)->GetFrequency()*Factor;
	}
      return 0;
    }

  ///
  const char *GetCompositionString() const;
protected:  
  ///
  int Freq;
  ///
  double Factor;
  ///
  CFreqCompPeriPoint *pnt;
};

///
class CFreqCompPeriPoint : public CAmpVarPeriPoint
{
public:
  ///
  CFreqCompPeriPoint(double f=ZEROFREQ, double a=ZEROAMP, double p=ZEROPHASE)
    :CAmpVarPeriPoint(f,a,p),IsCompo(0)
  {}

  ///
  virtual ~CFreqCompPeriPoint() { CleanComposition(); }

  ///
  void RecalcFrequencies() 
    { if (IsComposition())
      { mFrequency=F1.GetFrequency()+F2.GetFrequency();}
    }

  ///
  void SetFrequency(double freq) 
    { 
      CleanComposition(); 
      CAmpVarPeriPoint::SetFrequency(freq); 
    }

  ///
  int IsComposition() const { return IsCompo; }
  ///
  void CleanComposition();
  ///
  const char * GetCompositeString() const;
  ///
  void SetCompositeString(const char * f);

  ///
  int FindDependance(int freq);
  ///
  int AddDependency(double factor, int freq, CFreqCompPeriPoint * ptr);
  ///
  void RemoveDependency(int freq);
  ///
  void CheckActive();

  ///
  void SetSelection(Parts p)
    {
      if (IsComposition() && (p==Fre))
	{ return; /* cannot select composit frequency */ }
      CAmpVarPeriPoint::SetSelection(p);
    }

  ///
  double * GetFrequencyRef()
    {
      if (IsComposition())
	{ 
	  MYERROR("Tried to get a reference on a composition !!!");
	  return NULL;
	}
      return CAmpVarPeriPoint::GetFrequencyRef();
    }

  ///
  virtual void GetDepending(int what,int *f);
  ///
  virtual void SetDepending(int what,int f, CSimplePeriPoint* tmp);

  ///
  double Derive(Parts what, double t, int ID) const
    {
      if (mActive)
	{
	  switch(what)
	    {
	    case Fre:
	      {
		double der=GetAmplitude(ID)*cos(PhaseCalc(t,ID))*MY2PI*t;
		for (int i=0;i<Depending.entries();i++)
		  {
		    /* do this for every dependant frequency */
		    CFreqCompPeriPoint * tmp;
		    int f;
		    Depending[i].GetPointer(&f,&tmp);
		    if (tmp->GetActive())
		      {
			// do this only if other frequency is active
			der+=Depending[i].GetOtherFactor()
			  *tmp->Derive(Fre,t,ID);
		      }
		  }
		return der;
	      }
	    case Amp:
	      return (sin(PhaseCalc(t,ID)));
	    case Pha:
	      return (GetAmplitude(ID)*MY2PI*cos(PhaseCalc(t,ID)));
	    }
	}
      return 0;
    }

  //@ManMemo: Input/Output

  virtual ostream& WriteOut(ostream &s, int Selected) const;
  ///
  virtual int ReadIn(char *ptr);

protected:
  ///
  int IsCompo;
  ///
  FreqCompo F1;
  ///
  FreqCompo F2;

  ///
  TList<FreqCompo> Depending;
};

//@ManMemo: General I/O
inline ostream& operator<<(ostream& s,const CPeriPoint &t)
{ return t.WriteOut(s,0); }
///
inline istream& operator>>(istream& s,CSimplePeriPoint &t)
{ return t.ReadIn(s); }


#endif














