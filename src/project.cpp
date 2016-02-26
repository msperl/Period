/*
 * Program:     Period98
 *
 * File:        project.cpp
 * Purpose:     implementation-file for
 *              project
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */

#include <fstream.h>
#include <sys/stat.h>
#include <ctype.h>
#include <string.h>
#include "version.h"
#include "project.h"
#include "fproject.h"
#include "mswdefs.h"

CProject::CProject()
  :
   mErrorScale(1.0),
   Period(CONFIG_MAX_FREQUENCIES+2),
   freqstep(CONFIG_DEFAULT_ALIAS_STEP),
   DefaultFourier(CONFIG_DEFAULT_FOURIER_NAME,
		  CONFIG_DEFAULT_FOURIER_LOWER_FREQUENCY,
		  CONFIG_DEFAULT_FOURIER_UPPER_FREQUENCY),
   ProjectFile(""),
   Changed(0),
   Protocol(&cout),
   FileFormatVersion(0)
{
  // make this the Default file
  FourierActive=&DefaultFourier;
  Period.Add();
  Period.ResizeLeast(CONFIG_MAX_FREQUENCIES);
}

CProject::~CProject()
{
  CleanAll();
}

void CProject::CleanAll()
{
  CloseAllDisplays();
  // clean fourier-space...
  CleanFourier();
  // cleans periods
  CleanPeriod();
  // Remove data from Timestring
  Timestring.Clean();
  // OK, now that we have cleaned everything,
  // there have not been any new changes yet..
  Changed=0;
  ProjectFile="";
  UpdateAllData();
}


int CProject::FileExist(myString file) const
{
  // return 1 for OK, 0 for Bad
  struct stat stbuf;
  if ((file=="") && (stat(file.chars(),&stbuf)))
    {return 0;}


  FILE *open=fopen(file.chars(),"r");
  if (!open)
    {return 0;}

  int ret=1;
  if (fgetc(open)==EOF)
    { ret=0; }

  if (ferror(open))
    { ret=0; }

  fclose(open);

  return ret;
}

int CProject::CanWrite(myString file) const
{
  ofstream myfile(file.chars());
  if (!myfile)
    {
      return 0;
    }
  return 1;
}

ostream &CProject::Write()
  {
    // if selection has been changed, then write the new selection
    if (sel_change)
      {
	WriteSelection(*Protocol);
      }
    // as this is called, there have been some changes...
    // increment the number of changes...
    Changed++;
    // check if the Protocoll stream exists...
    if (Protocol==NULL) 
      {
	MYERROREXIT("Programm-error: protocol-stream was not qualified");
      }
    // remove heading if necessary
    AddaptProtocolSize();
    // where to insert
    AppendAtEndOfProtocol();
    // write the current time to the stream
    *Protocol<<PROTOCOL_DATE<<Date()<<endl;
    return *Protocol;
  }

myString CProject::Date() const
{
  // find out current time
  time_t now=time(NULL);
  char *date=ctime(&now);
  date[24]=0;
  return date;
}

void CProject::SaveProjectAs()
{
  ProjectFile="";
  // make it have changed...
  Changed++;
  // And save it...
  SaveProject();
}

void CProject::SaveProject()
{
  UpdateAllData();

  if (Changed==0)
    {
      // there is no need to save, as nothing has changed...
      InformUser(DIALOG_PROJECT_UNEDITED);
      return;
    }

  if (ProjectFile=="")
    {
      // get Filename
      
      myString File=GetFileName(0,
				DIALOG_PROJECT_SAVE_TITLE,
				"",
				DIALOG_PROJECT_EXTENTION,
				DIALOG_PROJECT_PATTERN);
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
      // make a copy of the filename...
      ProjectFile= File;
    }

  // Save file...

  {
    ofstream file(ProjectFile.chars());
    // check for file-error
    if (!file)
      {
	char text[1024];
	sprintf(text,DIALOG_CANT_WRITE_FILE,ProjectFile.chars());
	InformUser(text);
	return;
      }
    // write version
    file<<"[Version:"<<versiontxt<<"]\n\n";
    // write period
    file<<"[Period]\n";
    WritePeriod(file);
    // write timestring
    file<<"[TimeString]\n";
    WriteTimestring(file);
    // write all the fourier-parts
    WriteFourier(file);// WriteFourier writes it's own headers
    // write the log
    file<<"[Log]\n";
    WriteProtocol(file);
  }

  // File is saved, so we can continue as unchanged...
  Changed=0;
}

