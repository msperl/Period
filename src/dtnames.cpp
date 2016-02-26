/*
 * Program:     Period98
 *
 * File:        dtnames.cpp
 * Purpose:     implementation-file for
 *              ?
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */

#include "dtnames.h"
#include <string.h>
#include "error.h"
#include "dcolors.h"
#include "mswdefs.h"
#include <ctype.h>

CName::CName(int ID,myString const & name)
  :mID(ID),mSelect(0),mSelectCopy(0),
   mColor(0),
   mOrig(0),mAdjusted(0),
   mOrigSigma(0),mAdjustedSigma(0),
   mPoints(0),
   mOrigWeight(0),mAdjustedWeight(0),
   mOrigSigmaWeight(0),mAdjustedSigmaWeight(0),
   mWeightSum(0),
   mFrequencies(0),mAmplitude(NULL),mPhase(NULL),
   mWeight(1)
{
  mColor=ID%MAXCOLORS;
  ResetValues();
  SetName(name);
}

CName::CName(CName const & other)
  :mID(other.mID),mSelect(0),mSelectCopy(0),
   mColor(other.mColor),
   mOrig(other.mOrig),mAdjusted(other.mAdjusted),
   mOrigSigma(other.mOrigSigma),mAdjustedSigma(other.mAdjustedSigma),
   mPoints(other.mPoints),
   mOrigWeight(other.mOrigWeight),mAdjustedWeight(other.mAdjustedWeight),
   mOrigSigmaWeight(other.mOrigSigmaWeight),
   mAdjustedSigmaWeight(other.mAdjustedSigmaWeight),
   mWeightSum(other.mWeightSum),
   mAmplitude(other.mAmplitude),mPhase(other.mPhase),
   mWeight(other.mWeight)
{
  SetName(other.mName);
}

CName::~CName()
{}

void CName::ResetValues()
{
  // unweighted data
  mOrig=0;
  mOrigSigma=0;
  mAdjusted=0;
  mAdjustedSigma=0;
  mPoints=0;
  // weighted data
  mOrigWeight=0;
  mOrigSigmaWeight=0;
  mAdjustedWeight=0;
  mAdjustedSigmaWeight=0;
  mWeightSum=0;
  // Pointers
  mAmplitude=NULL;
  mPhase=NULL;
}

void CName::SetName(myString const & name)
{
  if (name=="")
    {
      mName="unknown";
    }
  else
    {
      mName="";
      for (int i=0;i<name.length();i++)
	{
	  char c=name[i];
	  if (isspace(c))
	    {
	      mName+='_';
	    }
	  else
	    {
	      mName+=c;
	    }
	}
    }
}

int operator < (CName const & one, CName const & other)
{
  return (one.mName<other.mName);
}

int operator <= (CName const & one, CName const & other)
{
  return (one.mName<=other.mName);
}

char* CName::GetColor()
{
  return Colors[mColor];
}

char* CName::GetTrueColor()
{
  return TrueColors[mColor];
}

void CName::SetColor(char * color)
{
  for (int i=0;i<MAXCOLORS;i++)
    {
      if (strcasecmp(Colors[i],color)==0)
	{
	  mColor=i;
	  return;
	}
    }
}

// now the unweighted data

double CName::GetAverageOrig() const 
{
  return (mPoints>0)?mOrig/mPoints:0;
}

double CName::GetAverageAdj() const 
{ 
  return (mPoints>0)?mAdjusted/mPoints:0;
}

double CName::GetSigmaOrig() const
{
  return (mPoints>0)?sqrt(mOrigSigma/(mPoints)):0; 
}

double CName::GetSigmaAdj() const
{
  return (mPoints>0)?sqrt(mAdjustedSigma/(mPoints)):0; 
}

// now the weighted data

double CName::GetAverageOrigWeight() const 
{ 
  return (mWeightSum>0.0)?mOrigWeight/mWeightSum:0; 
}

double CName::GetAverageAdjWeight() const 
{ 
  return (mWeightSum>0.0)?mAdjustedWeight/mWeightSum:0;
}

