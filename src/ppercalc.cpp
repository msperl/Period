/*
 * Program:     Period98
 *
 * File:        project.cpp
 * Purpose:     implementation-file for
 *              the period part of the project
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */

#include <string.h>

#include "project.h"

#include "fperiod.h"
#include "ldialtxt.h"

#ifdef wx_msw
#include <strstrea.h>
#else
#include <strstream.h>
#endif

void CProject::GeneralCalcPeriod(DataMode mode)
{
  if (!((mode==Observed)||(mode==Adjusted)))
    {
      MYERROR("Wrong calculation-mode..."<<(int)mode);
      return;
    }

  DataMode old=Timestring.GetDataMode();
  Timestring.SetDataMode(mode);
  Period.SetUseData(mode);

  // check if weights should be used
  if (!Period.GetUseWeight())
    {
      if (Timestring.GetSelectedPoints()!=Timestring.GetWeightSum())
	{
	  if (Confirm(DIALOG_PERIOD_USE_WEIGHTS)==1)
	    { Period.SetUseWeight(1); }
	}
    }
  Waiting(1,1);
  // Write Calculation-header
  char tempprot[10240];
  char txt[256];
  ostrstream pro(tempprot,1024);
  pro<<PROTOCOL_PERIOD_CALCULATE_HEADER<<endl;
  //WriteFrequenciesTabulated(pro,1);
  //sprintf(txt,PROTOCOL_ZEROPOINT,Period.GetZeropoint());
  //pro<<txt<<endl;
  if (Period.GetUseWeight())
    { pro<<PROTOCOL_PERIOD_USE_WEIGHTS<<endl; }
  //
  if (mode==Observed)
    { pro<<PROTOCOL_PERIOD_USE_OBSERVED<<endl; }
  else
    { pro<<PROTOCOL_PERIOD_USE_ADJUSTED<<endl; }
  // 
  pro<<PROTOCOL_PERIOD_CALCULATION_STARTED<<Date()<<endl;
  pro<<char(0)<<flush;
  // Calculate
  switch(Period.Calc(Timestring))
    {
    case 0:// normal calculations done
      {
	// Write Calculation-footer
	ostream &tmp=Write();
	tmp<<pro.str();
	tmp<<PROTOCOL_PERIOD_OUTPUT<<endl;
	WriteFrequenciesTabulated(tmp,1);
	sprintf(txt,PROTOCOL_PERIOD_RESULT,
		Period.GetZeropoint(),
		Period.GetResiduals(),
		Period.GetIterations());
	tmp<<txt<<endl;
	if (Period.GetIterations()>=Period.MaxIterations())
	  {
	    InformUser(DIALOG_PERIOD_MAXIMUM_ITERATIONS);	  
	  }
	break;
      }
    
    case 1:// Matrix cannot be inverted
      {
	InformUser(DIALOG_PERIOD_MATRIX_INVERSION);
	break;
      }
    case 2:// cancel pressed, may not be a stable solution
      {
	InformUser(DIALOG_PERIOD_CALCULATION_INTERRUPTED);
	break;
      }
    }
  Waiting(0);
  Timestring.SetDataMode(old);
}

void CProject::CalculatePeriod(DataMode mode)
{
  UpdatePEData();
  // check if we have got enough points
  if (GetSelectedPoints()==0)
    { InformUser(DIALOG_PERIOD_NOT_ENOUGH_POINTS);return; }
  // set Zeropoint
  Period.SetZeropoint(Timestring.Average(Period.GetUseWeight()));
  // store the selection
  Period.StoreSelection();
  // select all amplitudes and phases
  Period.SelectDefault();
  // calculate
  GeneralCalcPeriod(mode);
  // and restore selection
  Period.RestoreSelection();
  // and update all displays
  UpdatePEDisplays();
}

void CProject::ImprovePeriod(DataMode mode)
{
  UpdatePEData();
  // check if we have got enough points
  if (GetSelectedPoints()==0)
    { InformUser(DIALOG_PERIOD_NOT_ENOUGH_POINTS);return; }
  // make a calculate first
  CalculatePeriod(mode);
  // now do an improve
  // store the selection
  Period.StoreSelection();
  // select all frequencies, amplitudes and phases
  Period.SelectAll();
  // calculate
  GeneralCalcPeriod(mode);
  // and restore selection
  Period.RestoreSelection();
  // and update all displays
  UpdatePEDisplays();
}

