/*
 * Program:     Period98
 *
 * File:        dperipnt.cpp
 * Purpose:     implementation-file for
 *              peripoint-data-structure
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */
#include "dperipnt.h"
#include <string.h>
#include <ctype.h>

CSimplePeriPoint & CSimplePeriPoint::operator =(CSimplePeriPoint const & tmp)
{
  // Do not copy mNumber
  mFrequency=tmp.mFrequency;
  mAmplitude=tmp.mAmplitude;
  mPhase=tmp.mPhase;
  mSelection=tmp.mSelection;
  mActive=tmp.mActive;
  return * this;
}

int CSimplePeriPoint::Empty() const
{
  if (GetActive())
    return 0;
  if (GetFrequency()!=0.0)
    return 0;
  if (GetAmplitude(-1)!=0.0)
    return 0;
  return 1;
}

double CSimplePeriPoint::CalcEpoch(double t,int Intensity) const
{
  double tmp=PhaseCalc(t,-1)/MY2PI;
  if (Intensity)
    { tmp+= 0.25; }
  else
    { tmp-= 0.25; }
  double epoch=t-tmp/GetFrequency();
  return epoch;
}


void CSimplePeriPoint::Adapt()
{
  // is Amplitude Negative
  if (mAmplitude<0)
    {
      // has the phase been fixed?
      if (IsSelected(Pha)&&IsSelected(Amp))
	{
	  mAmplitude=-mAmplitude;
	  mPhase+=.5;
	}
    }
  // shift zeropoint...
  mPhase-=(long)mPhase;
  if (mPhase<0)
    {
      mPhase+=1;
    }
}

ostream& CSimplePeriPoint::WriteOut(ostream &s, int Selected) const
{
  s<<"F"<<mNumber+1<<"\t";
  int oldprec=s.precision(15);
  if (!GetActive()) { s<<"("; }
  // Frequency
  if (Selected && IsSelected(Fre))
    {s<<'*'<<GetFrequency()<<'*'; }
  else
    {s<<' '<<GetFrequency()<<' '; }
  // Amplitude
  s.precision(15);
  if (Selected && IsSelected(Amp))
    {s<<"\t*"<<GetAmplitude(0)<<'*'; }
  else
    {s<<"\t "<<GetAmplitude(0)<<' '; }

  // Phase
  if (Selected && IsSelected(Pha))
    {s<<"\t*"<<GetPhase(0)<<'*'; }
  else
    {s<<"\t "<<GetPhase(0)<<' '; }
  // Composition
  if (IsComposition())
    {
      s<<"\t"<<GetCompositeString();
    }
  
  if (!GetActive()) { s<<")"; }
  s.precision(oldprec);
  return s;
}

int CSimplePeriPoint::ReadIn(char *ptr)
{
  double tmpf=0,tmpa=0.006,tmpp=0;
  int active=1;
  // check if starting with a "("
  char c=0;
  int read=sscanf(ptr," %c",&c);
  // check and read in parameters
  if (c=='(')
    {
      active=0;
      read = sscanf(ptr," %c%lf%lf%lf",&c,&tmpf,&tmpa,&tmpp);
      read--;
    }
  else
    {
      active=1;
      read = sscanf(ptr,"%lf%lf%lf",&tmpf, &tmpa, &tmpp);
    }
  
  // Frequency
  if (read<1) { SetActive(0); return 1;}
  SetFrequency(tmpf);

  // Amplitude
  if (read<2) { return 0;}
  SetAmplitude(tmpa,0);
  
  // Phase
  if (read<3) {return 0;}
  SetPhase(tmpp,0);

  // set active
  SetActive(active);

  // exit function
  return 0;
}

////////////////////////////////////////////////////////////////
// CAmpVarPeriPoint
////////////////////////////////////////////////////////////////

void CAmpVarPeriPoint::SetAmpVar(CalcMode i)
{
  mIsAmpVar=i;
  delete [] mAmpVar;
  delete [] mPhaseVar;
  mAmpVar=0;
  mPhaseVar=0;
  mMaxID=0;
}

void CAmpVarPeriPoint::CreateAmplitudeVariationData(int MaxID)
{
  delete [] mAmpVar;
  delete [] mPhaseVar;
  mAmpVar=0;
  mPhaseVar=0;
  mMaxID=0;

  if (mIsAmpVar!=0)
    {
      mMaxID=MaxID+1;
      // this is some code to check for out of bounds...
      mAmpVar= new double [mMaxID];
      mPhaseVar= new double [mMaxID];
      // fill up the arrays
      for (int i=0;i<mMaxID;i++)
	{
	  mAmpVar[i]=mAmplitude;
	  mPhaseVar[i]=mPhase;
	}
    }
}

