/*
 * Program:     Period98
 *
 * File:        project.cpp
 * Purpose:     implementation-file for
 *              the timestring part of the project
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */

#include <string.h>
#include <ctype.h>
#include <fstream.h>
#include "project.h"
#include "ftimestr.h"
#include "dcolors.h"
#include "ldialtxt.h"
#include "mswdefs.h"
void CProject::Select()
{
  // select data
  Timestring.Select();
  // selection has changed...
  sel_change++;
  // update all displays
  UpdateTSDisplays();
}

void CProject::SetReverseScale(int scale)
{ 
  Timestring.SetReverseScale(scale); 
  if (scale)
    {Write()<<PROTOCOL_SCALE_REVERSE<<endl;}
  else
    {Write()<<PROTOCOL_SCALE_NORMAL<<endl;}
  UpdateTSDisplays();
} 

void CProject::SetErrorScale(myString tmp)
{
  double value=DoubleFromString(tmp);
  if (value>0.0)
    {
      mErrorScale=value;
      UpdateTSDisplays();
      //
      char msg[1024];
      sprintf(msg,PROTOCOL_ERROR_SCALE,value);
      Write()<<msg<<endl;;
    }
}

void CProject::WriteSelection(ostream &ost)
{
  UpdateTSData();
  char txt[1024];
  for (int i=0;i<4;i++)
    {
      sprintf(txt,PROTOCOL_SELECTED_FROM,NameSet(i).chars());
      ost<<txt;
      int ent=NumberOfNames(i);
      for (int j=0;j<ent;j++)
	{
	  if (GetIndexName(i,j).GetSelect())
	    {
	      if (j!=0)
		{
		  ost<<", ";
		}
	      ost<<GetIndexName(i,j).GetName();
	    }
	}
      ost<<endl;
    }
  sprintf(txt,PROTOCOL_SELECTED_POINTS,
	  GetSelectedPoints(),
	  GetTotalPoints());
  ost<<txt<<endl;
  // everything has been written, so no new selection changes
  sel_change=0;
}


void CProject::Subdivide(myString Stimegap, int column,
			 int useCounter, myString prefix)
{
  double timegap=DoubleFromString(Stimegap);
  char txt[1024];
  // Do Subdivision
  Timestring.Subdivide(timegap,column,useCounter,prefix);
  // write Protocol...
  ostream &ost=Write();
  sprintf(txt,PROTOCOL_SUBDIVIDE_CURRENT_SELECTION,
	  Timestring.NameSet(column).chars(),
	  timegap,
	  prefix.chars());
  ost<<txt<<endl;
  if (useCounter)
    {
      ost<<PROTOCOL_SUBDIVIDE_COUNTER<<endl;
    }
  else
    {
      ost<<PROTOCOL_SUBDIVIDE_TIME<<endl;
    }
  UpdateTSDisplays();
}

void CProject::RenameSelection(int Column, myString Name)
{
  char txt[256];
  // Do Rename
  Timestring.RenameSelection(Column,Name);
  // write Protocol...
  sprintf(txt,PROTOCOL_RENAME,
	  Name.chars(),Timestring.NameSet(Column).chars());
  Write()<<txt<<endl;
  UpdateTSDisplays();
}

void CProject::Refit(myString zeropoint)
{
  // set the Zeropoint
  Period.SetZeropoint(DoubleFromString(zeropoint));
  // recalculate points
  Period.SetResiduals(Timestring);
  // writing protocol
  ostream &pro=Write();
  char txt[256];
  pro<<PROTOCOL_REFIT<<endl;
  Period.WriteSelected(pro);
  sprintf(txt,PROTOCOL_ZEROPOINT,Period.GetZeropoint());
  pro<<txt<<endl;
  sprintf(txt,PROTOCOL_RESIDUALS,Period.GetResiduals());
  pro<<txt<<endl;
  UpdateTSDisplays();
}

void CProject::ChangeHeading(int i,char *Name)
{
  // WRITE PROTOCOL
  char text[2048];
  sprintf(text,PROTOCOL_CHANGE_HEADING,
	  NameSet(i).chars(),Name);
  Write()<<text<<endl;
  // SET VALUES
  Timestring.ChangeNameSet(i,Name);
  UpdateTSDisplays();
}

