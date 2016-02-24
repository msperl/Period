/*
 * Program:     Period98
 *
 * File:        project.h
 * Purpose:     header-file for
 *              the timestring part of the project
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */
public:
  /// gives the number of selected points in the timestring
  int GetSelectedPoints()
  { return Timestring.GetSelectedPoints(); }
  /// gives the total number of points in the timestring
  int GetTotalPoints()
  { return Timestring.GetTotalPoints(); }
  ///
  int GetReverseScale()
  { return Timestring.GetReverseScale(); } 
  ///
  void SetReverseScale(int scale);

  ///
  void SetUseNameWeight(int i);
  ///
  int GetUseNameWeight() const
  { return Timestring.GetUseNameWeight();}
  ///
  void SetUsePointWeight(int i);
  ///
  int GetUsePointWeight() const
  { return Timestring.GetUsePointWeight();}

  ///
  double GetError(int i)
  {
    double weight=Timestring[i].GetWeight();
    double error=GetErrorScale()/(weight*weight);
    return error;
  }

  ///
  void SetErrorScale(myString s);
  ///
  double GetErrorScale() { return mErrorScale;}

  ///
  myString GetStartTime();
  ///
  myString GetEndTime();
  
  /// reads in a timestring 
  void LoadTimeString(myString name="",int append=0);
  /// appends a timestring to the currently loaded one
  void AppendTimeString(myString name="")
  {LoadTimeString(name,1);}
  /// save the currently selected timestring with a different name
  void SaveTimeString(myString name="");
  /// save Timestring with phase-information
  void SaveTimeStringPhase(double Frequency);
  /// save binned Phased TimeString 
  void SaveTimeStringPhaseBinned(double Frequency,
				 double BinSpacing,
				 DataMode UseData,
				 int points,
				 double *Phase,
				 double *Amplitude,
				 double *Sigma
				 );
  /// returns the list of timestring- files loaded
  myString GetTSFileName() const { return Timestring.GetFileName(); }

  /// returns the baseline of the currently selected timestring
  double GetBaseLine() const{ return Timestring.BaseLine(); }
  /// returns the Nyquist-frequency of the currently selected timestring
  double GetNyquist() const { return Timestring.Nyquist(); }
  
  /// returns a reference to the timestring-subsystem itself
  /// this call is only intended for plot purposes
  CTimeString &GetTimeString() { return Timestring;}

  /// returns the number of different labels/names 
  /// for a certain column i in the database
  int NumberOfNames(int i) { return Timestring.NumberOfNames(i); }

  void SaveStat(int what,int weighted,myString file="");
  myString StatStrings(int what,int weighted,int header=1);
  myString GetNameStats(int ID, int index, int useweighted, int*ident);

  //
  // Weights
  //

  ///
  myString GetIDNameWeight(int i,int ID);
  ///
  myString GetIndexNameWeight(int i,int ID);

  //
  // Colors
  //

  ///
  char* GetIDNameColor(int i,int ID);
  ///
  char** GetColors();
  ///
  int GetMaxColors();

  //
  // Selection
  // 

  ///
  int GetIDNameSelect(int i,int ID)
  { return GetIDName(i,ID).GetSelect(); } 
  ///
  void SetIDNameSelect(int i,int ID, int val)
  { GetIDName(i,ID).SetSelect(val); } 
  ///
  ///
  int GetIndexNameSelect(int i,int ID)
  { return GetIndexName(i,ID).GetSelect(); } 

  //
  // String
  //

  ///
  myString GetIDNameStr(int i,int ID)
  { return GetIDName(i,ID).GetName(); } 
  ///
  myString GetIndexNameStr(int i,int ID)
  { return GetIndexName(i,ID).GetName(); } 
  ///
  void SetIDNameStr(int i,int ID, myString val)
  { GetIndexName(i,ID).SetName(val); } 

  //
  // ID
  //

  ///
  int GetIndexNameID(int i,int ID) 
  { return GetIndexName(i,ID).GetID(); }

  ///
  void ChangeHeading(int i, char* N1);
  /// gives the label for column i
  myString NameSet(int i) { return Timestring.NameSet(i); } 
  /// changes the name and weight for an id of ID in the column i
  void ChangeName(int i,int ID,
		  myString name,
		  myString weight="1.0",
		  char* color=NULL);
  /// relabel datapoint i with the new names
  void RelabelPoint(int i,
		    myString name1,myString name2,
		    myString name3,myString name4);
  /// relabels the Point to delete
  void DeletePoint(int i);
  /// set Delete labels
  void SetDeletePointInfo(int what, myString name)
    { Timestring.SetDeletePointInfo(what,name); }
  /// get Delete labels
  void GetDeletePointInfo(int * what, myString *name)
    { Timestring.GetDeletePointInfo(what,name); }
  /// Selects timepoints using the flags in the different nameID s as hints
  /// also write it out in the protocol
  void Select();
  /// returns the average of all amplitudes 
  ///of the currently selected timestring
  double GetAverage() const { return Timestring.Average(); }
  /// calculates an average of the currently selected timestring
  /// beware to get this value please GetAverage after calling this function
  void CalcAverage() { Timestring.CalcAverage(); }
  /// does zeropoint adjustment of all the n ids of column what
  /// in the array ids. 
  /// if Original is set use original data else the allready shifted data 
  void Adjust(int what,DataMode Original,int n,int *ids,int useweight);
  /// in all currently selected points copy the observed values
  /// to the adjusted values
  void MakeObservedAdjusted();
  /// recalculates the compüosed weights for all the currently selected points
  void CalcWeights() { Timestring.CalcWeights(); UpdateTSDisplays();}

  /// subdivides the currently selected data using the  
  void Subdivide(myString timegap=".3", int column=0,
		 int useCounter=0, myString prefix="time");
  /// renames all the currently selected points
  /// in column Column to the name Name
  void RenameSelection(int Column=0, myString Name="");

  ///
  void Refit(myString zeropoint);

  ///
  virtual void DisplayTSGraph()=0;
  ///
  virtual void DisplayTSTable()=0;

protected:

  // this is only for this ONE function
  friend void AdjustFillBox(int id);

  /// ask the user for a format to write out the data
  virtual myString FileFormat(myString format,int Phase=0)=0;
  /// ask the user for the format in which to read in the data
  virtual myString FileFormat(myString format,myString filename)=0;
  /// returns a reference to the name with the id number ID in column i
  CName & GetIDName(int i,int ID) { return Timestring.GetIDName(i,ID); }
  /// returns a reference to the name with the index l in column i
  CName & GetIndexName(int i,int l) { return Timestring.GetIndexName(i,l); }

private:

  ///
  void ReadTimestring(istream & file);
  ///
  void WriteTimestring(ostream & file);


protected:
  ///
  CTimeString Timestring;
  ///
  double mErrorScale;
