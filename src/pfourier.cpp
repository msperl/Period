/*
 * Program:     Period98
 *
 * File:        project.cpp
 * Purpose:     implementation-file for
 *              the fourier part of the project
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */

#include <fstream.h>
#include <string.h>

#ifdef wx_msw
#include <strstrea.h>
#else
#include <strstream.h>
#endif

#include "project.h"
#include "dfnoise.h"

#include "ffourier.h"
#include "ldialtxt.h"

void CProject::CleanFourier()
{
  FourierList.clean();
  FourierActive=&DefaultFourier;
}

void CProject::RenameActiveFourier(myString NewName)
{
  FourierActive->SetTitle(NewName);
  Changed++; 
}

void CProject::DeleteActiveFourier()
{
  if (FourierActive==&DefaultFourier)
    { return; }
  for (int i=0;i<FourierList.entries();i++)
    {
      if (FourierList[i]==FourierActive)
	{
	  // remove Plots
	  CloseDisplayID(FourierActive->GetFourierID());
	  // we have found the entry, let's remove it
	  delete FourierActive;
	  // and also from the list
	  FourierList.removeat(i);
	  // Now activate a plot
	  ActivateFourier(0);
	  // now return
	  Changed++; 
	  return;
	}
    }
}

void CProject::SaveFourier(int i,myString)
{
  myString File=GetFileName(0,
			    DIALOG_SAVE_FOURIER_TITLE,
			    "",
			    DIALOG_FOURIER_EXTENTION,
			    DIALOG_FOURIER_PATTERN);
  if (File=="")
    {
      // No filename given, let's return...
      return;
    }
  // check if we can write a file
  if (CanWrite(File)==0)
    {
      char text[1024];
      sprintf(text,DIALOG_CANT_WRITE_FILE,File.chars());
      InformUser(text);
      return;
    }
  // Save file...
  ofstream datafile(File.chars());
  FourierList[i]->WriteData(datafile);
  // write log
  char txt[1024];
  sprintf(txt,PROTOCOL_SAVE_FOURIER,
	  FourierList[i]->GetTitle().chars(),File.chars());
  Write()<<txt<<endl;
}
void CProject::LoadFourier(myString name)
{
  if (FileExist(name)==0)
    {
      char text[1024];
      sprintf(text,DIALOG_FILE_DOES_NOT_EXIST,name.chars());
      InformUser(text);
      return;
    }
}

void CProject::ReadFourier(istream &ist)
{
  char c;
  while (ist>>c,ist.putback(c),c!='[')
    {
      // allocate memory
      CFourier *tmp= new CFourier("noname");
      // Add to list
      FourierList.addtail(tmp);
      // reading info...
      tmp->ReadHeader(ist);
    }
  //  ist.putback(c);
}

void CProject::WriteFourier(ostream &ost)
{
  for (int i=0;i<FourierList.entries();i++)
    {
      // OK lets save the data
      ost<<"[Fourier]\n";
      FourierList[i]->WriteHeader(ost);
      ost<<endl;
    }
}

double CProject::GetStepRate(CFourier::StepQuality qstep,double step) const
{
  double tmp=Timestring.BaseLine();
  if (tmp==0)
    {
      // no baseline, no steps
      return 0;
    }
  // now calculate reciprocal
  tmp=1/tmp;
  // and multiply with weight
  switch (qstep)
    {
    case CFourier::Low:
      tmp/=10; break;
    case CFourier::Medium: 
      tmp/=15; break;
    case CFourier::High:
      tmp/=20; break;
    default:
      tmp=step;break;
    }
  return tmp;
}

char* CProject::GetStepQualityLabel(CFourier::StepQuality mode)
{
  switch (mode)
    {
    case CFourier::Low:    return QUALITY_FOURIER_LOW;
    case CFourier::Medium: return QUALITY_FOURIER_MEDIUM;
    case CFourier::High:   return QUALITY_FOURIER_HIGH;
    case CFourier::Custom: return QUALITY_FOURIER_CUSTOM;
    }
  return QUALITY_FOURIER_CUSTOM;
}