double CName::GetSigmaOrigWeight() const
{
  return (mWeightSum>0.0)?sqrt(mOrigSigmaWeight/(mWeightSum)):0; 
}

double CName::GetSigmaAdjWeight() const
{
  return (mWeightSum>0.0)?sqrt(mAdjustedSigmaWeight/(mWeightSum)):0; 
}

// now the statistical storage-function

void CName::AddPoint(double orig, double adj,double weight)
{
  // unweighted data
  mOrig+=orig;
  mOrigSigma+=orig*orig;
  mAdjusted+=adj;
  mAdjustedSigma+=adj*adj;
  mPoints+=1;
  // weighted data
  mOrigWeight+=weight*orig;
  mOrigSigmaWeight+=weight*orig*orig;
  mAdjustedWeight+=weight*adj;
  mAdjustedSigmaWeight+=weight*adj*adj;
  mWeightSum+=weight;
}

CNames::CNames()
{
  CleanUp();
}

CNames::~CNames()
{
  CleanUp();
}

int CNames::GetID(myString name)
{
  if (name=="")
    {
      name = "unknown";
    }
  // check if "history"-value
  if(
     (mLastObject!=NULL) &&
     (mLastObject->GetName()==name)
     )
    {
      return mLastObject->GetID();
    }
  // OK, a different value, search in list
  for (int i=0;i<mList.entries();i++)
    {
      if (mList[i].GetName()==name)
	{
	  // assign new history-values
	  
	  mLastObject=&(mList[i]);
	  // return the result
	  return mLastObject->GetID();
	}
    }
  // not found, let's add it
  mIndex++;
  mList.addtail(CName(mIndex,name));
  return mIndex;
}

CName & CNames::GetIDName(int id)
{
  // check if "history"-value
  if( (mLastObject!=NULL) && (id==mLastObject->GetID() ) )
    {
      return (*mLastObject);
    }
  // OK, a different value, search in list
  for (int i=0;i<mList.entries();i++)
    {
      if (mList[i].GetID()==id)
	{
	  // assign new history-values
	  mLastObject=&mList[i];
	  // return the result
	  return mList[i];
	}
    }
  mLastObject=NULL;
  MYERROREXIT("index "<<id<<" was not found... quitting...");
  return mList[0];
}

CName &CNames::GetIndexName(int i)
{
  if ( (i<0) || (i>=mList.entries()) )
    {
      MYERROREXIT("index "<<i<<" is out of bounds...");
    }
  return mList[i];
}

void CNames::CleanUp()
{
  // remove entries from list
  mList.clean();
  // set index-counter back to 0
  mIndex=-1;
  // empty "history"-value
  mLastObject=NULL;
  // add entry 0 - unknown
  (void) GetID("");
}

void CNames::GarbageCollect()
{
  int i=0;
  while (i<mList.entries())
    {
      if ((mList[i].GetPoints()==0))//&&(mList[i].GetWeight()==1.0))
	{
	  // delete this entry...
	  mList.removeat(i);
	  // restart this loop anew...
	  i=0;
	  //	  break;
	}
      else
	{
	  // work on next one...
	  i++;
	}
    }
}

void CNames::StoreSelection()
{
  for (int i=0;i<mList.entries();i++)
    {
      mList[i].StoreSelection();
    }
}

void CNames::RestoreSelection()
{
  for (int i=0;i<mList.entries();i++)
    {
      mList[i].RestoreSelection();
    }
}

int CNames::GetActiveNames() const
{
  int active=0;
  for (int i=0;i<mList.entries();i++)
    {
      if (mList[i].GetPoints()!=0)
	{
	  active++;
	}
    }
  return active;
}

int CNames::GetHighestActiveIDName() const
{
  int active=0;
  for (int i=0;i<mList.entries();i++)
    {
      // Names have to be active
      if (mList[i].GetPoints()!=0)
	{
	  int ID=mList[i].GetID();
	  //the ID must be the highest
	  if (ID>active)
	    {
	      active=ID;
	    }
	}
    }
  return active;
}
