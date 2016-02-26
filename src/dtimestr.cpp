/*
 * Program:     Period98
 *
 * File:        dtimestr.cpp
 * Purpose:     implementation-file for
 *              ?
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */

#include "dtimestr.h"
#include <stdio.h>
#include <string.h>

#include <math.h>
#include <fstream.h>
#include <ctype.h>
#include <stdlib.h>

CTimeString::CTimeString()
//:mUseWeight(0)
{
  mSelected=0;
  mSelection=0;
  SetInputFormat("to1234");
  SetOutputFormat("to1234");
  InitNameSets();
  Clean();
}

CTimeString::CTimeString(myString const &name,myString const &from)
//:mUseWeight(0)
{
  mSelected=0;
  mSelection=0;
  mMode=Observed;
  SetInputFormat(from);
  SetOutputFormat("to1234");
  InitNameSets();
  Clean();
  Load(name);
  SelectAll();
}

void CTimeString::InitNameSets()
{
  mNameSet[0]="";
  mNameSet[1]="";
  mNameSet[2]="";
  mNameSet[3]="";
}

CTimeString::~CTimeString()
{
  Clean();
}

void CTimeString::Clean()
{
  // the default is magnitudes, so reverse scale
  SetReverseScale(1);
  // clean the arrays
  mTimeArray.clean();
  SelectClear();
  for (int i=0;i<4;i++)
    {
      mNames[i].CleanUp();
    }
  // set the default-names
  ChangeNameSet(0,"Date");
  ChangeNameSet(1,"Observatory");
  ChangeNameSet(2,"Observer");
  ChangeNameSet(3,"Other");
  mWeightSum=0;
  mFileName="";
  // reset the index for loading files...
  CTimePoint tmp(1);
  // set defaults for deleted
  mDeleteWhat=3;
  mDeleteName="deleted";
  mPointWeight=0;
  mNameWeight=1;
}

void CTimeString::SelectClear()
{
  delete mSelection;
  mSelection=0;
  mSelected=0;
  mWeightSum=0;
}

int CTimeString::IsSelected(SelectedTime time)
{
  // this is the one to change to change Selection-beaviour
  for (int i=0;i<4;i++)
    {
      if (mNames[i].GetIDName(time->GetIDName(i)).GetSelect()==0)
	{
	  // not selected in all
	  return 0;
	}
    }
  return 1;
}

int CTimeString::CreateName(int column,int &useCounter,double time, myString prefix)
{
  myString filename=prefix;
  int value;
  if (useCounter==0)
   { value=int(time); }
  else
    { value=useCounter; useCounter++; }
  char txt[256];
  sprintf(txt,"%i",value);
  filename+=txt;
  int ID=GetID(column,filename);
  GetIDName(column,ID).SetSelect(1);
  return ID;
}

void CTimeString::Subdivide(double timegap, int column,
			    int useCounter, myString prefix)
{
  CheckBounds(column);
  // calculate first name
  double lasttime=(*this)[0].GetTime();
  int ID=CreateName(column,useCounter,lasttime,prefix);
  // now set all names
  for (int i=0;i<mSelected;i++)
    {
      double time=(*this)[i].GetTime();
      if (time-lasttime>timegap)
	{
	  ID=CreateName(column,useCounter,time,prefix);
	}
      (*this)[i].SetIDName(column,ID);
      lasttime=time;
    }
  // Now reselect all
  Select();
  // and now some GarbageCollection
  GarbageCollect();
}

void CTimeString::RenameSelection(int column, myString name)
{
  CheckBounds(column);
  int ID=GetID(column,name);
  GetIDName(column,ID).SetSelect(1);
  // now set all names
  for (int i=0;i<mSelected;i++)
    {
      (*this)[i].SetIDName(column,ID);
    }
  // Now reselect all
  Select();
  // and now some GarbageCollection
  GarbageCollect();
}

void CTimeString::SetWeight(SelectedTime time)
{
  // this is the one to change to set weights accordingly
  double weight=1.0;
  for (int i=0;i<4;i++)
    {
      weight*=mNames[i].GetIDName(time->GetIDName(i)).GetWeight();
    }
  double finalweight;
  // check if user-weights shoudl be used
  if (mNameWeight)
    {
      finalweight=weight;
    }
  else
    {
      finalweight=1.0;
    }
  // check if weights for specific point should be used
  if (mPointWeight)
    {
      finalweight*=time->GetPointWeight();
    }
  // now set weights
  time->SetWeight(finalweight);
}

