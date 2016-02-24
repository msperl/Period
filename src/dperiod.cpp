/*
 * Program:     Period98
 *
 * File:        dperiod.cpp
 * Purpose:     implementation-file for
 *              the period-data-structure
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */

#include "dperiod.h"
#include <stdio.h>
#include <ctype.h>
#include "error.h"

#include <fstream.h>
#include <string.h>

#define DOUBLEOUT "%8g"
#define INTOUT    "%5i"

CPeriod::CPeriod(int size)
  :mBaseMatrix(0,0),mWorkMatrix(0,0),
   mVariables(0),mIterations(0),
   mFlamda(0.001),mFlamdadef(0.001),
   mZeropoint(0),mChisqr(0),mTimeString(0),
   mFrequencies(0),mMaximumFrequencies(size),
   mActive(0),mActiveIndex(0),
   mFileName(""),mUseWeight(0),
   mUseData(Observed),mWhat(0)
{
  mData=new CPeriPoint[size];
  for (int j=0;j<size;j++)
    {
      // set Frequency-values right !!!
      mData[j].SetNumber(j);
    }
}

CPeriod::~CPeriod()
{
  delete [] mData;
  delete [] mActiveIndex;
}

int CPeriod::GetActiveFrequencies() const
{
  int count=0;
  for (int fr=0;fr<GetFrequencies();fr++)
    {
      if (mData[fr].GetActive())
	{
	  count++;
	}
    }
  return count;
}

void CPeriod::SetActiveIndex()
{
  delete [] mActiveIndex;
  mActive=GetActiveFrequencies();
  mActiveIndex= new int[mActive];
  int i,j=0;
  for (i=0;i<GetFrequencies();i++)
    {
      if ((*this)[i].GetActive())
	{
	  mActiveIndex[j]=i;
	  j++;
	}
    }
}

void CPeriod::SelectDefault()
{
  for(int fr=0;fr<GetFrequencies();fr++)
    {
      mData[fr].UnsetSelection(Fre);
      mData[fr].SetSelection(Amp);
      mData[fr].SetSelection(Pha);
    }
}

void CPeriod::SelectAll()
{
  for(int fr=0;fr<GetFrequencies();fr++)
    {
      mData[fr].SetSelection(Fre);
      mData[fr].SetSelection(Amp);
      mData[fr].SetSelection(Pha);
    }
}

void CPeriod::StoreSelection()
{
  for(int fr=0;fr<GetFrequencies();fr++)
    { mData[fr].StoreSelection(); }
}

void CPeriod::RestoreSelection()
{
  for(int fr=0;fr<GetFrequencies();fr++)
    { mData[fr].RestoreSelection(); }
}

CPeriPoint &CPeriod::operator[](int i) const
{
  if ((i<0) || (i>=mFrequencies))
    {
      MYERROREXIT("CPeriod["<<i<<"]: out of range of "
		  "[0,"<<mFrequencies<<")...");
    }
  return mData[i];
}

int CPeriod::FindEmpty() const
{
  for (int i=0;i<mFrequencies;i++)
    {
      if (mData[i].Empty())
	return i;
    }
  return mFrequencies-1;
}

void CPeriod::Add()
{
  ResizeLeast(mFrequencies);
}

void CPeriod::Remove()
{
  mFrequencies--;
  if (mFrequencies<0)
    {
      mFrequencies=0;
    }
  else
    {
      mData[mFrequencies].CleanComposition();
      // this needs to be DONE !!!
      //*this[Freqs].CleanDepend();
    }
  
}

void CPeriod::ResizeLeast(int i)
{
  i++;
  if (i>mFrequencies)
    {
      if (i<mMaximumFrequencies)
	{
	  for (int j=mFrequencies;j<i;j++)
	    {
	      // set Frequency-values right !!!
	      mData[j].SetNumber(j);
	      mData[j].CSimplePeriPoint::SetActive(0);
	    }
	  mFrequencies=i;
	}
    }
}

void CPeriod::Recalc()
{
  // first update Pointers
  SetPointers(-1);
  // 
  for(int i=0;i<mFrequencies;i++)
    {
      mData[i].RecalcFrequencies();
    }
}


void CPeriod::Adapt()
{
  for (int i=0;i<mFrequencies;i++)
    {
      mData[i].Adapt();
    }
}