void CProject::ImproveSpecialPeriod(DataMode mode)
{
  UpdateAllData();
  if (GetSelectedPoints()==0)
    { InformUser(DIALOG_PERIOD_NOT_ENOUGH_POINTS);return; }
  if (GetActiveFrequencies()==0)
    { InformUser(DIALOG_PERIOD_NO_FREQUENCIES);return; }
  int result=GetPeriodSelection();
  if (result==1)
    {
      GeneralCalcPeriod(mode);
    }
  if (result==2)
    {
      // set new Datamode mode
      DataMode old=Timestring.GetDataMode();
      Timestring.SetDataMode(mode);
      Period.SetUseData(mode);
      // Amplitude Variation
      int freqs=0,what=0;
      CalcMode cmode=AmpVar;
      int* tmp=GetAmpVarData(&freqs,&what,&cmode);
      if (tmp==NULL)
	{
	  // normal calculation
	  // GeneralCalcPeriod(mode);
	  // do not calculate at all
	}
      else
	{
	  // Amplitude Variations...
	  Period.StoreSelection();
	  CalculateAmpVarPeriod(freqs,tmp,what,cmode);
	  Period.RestoreSelection();
	}
      // restore old datamode
      Period.SetUseData(old);
    }
  // and update all displays
  UpdatePEDisplays();
}

#define AMPVARMSGBUFFER 65000

