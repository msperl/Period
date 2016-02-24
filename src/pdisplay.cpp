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

#include "project.h"

int CProject::FindDisplay(CMyDisplayP disp)
{
  for (int i=0;i<Displays.entries();i++)
    {
      
      if (disp == Displays[i])
	{
	  return i;
	}
    }
  return -1;
}

void CProject::CloseAllDisplays()
{
  int i=0;
  while (i<Displays.entries())
    {
      CMyDisplayP tmp=Displays[i];
      // check if we have to delete the displays
      if (tmp->ID()>-1000)
	{
	  // delete display
	  delete tmp;
	  // this also deregisters the display, 
	  // so we do not need to deregister ourself
	}
      else
	{
	  // this is a basic display, so go to the next one
	  i++;
	}
    }
}

void CProject::CloseDisplayID(int ID)
{
  int count=0;
  int i,c;
  // how many are there of that ID
  for (i=0;i<Displays.entries();i++)
    {if (Displays[i]->ID()==ID){count++;}}
  // put them into an array
  CMyDisplayP *array=new CMyDisplayP[count];
  c=0;
  for (i=0;i<count;i++)
    {
      if (Displays[i]->ID()==ID)
	{
	  array[c]=Displays[i];
	  c++;
	}
    }
  // and finally delete them
  for (i=0;i<count;i++)
    {
      delete array[i];
    }
  // delete array
  delete [] array;
}

void CProject::UpdateDataID(int ID)
{
  int i;
  for (i=0;i<Displays.entries();i++)
    {
      CMyDisplayP tmp=Displays[i];
      if (tmp->ID()==ID)
	{
	  tmp->UpdateData();
	}
    }
}

void CProject::UpdateDisplayID(int ID)
{
  int i;
  for (i=0;i<Displays.entries();i++)
    {
      CMyDisplayP tmp=Displays[i];
      if (tmp->ID()==ID)
	{
	  tmp->UpdateDisplay();
	}
    }
}

void CProject::UpdatePEDisplays()
{
  UpdateDisplayID(DISPLAY_DIALOG_PERIOD);
  UpdateDisplayID(DISPLAY_TIMESTRING);
  UpdateDisplayID(DISPLAY_TIMETABLE);
  UpdateDisplayID(DISPLAY_PHASEPLOT);
}

void CProject::UpdateTSDisplays()
{
  UpdateDisplayID(DISPLAY_DIALOG_TIMESTRING);
  UpdateDisplayID(DISPLAY_TIMESTRING);
  UpdateDisplayID(DISPLAY_TIMETABLE);
  UpdateDisplayID(DISPLAY_PHASEPLOT);
}

void CProject::UpdateAllDisplays()
{
  for (int i=0;i<Displays.entries();i++)
    {
      Displays[i]->UpdateDisplay();
    }
}

void CProject::UpdateAllData()
{
  for (int i=0;i<Displays.entries();i++)
    {
      Displays[i]->UpdateData();
    }
}


void CProject::RegisterDisplay(CMyDisplayP disp)
{
  if (FindDisplay(disp)!=-1)
    {
      MYERROREXIT("Registered display exists allready...");
    }
  Displays.addhead(disp);
}

void CProject::DeregisterDisplay(CMyDisplayP disp)
{
  int i=FindDisplay(disp);
  if (i!=-1)
    {
      Displays.removeat(i);
    }
  else
    {
      MYERROREXIT("Display not registered...");
    }
}