void CTimeString::GarbageCollect()
{
  int i;
  // make a copy of the current selection
  for (i=0;i<4;i++)
    {
      mNames[i].StoreSelection();
    }
  // Select all
  SelectAll();
  // Calc Average
  CalcAverage();
  // Make GarbageCollection
  for (i=0;i<4;i++)
    {
      mNames[i].GarbageCollect();
    }
  // restore the current selection
  for (i=0;i<4;i++)
    {
      mNames[i].RestoreSelection();
    }
  // Select data
  Select();
}

void CTimeString::Select()
{
  // count selection
  int total=0;
  int i;
  int ent=mTimeArray.entries();
  mTimeArray.startiterate(0);
  for (i=0;i<ent;i++)
    {
      SelectedTime tmp=mTimeArray.iterate();
      if (tmp==0)
	{
	  MYERROR("Iteration-array gives 0 !!!");
	}
      else
	{
	  if (IsSelected(tmp))
	    {
	      // this entry is selected, so increment total
	      total++;
	    }
	}
    }
  // clean Selection
  SelectClear();
  // now Make Selection 
  mSelected=total;
  mSelection= new SelectedTime[mSelected];
  mTimeArray.startiterate(0);
  int j=0;
  for (i=0;i<ent;i++)
    {
      SelectedTime tmp=mTimeArray.iterate();
      if (tmp==0)
	{
	  MYERROR("Iteration-array gives 0 !!!");
	}
      else
	{
	  if (IsSelected(tmp))
	    {
	      // this entry is selected, so include it
	      mSelection[j]=tmp;
	      // Set The relevant Weight
	      SetWeight(tmp);
	      j++;
	    }
	}
    }
  CalcAverage();
  CalcWeights();
}

void CTimeString::CalcWeights()
{
  double tmp;
  int id;
  mWeightSum=0;
  // iterate over all selected datapoints
  for (int i=0;i<mSelected;i++)
    {
      SetWeight(mSelection[i]);
      // add up weights
      mWeightSum+=mSelection[i]->GetWeight();
    }
}

void CTimeString::SelectAll()
{
  // for all entries..
  for (int i=0;i<4;i++)
    {
      for (int j=0;j<mNames[i].NumberOfNames();j++)
	{
	  mNames[i].GetIndexName(j).SetSelect(1);
	}
    }
  // now select all
  Select();
}

CTimePoint &CTimeString::operator[](int i) const
{
  return *mSelection[i];
}

void CTimeString::SetInputFormat(myString const &tmp)
{
  mInputFormat=tmp;
  // here should go some sort of check if tmp holds true value
	
}

void CTimeString::SetOutputFormat(myString const &tmp)
{
  mOutputFormat=tmp;
  // here should go some sort of check if tmp holds true value
}

void CTimeString::Save(myString const &name)
{
  ofstream file(name.chars());
  if (!file)
    {
      // file-error
      return ;
    }
  // set output-format
  file.precision(12);
  // check for File-error MISSING !!!
  file<<(*this);
}

void CTimeString::SavePhase(myString const &name, double Frequency)
{
  ofstream file(name.chars());
  if (!file)
    {
      // file-error
      return ;
    }
  // set output-format
  file.precision(12);
  // check for File-error MISSING !!!
  WriteOut(file,Frequency);
}

void CTimeString::WriteAll(ostream &str,char *head)
{
  // store default-precission and set new 
  int prec=str.precision();
  str.precision(15);
  // get the number of entries
  int i,ent=mTimeArray.entries();
  mTimeArray.startiterate(0);
  for (i=0;i<ent;i++)
    {
      SelectedTime tmp=mTimeArray.iterate();
      if (tmp==0)
	{
	  MYERROR("Iteration-array gives 0 !!!");
	}
      else
	{
	  str<<head
	     <<"\t"<<tmp->GetRunningIndex()
	     <<"\t"<<tmp->GetTime()
	     <<"\t"<<tmp->GetObserved()
	     <<"\t"<<tmp->GetAdjusted()
	     <<"\t"<<tmp->GetCalculated()
	     <<"\t"<<tmp->GetPointWeight()
	     <<"\t"<<GetIDName(0,tmp->GetIDName(0)).GetName()
	     <<"\t"<<GetIDName(1,tmp->GetIDName(1)).GetName()
	     <<"\t"<<GetIDName(2,tmp->GetIDName(2)).GetName()
	     <<"\t"<<GetIDName(3,tmp->GetIDName(3)).GetName()
	     <<endl;
	}
    }
  // restore old precission
  str.precision(prec);
  // and return
}