CAmpVarPeriPoint & CAmpVarPeriPoint::operator =(CAmpVarPeriPoint const & tmp)
{
  SetAmpVar(NoVar);
  CSimplePeriPoint::operator=(tmp);
  return *this;
}

////////////////////////////////////////////////////////////////
// CAmpVarPeriPoint
////////////////////////////////////////////////////////////////

const char * FreqCompo::GetCompositionString() const
{
  if ((Freq==-1)||(Factor==0))
    {
      return "";
    }
  // OK return the composition
  static char tmp[256];
  if (Factor==1)
    {
      // no sign and factor needed
      sprintf(tmp,"f%i",Freq+1);
    }
  else
    {
      if (Factor==-1)
	{
	  // just the minus sign needed
	  sprintf(tmp,"-f%i",Freq+1);
	}
      else
	{
	  // everything needed
	  sprintf(tmp,"%gf%i",Factor,Freq+1);
	}
    }
  // return the string
  return tmp;
}

void FreqCompo::SetPointer(int freq, CFreqCompPeriPoint * ptr)
{
  if (Freq!=freq)
    {
      MYERROR("There is something wrong with Setting a dependency pointer!!!");
    }
  pnt=ptr;
  // maybe we should register
}

void CFreqCompPeriPoint::SetCompositeString(const char * f)
{
  // no composition
  CleanComposition();
  // now set it
  
  // strip spaces
  char tmp[256];
  size_t i=0,j=0;
  for (i=0;i<strlen(f);i++)
    {
      if (!isspace(f[i]))
	{
	  tmp[j]=f[i];
	  j++;
	}
    }
  tmp[j]=0;
  // Now is this a composition?
  if (tmp[0]=='=')
    {
      // Yes it seems to be...
      //is next character a nummeric?
      int pos=1;
      double fac=1;
      int freq=-1;
      char num[256];
      char c=tmp[pos];
      if (isdigit(c)||(c=='+')||(c=='-'))
	{
	  fac=0;
	  // lets get the number
	  int i=0;
	  if ((c=='+')||(c=='-'))
	    {
	      // copy the sign
	      num[i++]=c;
	      pos++;
	    }
	  while (isdigit(tmp[pos])||(tmp[pos]=='.'))
	    {
	      // copy character
	      num[i++]=tmp[pos++];
	    }
	  num[i]=0;
	  // now get the factor
	  sscanf(num,"%lf",&fac);
	}
      if (tmp[pos]=='*') { pos++; /* ignore the factor sign*/}
      if ((tmp[pos]!='f')&&(tmp[pos]!='F'))
	{
	  // OK, there is no composition, 
	  // let's deactivate us 
	  // and set Frequency to 0
	  SetActive(0);
	  SetFrequency(0);
	  // and return
	  return;
	}
      // now get Frequency number !!!
      i=0;
      pos++;
      while (isdigit(tmp[pos]))
	{
	  // copy character
	  num[i++]=tmp[pos++];
	}
      num[i]=0;
      // now get the frequency
      sscanf(num,"%i",&freq);
      // to make it 0-based
      freq--;      
      // check if it is correct
      if ((freq==-1)||(fac==0)||(freq==mNumber))
	{
      	  // OK, there is only a bad composition, 
	  // let's deactivate us 
	  // and set Frequency to 0
	  SetActive(0);
	  SetFrequency(0);
	  // and return
	  return;
	}

      F1.SetOtherFactor(fac);
      F1.SetOtherFrequency(freq);
      // OK, we are a Composition
      IsCompo=1;

      //
      // now try to test for the second part
      //

      c=tmp[pos];
      if ((c=='+')||(c=='-'))
	{
	  if ((tmp[pos+1]=='f')||(tmp[pos+1]=='F'))
	    {
	      if (c=='-')
		{
		  fac=-1;
		}
	      else
		{
		  fac=1;
		}
	      pos++;
	    }
	  else
	    {
	      fac=0;
	      // lets get the number
	      int i=0;
	      // copy the sign
	      num[i++]=c;
	      pos++;
	      while (isdigit(tmp[pos])||(tmp[pos]=='.'))
		{
		  // copy character
		  num[i++]=tmp[pos++];
		}
	      num[i]=0;
	      // now get the factor
	      sscanf(num,"%lf",&fac);
	      if (tmp[pos]=='*') { pos++; /* ignore the factor sign*/}
	    }
	  if ((tmp[pos]!='f')&&(tmp[pos]!='F'))
	    {
	      // OK, no 2nd field
	      if (F1.GetOtherFactor()==1)
		{
		  // we do not want to similar frequencies !!!
		  CleanComposition();
		  SetActive(0);
		  SetFrequency(0);
		}
	      return;
	    }

	  // now get Frequency number !!!
	  i=0;
	  pos++;
	  while (isdigit(tmp[pos]))
	    {
	      // copy character
	      num[i++]=tmp[pos++];
	    }
	  num[i]=0;
	  // now get the frequency
	  sscanf(num,"%i",&freq);
	  // to make it 0-based
	  freq--;      
	  if ((freq==-1)||(fac==0))
	    {
	      // OK, no second field
	      if (F1.GetOtherFactor()==1)
		{
		  // we do not want to similar frequencies !!!
		  CleanComposition();
		  SetActive(0);
		  SetFrequency(0);
		}
	      return;
	    }
	  // check if it is correct
	  if (freq==mNumber)
	    {
	      // OK, there is only a bad composition, 
	      // let's deactivate us 
	      // and set Frequency to 0
	      CleanComposition();
	      SetActive(0);
	      SetFrequency(0);
	      // and return
	      return;
	    }

	  // OK, now do sanity-check  - we do not want af1+bf2
	  if (freq==F1.GetOtherFrequency())
	    {
	      fac=fac+F1.GetOtherFactor();
	      if (fac==0)
		{
		  CleanComposition();
		  SetActive(0);
		  SetFrequency(0);
		}
	      else
		{
		  F1.SetOtherFactor(fac);
		}
	    }
	  else
	    {
	      F2.SetOtherFactor(fac);
	      F2.SetOtherFrequency(freq);
	    }
	}
      else
	{
	  // Only one frequency ...
	  if (F1.GetOtherFactor()==1)
	    {
	      // we do not want two similar frequencies !!!
	      CleanComposition();
	      SetActive(0);
	      SetFrequency(0);
	    }
	}
    }
  else
    {
      // No it is just a number...
      //CAmpVarPeriPoint::SetFrequencyString(f);
    }
}

