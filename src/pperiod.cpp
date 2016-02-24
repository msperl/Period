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
#include "mswdefs.h"

void CProject::CleanPeriod()
{
  CPeriPoint empty;
  // deactivate this period
  empty.SetActive(0);
  // now copy it everywhere..
  for (int i=0;i<Period.GetFrequencies();i++)
    {
      Period[i]=empty;
    }
}

myString CProject::GetNumber(int i) const
{
  char tmp[32];
  sprintf(tmp,FORMAT_NUMBER,Period[i].GetNumber()+1);
  return tmp;
}

myString CProject::GetFrequency(int i) const 
{
  /*
  // is this frequency a composition
  if (Period[i].IsComposition())
    {
      // return composition-string
      return Period[i].GetCompositeString();
    }
  */
  // return value
  char tmp[32];
  sprintf(tmp,FORMAT_FREQUENCY,Period[i].GetFrequency());
  return tmp;
}

void CProject::SetFrequency(int fr, myString f)
{
  int i=f.length();
  char *tmp=(char*)f.chars();
  int isCompo=0;
  for (int j=0;j<i;j++)
    {
      if (f[j]=='=') { isCompo=1; break;}
    }
  int count=0;
  for (i=i-1;i>=0;i--)
    {
      if (tmp[i]=='+')
	{
	 count++;
	 tmp[i]=' ';
	}
      else if (tmp[i]=='-')
	{
	  count--;
	  tmp[i]=' ';
	}
      else if (tmp[i]!=' ')
	{
	  break;
	}
    }

  if (isCompo)
    {
      Period.SetCompositeString(fr,f.chars());
      return;
    }
  if (count==0)
    {
      double fre;
      if (sscanf(f.chars(),"%lf",&fre))
	{
	  // set unchanged
	  Period.SetFrequency(fr,fre);
	}
    }
  else
    {
      double freqold;
      if (sscanf(f.chars(),"%lf",&freqold))
	{
	  double freqnew=freqold+count*freqstep;
	  char txt[1024];
	  sprintf(txt,PROTOCOL_CHANGE_FREQUENCY,
		  fr+1,count,freqstep,freqold,freqnew);
	  Write()<<txt<<endl;
	  // now set it...
	  Period.SetFrequency(fr,freqnew);
	}
    }
}

myString CProject::GetAmplitude(int i, int what) const
{
  char tmp[32];
  sprintf(tmp,FORMAT_AMPLITUDE,Period[i].GetAmplitude(what));
  return tmp;
}

void CProject::SetAmplitude(int i, myString a)
{
  double tmp;
  if (sscanf(a.chars(),"%lf",&tmp))
    {
      // if scanned ok, let's set it
      Period[i].SetAmplitude(tmp,-1);
    }
}

myString CProject::GetPhase(int i,int what) const
{
  char tmp[32];
  sprintf(tmp,FORMAT_PHASE,Period[i].GetPhase(what));
  return tmp;
}

void CProject::SetPhase(int i, myString p)
{
  double tmp;
  if (sscanf(p.chars(),"%lf",&tmp))
    {
      // if scanned ok, let's set it
      Period[i].SetPhase(tmp,-1);
    }
}

myString CProject::GetResiduals() const
{
  char tmp[32];
  sprintf(tmp,FORMAT_RESIDUALS,Period.GetResiduals());
  return tmp;
}

myString CProject::GetZeropoint() const
{
  char tmp[32];
  sprintf(tmp,FORMAT_ZEROPOINT,Period.GetZeropoint());
  return tmp;
}

void CProject::SetFrequencyAdjustment(myString step)
{
  double tmp=DoubleFromString(step);
  if (tmp!=0.0)
    {
      freqstep=tmp;
    }
}

myString CProject::GetFrequencyAdjustment() const
{
  char txt[32];
  sprintf(txt,FORMAT_FREQUENCY,freqstep);
  return txt;
}

myString CProject::GetActiveFrequenciesString() const
{
  char txt[32];
  sprintf(txt,FORMAT_ACTIVE_FREQUENCIES,Period.GetActiveFrequencies());
  return txt;
}

void CProject::LoadPeriod(myString name)
{
  // get name if necessary
  if (name=="")
    {
      name=GetFileName(1,
		       DIALOG_LOAD_FREQUENCIES_TITLE,
		       "",
		       DIALOG_PERIOD_EXTENTION,
		       DIALOG_PERIOD_PATTERN);
    }
  if (name=="")
    {
      // no filename given?
      return;
    }

  // test if file exists
  if (FileExist(name)==0)
    {
      char text[1024];
      sprintf(text,DIALOG_FILE_DOES_NOT_EXIST,name.chars());
      InformUser(text);
      return;
    }
  
  // load data
  Period.Load(name);
  
  // write to protocol
  ostream &pro=Write();
  // write the header
  char txt[1024];
  sprintf(txt,PROTOCOL_LOADED_FREQUENCIES_FROM,name.chars());
  pro<<txt<<endl;
  // write out the Frequencies
  WriteFrequenciesTabulated(pro,0);
  // update displays
  UpdatePEDisplays();
}

void CProject::SavePeriod(myString name)
{
  UpdatePEData();
  // get name if necessary
  if (name=="")
    {
      name=GetFileName(0,
		       DIALOG_SAVE_FREQUENCIES_TITLE,
		       Period.GetFileName().chars(),
		       DIALOG_PERIOD_EXTENTION,
		       DIALOG_PERIOD_PATTERN);
    }
  if (name=="")
    {
      // no filename given?
      return;
    }
  // check if we can write a file
  if (CanWrite(name)==0)
    {
      char text[1024];
      sprintf(text,DIALOG_CANT_WRITE_FILE,name.chars());
      InformUser(text);
      return;
    }

  // save data
  Period.Save(name);

  // write to protocol
  char txt[1024];
  sprintf(txt,PROTOCOL_SAVED_FREQUENCIES_TO,name.chars());
  Write()<<txt<<endl;
}