void CTimeString::ReadIn(istream & input,int version)
{
  double t, m,a,c,pw;
  char name[256];
  int n1,n2,n3,n4;
  int index=0;
  // set defaults
  t=m=a=-100;c=0;n1=n2=n3=n4=0;
  pw=1.0;
  // read in data
  input>>index; // index
  input>>t; // time
  input>>m; // magnitude
  input>>a; // adjusted
  input>>c; // calculated
  if (version>10000)
    {
      input>>pw; // weight
    }
  input>>name; // Name 1
  n1=mNames[0].GetID(name);
  input>>name; // Name 2
  n2=mNames[1].GetID(name);
  input>>name; // Name 3
  n3=mNames[2].GetID(name);
  input>>name; // Name 4
  n4=mNames[3].GetID(name);
  // now add point
  mTimeArray.add(CTimePoint(t,m,a,c,pw,n1,n2,n3,n4,index));
}

void CTimeString::Load(myString const &fname)
{
  FILE*file;
  char line[2048];
  double t, m,a,c,pw;
  int n1,n2,n3,n4;
  file=fopen(fname.chars(),"r");
  if (file==0)
    {
      MYERROR("Problems opening file:"<<fname<<"- does it exist?");
      fclose(file);
      return ;
    }

  // read in the entries

  int inputlen=mInputFormat.length();
  char *defaulttxt="unknown";
  int n1d=mNames[0].GetID(defaulttxt);
  int n2d=mNames[1].GetID(defaulttxt);
  int n3d=mNames[2].GetID(defaulttxt);
  int n4d=mNames[3].GetID(defaulttxt);
  while (fgets(line,2047,file)!=0)
    {
      char *pnt=line;
      // clear the input
      t=m=a=-100;c=0;
      pw=1.0;
      n1=n1d;
      n2=n2d;
      n3=n3d;
      n4=n4d;
      for(int i=0;i<inputlen;i++)
	{
	  char entry[1024];
	  int len=0;
	  // skip whitspaces
	  while (isspace(*pnt))
	    {
	      pnt++;
	    }
	  // is it a comment? - e.g does it start with "#",";",":" ?
	  switch (*pnt)
	    {
	    case '#':
	    case ';':
	    case '%':
	      // ignoreline
	      break;
	    case 0: // nothing is in line
	      break;
	    default:
	      {
		while ((*pnt!=0) && (!isspace(*pnt)) )
		  {
		    entry[len]=*pnt;
		    pnt++;
		    len++;
		  }
		// terminate string
		entry[len]=0;	      	      
		// parse string
		switch(mInputFormat[i])
		  {
		  case 't': // time
		    sscanf(entry,"%lf",&t);
		    break;
		  case 'o': // magnitude
		    sscanf(entry,"%lf",&m);
		    break;
		  case 'a': // adjusted
		    sscanf(entry,"%lf",&a);
		    break;
		  case 'c': // calculated
		    sscanf(entry,"%lf",&c);
		    break;
		  case 'g': // point weight
		    sscanf(entry,"%lf",&pw);
		    break;
		  case 'p': // Data Residuals
		    // As we may not adjust the Observed data
		    // we have to set the value of calculate
		    // to the residuals
		    sscanf(entry,"%lf",&c);
		    c=m-c;
		    break;
		  case 'b': // Adjusted Residuals
		    {
		      double r=0;
		      sscanf(entry,"%lf",&r);
		      if ( (a==-100) && (c!=0) ) {
			  // set Amplitude
			  a=c+r;
			}
		      else {
			  // Set Calculated
			  c=a-r;
			}
		    }
		    break;
		  case '1': // name 1
		    n1=mNames[0].GetID(entry);
		    break;
		  case '2': // name 2
		    n2=mNames[1].GetID(entry);
		    break;
		  case '3': // name 3
		    n3=mNames[2].GetID(entry);
		    break;
		  case '4': // name 4
		    n4=mNames[3].GetID(entry);
		    break;
		  case 'i': // ignore next
		  case 'n':
		    break;
		  default:
		    MYERROR("entry "<<mInputFormat[i]<<
			  " in formatstring not know...\n"
			  "ignoring column %i");
		    // writing error
		    break;
		  }
	      }
	    }
	}
      // Add only if point valid...
      if (t!= -100)
	{
	  mTimeArray.add(CTimePoint(t,m,a,c,pw,n1,n2,n3,n4));
	}
    }
  fclose(file);

  // copy name
  if (mFileName=="")
    {
      mFileName=fname;
    }
  else
    {
      // we are appending
      mFileName+=", "+fname;
    }
  // now make a Garbage Collection
  GarbageCollect();
}

