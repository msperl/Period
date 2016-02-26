/*
 * Program:     Period98
 *
 * File:        dtnames.h
 * Purpose:     header-file for
 *              ?
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */

#ifndef __names_h
#define __names_h

#include "lists.h"
#include "str.h"
#include <math.h>

///
class CName
{
public:
  ///
  CName(int ID,myString const & name);
  ///
  CName(CName const &other);
  ///
  ~CName();
  
  ///
  void SetName(myString const & name);
  ///
  myString GetName() const { return mName; }
  
  ///
  int GetID() const { return mID; }

  ///
  int GetSelect() const { return mSelect; }
  ///
  void SetSelect(int flag) { mSelect=flag; }

  ///
  double GetWeight() { return mWeight; }
  ///
  void SetWeight(double w) { if (w>0) { mWeight=w; } } 

  ///
  char* GetColor();
  ///
  char* GetTrueColor();
  ///
  void SetColor(char * color);
  
  ///
  void ResetValues();
  ///
  void AddPoint(double orig, double adj, double weight);
  
  ///
  int GetPoints() const { return mPoints; }
  ///
  double GetAverageOrig() const;
  ///
  double GetAverageAdj() const;

  ///
  double GetSigmaOrig() const;
  ///
  double GetSigmaAdj() const;

  ///
  double GetAverageOrigWeight() const ;
  ///
  double GetAverageAdjWeight() const;

  ///
  double GetSigmaOrigWeight() const;
  ///
  double GetSigmaAdjWeight() const;
  ///
  double GetWeightedSum() const { return mWeightSum; }

  ///
  friend int operator < (CName const & one, CName const & other);
  friend int operator <= (CName const & one, CName const & other);

  ///
  void StoreSelection() { mSelectCopy=mSelect; }
  ///
  void RestoreSelection() { mSelect=mSelectCopy; }

private:
  ///
  myString mName;
  ///
  int mID;
  ///selection
  int mSelect;
  ///
  int mSelectCopy;
  ///
  int mColor;
  /// average
  double mOrig;
  ///
  double mAdjusted;
  /// Sigma
  double mOrigSigma;
  ///
  double mAdjustedSigma;
  ///
  int mPoints;
  ///
  double mOrigWeight;
  ///
  double mAdjustedWeight;
  ///
  double mOrigSigmaWeight;
  ///
  double mAdjustedSigmaWeight;
  ///
  double mWeightSum;
  /// Period-solution for Amplitude Variations
  int mFrequencies;
  /// Pointer to Amplitude-array for Amplitude-variations
  double *mAmplitude;
  /// Pointer to Phase-array for Phase-variations
  double *mPhase;
  /// weights
  double mWeight;
};

///
class CNames
{
public:
  ///
  CNames();
  ///
  ~CNames();

  //@ManMemo: Returns the Id for the given name, creating a new entry if necessary
  int GetID(myString name);
  ///
  int SetColor(int c);
  //@ManMemo: returns the number of names
  int NumberOfNames() const { return mList.entries(); }

  //@ManMemo: allocates Data for Amplitude Variations
  void AddAmpVarData(int Freqs);
  //@ManMemo: removes the allocated data
  void RemoveAmpVarData();

  // return the Name fitting the ID
  //@ManMemo:CName & operator [] (int id) { return GetIDName(id); }
  CName & GetIDName(int id);
  //@ManMemo: returns entry number i
  CName & GetIndexName(int i);

  ///
  int GetHighestActiveIDName() const ;
  
  //@ManMemo: to remove all Name-labels
  void CleanUp();

  //@ManMemo: to store/restore Selection
  void StoreSelection();
  ///
  void RestoreSelection();

  //@ManMemo: For Amplitude Variations
  int GetActiveNames() const;
    
  //@ManMemo: to remove unnecessary Names
  void GarbageCollect();
private:
  ///
  TList<CName> mList;
  ///
  int mIndex;
  //@ManMemo: for speedup-purpose on search
  CName * mLastObject;
};

#endif