void CPeriod::SetPointers(int f)
{
  if (f==-1)
    { for (int i=0;i<mFrequencies;i++) { SetPointers(i); } }
  else
    {
      // check for f needed
      if ((f<0)||(f>=mFrequencies))
	{ 
	  MYERROR("Tried to update a bad pointer !!!");
	  exit(-1); 
	}
      int freq;
      // the first one
      mData[f].GetDepending(0,&freq);
      if (freq!=-1)
	{
	  // check if freq is good
	  if ((freq<0)||(freq>=mFrequencies))
	    {
	      MYERROR("Bad pointer requested !!!");
	      exit(-1);
	    }
	  mData[f].SetDepending(0,freq,&mData[freq]);
	  // the second one
	  mData[f].GetDepending(1,&freq);
	  if (freq!=-1)
	    {
	      // check if freq is good
	      if ((freq<0)||(freq>=mFrequencies))
		{ 
		  MYERROR("Bad pointer requested !!!");
		  exit(-2); 
		}
	      mData[f].SetDepending(1,freq,&mData[freq]);
	    }
	}
    }
}

void CPeriod::SetResiduals(CTimeString &times)
{
  // check if there is something calculationg the data
  double ti;
  int id;
  for(int i=0;i<times.GetSelectedPoints();i++)
    {
      ti=times[i].GetTime();
      id=times[i].GetIDName(mWhat);
      times[i].SetCalculated(Predict(ti,id));
    }
  // set ChiSquare
  mChisqr=ChiSqr(times);
}

void CPeriod::Load(myString const & name)
{
  ifstream file(name.chars());
  file>>(*this);
  mFileName= name;
}

void CPeriod::Save(myString const & name)
{
  ofstream file(name.chars());
  file<<(*this);
}

ostream& operator<<(ostream& s,const CPeriod &t)
{
  for(int i=0;i<t.GetFrequencies();i++)
    {
      if ((t[i].GetActive()!=0) || (t[i].GetFrequency()!=0))
	{
	  s<<t[i]<<endl;
	}
    }
  return s;
}

ostream &CPeriod::AdditionalOut(ostream &s)
{
  // store default-precission and set new 
  int prec=s.precision(15);
  // output myself
  s<<*this;
  // output additional data...
  s<<"Zeropoint="<<GetZeropoint()<<endl;
  s<<"Residuals="<<GetResiduals()<<endl;
  s<<"Iteration="<<GetIterations()<<endl;
  // restore old precission
  s.precision(prec);
  // and return
  return s;
}

ostream &CPeriod::WriteSelected(ostream &s)
{
  for (int i=0;i<GetFrequencies();i++)
    {
      if ((*this)[i].GetActive())
	{
	  //	  s<<"F"<<i+1<<"\t";
	  (*this)[i].WriteOut(s,1);
	  s<<endl;
	}
    }
  return s;
}

istream& operator>>(istream &s, CPeriod &t)
{
  int Freq;
  char c=0;
  char linestr[256];
  while (
	 // read in character
	 (s>>c)
	 &&
	 (!s.eof()))
    {
      // check if it is "F"
      switch (c)
	{
	case 'f':
	case 'F':
	  // this is a Frequency line, let's interprete it...
	  {
	    // read the line
	    s.getline(linestr,255);
	    int result=sscanf(linestr,"%i",&Freq);
	    // find next interresting part
	    char *ptr=linestr;
	    while ((*ptr!=0) && (!isspace(*ptr))) {ptr++;}
	    // check for sanity
	    if ( (result!=1) ||(Freq<1) || (*ptr==0) )
	      {
		MYERROR("This line-format is not recognized...\nLine:"
			<<linestr<<
			"after the F a number is missing "
			"or lower than zero !!!");
	      }
	    else
	      {
		
		// setup data before start reading...
		// make Freq 0-based !!!
		Freq--;
		// Resize the Period Array
		t.ResizeLeast(Freq);
		// read rest of line into the coresponing Frequency
		t[Freq].ReadIn(ptr);
	      }
	  }
	  break;
	case ';':
	case '#':
	case '/':
	case '%':
	  // this is a comment-line, read it and ignore it...
	  s.getline(linestr,255);
	  break;
	case '[':
	  s.putback('[');
	  t.Recalc();
	  return s;
	default:
	  s.getline(linestr,255);
	  MYERROR("This line-format is not recognized...\nLine:"<<c<<linestr);
	  t.Recalc();
	  return s;
	}
    }
  t.Recalc();
  t.CheckActive();
  return s;
}

void CPeriod::CreateArtificialData(myString file,
				   double from,
				   double to,
				   double *step,
				   double leading,
				   int append)
{

  from-=leading;
  to+=leading;
  if (*step<=0)
    {
      *step=1;
    }
  // Get write-flags
  int flags=0;
  if (append)
    {
      flags=ios::out|ios::ate;
    }
  else
    {
      flags=ios::out|ios::trunc;
    }
  // write file
  ofstream str(file.chars(),flags);
  str.precision(15);
  for(double i=from;i<=to;i+=*step)
    {
      str<<i<<"\t"<<Predict(i,-1)<<endl;
    }
}