double CTimeString::BaseLine() const
{
  return (GetSelectedPoints()==0)?0:
    mSelection[GetSelectedPoints()-1]->GetTime()-mSelection[0]->GetTime();
}

int CTimeString::CalculateExpectationValue(double &mi, double &ma) const
{
  int const size=100;
  int i,box[size+1];
  // clean box ...
  for(i=0;i<=size;i++)
    {
      box[i]=0;
    }
  double diff;
  // find scale
  double scale=(ma-mi)/size;
  double bo;
  // summ up peaks
  for (i=1;i<GetSelectedPoints();i++)
    {
      diff=(mSelection[i]->GetTime()) - (mSelection[i-1]->GetTime());
      bo=floor((diff-mi)/scale);
      if ( (bo>=0) && (bo<=size) )
	{
	  // increment if in box
	  box[int(bo)]++;
	}
    }
  // find highest
  int found=0, foundval=0;
  for(i=0;i<=size;i++)
    {
      if (box[i]>foundval)
	{
	  found=i;
	  foundval=box[i];
	}
    }


  mi+=found*scale;
  ma=mi+1*scale;

  // that is exact enough !!!
  if ((ma-mi)<.0001)
    {
      return 0;
    }
  
  return foundval;
}

double CTimeString::Nyquist() const
{
  if (GetSelectedPoints()==0)
    {
      return 0;
    }
  // min and max dt's
  double min=9999999,max=0;
  double diff;
  // find min and max
  for (int i=1;i<GetSelectedPoints();i++)
    {
      diff=(mSelection[i]->GetTime()) - (mSelection[i-1]->GetTime());
      min=( min < diff ) ? min : diff ;
      max=( max > diff ) ? max : diff ;
    }
  double maxcp=max;
  // loop until peak is higher then 100
  int v;
  while ((v=CalculateExpectationValue(min,max))>100);
  {
    // nothing to do !!!
  }
  if (min==0)
    {
      min=0.00001;
      max=maxcp;
      while ((v=CalculateExpectationValue(min,max))>100);
      {
	// nothing to do !!!
      }
    }
  // return Nyquist - estimate...
  return 1/(min+max);
}



// Here needs something to be done !!!!
double CTimeString::Average(int useWeight) const
{
  double sum=0;
  double t,a;
  // calculate without weights
  if (!useWeight)
    {
      for(int i=0;i<GetSelectedPoints();i++)
	{
	  Point(i,&t,&a);
	  sum+=a;
	}
      return sum/GetSelectedPoints();
    }
  // calculate with weights
  for(int i=0;i<GetSelectedPoints();i++)
    {
      Point(i,&t,&a);
      sum+=a*(mSelection[i]->GetWeight());
    }
  return sum/GetWeightSum();
}

void CTimeString::ChangeNameSet(int i, myString const & tmp)
{
  CheckBounds(i);
  mNameSet[i]=tmp;
}

void CTimeString::CalcAverage()
{
  int i,j;
  // Clean all entries
  for (i=0;i<4;i++)
    {
      for (j=0;j<mNames[i].NumberOfNames();j++)
	{
	  mNames[i].GetIndexName(j).ResetValues();
	}
    }
  // Sum up
  CTimePoint * tmp;
  for (i=0;i<GetSelectedPoints();i++)
    {
      // new point
      tmp=&((*this)[i]);
      // add up for each name
      for (j=0;j<4;j++)
	{
	  mNames[j].GetIDName(tmp->GetIDName(j))
	    .AddPoint(tmp->GetDataResidual(),
		      tmp->GetAdjustedResidual(),
		      tmp->GetWeight());
	}
    }
}