void CProject::LoadProject(myString name)
{
  myString File=name;
  // ask for Confirmation to clean project in case
  if (name=="")
    {
      if (Changed!=0)
	{
	  if (Confirm(DIALOG_DELETE_OLD_PROJECT)==0)
	    {
	      // we may not delete old settings...
	      return;
	    }
	  Changed=0;
	}
      // ask for filename
      File=GetFileName(1,
		       DIALOG_PROJECT_LOAD_TITLE,
		       "",
		       DIALOG_PROJECT_EXTENTION,
		       DIALOG_PROJECT_PATTERN);
      if (File=="")
	{
	  // No File loaded...
	  return;
	}
    }
  // check if file exists
  if (FileExist(File)==0)
    {
      char text[1024];
      sprintf(text,DIALOG_FILE_DOES_NOT_EXIST,name.chars());
      InformUser(text);
      return;
    }
  // Now clean data...
  CleanAll();
  // now load it...
  Waiting(1);
  {
    ifstream file(File.chars());
    // check for errors...
    if (!file)
      {
	char text[1024];
	sprintf(text,DIALOG_FILE_READ_ERROR,name.chars());
	InformUser(text);
	return;
      }
    char tmp[256];
    char c;
    while (!file.eof())
      {
	// read in command
	file>>tmp;
	if (strncasecmp(tmp,"[Version:",9)==0)
	  {
	    // parse version
	    myString version=tmp;
	    version=version.Mid(10);
	    myString mayor,minor,micro;
	    int pos=0;
	    char c;
	    // edit mayor
	    while (isdigit((c=version[pos])))
	      {
		mayor+=c;
		pos++;
	      }
	    pos++;
	    // edit minor
	    while (isdigit((c=version[pos])))
	      {
		minor+=c;
		pos++;
	      }
	    pos++;
	    // edit micro
	    while (isdigit((c=version[pos])))
	      {
		micro+=c;
		pos++;
	      }
	    // Calculates the version
	    FileFormatVersion=
	      10000*IntegerFromString(mayor)
	      + 100*IntegerFromString(minor)
	      +     IntegerFromString(micro);
	  }
	else if (strcasecmp(tmp,"[Timestring]")==0)
	  {
	    ReadTimestring(file);
	  }
	else if (strcasecmp(tmp,"[Period]")==0)
	  {
	    ReadPeriod(file);
	  }
	else if (strcasecmp(tmp,"[Fourier]")==0)
	  {
	    ReadFourier(file);
	  }
	else if (strcasecmp(tmp,"[Log]")==0)
	  {
	    ReadProtocol(file);
	  }
	else
	  {
	    char txt[1024];
	    sprintf(txt,DIALOG_UNKNOWN_PROJECT_KEYWORD,tmp);
	    InformUser(txt);
	    // wait for eof or [
	    while((!file.eof())&&(file>>c,c!='['))
	      {
		// do nothing
	      }
	    // if there is a new comment then put [ back
	    if (c=='[') file.putback(c);
	  }
      }
  }
  // make a copy of the filename...
  ProjectFile=File;
  // Activate the first fourier loaded
  ActivateFourier(0);
  // File is loaded, so we can continue as unchanged...
  Changed=0;
  Waiting(0);
  UpdateAllDisplays();
}

void CProject::NewProject()
{
  // ask for Confirmation
  if (Changed!=0)
    {
      if (Confirm(DIALOG_DELETE_OLD_PROJECT) == 1)
	{
	  // we may not delete old settings, 
          // so let's save it
	  SaveProject();
	}
      Changed=0;
    }
  // OK, now lets clean it...
  CleanAll();
  // CleanProject
  CleanProtocol();
}


void CProject::ReadProtocol(istream &ist)
{
  const int buffsize=2048;
  char c,tmpin[buffsize],tmpout[buffsize];
  int putin,putout;
  while((!ist.eof())&&(ist.get(c)))
    {
      // put character back
      ist.putback(c);
      // check if new command-block
      if (c=='[')
	{
	  break;
	}
      // write this out...
      for (putin=0;putin<buffsize;putin++)
	{ tmpin[putin]=0; tmpout[putin]=0;}
      ist.getline(tmpin,buffsize-1);
      putout=0;
      // check if last character is not empty
      int len=strlen(tmpin);
      for (putin=0;putin<len;putin++)
	{
	  c=tmpin[putin];
	  if (c==0)
	    {
	      tmpout[putout]=0;
	      putout++;
	      break;
	    }
	  // add character to buffer if it is valid
	  if (!iscntrl(c)) 
	    {
	      tmpout[putout]=c;
	      putout++;
	    }
	}
      (*Protocol)<<tmpout<<endl;
    }
  // if there is a new comment then put [ back
  if (c=='[') ist.putback(c);
}

void CProject::WriteProtocol(ostream &ost)
{
  WriteSpecificProtocol(ost);
}

double CProject::DoubleFromString(myString const & str) const
{
  double val=0;
  sscanf(str.chars(),"%lf",&val);
  return val;
}

int CProject::IntegerFromString(myString const & str) const
{
  int val=0;
  sscanf(str.chars(),"%i",&val);
  return val;
}