int CProject::FourierCheck(double *from, double *to, int *weight)
{
  if (Timestring.GetSelectedPoints()<5)
  {
    InformUser(FOU_NOTENOUGHPOINTS);
    return 1;
  }
  // check if weights should be used
  if (!Period.GetUseWeight())
    {
      if (Timestring.GetSelectedPoints()!=Timestring.GetWeightSum())
	{
	  if (Confirm(DIALOG_USE_WEIGHTS)==1)
	    { *weight=1; }
	}
    }
  // check if to is higher than nyquist
  if (*to>GetNyquist())
    {
      char text[2048];
      sprintf(text,FOU_HIGHERTHANNYQUIST,*to,GetNyquist());
      if (!Confirm(text))
	{return 1;}
    }
  return 0;
}

void CProject::CalculateFourier(myString title,
				double from,double to,
				CFourier::StepQuality stepq,double step,
				DataMode Mode,
				CompactMode Compact,int weight)
{
  // check for ranges
  if (FourierCheck(&from,&to,&weight)) { return;}
  Waiting(1,1);

  // check for equality
  int i;
  for(i=0;i<FourierList.entries();i++)
    {
      if (FourierList[i]->GetTitle()==title)
	{
	  // if so add a "+" to the title
	  title+="+";
	}
    }

  double steprate=GetStepRate(stepq,step);

  // create the fourier-data
  CFourier *tmp=new CFourier(title,
			     from,to,
			     stepq,steprate,
			     Mode,
			     Compact,weight);

  // write the start of the protocol

  char tempprot[1024];
  for (i=0;i<1024;i++) { tempprot[i]=0; }
  ostrstream pro(tempprot,1024);
  char txt[1024];
  pro<<Date()<<endl;
  if (stepq==CFourier::Custom)
    {
      sprintf(txt,PROTOCOL_FOURIER_CALC_CUSTOMQUALITY,
	      title.chars(),
	      from,
	      to,
	      steprate,
	      Date().chars());
    }
  else
    {
      sprintf(txt,PROTOCOL_FOURIER_CALC_PRESETQUALITY,
	      title.chars(),
	      from,
	      to,
	      GetStepQualityLabel(stepq),
	      steprate,Date().chars());
    }    
  pro<<txt<<endl;
  char *weighttmp="";
  if (weight)
    {
      weighttmp=PROTOCOL_FOURIER_WEIGHTED;
    }
  switch (Mode)
    {
    case Observed:
      sprintf(txt,PROTOCOL_FOURIER_CALC_USE_ORIG,
	      Timestring.GetSelectedPoints(),
	      weighttmp);
      break;
    case Adjusted:
      sprintf(txt,PROTOCOL_FOURIER_CALC_USE_ADJUSTED,
	      Timestring.GetSelectedPoints(),
	      weighttmp);
      break;
    case SpectralWindow:
      sprintf(txt,PROTOCOL_FOURIER_CALC_USE_WINDOW,
	      Timestring.GetSelectedPoints(),
	      weighttmp);
      break;
    case DataResiduals:
      sprintf(txt,PROTOCOL_FOURIER_CALC_USE_RESORIG,
	      Timestring.GetSelectedPoints(),
	      weighttmp,
	      Period.GetActiveFrequencies());
      break;
    case AdjustedResiduals:
      sprintf(txt,PROTOCOL_FOURIER_CALC_USE_RESADJU,
	      Timestring.GetSelectedPoints(),
	      weighttmp,
	      Period.GetActiveFrequencies());
      break;
    case Calculated:
      break;
    }
  pro<<txt<<endl;
  // find out if Zeropoint should be subtracted
  double zero=0;
  if ((Mode==Observed)||(Mode==Adjusted))
    {
      DataMode old=Timestring.GetDataMode();
      Timestring.SetDataMode(Mode);
      zero=Timestring.Average(weight);
      sprintf(txt,DIALOG_FOURIER_SHALL_SUBTRACT_ZERO,zero);
      if (Confirm(txt))
	{
	  Timestring.SetDataMode(Mode);
	  // add in protocol
	  sprintf(txt,PROTOCOL_FOURIER_SUBTRACT_ZERO,zero);
	  pro<<txt<<endl;
	}
      Timestring.SetDataMode(old);
    }
  // make the calculations
  tmp->Transform(Timestring,zero);
  // write the end of the protocol
  if (Compact==AllData)
    { sprintf(txt,PROTOCOL_FOURIER_CALC_POINTS,tmp->Points()); }
  else
    { sprintf(txt,PROTOCOL_FOURIER_CALC_PEAK_POINTS,tmp->Points()); }
  pro<<txt<<endl;

  // create peak-result in string
  CPeriPoint peak=tmp->GetPeak();
  char peakstr[256];
  sprintf(peakstr,PROTOCOL_FOURIER_HIGHEST_AMPLITUDE,
	  peak.GetFrequency(),
	  peak.GetAmplitude(-1));
  pro<<peakstr<<endl;
  Write()<<pro.str()<<endl;

  if (Mode!=SpectralWindow)
    {
      char text[512];
      sprintf(text,DIALOG_FOURIER_INCLUDE_FREQUENCY,peakstr);
      
      // ask if the peak should be included in Frequency-list
      if (Confirm(text)==1)
	{
	  // add peak to frequency-list
	  int i=Period.FindEmpty();
	  Period[i]=tmp->GetPeak();
	  // and DEACTIVATE it
	  Period[i].SetActive(0);
	  // write to protocol
	  sprintf(text,PROTOCOL_FOURIER_INCLUDE,i+1);
	  Write()<<text<<endl;;
	}
    }
  // add calculations to the Fourier-list
  FourierList.addtail(tmp);
  FourierActive=tmp;
  Waiting(0);
}

