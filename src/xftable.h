/*
 * Program:     Period98
 *
 * File:        xftable.h
 * Purpose:     header-file for
 *              ?
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */

#ifndef __f_Table_h_
#define __f_Table_h_
#include <wx.h>
#include "project.h"
#include "xgtable.h"
#include <math.h>
#include "display.h"

///
class f_Table_Frame : public CMyTable
{
public:
  ///
  f_Table_Frame(CProject *data,int type,
		wxFrame* parent, char*Name,
		int x=-1, int y=-1,int width=-1, int height=-1);
  ///
  ~f_Table_Frame();

  ///
  myString GetDocTitle();

protected:
  ///
  myString GetHeader(int column) const;
  ///
  int NumberOfLines() const;
  ///
  myString GetData(int line,int column) const;

  ///
  int NumberOfTabs() const;
  ///
  int TabWidth(int i) const;

private:
  ///
  CFourier *mData;
};

#endif