void CTimeString::Adjust(int what, DataMode Orig, int n, int *ids,int useweights)
{
  for (int i=0;i<n;i++)
    {
      int id=ids[i];
      double Ori,Adj;
      if (useweights)
	{
	  Ori=(mNames[what].GetIDName(id)).GetAverageOrigWeight();
	  Adj=(mNames[what].GetIDName(id)).GetAverageAdjWeight();
	}
      else
	{
	  Ori=(mNames[what].GetIDName(id)).GetAverageOrig();
	  Adj=(mNames[what].GetIDName(id)).GetAverageAdj();
	}
      for (int j=0;j<GetSelectedPoints();j++)
	{
	  // find out if this entry has to be changed
	  if ((*this)[j].GetIDName(what)==id)
	    {
	      // find out previous value
	      double value;
	      // adjust value
	      if (Orig==Observed)
		{
		  value=(*this)[j].GetObserved()-Ori;
		}
	      else
		{
		  value=(*this)[j].GetAdjusted()-Adj;
		}
	      // write value back
	      (*this)[j].SetAdjusted(value);
	    }
	}
    }
}

void CTimeString::MakeObservedAdjusted()
{
  for (int i=0;i<GetSelectedPoints();i++)
    {
      double value=(*this)[i].GetObserved();
      (*this)[i].SetAdjusted(value);
    }
}

// sorting-Structure
struct IndexArray { 
  int RunningIndex;
  int SelectIndex;};

// sorting - routine
int RunningIndexCompare(const void *t1,const void *t2)
{
  int index1=((const IndexArray *)t1)->RunningIndex;
  int index2=((const IndexArray *)t2)->RunningIndex;
  if (index1>index2) return 1;
  if (index1<index2) return -1;
  return 0;
}

ostream &operator<<(ostream &str,CTimeString &tmp)
{
  return tmp.WriteOut(str,0.0);
}

ostream &CTimeString::WriteOut(ostream& str,
			       double Frequency)
{
  int l=mOutputFormat.length();
  // sort data
  // now create and fill array to sort
  int selected=mSelected;
  IndexArray * sortarray= new IndexArray[selected];
  int i;
  for (i=0;i<selected;i++)
    {
      sortarray[i].RunningIndex=mSelection[i]->GetRunningIndex();
      sortarray[i].SelectIndex=i;
    }
  // now sort array
  qsort(sortarray,selected,sizeof(IndexArray),RunningIndexCompare);
  // now write out the data...
  for(i=0;i<mSelected;i++)
    {
      // get the sorted selected index
      CTimePoint tm(*mSelection[sortarray[i].SelectIndex]);
      for(int j=0;j<l;j++)
	{
	  switch(mOutputFormat[j])
	    {
	    case 't':
	      if (Frequency!= 0.0)
		{
		  str<<tm.GetPhasedTime(Frequency);
		}
	      else
		{
		  str<<tm.GetTime();
		}
	      break;
	    case 'o':
	      str<<tm.GetObserved(); break;
	    case 'a':
	      str<<tm.GetAdjusted(); break;
	    case 'c':
	      str<<tm.GetCalculated(); break;
	    case 'p':
	      str<<tm.GetDataResidual(); break;
	    case 'b':
	      str<<tm.GetAdjustedResidual(); break;
	    case 'g':
	      str<<tm.GetPointWeight(); break;
	    case 'w':
	      str<<tm.GetWeight(); break;
	    case 'n':
	      str<<i; break;
	    case '1':
	      str<<GetIDName(0,tm.GetIDName(0)).GetName(); break;
	    case '2':
	      str<<GetIDName(1,tm.GetIDName(1)).GetName(); break;
	    case '3':
	      str<<GetIDName(2,tm.GetIDName(2)).GetName(); break;
	    case '4':
	      str<<GetIDName(3,tm.GetIDName(3)).GetName(); break;
	    }
	  str<<"\t";
	}
      str<<endl;
    }
  delete sortarray;
  return str;
}