int CProject::CalculateNoiseSpectra(double from, double to, double CalcDist,
				    double box, 
				    CFourier::StepQuality stepq,double step, 
				    DataMode Mode, int weight,
				    double *freq,double *noise)
{
  // check for ranges
  if (FourierCheck(&from,&to,&weight)) { return -1;}
  Waiting(1,1);

  char tempprot[8192];
  // clean array
  for (int ttt=0;ttt<8192;ttt++) { tempprot[ttt]=0; }
  ostrstream pro(tempprot,8192);
  char txt[1024];
  double steprate=GetStepRate(stepq,step);
  if (step>0)
    {
      sprintf(txt,PROTOCOL_NOISESPEC_CALC_CUSTOMQUALITY,
	      from,
	      to,
	      CalcDist,
	      box,
	      steprate,
	      Date().chars());
    }
  else
    {
      sprintf(txt,PROTOCOL_NOISESPEC_CALC_PRESETQUALITY,
	      from,
	      to,
	      CalcDist,
	      box,
	      GetStepQualityLabel(stepq),
	      steprate,
	      Date().chars());
    }
  pro<<txt<<endl;
  char *weighttmp="";
  if (weight)
    {
      weighttmp=PROTOCOL_FOURIER_WEIGHTED;
    }
  switch (Mode)
    {
    case Observed:
      sprintf(txt,PROTOCOL_FOURIER_CALC_USE_ORIG,
	      Timestring.GetSelectedPoints(),weighttmp);
      break;
    case Adjusted:
      sprintf(txt,PROTOCOL_FOURIER_CALC_USE_ADJUSTED,
	      Timestring.GetSelectedPoints(),weighttmp);
      break;
    case SpectralWindow:
      sprintf(txt,PROTOCOL_FOURIER_CALC_USE_WINDOW,
	      Timestring.GetSelectedPoints(),weighttmp);
      break;
    case DataResiduals:
      sprintf(txt,PROTOCOL_FOURIER_CALC_USE_RESORIG,
	      Timestring.GetSelectedPoints(),weighttmp,Period.GetActiveFrequencies());
      break;
    case AdjustedResiduals:
      sprintf(txt,PROTOCOL_FOURIER_CALC_USE_RESADJU,
	      Timestring.GetSelectedPoints(),weighttmp,Period.GetActiveFrequencies());
      break;
    case Calculated:
      break;
    }
  pro<<txt<<endl;
  double zero=0;
  if ((Mode==Observed)||(Mode==Adjusted))
    {
      DataMode old=Timestring.GetDataMode();
      Timestring.SetDataMode(Mode);
      zero=Timestring.Average(weight);
      sprintf(txt,DIALOG_NOISE_SHALL_SUBTRACT_ZERO,zero);
      if (Confirm(txt))
	{
	  Timestring.SetDataMode(Mode);
	  // add in protocol
	  sprintf(txt,PROTOCOL_FOURIER_SUBTRACT_ZERO,zero);
	  pro<<txt<<endl;
	}
      Timestring.SetDataMode(old);
    }

  // make the calculations

  int pos=0;

  // find out how many steps
  int steps=0;
  double p;
  for (p=from+CalcDist/2;p<to;p+=CalcDist)
    {
      steps++;
    }
  
  double PercentageStart=0;
  double PercentageStep=100.0/(steps+1);
  
  pro<<PROTOCOL_NOISESPEC_HEAD<<endl;

  for (p=from+CalcDist/2;p<to;p+=CalcDist)
    {
      double Noise=NoiseCalc(Timestring,zero,p-box/2,p+box/2,
			     steprate,weight,
			     Mode,
			     PercentageStart,PercentageStep);
      // check if chacel has been pressed
      if (Noise==-1)
	{
	  // if so then quit function
	  Waiting(0);
	  return -1;
	}
      char txt[256];
      sprintf(txt,PROTOCOL_NOISESPEC_FORMAT,p,Noise);
      pro<<txt<<endl;
      freq[pos]=p;
      noise[pos]=Noise;
      pos++;
      PercentageStart+=PercentageStep;
    }

  Write()<<pro.str()<<endl;
  Waiting(0);
  return pos;
}

