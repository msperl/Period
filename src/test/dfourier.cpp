/*
 * Program:     Period98
 *
 * File:        dfourier.cpp
 * Purpose:     implementation-file for
 *              fourier-data-structure
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */

#include "dfourier.h"
#include <string.h>
#include "dgtimein.h"
#include <fstream.h>
#include "mswdefs.h"
void SetDone(int Done)
{
  // mDone=Done;
  char tmp[256];
  sprintf(tmp,"%i %% done...",Done);
  InformUserOfProgress(tmp);
}


CFourierPoint::CFourierPoint()
: mFrequency(0),mAmplitude(0)
{;}

CFourierPoint::CFourierPoint(double f,double a)
: mFrequency(f),mAmplitude(a)
{;}

int CFourierPoint::ReadIn(char *ptr)
{
  // at first clean all values
  mFrequency=0.0;
  mAmplitude=1.0;
  double freq=0.0;
  double amp=1.0;
  // now read in
  int read=sscanf(ptr,"%lf%lf",&freq,&amp);
  if (read<1) { return 1; }
  mFrequency=freq;
  if (read<2) { return 1; }
  mAmplitude=amp;
  return 0;
}

// comperator needet for sorted list
int operator<(CFourierPoint const &f1, CFourierPoint const &f2)
{
  return (f1.GetFrequency()<f2.GetFrequency());
}
int operator<=(CFourierPoint const &f1, CFourierPoint const &f2)
{
  return (f1.GetFrequency()<=f2.GetFrequency());
}

// output-operator
ostream &operator<<(ostream &str,CFourierPoint const &f1)
{
  double amp=f1.GetAmplitude();
  return str<<f1.GetFrequency()<<'\t'
	    <<amp;
}

istream &operator>>(istream &str, CFourierPoint &f1)
{
    char buffer[1024];
    str.getline(buffer,1023);
    if (f1.ReadIn(buffer))
      {
	// there has been an error, so let us set a flag
	str.clear(ios::badbit);
      }
    return str;
}

// CFourier

// initialise static elements 
int CFourier::mFourierIDCounter=0;

// constructor

CFourier::CFourier(myString title,double from,double to,
		   StepQuality typ,double steprate,
		   DataMode Mode,CompactMode Compact,
		   int Weight)
  :mFrom(0),mTo(0),
   mMode(Mode),mCompact(Compact),mDone(1000),
   mGraphDisplay(0),mDataDisplay(0),
   mUseWeight(Weight),
   mPoints(0),mData(NULL)
{
  // set id
  mFourierID=mFourierIDCounter;
  // increase counter
  mFourierIDCounter++;
  // set other stuff
  SetTitle(title);
  SetFrom(from);
  SetTo(to);
  SetStepping(typ,steprate);
  // Clean the peak
  mPeak=CPeriPoint(0,0,0);
}

CFourier::~CFourier()
{ delete [] mData; mData=NULL; mPoints=0; }

// overload add
void CFourier::add(CFourierPoint const &point)
{

  if (mData!=NULL)
    {
      MYERROREXIT("You cannot add new points...");
    }

  TSortUpList<CFourierPoint>::add(point);
  // find out if highest peak, and set the Noise for this point
  SetPeak(point.GetFrequency(),point.GetAmplitude(),0);
}

void CFourier::MakeStatic()
{
  // allocate memory
  mPoints=Points();
  mData= new CFourierPoint[mPoints];
  int i=0;
  startiterate(0);
  // copy these values
  for (i=0;i<mPoints;i++)
    {
      mData[i]=*(iterate());
    }
  // now clean the list
  clean();
}

istream &CFourier::ReadData(istream &stream)
{
  // clean Data at first
  DeallocateData();
  // Now start reading
  char text[256];
  while (!stream.eof())
    {
      // gets the next line
      stream.getline(text,256);
      if (stream.good()&&(text[0]!=0))
	{
	  // read in the point
	  CFourierPoint tmp;
	  if (tmp.ReadIn(text)==0)
	    {
	      // add the point
	      add(tmp);
	    }
	}
    }
  MakeStatic();
  return stream;
}

ostream &CFourier::WriteData(ostream &stream) const
{
  // store default-precission and set new 
  int prec=stream.precision();
  stream.precision(15);
  // write out all data
  for(int i=0;i<Points();i++)
    {
      stream<<(*this)[i]<<endl;
    }
  // restore old precission
  stream.precision(prec);
  // and return
  return stream;
}

void CFourier::DeallocateData()
{
  // nothing much to do but to call clean from the list-parent
  clean();
}

void CFourier::SetTitle(myString const &title)
{
  mTitle=title;
}

void CFourier::SetFrom(double From)
{
  if (From<0)
    {
      From=0;
    }
  mFrom=From;
}

void CFourier::SetTo(double to)
{
  if (to<mFrom)
    {
      to=mFrom+1.0;
    }
  mTo=to;
}

int CFourier::PointEstimate(double)
{
  return (int) ((GetTo()-GetFrom())/GetStepping())+1;
}

ostream & CFourier::WriteHeader(ostream &stream) const
{
  stream<<"Title=\t"<<GetTitle()
	<<"\nFrom=\t"<<GetFrom()
	<<"\nTo=\t"<<GetTo()
	<<"\nStepping=\t"<<(int)GetStepQuality()
	<<"\t"<<GetStepping()
	<<"\nMode=\t"<<(int)GetMode()
	<<"\nCompact=\t"<<(int)GetCompact()
	<<endl;
  stream.precision(12);
  for(int i=0;i<Points();i++)
    {
      stream<<"Point\t"<<(*this)[i]<<endl;
    }
  return stream;
}

istream &CFourier::ReadHeader(istream &ist)
{
  // nothing done yet !!!
  char c,tmp[256];
  double dtmp;
  int itmp;
  while(ist>>c,ist.putback(c),c!='[')
    {
      // read in argument
      ist>>tmp;
      if (strcasecmp(tmp,"Title=")==0)
	{
	  char c;
	  ist>>c;
	  ist.putback(c);
	  ist.getline(tmp,255);
	  SetTitle(tmp);
	}
      else if (strcasecmp(tmp,"From=")==0)
	{
	  ist>>dtmp;
	  SetFrom(dtmp);
	}
      else if (strcasecmp(tmp,"To=")==0)
	{
	  ist>>dtmp;
	  SetTo(dtmp);
	}
      else if (strcasecmp(tmp,"Stepping=")==0)
	{
	  ist>>itmp>>dtmp;
	  SetStepping((StepQuality)itmp,dtmp);
	}
      else if (strcasecmp(tmp,"Mode=")==0)
	{
	  ist>>itmp;
	  switch (itmp)
	    {
	    case 0: SetMode(Observed); break;
	    case 1: SetMode(Adjusted); break;
	    case 2: SetMode(SpectralWindow); break;
	    case 3: SetMode(DataResiduals); break;
	    case 4: SetMode(AdjustedResiduals); break;
	    case 5: SetMode(Calculated); break;
	    }
	}
      else if (strcasecmp(tmp,"Compact=")==0)
	{
	  ist>>itmp;
	  switch (itmp)
	    {
	    case 0: SetCompact(AllData);
	    case 1: SetCompact(PeaksOnly);
	    }
	}
      else if (strcasecmp(tmp,"Point")==0)
	{
	  CFourierPoint read;
	  // read data from input
	  ist>>read;
	  // add it to the data
	  add(read);
	}
      else
	{
	  char tmp1[256];
	  // and read rest of line
	  ist.getline(tmp1,256);
	}
    }

  // for faster access of the data
  MakeStatic();
  // finally return
  return ist;
}