myString CTimeString::PredictFileFormat(myString filename)
{
  // the buffer for a line
  const int linesize=4096;
  char linebuffer[linesize];
  // the structures for the format
  const int form_size=256;
  int lastform=0;
  myString formstr[form_size];
  int formcnt[form_size];
  for (int i=0;i<form_size;i++)
    { formcnt[i]=0; }
  // open file
  ifstream file(filename.chars());
  if (!file)
    {
      return "NOFILE";
    }
  while (!file.eof())
    {
    file.getline(linebuffer,linesize);
    if (file.good())
      {
	myString form=PredictLine(linebuffer);
	int j;
	for (j=0;j<lastform;j++)
	  {
	    if (form==formstr[j])
	      {
		// ok, found, so add up
		formcnt[j]++;
		break;
	      }
	  }
	//  not found yet, so add to list
	if (j==lastform)
	  {
	    formstr[lastform]=form;
	    formcnt[lastform]=1;
	    lastform++;
	    if (lastform==256)
	      { lastform=255; }
	  }
      }
    }
  // now decode current
  int highest=0;
  int highestvalue=0;

  // find highest
  for (int j=0;j<lastform;j++)
    {
      // is this higher then previous?
      if (highestvalue<formcnt[j])
	{
	  // assign new highest
	  highest=j;
	  highestvalue=formcnt[j];
	}
    }
  
  // and return the result
  return formstr[highest];
}

myString CTimeString::PredictLine(const char * linebuffer1)
{
  // copy string
  char *linebuffer=new char[strlen(linebuffer1)+2];
  unsigned int i,len;
  for (i=0;i<=strlen(linebuffer1);i++)
    { linebuffer[i]=linebuffer1[i]; }
  // define the curent format
  myString current;
  // new routine for linux
  char *pnt=linebuffer;
  while(*pnt!=0)
    {
      // read entry from line
      char entry[1024];
      len=0;
      while (isspace(*pnt))
	{
	  pnt++;
	}
      if (*pnt!=0)
	{
	  while ((*pnt!=0) && (!isspace(*pnt)) )
	    {
	      entry[len]=*pnt;
	      pnt++;
	      len++;
	    }
	  // terminate string
	  entry[len]=0;
	  // per default integer
	  char type='i';
	  // find out if the data is float integer or string
	  for (i=0;i<len;i++)
	    {
	      char c=entry[i];
	      if ( (!isdigit(c)) && (c!='-') && (c!='+') )
		{
		  if ( (c=='E') || (c=='e') || (c=='.') )
		    { // OK, assume we are float, and contin ue checking
		      type='f'; }
		  else
		    { // Any other character HAS to be a normal string,
		      type='s';
		      break; } // we do not need to continue checking
		}
	    }
	  // add type to result
	  current+=type;
	}
    }
  // delete the line
  delete linebuffer;

  // now try to make some sense out of this...
  int floatstart=0;
  int textstart=0;
  // make a copy
  int last=current.length();
  char* result= new char[last+1];
  strcpy(result,current.chars());
  // parse each entry
  for (int decode=0;decode<last;decode++)
    {
      // nothing to do for streight integers...
      // start with floats now...
      if (result[decode]=='f')
	{
	  char resu;
	  switch (floatstart)
	    {
	    case 0: resu='t'; break;
	    case 1: resu='o'; break;
	    case 2: resu='a'; break;
	    case 3: resu='w'; break;
	    case 4: resu='c'; break;
	    case 5: resu='r'; break;
	    case 6: resu='R'; break;
	    default:resu='n'; break;
	    }
	  result[decode]=resu;
	  // next in list...
	  floatstart++;
	}
      // start with text now...
      else if (result[decode]=='s')
	{
	  char resu;
	  switch (textstart)
	    {
	    case 0: resu='1'; break;
	    case 1: resu='2'; break;
	    case 2: resu='3'; break;
	    case 3: resu='4'; break;
	    default:resu='n'; break;
	    }
	  result[decode]=resu;
	  // next in list
	  textstart++;
	}
      else
	{
	  // unknown...
	  result[decode]='n';
	}
    }
  // clean up allocated string and save it to return
  myString final=result;
  delete result;
  // return the result...
  return final;
}