double CProject::CalculateNoise(double point, double box, 
				CFourier::StepQuality stepq,double step,
				DataMode Mode,int weight)
{
  if (FourierCheck(&point,&point,&weight)) { return -1.0;}
  Waiting(1,1);
  // Here goes the work...
  // write the start of the protocol

  char tempprot[1024];
  ostrstream pro(tempprot,1024);
  char txt[1024];
  double steprate=GetStepRate(stepq,step);
  if (step>0)
    {
      sprintf(txt,PROTOCOL_NOISE_CALC_CUSTOMQUALITY,
	      point,
	      box,
	      steprate,
	      Date().chars());
    }
  else
    {
      sprintf(txt,PROTOCOL_NOISE_CALC_PRESETQUALITY,
	      point,
	      box,
	      GetStepQualityLabel(stepq),
	      steprate,
	      Date().chars());
    }
  pro<<txt<<endl;
  char *weighttmp="";
  if (weight)
    {
      weighttmp=PROTOCOL_FOURIER_WEIGHTED;
    }
  switch (Mode)
    {
    case Observed:
      sprintf(txt,PROTOCOL_FOURIER_CALC_USE_ORIG,
	      Timestring.GetSelectedPoints(),weighttmp);
      break;
    case Adjusted:
      sprintf(txt,PROTOCOL_FOURIER_CALC_USE_ADJUSTED,
	      Timestring.GetSelectedPoints(),weighttmp);
      break;
    case SpectralWindow:
      sprintf(txt,PROTOCOL_FOURIER_CALC_USE_WINDOW,
	      Timestring.GetSelectedPoints(),weighttmp);
      break;
    case DataResiduals:
      sprintf(txt,PROTOCOL_FOURIER_CALC_USE_RESORIG,
	      Timestring.GetSelectedPoints(),weighttmp,Period.GetActiveFrequencies());
      break;
    case AdjustedResiduals:
      sprintf(txt,PROTOCOL_FOURIER_CALC_USE_RESADJU,
	      Timestring.GetSelectedPoints(),weighttmp,Period.GetActiveFrequencies());
      break;
    case Calculated:
      break;
    }
  pro<<txt<<endl;
  // Subtract zeropoint
  double zero=0;
  if ((Mode==Observed)||(Mode==Adjusted))
    {
      DataMode old=Timestring.GetDataMode();
      Timestring.SetDataMode(Mode);
      zero=Timestring.Average(weight);
      sprintf(txt,DIALOG_NOISE_SHALL_SUBTRACT_ZERO,zero);
      if (Confirm(txt))
	{
	  Timestring.SetDataMode(Mode);
	  // add in protocol
	  sprintf(txt,PROTOCOL_FOURIER_SUBTRACT_ZERO,zero);
	  pro<<txt<<endl;
	}
      Timestring.SetDataMode(old);
    }

  // make the calculations
  double Noise=NoiseCalc(Timestring,zero,point-box/2,point+box/2,
			 steprate,weight,Mode);
  // check if chacel has been pressed
  if (Noise==-1)
    {
      // if so then quit function
      return -1;
    }
  sprintf(txt,PROTOCOL_NOISE_RESULT,Noise);
  pro<<txt<<endl;

  Write()<<pro.str()<<endl;
  
  Waiting(0);

  return Noise;
}




