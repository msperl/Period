/*
 * Program:     Period98
 *
 * File:        project.h
 * Purpose:     header-file for
 *              the fourier part of the project
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */
public:

  /// returns a reference to the active fourier-subsystem itself
  /// this call is only intended for plot purposes
  CFourier * GetFourierActive()
  { return FourierActive; }
  /// returns the number of entries that 
  /// are in the list of fourier-calculations
  int GetFourierEntries() const
  { return FourierList.entries(); }
  /// returns the name of the fourier-calculation at point i in the list
  myString GetFourierTitle(int i) const
  { return FourierList[i]->GetTitle(); }
  /// activates one entry of the fourier-caluclation
  void ActivateFourier(int i)
  {
    if (FourierList.entries()!=0)
      { FourierActive=FourierList[i]; }
    else
      { FourierActive=&DefaultFourier; }
  }
  /// deletes the currently active entry from the fourier-list
  void DeleteActiveFourier();
  /// renames the currently active entry from the fourier-list
  void RenameActiveFourier(myString NewName);

  /// returns a string with the definition for the quality
  char* GetStepQualityLabel(CFourier::StepQuality mode);

  /// calculates a steprate for a given quality
  double GetStepRate(CFourier::StepQuality qstep, double step) const;
  /// performs a fourier-calculation in the range [from:to] 
  /// using an nominal steping of step 
  /// (if negative the real value is computed),
  /// Mode as indicator for what data should be used,
  /// Compact as an indicator of what compaction-routines should be used
  /// and weight as an indicator if the weights for dthe data should be used
  void CalculateFourier(myString title,
			double from=0,double to=0,
			CFourier::StepQuality qstep=CFourier::High,
			double step=0.1,
			DataMode Mode=Observed,
			CompactMode Compact=PeaksOnly,int weight=0);

  /// performs a noise calculation for the frequency point
  /// summing up the spectra in box of size box centered on this point
  /// using an nominal steping of step,
  /// Mode as indicator for what data should be used,
  /// and weight as an indicator if the weights for dthe data should be used
  double CalculateNoise(double point=10, double box=10, 
			CFourier::StepQuality stepq=CFourier::Custom,
			double step=0.1,
			DataMode Mode=Observed,int weight=0);
  /// performs a noise spectra calculation in the range [from:to]
  /// summing up the spectra in box of size box centered on this point
  /// using an nominal steping of step,
  /// Mode as indicator for what data should be used,
  /// and weight as an indicator if the weights for dthe data should be used
  int CalculateNoiseSpectra(double from, double to, double CalcDist,
			    double box,
			    CFourier::StepQuality stepq,double step, 
			    DataMode Mode, int weight,
			    double *freq,double *noise);
  
  /// stores the fourier-spectra i with filename name
  void SaveFourier(int i,myString name="");
  /// loads the fourier-spectra with filename name
  void LoadFourier(myString name="");

  ///
  virtual void DisplayFourierGraph()=0;
  ///
  virtual void DisplayFourierTable()=0;

private:
  ///
  void CleanFourier();
  ///
  void ReadFourier(istream & file);
  ///
  void WriteFourier(ostream & file);

protected:
  ///
  typedef CFourier * PFourier;
  ///
  CFourier DefaultFourier;
  ///
  PFourier FourierActive;
  ///
  TList<PFourier> FourierList;