void CProject::CalculateAmpVarPeriod(int freqpnt, int *freqdat, 
				     int what, CalcMode mode)
{
  UpdatePEData();
  // check if weights should be used
  if (!Period.GetUseWeight())
    {
      if (Timestring.GetSelectedPoints()!=Timestring.GetWeightSum())
	{
	  if (Confirm(DIALOG_PERIOD_USE_WEIGHTS)==1)
	    { Period.SetUseWeight(1); }
	}
    }

  // Set Frequencies for Amplitude Variations
  int i;
  for (i=0;i<freqpnt;i++)
    {
      Period[freqdat[i]].SetAmpVar(mode);
    }
  // what names should be used ?
  Period.SetUseID(what);

  // PROTOCOL

  // Write Calculation-header
  char tempprot[AMPVARMSGBUFFER];
  for (i=0;i<AMPVARMSGBUFFER;i++) { tempprot[i]=0; }
  char txt[1024];
  ostrstream pro(tempprot,AMPVARMSGBUFFER);
  pro<<PROTOCOL_AMPVAR_CALCULATE_HEADER<<endl;
  //WriteFrequenciesTabulated(pro,1);
  //sprintf(txt,PROTOCOL_ZEROPOINT,Period.GetZeropoint());
  //pro<<txt<<endl;
  if (Period.GetUseWeight())
    { pro<<PROTOCOL_PERIOD_USE_WEIGHTS<<endl; }
  pro<<PROTOCOL_AMPVAR_NAMES_SELECTED<<NameSet(what)<<endl;
  pro<<PROTOCOL_PERIOD_CALCULATION_STARTED<<Date()<<endl;
  pro<<char(0)<<flush;  
  Waiting(1,1);
  // Calculate
  switch (Period.Calc(Timestring))
    {
    case 0:
      {
	// Write Calculation-footer
	ostream &tmp=Write();
	tmp<<pro.str();
	tmp<<PROTOCOL_PERIOD_OUTPUT<<endl;
	// now write the protocol
	char txt1[AMPVARMSGBUFFER];
	for (i=0;i<AMPVARMSGBUFFER;i++) { txt1[i]=0; }
	ostrstream pro1(txt1,AMPVARMSGBUFFER);
	
	// general data..
	CTimeString &Timestring=GetTimeString();
	CPeriod const &Period=GetPeriod();
	
	int freqs=Period.GetFrequencies();
	
	// now fill in the data...
	for (int fre=0;fre<freqs;fre++)
	  {
	    int fr=fre;
	    if (Period[fr].GetActive())
	      {
		switch (Period[fr].GetAmpVariation())
		  {
		  case NoVar:
		    {
		      sprintf(txt,PROTOCOL_AMPVAR_NOVAR,
			      GetNumber(fr).chars(),
			      GetFrequency(fr).chars(),
			      Period[fr].GetAmplitude(-1),
			      Period[fr].GetPhase(-1)
			      );
		      // write out
		      pro1<<txt<<endl;
		      tmp<<txt<<endl;
		      break;
		    }
		  case AmpVar:
		    {
		      // first write out the header
		      sprintf(txt,PROTOCOL_AMPVAR_AMPVAR_HEAD,
			      GetNumber(fr).chars(),
			      GetFrequency(fr).chars(),
			      Period[fr].GetPhase(-1)
			      );
		      // write out
		      pro1<<txt<<endl;
		      tmp<<txt<<endl;
		      // now write out the relevant data
		      for (int na=0;na<Timestring.NumberOfNames(what);na++)
			{
			  int id=
			    Timestring.GetIndexName(what,na).GetID();
			  int points=
			    Timestring.GetIndexName(what,na).GetPoints();
			  myString name=
			    Timestring.GetIndexName(what,na).GetName();
			  if (points!=0)
			    {
			      sprintf(txt,PROTOCOL_AMPVAR_AMPVAR,
				      name.chars(),
				      Period[fr].GetAmplitude(id)
				      );
			      // write out
			      pro1<<txt<<endl;
			      tmp<<txt<<endl;
			    }
			}
		      break;  
		    }
		  case PhaseVar:
		    {
		      // first write out the header
		      sprintf(txt,PROTOCOL_AMPVAR_PHASEVAR_HEAD,
			      GetNumber(fr).chars(),
			      GetFrequency(fr).chars(),
			      Period[fr].GetAmplitude(-1)
			      );
		      // write out
		      pro1<<txt<<endl;
		      tmp<<txt<<endl;
		      // now write out the relevant data
		      for (int na=0;na<Timestring.NumberOfNames(what);na++)
			{
			  int id=
			    Timestring.GetIndexName(what,na).GetID();
			  int points=
			    Timestring.GetIndexName(what,na).GetPoints();
			  myString name=
			    Timestring.GetIndexName(what,na).GetName();
			  if (points!=0)
			    {
			      sprintf(txt,PROTOCOL_AMPVAR_PHASEVAR,
				      name.chars(),
				      Period[fr].GetPhase(id)
				      );
			      // write out
			      pro1<<txt<<endl;
			      tmp<<txt<<endl;
			    }
			}
		      break;  
		    }
		  case AllVar:
		    {
		      // first write out the header
		      sprintf(txt,PROTOCOL_AMPVAR_ALLVAR_HEAD,
			      GetNumber(fr).chars(),
			      GetFrequency(fr).chars()
			      );
		      // write out
		      pro1<<txt<<endl;
		      tmp<<txt<<endl;
		      // now write out the relevant data
		      for (int na=0;na<Timestring.NumberOfNames(what);na++)
			{
			  int id=
			    Timestring.GetIndexName(what,na).GetID();
			  int points=
			    Timestring.GetIndexName(what,na).GetPoints();
			  myString name=
			    Timestring.GetIndexName(what,na).GetName();
			  if (points!=0)
			    {
			      sprintf(txt,PROTOCOL_AMPVAR_ALLVAR,
				      name.chars(),
				      Period[fr].GetAmplitude(id),
				      Period[fr].GetPhase(id)
				      );
			      // write out
			      pro1<<txt<<endl;
			      tmp<<txt<<endl;
			    }
			}
		      break;  
		    }
		  }
	      }
	  }
	
	// now write the rest..
	sprintf(txt,PROTOCOL_PERIOD_RESULT,
		Period.GetZeropoint(),
		Period.GetResiduals(),
		Period.GetIterations());
	pro1<<txt<<flush;
	tmp<<txt<<flush;
	// Have we reached Maximum Number Iterations?
	if (Period.GetIterations()>=Period.MaxIterations())
	  {
	    InformUser(DIALOG_PERIOD_MAXIMUM_ITERATIONS);	  
	  }
	// now display the data...
	DisplayAmpVarData(pro1.str());
	break;
      }
    case 1:// Matrix cannot be inverted
      {
	InformUser(DIALOG_PERIOD_MATRIX_INVERSION);
	break;
      }
    case 2:// cancel pressed, may not be a stable solution
      {
	InformUser(DIALOG_PERIOD_CALCULATION_INTERRUPTED);
	break;
      }
    }
  
  Waiting(0);
  // Cleans Frequencies of Amplitude Variations
  for (i=0;i<freqpnt;i++)
    {
      Period[freqdat[i]].SetAmpVar(NoVar);
    }
  UpdatePEDisplays();
}