void CFreqCompPeriPoint::CleanComposition()
{
  // there is no composition any longer !!!
  IsCompo=0;
  // deallocate depending frequencies
  int freq;
  CFreqCompPeriPoint * ptr;
  // start with first
  F1.GetPointer(&freq,&ptr);
  if (ptr!=0)
    { ptr->RemoveDependency(mNumber); } // deregister
  // now do the second
  F2.GetPointer(&freq,&ptr);
  if (ptr!=0)
    { ptr->RemoveDependency(mNumber); } // deregister
  // now clean pointers
  F1.SetOtherFrequency(-1);
  F2.SetOtherFrequency(-1);
}

const char * CFreqCompPeriPoint::GetCompositeString() const 
{
  static char result[256];
  //
  if (!IsComposition())
    {
      return "";
    }
  // now get the string
  char f1[256],f2[256];
  strcpy(f1,F1.GetCompositionString());
  strcpy(f2,F2.GetCompositionString());
  if ((f2[0]!='-') && (f2[0]!=0))
    {
      sprintf(result,"=%s+%s",f1,f2);
    }
  else
    {
      sprintf(result,"=%s%s",f1,f2);
    }  
  return result;
}

int CFreqCompPeriPoint::FindDependance(int freq)
{
  for (int i=0;i<Depending.entries();i++)
    {
      if (Depending[i].GetOtherFrequency()==freq)
	{ return i; }
    }
  return -1;
}

int CFreqCompPeriPoint::AddDependency(double factor, int freq, CFreqCompPeriPoint * ptr)
{
  if (ptr==0)
    {
      MYERROREXIT("NULL-pointer given to AddDependency!!!");
      return -1;
    }
  if (IsComposition())
    {
      // if we ourself are a dependancy don't do it !!!
      return -1;
    }
  int found=FindDependance(freq);
  if (found==-1)
    {
      // not found so add one
      FreqCompo tmp;
      tmp.SetOtherFrequency(freq);
      tmp.SetOtherFactor(factor);
      tmp.SetPointer(freq,ptr);
      Depending.addhead(tmp);
    }
  else
    {
      // just change the settings !!!
      Depending[found].SetOtherFactor(factor);
      Depending[found].SetPointer(freq,ptr);
    }
  return 0;
}

