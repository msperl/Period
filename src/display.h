/*
 * Program:     Period98
 *
 * File:        display.h
 * Purpose:     header-file for
 *              the general parts of the project
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */
#ifndef __Display_h_
#define __Display_h_

#define DISPLAY_TIMESTRING -1
#define DISPLAY_TIMETABLE -2
#define DISPLAY_PHASEPLOT -3

#define DISPLAY_DIALOG_TIMESTRING -1000
#define DISPLAY_DIALOG_PERIOD -1001
#define DISPLAY_DIALOG_FOURIER -1002

class CProject;

class CMyDisplay
{
 public:
  CMyDisplay(CProject *project,int ID);
  virtual ~CMyDisplay();
  int ID() { return myID;}
  CProject *Project() { return Registered;}
  virtual void UpdateDisplay(int i=0);
  virtual void UpdateData();
  
 protected:
  CProject * Registered;
  int myID;
};

typedef CMyDisplay* CMyDisplayP;

#endif
