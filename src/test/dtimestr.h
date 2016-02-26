/*
 * Program:     Period98
 *
 * File:        dtimestr.h
 * Purpose:     header-file for
 *              ?
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */

#ifndef __timestr_h__
#define __timestr_h__

#include "lists.h"
#include "str.h"
#include "dtimepnt.h"
#include "dtnames.h"
#include <iostream.h>

#include "constant.h"
#include "error.h"

///
typedef TSortUpList<CTimePoint> CTimeArray;
///
typedef CTimePoint* SelectedTime;

///
enum DataMode {Observed,Adjusted,SpectralWindow,DataResiduals,
	       AdjustedResiduals,Calculated};

///
class CTimeString
{
public:
  ///
  CTimeString();
  ///
  CTimeString(myString const & Name, myString const &form="ta");
  ///
  ~CTimeString();

  ///
  int GetReverseScale()
  { return ReverseScale; } 
  ///
  void SetReverseScale(int scale)
  { ReverseScale=scale; } 

  
  //@ManMemo: memory-functions
  void SelectClear();
  ///
  void Select();
  ///
  void SelectAll();
  ///
  int IsSelected(SelectedTime time);

  ///
  void SetWeight(SelectedTime time);
  ///
  void CalcWeights();
  ///
  double GetWeightSum() const { return mWeightSum; }

  ///
  void SetUseNameWeight(int i) {mNameWeight=i;}
  ///
  int GetUseNameWeight() const { return mNameWeight;}
  ///
  void SetUsePointWeight(int i) {mPointWeight=i;}
  ///
  int GetUsePointWeight() const { return mPointWeight;}


  void CalcAverage();
  ///
  void Adjust(int what, DataMode Original, int n, int *ids,int useweight);
  ///
  void MakeObservedAdjusted();
  
  ///file I/O
  void Load(myString const &Name);
  ///
  void ReadIn(istream& str,int version=0);
  /// 
  void AddData(CTimePoint value)
    { mTimeArray.add(value);}
  ///
  void Save(myString const &Name);
  ///
  void SavePhase(myString const &Name, double Frequency);
  ///
  ostream &WriteOut(ostream& str, double Frequency=0.0);
  /// 
  void WriteAll(ostream& err,char *head="");
  //values of interest

  inline int GetSelectedPoints() const { return mSelected; }
  ///
  inline int GetTotalPoints() const
  { return mTimeArray.entries(); }
  ///
  inline myString GetFileName() const { return mFileName; }
  /// 
  void SetFileName(myString const &name) { mFileName=name; }
  
  ///
  double BaseLine() const;
  ///
  double Nyquist() const;
  ///
  double Average(int useWeights=0) const;
  
  ///
  void SetInputFormat(myString const &fi);
  ///
  inline myString GetInputFormat() const { return mInputFormat; }

  ///
  void SetOutputFormat(myString const &fo);
  ///
  inline myString GetOutputFormat() const { return mOutputFormat; }

  ///
  void SetDataMode(DataMode i) { mMode=i;}
  ///
  inline DataMode GetDataMode() const { return mMode;}

  ///
  CTimePoint &operator[](int i) const;
  ///
  inline CTimePoint &Point(int i, double *t, double *a) const
  {
    return Point(i,mMode,t,a);
  }

  ///
  CTimePoint &Point(int i,DataMode mode, double *t, double *a) const
  {
      CTimePoint &tmp=*mSelection[i];
      *t=tmp.GetTime();
      switch(mode)
	{
	case Observed: *a=tmp.GetObserved(); break;
	case Adjusted: *a=tmp.GetAdjusted(); break;
	case SpectralWindow: *a=1; break;
	case DataResiduals: *a=tmp.GetDataResidual(); break;
	case AdjustedResiduals: *a=tmp.GetAdjustedResidual(); break;
	case Calculated: *a=tmp.GetCalculated(); break;
	}
      return *mSelection[i];
    }

  ///
  void CheckBounds(int i) const 
  {
    if ( (i<0) || (i>3) )
      {
	MYERROREXIT("Get Name out of Bounds...");
      }
  }

  ///
  void AddAmpVarData(int i, int freq)
  {
    CheckBounds(i);
    mNames[i].AddAmpVarData(freq);
  }

  ///
  void RemoveAmpVarData(int i)
  {
    CheckBounds(i);
    mNames[i].RemoveAmpVarData();
  }
  
  ///
  myString NameSet(int i)
  {
    CheckBounds(i);
    return mNameSet[i];
  }

  ///
  void InitNameSets();
  ///
  void ChangeNameSet(int i, myString const & tmp);
  
  ///
  int NumberOfNames(int i)
  {
    CheckBounds(i);
    return mNames[i].NumberOfNames();
  }

  ///
  int GetID(int i, myString const & name)
  {
    CheckBounds(i);
    return mNames[i].GetID(name);
  }
  
  ///
  CName &GetIDName(int i, int id)
  {
    CheckBounds(i);
    return mNames[i].GetIDName(id);
  }
  
  ///
  CName &GetIndexName(int i, int l)
  {
    CheckBounds(i);
    return mNames[i].GetIndexName(l);
  }

  ///
  int GetActiveNames(int i) const
  {
    CheckBounds(i);
    return mNames[i].GetActiveNames();
  }

  ///
  int GetHighestActiveIDName(int i) const
  {
    CheckBounds(i);
    return mNames[i].GetHighestActiveIDName();
  }
  
  ///
  int CreateName(int column,int &useCounter,double time, myString prefix);
  ///
  void Subdivide(double timegap, int column,
		 int useCounter, myString prefix);
  ///
  void RenameSelection(int Column=0, myString Name="");
  
  ///
  void Clean();
  ///
  void GarbageCollect();

  /// set Delete labels
  void SetDeletePointInfo(int what, myString name)
    { mDeleteWhat=what; mDeleteName=name; }
  /// get Delete labels
  void GetDeletePointInfo(int * what, myString *name)
    { *what=mDeleteWhat; *name=mDeleteName; }
 
private:
  ///
  int CalculateExpectationValue(double &mi, double &ma) const;
public:
  ///
  myString PredictFileFormat(myString file);
  ///
  myString PredictLine(const char * tmp);

private:    
  ///
  CTimeArray mTimeArray;
  ///
  CNames mNames[4];
  ///
  SelectedTime *mSelection;
  ///
  int mSelected;
  ///
  myString mFileName;
  ///
  myString mInputFormat;
  ///
  myString mOutputFormat;
  ///
  DataMode mMode;
  ///
  myString mNameSet[4];
  ///
  double mWeightSum;
  ///
  int mDeleteWhat;
  ///
  myString mDeleteName;
  ///
  int ReverseScale;
  ///
  int mPointWeight;
  int mNameWeight;
};

ostream &operator<<(ostream &str, CTimeString &tmp);

#endif



