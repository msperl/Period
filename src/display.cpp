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

#include "display.h"
#include "xgprojec.h"

CMyDisplay::CMyDisplay(CProject *project,int type)
  :Registered(project),myID(type)
{
  Registered->RegisterDisplay(this);
}
CMyDisplay::~CMyDisplay()
{
  Registered->DeregisterDisplay(this);
}

void CMyDisplay::UpdateDisplay(int)
{
  // this should never be called !!!
  MYERROR("CMyDisplay::UpdateDisplay called");
}

void CMyDisplay::UpdateData()
{
  // this should never be called !!!
  MYERROR("CMyDisplay::UpdateData called");
}
