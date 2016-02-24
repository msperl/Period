/*
 * Program:     Period98
 *
 * File:        pperiod.h
 * Purpose:     header-file for
 *              period part of the project
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */

public:

  /// returns a reference to the period-subsystem itself
  /// this call is only intended for plot purposes
  CPeriod &GetPeriod() { return Period;}

  /// loads a table table of frequencies
  void LoadPeriod(myString name="");
  /// saves the table of frequencies
  void SavePeriod(myString name="");
  /// clean the table of frequencies
  void CleanPeriod();
  
  /// gives the number of frequencies currently active
  int GetActiveFrequencies() const { return Period.GetActiveFrequencies(); }
  /// gives the number of frequencies currently active
  myString GetActiveFrequenciesString() const;

  /// gives the total number of Frequencies
  int GetTotalFrequencies() const { return Period.GetFrequencies(); }
  
  /// returns the calculated solution for the zeropoint
  myString GetZeropoint() const;

  /// returns the residuals of the last calculation
  myString GetResiduals() const;

  /// returns the number of loops that where necessary 
  /// to come to the last solution
  int GetLoops() const { return Period.GetIterations(); }

  ///
  DataMode GetPeriodUseData() const { return Period.GetUseData(); }

  /// sets a flag to use weighted data if flag is set
  void SetPeriodUseWeight(int flag) { Period.SetUseWeight(flag); Changed++;}
  /// returns the status of the flag indicating 
  /// if weights should be used for calculations
  int GetPeriodUseWeight() const { return Period.GetUseWeight(); }
  
  /// returns 0 if the frequency i is not selected
  int GetActive(int i) const { return Period[i].GetActive(); }
  /// activates or deactivatesthe frequency i
  void SetActive(int i, int act) { Period[i].SetActive(act); Changed++; }

  /// returns the running index of the frequency i 
  /// in conformation to the output of the frequencies
  myString GetNumber(int i) const ;
  /// returns a string of the value of the frequency i 
  /// (if a composition the definition of the compoition)
  myString GetFrequency(int i) const;
  /// sets frequency i to a certain value f (compositions are allowed)
  void SetFrequency(int i, myString f);

  /// returns <>0 if the frequency is a composition 
  myString GetComposite(int i) const { return Period[i].GetCompositeString(); }
  
  /// returns the amplitude of frequency i
  myString GetAmplitude(int i, int what=-1) const;
  /// sets the amplitude of frequency i to a
  void SetAmplitude(int i, myString a);

  /// returns the phase of frequency i
  myString GetPhase(int i, int what=-1) const;
  /// sets the phase of frequency i to p
  void SetPhase(int i, myString p);

  /// sanity-checking on active frequencies 
  /// (especially regarding compositions of which one 
  /// or more dependant frequencies are inactive)
  void CheckActive() { Period.CheckActive(); Changed++; }

  /// calculation of a least square fit using all 
  /// amplitudes and phases as variable
  void CalculatePeriod(DataMode mode);
  /// calculation of a least square fit using all 
  /// frequencies, amplitudes and phases as variable
  void ImprovePeriod(DataMode mode);
  /// calculation of a least square fit using the given 
  /// frequencies, amplitudes and phases as variables
  void ImproveSpecialPeriod(DataMode mode);

  /// calculation of a least square fit 
  /// using the n id's of column what in array id's
  /// for special handling
  void CalculateAmpVarPeriod(int n, int *IDs, int what, CalcMode mode);
  
  /// returns a predicted value for a certain time
  /// using the currently active set of frequencies/amplitudes/phases
  myString Predict(myString time);

  /// calculates the epoch for all active frequencies
  myString ShowEpoch(myString t, int Intensity);

  /// creates a file filled with predicted values
  /// using the currently active set of frequencies/amplitudes/phases
  /// in the range [from:to] with a possible leadin/leadout of leadin
  /// and step steps as a nominal distance
  /// and append to a file if append is set
  void CreateArtificialData(myString file,myString from, myString to,
			    myString step,myString leading, int append);

  /// sets the frequency-adjustment that is used for aliasing/purposes
  void SetFrequencyAdjustment(myString step); 
  /// returns the current frequency adjustment 
  ///that may be used for aliasing/purposes
  myString GetFrequencyAdjustment() const ;  

  ///
  virtual void DisplayPeriodGraph()=0;

protected:

  ///
  virtual void GeneralCalcPeriod(DataMode mode);
  ///
  virtual int GetPeriodSelection()=0;
  ///
  virtual int *GetAmpVarData(int *freqs, int* what, CalcMode *mode)=0;
  ///
  virtual void DisplayAmpVarData(char * txt)=0;
  ///
  void WriteFrequenciesTabulated(ostream & str,int selected);

private:
  ///
  void ReadPeriod(istream & file);
  ///
  void WritePeriod(ostream & file);

protected:
  ///
  CPeriod Period;
  ///
  double freqstep;
 
