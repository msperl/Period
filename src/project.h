/*
 * Program:     Period98
 *
 * File:        project.h
 * Purpose:     header-file for
 *              the general parts of the project
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */
#ifndef __project_h_
#define __project_h_

#include <iostream.h>
#include <time.h>

#include "str.h"
#include "dtimestr.h"
#include "dperiod.h"
#include "dfourier.h"
#include "dglistfo.h"
#include "display.h"

///
class CProject
{
public:
  /// basic constructor of the project
  CProject();
  ///  destructor of the project
  virtual ~CProject();

  /// return a stream to write the protocol
  ostream &Write();
  /// returns the current date and time
  myString Date() const;
  /// function to see if a file exists
  int FileExist(myString file) const;
  /// function to see if a file is writeable
  int CanWrite(myString file) const;
  
  /// save the project
  /// if the project has not allready been saved
  /// the user is asked to enter a filename
  void SaveProject();
  /// saves the project under a different name
  /// the user is asked to give a new name
  void SaveProjectAs();
  /// loads a project-file
  void LoadProject(myString file="");
  /// removes all data  from an old project and starts a new one
  void NewProject();
  /// Have there been some relevant changes to the project
  int HasChanged() const { return Changed; }
  /// force the project into the not-changed state
  void NoChanges() { Changed=0; }

#include "pdisplay.h"
#include "ptimestr.h"
#include "pperiod.h"
#include "pfourier.h"
  
  //
  // global virtual functions
  //
public:  

  //@ManMemo: Help stuff
  virtual void HelpContents()=0;
  ///
  virtual void HelpKeyword(char *s=NULL)=0;

  /// virtual function to switch the waiting-option on/off
  virtual void Waiting(int on=0, int cancel=0)=0;
  /// virtual function to inform the user of an event
  virtual void InformUser(char *text)=0;
  /// virtual function that asks the user for confirmation
  virtual int Confirm(char *text)=0;
  /// virtual function that asks the user for a file to load/save
  virtual myString GetFileName(int Load,char* title,myString name="",
			       char *ext=NULL,char* pat="*.*")=0;

protected:
  
  ///
  virtual void WriteSpecificProtocol(ostream &ost)=0;
  ///
  virtual void CleanProtocol()=0;
  ///
  virtual void AppendAtEndOfProtocol() {}
  ///
  virtual void AddaptProtocolSize() {}

  ///
  double DoubleFromString(const myString& str) const;
  ///
  int IntegerFromString(const myString& str) const;
  
private:
  
  ///
  void CleanAll();
  
  ///
  void ReadProtocol(istream & file);

  ///
  void WriteProtocol(ostream & file);
  ///
  void WriteSelection(ostream &ost);

protected:
  
  ///
  myString ProjectFile;
  ///
  int Changed;

  ///
  ostream *Protocol;

  ///
  int sel_change;

  ///
  TList<CMyDisplayP> Displays;

  ///
  int FileFormatVersion;
  
};

#endif






