/*
 * Program:     Period98
 *
 * File:        dtimepnt.cpp
 * Purpose:     implementation-file for
 *              timestring-data-point
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */

#include "dtimepnt.h"

CTimePoint::CTimePoint(double t, double o, double a, double c, double pw,
		       int n1, int n2, int n3, int n4,int index)
  :mTime(t),mObserved(o),mCalculated(c),mWeight(1.0),mPointWeight(pw)
{
  mRunningIndex=mNextIndex++;
  if (index)
    {
      mRunningIndex=index;
    }
  if (a<=-100)
    {
      a=o;
    }
  mAdjusted=a;
  mNames[0]=n1;mNames[1]=n2;mNames[2]=n3;mNames[3]=n4;
}

int CTimePoint::mNextIndex=0;

CTimePoint::CTimePoint(int i)
  :mTime(0),mObserved(0),mCalculated(0),mWeight(1)
{
  if (i)
    {
      mNextIndex=0;
    }
  mRunningIndex=mNextIndex++;
  mAdjusted=0;
  mNames[0]=0;mNames[1]=0;mNames[2]=0;mNames[3]=0;
}


CTimePoint::~CTimePoint()
{
  // nothing to do !!!
}

int operator<(CTimePoint const &t1, CTimePoint const &t2)
{
  return(t1.GetTime()<t2.GetTime());
}

int operator<=(CTimePoint const &t1, CTimePoint const &t2)
{
  return(t1.GetTime()<=t2.GetTime());
}

// output
ostream& operator<<(ostream &str, CTimePoint const &t)
{
  return str<<t.GetRunningIndex()<<"\t"
	    <<t.GetTime()<<"\t"<<t.GetObserved()<<"\t"
	    <<t.GetAdjusted()<<"\t"<<t.GetCalculated()<<"\t"
	    <<t.GetDataResidual()<<"\t"<<t.GetAdjustedResidual()<<"\t"
	    <<t.GetWeight()<<"\t"
            <<t.GetPointWeight()<<"\t"
	    <<t.GetIDName(0)<<"\t"<<t.GetIDName(1)<<"\t"
	    <<t.GetIDName(2)<<"\t"<<t.GetIDName(3);
}