void CProject::Adjust(int what,DataMode Orig,int n,int *ids,int useweights)
{
  char txt[1024];
  // Adjust Data
  Timestring.Adjust(what,Orig,n,ids,useweights);
  // write protocol
  ostream &ost=Write();
  
  if (Orig==Observed)
    {
      if (useweights)
	{
	  sprintf(txt,PROTOCOL_ADJUST_WEIGHTED_ORIGINAL,
		  NameSet(what).chars());
	}
      else
	{
	  sprintf(txt,PROTOCOL_ADJUST_ORIGINAL,
		  NameSet(what).chars());
	}	
    }
  else
    {
      if (useweights)
	{
	  sprintf(txt,PROTOCOL_ADJUST_WEIGHTED_ADJUSTED,
		  NameSet(what).chars());
	}
      else
	{
	  sprintf(txt,PROTOCOL_ADJUST_ADJUSTED,
		  NameSet(what).chars());
	}
    }
  ost<<txt<<endl;
  for (int i=0;i<n;i++)
    {
      double shift;
      if (Orig==Observed)
	{ 
	  if (useweights)
	    {
	      shift=GetIDName(what,ids[i]).GetAverageOrigWeight();
	    }
	  else
	    {
	      shift=GetIDName(what,ids[i]).GetAverageOrig();
	    }
	}
      else
	{
	  if (useweights)
	    {
	      shift=GetIDName(what,ids[i]).GetAverageOrigWeight();
	    }
	  else
	    {
	      shift=GetIDName(what,ids[i]).GetAverageOrig();
	    }
	}
      sprintf(txt,PROTOCOL_ADJUST_POINTS_ADJUSTED,
	      GetIDName(what,ids[i]).GetPoints(),
	      GetIDName(what,ids[i]).GetName().chars(),
	      shift);
      ost<<txt<<endl; 
    }

  if (Orig==Observed)
    { 
      InformUser(DIALOG_ADJUST_ORIGINAL_NEW_FIT);
    }
  else
    {
      InformUser(DIALOG_ADJUST_ADJUSTED_NEW_FIT);
    }
  UpdateTSDisplays();
}

myString CProject::GetNameStats(int id,int i,int useweighted,int *ident)
{
  char tmp[256];
  CName *data=&(GetIndexName(id,i));
  double sigma,average;
  sigma=0;
  average=0;
  char* isadj;
  *ident=0;
  if (data->GetSelect()&&(data->GetPoints()!=0))
    {
      if (useweighted)
	{
	  if (GetPeriodUseData()==Observed)
	    {
	      sigma=data->GetSigmaOrigWeight();
	      average=data->GetAverageOrigWeight();
	    }
	  else
	    {
	      sigma=data->GetSigmaAdjWeight();
	      average=data->GetAverageAdjWeight();
	    }
	}
      else
	{
	  if (GetPeriodUseData()==Observed)
	    {
	      sigma=data->GetSigmaOrig();
	      average=data->GetAverageOrig();
	    }
	  else
	    {
	      sigma=data->GetSigmaAdj();
	      average=data->GetAverageAdj();
	    }
	}
      // check if mean values are the same for both
	  isadj="!!!";
      if (fabs(data->GetAverageAdj()-data->GetAverageOrig())>1e-14)
	{
	  isadj=DIALOG_ADJUST_YES;
	}
      else 
	{
	  isadj=DIALOG_ADJUST_NO;
	}
      *ident=data->GetID();
    }
  else
    {
      *ident=-99;isadj="!!!";
    }
  sprintf(tmp,DIALOG_ADJUST_FORMAT,
	  data->GetName().chars(),
	  average,sigma,
	  data->GetPoints(),
	  isadj
	  );
  return tmp;
}

void CProject::SaveStat(int what,int weighted,myString filename)
{
  if (filename=="")
    {
      filename=GetFileName(0,DIALOG_ADJUST_SAVE_STAT);
    }
  if (filename=="")
    {return;}
  {
    ofstream file(filename.chars());
    file<<StatStrings(what,weighted,0);
  }
}

