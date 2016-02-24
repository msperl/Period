/*
 * Program:     Period98
 *
 * File:        xttable.h
 * Purpose:     header-file for
 *              ?
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */

#ifndef __t_Table_h_
#define __t_Table_h_
#include <wx.h>
#include "project.h"
#include "xgtable.h"
#include <math.h>

///
class t_Table_Frame : public CMyTable
{
public:
  ///
  t_Table_Frame(CProject *data,int type,
		wxFrame* parent, char*Name,
		int x=-1, int y=-1,int width=-1, int height=-1);
  ///
  ~t_Table_Frame();

  ///
  myString GetDocTitle();

protected:
  ///
  myString GetHeader(int column) const;
  ///
  int NumberOfLines() const;
  ///
  myString GetData(int line,int column) const;
  
  /// Eventhandler for Mouse-events
  void OnMouseEvent(wxMouseEvent& event,int x,int y);

  ///
  int NumberOfTabs() const;
  ///
  int TabWidth(int i) const;

private:
  ///
  CTimeString *mData;
};

#endif