myString CProject::Predict(myString t)
{
  UpdatePEData();
  double time=DoubleFromString(t);
  double pre=Period.Predict(time,-1);
  char txt[1024];
  sprintf(txt,PROTOCOL_PREDICT,time,pre);
  Write()<<txt<<endl;
  char tmp[32];
  sprintf(tmp,FORMAT_AMPLITUDE,pre);
  return tmp;
}

myString CProject::ShowEpoch(myString t,int Intensity)
{
  UpdateAllData();
  double time=DoubleFromString(t);
  // empty result
  myString result="";
  // fill result
  char tmp[256];
  for (int i=0;i<GetTotalFrequencies();i++)
    {
      if (Period[i].GetActive())
	{
	  // write out line
	  sprintf(tmp,PROTOCOL_EPOCH_FORMAT"\n",
		  GetNumber(i).chars(),
		  GetFrequency(i).chars(),
		  Period[i].CalcEpoch(time,Intensity)
		  );
	  result+=tmp;
	}
    }
  // write protocol
  if (Intensity)
    {
      sprintf(tmp,PROTOCOL_EPOCH_INTENSITY,time);
    }
  else
    {
      sprintf(tmp,PROTOCOL_EPOCH_MAGNITUDE,time);
    }
   
  Write()<<tmp
	 <<PROTOCOL_EPOCH_FORMAT_HEADER<<endl
	 <<result<<endl;
  // return result
  return result;
}

void CProject::CreateArtificialData(myString file,
				    myString Sfrom,
				    myString Sto,
				    myString Sstep,
				    myString Sleading,
				    int append)
{
  UpdatePEData();
  // convert data
  double from=DoubleFromString(Sfrom);
  double to=DoubleFromString(Sto);
  double step=DoubleFromString(Sstep);
  double leading=DoubleFromString(Sleading);
  // create data
  Period.CreateArtificialData(file,from,to,&step,leading,append);
  // Create protocol
  if (append)
    {
      char txt[1024];
      sprintf(txt,PROTOCOL_ARTIFICIAL_DATA_APPEND,file.chars(),from,to,step,leading);
      Write()<<txt<<endl;
    }
  else
    {
      char txt[1024];
      sprintf(txt,PROTOCOL_ARTIFICIAL_DATA_CREATE,file.chars(),from,to,step,leading);
      Write()<<txt<<endl;
    }
}

void CProject::WriteFrequenciesTabulated(ostream & str,int selected)
{
  char text[256];
  for (int i=0;i<Period.GetFrequencies();i++)
    {
      if (Period[i].GetActive())
        {
	  double fre,amp,pha;
	  char fresel=' ',ampsel=' ',phasel=' ';
	  int num=Period[i].GetNumber()+1;
	  // get values
	  fre=Period[i].GetFrequency();
	  amp=Period[i].GetAmplitude(0);
	  pha=Period[i].GetPhase(0);
	  // get labels
          if (selected && Period[i].IsSelected(Fre)) { fresel='*'; }
          if (selected && Period[i].IsSelected(Amp)) { ampsel='*'; }
          if (selected && Period[i].IsSelected(Pha)) { phasel='*'; }
          // is composition
	  if (Period[i].IsComposition()) 
	    { 
	      myString compo=Period[i].GetCompositeString(); 
	      sprintf(text,FORMAT_PERIOD_PROTOCOL_COMPO,
		      num,
		      fresel,compo.chars(),fresel,
		      ampsel,amp,ampsel,
		      phasel,pha,phasel,
		      fre);
	    }
	  else
	    {
	      sprintf(text,FORMAT_PERIOD_PROTOCOL,
		      num,
		      fresel,fre,fresel,
		      ampsel,amp,ampsel,
		      phasel,pha,phasel
		      );
	    }
	  str<<text<<endl;
        }
    }
}

void CProject::WritePeriod(ostream &ost)
{
  int oldprec=ost.precision(15);
  ost<<"Zeropoint=\t"<<Period.GetZeropoint()<<"\n";
  ost<<"Residuals=\t"<<Period.GetResiduals()<<"\n";
  ost<<"Aliasgap=\t"<<GetFrequencyAdjustment()<<"\n";
  ost<<Period<<"\n";
  ost.precision(oldprec);
}

void CProject::ReadPeriod(istream &ist)
{
  double dtmp;
  char c,tmp[256];
  Period.SetZeropoint(0);
  Period.SetResiduals(0);
  while(ist>>c,ist.putback(c),c!='[')
    {
      if ((c=='F')||(c=='f'))
	{
	  // load periods
	  ist>>Period;
	}
      else
	{
	  // read in argument
	  ist>>tmp;
	  if (strcasecmp(tmp,"Zeropoint=")==0)
	    {
	      ist>>dtmp;
	      Period.SetZeropoint(dtmp);
	    }
	  else if (strcasecmp(tmp,"Residuals=")==0)
	    {
	      ist>>dtmp;
	      Period.SetResiduals(dtmp);
	    }
	  else if (strcasecmp(tmp,"Aliasgap=")==0)
	    {
	      char adjust[256];
	      ist>>adjust;
	      SetFrequencyAdjustment(adjust);
	    }
	  else
	    {
	      MYERROR("Unknown Project-data in Period-part:\n"<<tmp);
	    }
	}
    }
}