myString CProject::StatStrings(int what,int weighted,int header)
{
  myString tmp;
  if (header) { tmp=DIALOG_ADJUST_HEADER"\n"; }
  for (int i=0;i<NumberOfNames(what);i++)
    {
      int ident;
      myString line=GetNameStats(what,i,weighted,&ident)+"\n";
      if (ident!=-99)
        {tmp+=line;}
    }
  return tmp;
}

void CProject::MakeObservedAdjusted()
{
  Timestring.MakeObservedAdjusted();
  char txt[256];
  sprintf(txt,PROTOCOL_RESTORE_ORIGINAL_VALUES,GetSelectedPoints());
  Write()<<txt<<endl;
  UpdateTSDisplays();
}

void CProject::LoadTimeString(myString name,int append)
{
  if (name=="")
    {
      char *txt=(append)?DIALOG_APPEND_TIMESTRING_TITLE:
	                 DIALOG_LOAD_TIMESTRING_TITLE;
      name=GetFileName(1,txt,"",
		       DIALOG_TIMESTRING_EXTENTION,
		       DIALOG_TIMESTRING_PATTERN);
    }
  if (name=="")
    {
      // no filename given?
      return;
    }
  // check if file exists
  if (FileExist(name)==0)
    {
      char text[1024];
      sprintf(text,DIALOG_FILE_DOES_NOT_EXIST,name.chars());
      InformUser(text);
      return;
    }
  Waiting(1);
  // check possible input-format
  myString formatoffered=Timestring.PredictFileFormat(name);
  // and then check if it fits the user
  myString format=FileFormat(formatoffered,name);
  if (format=="")
    {
      //cancel pressed, so don't load
      Waiting(0);
      return;
    }
  // then set format...
  Timestring.SetInputFormat(format);
  // are we appending?
  if (!append)
    {
      // remove all datapoints
      Timestring.Clean();
    }
  // points before loading
  int PointsBefore=GetTotalPoints();
  // load data
  Timestring.Load(name);
  
  // write to protocol
  char txt[1024];
  char *msg=(append)?PROTOCOL_APPEND_TIMESTRING:PROTOCOL_LOAD_TIMESTRING;
  sprintf(txt,msg,
	  GetTotalPoints()-PointsBefore,
	  name.chars(),
	  Timestring.GetInputFormat().chars());
      
  Write()<<txt<<endl;

  // select all data
  Timestring.SelectAll();
  // now reselect all and write log
  Select();
  Waiting(0);
  // now update all TS-displays
  UpdateTSDisplays();
}

void CProject::SaveTimeStringPhase(double Frequency)
{
  // and then check if it fits the user
  myString format=FileFormat(Timestring.GetOutputFormat(),1);
  // is the format empty, so let's return
  if (format=="") { return;}
  // get name if necessary
  myString name=GetFileName(0,DIALOG_SAVE_PHASESTRING_TITLE,"",
			    DIALOG_PHASESTRING_EXTENTION,
			    DIALOG_PHASESTRING_PATTERN);
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
  // store old format
  myString OldFormat=Timestring.GetOutputFormat();
  // then set format...
  Timestring.SetOutputFormat(format);
  // save data
  Timestring.SavePhase(name,Frequency);
  // write to protocol
  char txt[1024];
  sprintf(txt,PROTOCOL_SAVE_PHASESTRING,
	  GetSelectedPoints(),
	  name.chars(),
	  Frequency,
	  Timestring.GetOutputFormat().chars());
  Write()<<txt<<endl;
  // restore old format
  Timestring.SetOutputFormat(OldFormat);
}

void CProject::SaveTimeStringPhaseBinned(
					 double Frequency,
					 double BinSpacing,
					 DataMode UseData,
					 int points,
					 double *Phase,
					 double *Amplitude,
					 double *Sigma
					 )

