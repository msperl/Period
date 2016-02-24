/*
 * Program:     Period98
 *
 * File:        xftable.cpp
 * Purpose:     implementation-file for
 *              ?
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */

#include "xftable.h"
#include "xgmenu.h"
#include "str.h"
#include "ldialtxt.h"

f_Table_Frame::f_Table_Frame(CProject *data,int type,
			     wxFrame* parent, char*Name,
			     int x, int y,
			     int width, int height)
  :CMyTable(data,type,parent,Name,x,y,width,height,
	    wxSDI|wxDEFAULT_FRAME,FOUT_NAME)
{
   mData=data->GetFourierActive();
  // Set MenuBar
  SetMenuBar(CreateMenuBar_Simple());
  // Create the Canvas
  CreateCanvas();
  // rename frame
  char txt[256];
  sprintf(txt,FOUT_NAME": %s",mData->GetTitle().chars());
  SetTitle(txt);
  // Update Display
  // UpdateDisplay();
  // Show the Frame
  Show(TRUE);
}

f_Table_Frame::~f_Table_Frame()
{
}

myString f_Table_Frame::GetDocTitle()
{
  return mData->GetTitle();
}

myString f_Table_Frame::GetHeader(int column) const
{
  switch (column)
    {
    case 0:
      return FOUT_FRE; break;
    case 1:
      return FOUT_AMP; break;
    case 2:
      return FOUT_POW; break;
    }
  return FOUT_UKN;
}

int f_Table_Frame::NumberOfLines() const
{
  return mData->Points();
}

myString f_Table_Frame::GetData(int line,int column) const
{
  char txt[32];
  double val=0;
  switch (column)
    {
    case 0:
      val=(*mData)[line].GetFrequency();
      break;
    case 1:
      val=(*mData)[line].GetAmplitude();
      break;
    case 2:
      val=(*mData)[line].GetAmplitude();
      val=val*val;
      break;
    default:
      return FOUT_UKN;
      break;
    }
  sprintf(txt,"%g",val);
  return txt;
}

int f_Table_Frame::NumberOfTabs() const
{
  return 3;
}

int f_Table_Frame::TabWidth(int) const
{
  return 100;
}


