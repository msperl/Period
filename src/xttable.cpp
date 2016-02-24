/*
 * Program:     Period98
 *
 * File:        xttable.cpp
 * Purpose:     implementation-file for
 *              ?
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */

#include "xttable.h"
#include "xgmenu.h"
#include "str.h"
#include "ldialtxt.h"
#include "xgprojec.h"
#include "xgmainfr.h"
#include "xgeneral.h"
#include "xtrename.h"

#include "fgeneral.h"

t_Table_Frame::t_Table_Frame(CProject *data,int type,
			     wxFrame* parent, char*Name,
			     int x, int y,
			     int width, int height)
  :CMyTable(data,type,parent,Name,x,y,width,height,
	    wxSDI|wxDEFAULT_FRAME,TIMET_NAME)
{
  mData=&(data->GetTimeString());
  // Set MenuBar
  SetMenuBar(CreateMenuBar_Simple());
  CreateCanvas();
  // Update Display
   UpdateDisplay();
  // Show the window
  Show(TRUE);
}

t_Table_Frame::~t_Table_Frame()
{
}

myString t_Table_Frame::GetDocTitle()
{
  return mData->GetFileName();
}

myString t_Table_Frame::GetHeader(int column) const
{
  switch (column)
    {
    case 0:
      return TIMET_TIME; break;
    case 1:
      return TIMET_OBS; break;
    case 2:
      return TIMET_ADJ; break;
    case 3:
      return TIMET_CALC; break;
    case 4:
      return TIMET_RESOBS; break;
    case 5:
      return TIMET_RESADJ; break;
    case 6:
      return TIMET_WEIGHT; break;
    case 7:
      return TIMET_PNT_WEIGHT; break;
    case 8:
      return mData->NameSet(0); break;
    case 9:
      return mData->NameSet(1); break;
    case 10:
      return mData->NameSet(2); break;
    case 11:
      return mData->NameSet(3); break;
    }
  return TIMET_UKN;
}

int t_Table_Frame::NumberOfLines() const
{
  return mData->GetSelectedPoints();
}

myString t_Table_Frame::GetData(int line,int column) const
{
  double value;
  char txt[32];
  switch (column)
    {
    case 0:
      value=(*mData)[line].GetTime();
      sprintf(txt,FIXEDFORMAT_TIME,value);
      return txt;
      break;
    case 1:
      value=(*mData)[line].GetObserved();
      sprintf(txt,FIXEDFORMAT_AMPLITUDE,value);
      return txt;
      break;
    case 2:
      value=(*mData)[line].GetAdjusted();
      sprintf(txt,FIXEDFORMAT_AMPLITUDE,value);
      return txt;
      break;
    case 3:
      value=(*mData)[line].GetCalculated();
      sprintf(txt,FIXEDFORMAT_AMPLITUDE,value);
      return txt;
      break;
    case 4:
      value=(*mData)[line].GetDataResidual();
      sprintf(txt,FIXEDFORMAT_AMPLITUDE,value);
      return txt;
      break;
    case 5:
      value=(*mData)[line].GetAdjustedResidual();
      sprintf(txt,FIXEDFORMAT_AMPLITUDE,value);
      return txt;
      break;
    case 6:
      value=(*mData)[line].GetWeight();
      sprintf(txt,FIXEDFORMAT_WEIGHTS,value);
      return txt;
      break;
    case 7:
      value=(*mData)[line].GetPointWeight();
      sprintf(txt,FIXEDFORMAT_WEIGHTS,value);
      return txt;
      break;
    case 8:
      return mData->GetIDName(0,(*mData)[line].GetIDName(0)).GetName(); break;
    case 9:
      return mData->GetIDName(1,(*mData)[line].GetIDName(1)).GetName(); break;
    case 10:
      return mData->GetIDName(2,(*mData)[line].GetIDName(2)).GetName(); break;
    case 11:
      return mData->GetIDName(3,(*mData)[line].GetIDName(3)).GetName(); break;
    }
  return "???";
}

int t_Table_Frame::NumberOfTabs() const
{
  return 12;
}

int t_Table_Frame::TabWidth(int) const
{
  return 150;
}

void t_Table_Frame::OnMouseEvent(wxMouseEvent& event,int,int row)
{
  
  if (row<0) { return; }
  // is the cursor moving?
  if (event.RightDown())
    {
      if (event.ControlDown())
	{
	  DialogDeletePoint(row);
	}
      else
	{
	  DialogRelabelPoint(row); 
	}
    }
}