{
  // get name if necessary
  myString name=GetFileName(0,DIALOG_SAVE_PHASESTRING_TITLE,"",
			    DIALOG_PHASESTRING_EXTENTION,
			    DIALOG_PHASESTRING_PATTERN);
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
  {
    ofstream file(name.chars());
    int counter;
    char text[256];
    for (counter=0;counter<points;counter++)
      {
	sprintf(text,FORMAT_PHASE"\t"FORMAT_AMPLITUDE"\t"FORMAT_AMPLITUDE,
		Phase[counter],Amplitude[counter],Sigma[counter]);
	file<<text<<endl;
      }
  }
  // write to protocol
  char txt[1024];
  sprintf(txt,PROTOCOL_SAVE_PHASESTRINGBINNED,
	  points,
	  name.chars(),
	  Frequency,
	  BinSpacing,
	  UseData
	  );
  Write()<<txt<<endl;
}

void CProject::SaveTimeString(myString name)
{
  // and then check if it fits the user
  myString format=FileFormat(Timestring.GetOutputFormat());
  // is the format empty, so let's return
  if (format=="") { return;}
  // then set format...
  Timestring.SetOutputFormat(format);

  // get name if necessary
  if (name=="")
    {
      name=GetFileName(0,DIALOG_SAVE_TIMESTRING_TITLE,"",
		       DIALOG_TIMESTRING_EXTENTION,
		       DIALOG_TIMESTRING_PATTERN);
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
  Timestring.Save(name);
  // set input-format to output-format
  Timestring.SetInputFormat(Timestring.GetOutputFormat());
  // write to protocol
  char txt[1024];
  sprintf(txt,PROTOCOL_SAVE_TIMESTRING,
	  GetSelectedPoints(),
	  name.chars(),
	  Timestring.GetOutputFormat().chars());
  Write()<<txt<<endl;
}

void CProject::ChangeName(int i,int ID,myString name, myString Sweight, char* color)
{
  //
  double weight=DoubleFromString(Sweight);
  // Get Original settings
  CName *tmp=&(Timestring.GetIDName(i,ID));
  // check if new name allready exists
  int names=Timestring.NumberOfNames(i);
  for (int index=0;index<names;index++)
    {
      CName &other=GetIndexName(i,index);
      if ( (ID!=other.GetID()) && (other.GetName()==name) )
	{
	  // Show message
	  char msg[2048];
	  sprintf(msg,TIME_CHANGE_DOUBLE,name.chars());
	  InformUser(msg);
	  // set flag
	  name=tmp->GetName();
	  break;
	}
    }
  // now write the log...
  char txt[1024];
  sprintf(txt,PROTOCOL_CHANGE_NAME,
	  Timestring.NameSet(i).chars(),
	  tmp->GetName().chars(),
	  tmp->GetWeight(),
	  tmp->GetColor(),
	  name.chars(),
	  weight,
	  color);
  Write()<<txt<<endl;
  // set values
  tmp->SetName(name);
  tmp->SetWeight(weight);
  tmp->SetColor(color);
  UpdateTSDisplays();
}

void CProject::DeletePoint(int i)
{
  // get defaults
  int what;
  myString name;
  GetDeletePointInfo(&what,&name);
  // assign to array
  myString tmp[4];
  tmp[what]=name;
  // and finally relabel
  RelabelPoint(i,tmp[0],tmp[1],tmp[2],tmp[3]);
  // update displays
  UpdateTSDisplays();
}

void CProject::RelabelPoint(int i,
			    myString name0,myString name1,
			    myString name2,myString name3)
{
  ostream & tmp=Write();

  CTimePoint &pnt=Timestring[i];
  char tmpstr[512];
  sprintf(tmpstr,PROTOCOL_RELABEL_POINT,
	  pnt.GetTime(),
	  pnt.GetObserved(),
	  pnt.GetAdjusted());
	  
  tmp<<tmpstr<<endl;
  // create log
  if (name0!="")
    {
      int n0=Timestring.GetID(0,name0);
      int oldid=pnt.GetIDName(0);
      if (oldid!=n0)
	{
	  // write log
	  sprintf(tmpstr,PROTOCOL_RELABEL_FROM_TO,
		  Timestring.GetIDName(0,oldid).GetName().chars(),
		  name0.chars());
	  tmp<<tmpstr;
	  // rename
	  pnt.SetIDName(0,n0);
	}
    }
  if (name1!="")
    {
      int n1=Timestring.GetID(1,name1);
      int oldid=pnt.GetIDName(1);
      if (oldid!=n1)
	{
	  // write log
	  sprintf(tmpstr,PROTOCOL_RELABEL_FROM_TO,
		  Timestring.GetIDName(1,oldid).GetName().chars(),
		  name1.chars());
	  tmp<<tmpstr;
	  // rename
	  pnt.SetIDName(1,n1);
	}
    }
  if (name2!="")
    {
      int n2=Timestring.GetID(2,name2);
      int oldid=pnt.GetIDName(2);
      if (oldid!=n2)
	{
	  // write log
	  sprintf(tmpstr,PROTOCOL_RELABEL_FROM_TO,
		  Timestring.GetIDName(2,oldid).GetName().chars(),
		  name2.chars());
	  tmp<<tmpstr;
	  // rename
	  pnt.SetIDName(2,n2);
	}
    }
  if (name3!="")
    {
      int n3=Timestring.GetID(3,name3);
      int oldid=pnt.GetIDName(3);
      if (oldid!=n3)
	{
	  // write log
	  sprintf(tmpstr,PROTOCOL_RELABEL_FROM_TO,
		  Timestring.GetIDName(3,oldid).GetName().chars(),
		  name3.chars());
	  tmp<<tmpstr;	  
	  // rename
	  pnt.SetIDName(3,n3);
	}
    }
  // now try Garbage Collection
  Timestring.GarbageCollect();
  // change selections
  Select();
  // update displays
  UpdateTSDisplays();
}

void CProject::ReadTimestring(istream &ist)
{
  char c,tmp[1024];
  while(ist>>c,ist.putback(c),c!='[')
    {
      // read in argument
      ist>>tmp;
      if (strcasecmp(tmp,"Filename=")==0)
	{
	  // skip tab
	  ist.putback(ist.get());
	  // read arguments
	  ist.getline(tmp,1024);
	  // find first nonwhite name
	  char *tmp1=tmp;
	  while ( 
		 (isspace(*tmp1))
		 &&
		 !(*tmp1==0)
		 )
	    {
	      tmp1++;
	    }
	  // set filename
	  Timestring.SetFileName(tmp1);
	}
      else if (strcasecmp(tmp,"SelectAll")==0)
	{
	  Timestring.SelectAll();
	}
      else if (strcasecmp(tmp,"Select")==0)
	{
	  Timestring.Select();
	}
      else if (strcasecmp(tmp,"Weight")==0)
	{
	  int where;
	  double weight;
	  ist>> where;
	  ist>>tmp;
	  ist>>weight;
	  int ID=Timestring.GetID(where,tmp);
	  Timestring.GetIDName(where,ID).SetWeight(weight);
	}
      else if (strcasecmp(tmp,"WeightName")==0)
	{
	  int what;
	  ist>>what;
	  // read in line
	  Timestring.SetUseNameWeight(what);
	}
      else if (strcasecmp(tmp,"WeightPoint")==0)
	{
	  int what;
	  ist>>what;
	  // read in line
	  Timestring.SetUsePointWeight(what);
	}
      else if (strcasecmp(tmp,"SelectName")==0)
	{
	  int where;
	  ist>>where;
	  ist>>tmp;
	  int ID=Timestring.GetID(where,tmp);
	  Timestring.GetIDName(where,ID).SetSelect(1);
	}      
      else if (strcasecmp(tmp,"Attributes")==0)
	{
	  char a1[1024],a2[1024],a3[1024],a4[1024];
	  ist>>a1>>a2>>a3>>a4;
	  Timestring.ChangeNameSet(0,a1);
	  Timestring.ChangeNameSet(1,a2);
	  Timestring.ChangeNameSet(2,a3);
	  Timestring.ChangeNameSet(3,a4);
	}      
      else if (strcasecmp(tmp,"Color")==0)
	{
	  int where;
	  ist>> where;
	  ist>>tmp;
	  char quote;
	  ist>>quote;
	  char color[256];
	  ist.get(color,256,'"');
	  ist>>quote;
	  int ID=Timestring.GetID(where,tmp);
	  Timestring.GetIDName(where,ID).SetColor(color);
	}
      else if (strcasecmp(tmp,"DeleteLabel")==0)
	{
	  // set defaults
	  int what;
	  char name[256];
	  ist>>what>>name;
	  SetDeletePointInfo(what,name);
	}
      else if (strcasecmp(tmp,"Magnitude")==0)
	{
	  int what;
	  ist>>what;
	  // read in line
	  SetReverseScale(what);
	}
      else if (strcasecmp(tmp,"Data")==0)
	{
	  // read in line
	  Timestring.ReadIn(ist,FileFormatVersion);
	}
      else
	{
	  MYERROR("Unknown Project-data in Timestring-part:"<<tmp);
	}
    }
  // now try Garbage Collection
  Timestring.GarbageCollect();
}

void CProject::WriteTimestring(ostream &ost)
{
  ost<<"Filename=\t"<<Timestring.GetFileName()<<"\n";
  // writeout the selected names and their weights if not default
  for (int i=0;i<4;i++)
    {
      int ent=NumberOfNames(i);
      for (int j=0;j<ent;j++)
	{
	  if (GetIndexName(i,j).GetSelect())
	    {
	      ost<<"SelectName\t"<<i<<"\t"
		 <<GetIndexName(i,j).GetName()<<endl;
	    }
	  if (GetIndexName(i,j).GetWeight()!=1.0)
	    {
	      ost<<"Weight\t"<<i<<"\t"
		 <<GetIndexName(i,j).GetName()<<"\t"
		 <<GetIndexName(i,j).GetWeight()<<endl;
	    }
	  {
	    ost<<"Color\t"<<i<<"\t"
	       <<GetIndexName(i,j).GetName()<<"\t"
	       <<'"'<<GetIndexName(i,j).GetColor()<<'"'<<endl;
	  }
	}
    }
  // delete default label
  {
    // set defaults
    int what;
    myString name;
    GetDeletePointInfo(&what,&name);
    // and write it out
    ost<<"DeleteLabel\t"<<what<<"\t"<<name<<endl;
  }
  ost<<"Attributes"
     <<"\t"<<NameSet(0)
     <<"\t"<<NameSet(1)
     <<"\t"<<NameSet(2)
     <<"\t"<<NameSet(3)<<endl;
  ost<<"Magnitude\t"<<GetReverseScale()<<endl;
  ost<<"WeightName\t"<<GetUseNameWeight()<<endl;
  ost<<"WeightPoint\t"<<GetUsePointWeight()<<endl;
  // write out the data
  Timestring.WriteAll(ost,"Data");
  // and select all needed
  ost<<"Select"<<endl;
}

void CProject::SetUseNameWeight(int i)
{
  Timestring.SetUseNameWeight(i);
  if (i)
    { Write()<<PROTOCOL_WEIGHT_NAME_ON<<endl; }
  else
    { Write()<<PROTOCOL_WEIGHT_NAME_OFF<<endl; }
  Select();
}

void CProject::SetUsePointWeight(int i)
{
  Timestring.SetUsePointWeight(i);
  if (i)
    { Write()<<PROTOCOL_WEIGHT_POINT_ON<<endl; }
  else
    { Write()<<PROTOCOL_WEIGHT_POINT_OFF<<endl; }
  Select();
}

myString CProject::GetStartTime()
{
  if (GetSelectedPoints())
    {
      char txt[32];
      sprintf(txt,FORMAT_TIME,Timestring[0].GetTime());
      return txt;
    }
  return "";
}

myString CProject::GetEndTime()
{
  if (GetSelectedPoints())
    {
      char txt[32];
      sprintf(txt,FORMAT_TIME,Timestring[GetSelectedPoints()-1].GetTime());
      return txt;
    }
  return "";
}

myString CProject::GetIDNameWeight(int i, int ID)
{
  char txt[32];
  sprintf(txt,FORMAT_WEIGHTS,GetIDName(i,ID).GetWeight()); 
  return txt;
} 

char* CProject::GetIDNameColor(int i, int ID)
{
  return GetIDName(i,ID).GetColor(); 
}

char** CProject::GetColors()
{
  return Colors;
}

int CProject::GetMaxColors()
{
  return MAXCOLORS;
}