void CFreqCompPeriPoint::RemoveDependency(int freq)
{
  int found=FindDependance(freq);
  if (found!=-1)
    {
      // OK we found it, so remove it
      Depending.removeat(found);
    }
  else
    {
      // give a warning !!!
      MYERROR("A dependance for Frequency "<<freq<<" was not found !!!");
    }
}

void CFreqCompPeriPoint::GetDepending(int what,int *f)
{
  *f=-1;
  if (IsComposition())
    {
      if (what==0)
	{
	  *f=F1.GetOtherFrequency();
	}
      if (what==1)
	{
	  *f=F2.GetOtherFrequency();
	}
    }
}

void CFreqCompPeriPoint::SetDepending(int what,int f, CSimplePeriPoint* tmp)
{
  if (IsComposition())
    {
      if (what==0)
	{
	  // register Pointer
	  F1.SetPointer(f,(CFreqCompPeriPoint*)tmp);
	  // infor other class
	  ((CFreqCompPeriPoint*)tmp)->AddDependency(F1.GetOtherFactor(),
						    GetNumber(),
						    this);
	}
      if (what==1)
	{
	  // register Pointer
	  F2.SetPointer(f,(CFreqCompPeriPoint*)tmp);
	  // inform other class
	  ((CFreqCompPeriPoint*)tmp)->AddDependency(F1.GetOtherFactor(),
						    GetNumber(),
						    this);
 	}
    }
}

void CFreqCompPeriPoint::CheckActive()
{
  CFreqCompPeriPoint *tmp;
  int freq;
  // now deactivate us if composing one is not active
  F1.GetPointer(&freq,&tmp);
  if (tmp){if (!(tmp->GetActive())) {SetActive(0);}}
  F2.GetPointer(&freq,&tmp);
  if (tmp){if (!(tmp->GetActive())) {SetActive(0);}}
}

ostream& CFreqCompPeriPoint::WriteOut(ostream &s, int Selected) const
{
  s<<"F"<<mNumber+1<<"\t";
  int oldprec=s.precision(15);
  if (!GetActive()) { s<<"("; }
  // Frequency
  if (Selected && IsSelected(Fre))
    {
      if (IsComposition())
	{s<<'*'<<GetCompositeString()<<'*'; }
      else
	{s<<'*'<<GetFrequency()<<'*'; }
    }
  else
    {
      if (IsComposition())
	{s<<GetCompositeString(); }
      else
	{s<<GetFrequency(); }
    }
  // Amplitude
  if (Selected && IsSelected(Amp))
    {s<<"\t*"<<GetAmplitude(0)<<'*'; }
  else
    {s<<"\t "<<GetAmplitude(0)<<' '; }

  // Phase
  if (Selected && IsSelected(Pha))
    {s<<"\t*"<<GetPhase(0)<<'*'; }
  else
    {s<<"\t "<<GetPhase(0)<<' '; }
  // Composition
  if (IsComposition())
    {
      s<<"\t"<<GetFrequency();
    }
  
  if (!GetActive()) { s<<")"; }
  s.precision(oldprec);
  return s;
}

int CFreqCompPeriPoint::ReadIn(char * ptr)
{
  double tmpa=0.006,tmpp=0;
  char tmpf[256];
  double tmpfv;
  int active=1;
  // check if starting with a "("
  char c='*';
  int read=sscanf(ptr," %c",&c);
  // check and read in parameters
  if (c=='(')
    {
      active=0;
      read = sscanf(ptr," %c%s%lf%lf",&c,tmpf,&tmpa,&tmpp);
      read--;
    }
  else
    {
      read = sscanf(ptr,"%s%lf%lf",tmpf, &tmpa, &tmpp);
      active=1;
    }

  // Frequency
  if (read<1) { SetActiveNoCheck(0); return 1; }
  if (tmpf[0]=='=')
    {
      // set composite
      SetCompositeString(tmpf);
      SetActiveNoCheck(active);
    }
  else
    {
      // read in ok ?
      if (sscanf(tmpf,"%lg",&tmpfv))
	{
	  // set frequency
	  SetFrequency(tmpfv);
	  SetActive(active);
	}
      else
	{
	  return 1;
	}
    }

  // Amplitude
  // has there been an error reading, if so, return
  if (read<2) { return 0;}
  SetAmplitude(tmpa,0);
  // Phase
  if (read<3) {return 0;}
  SetPhase(tmpp,0);
  // exit function
  return 0;
}










