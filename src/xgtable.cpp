/*
 * Program:     Period98
 *
 * File:        xgtable.cpp
 * Purpose:     implementation-file for
 *              ?
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */

#include <math.h>
#include "xgtable.h"
#include "xgmenu.h"
#include "wxtxtprint.h"

CMyTable::CMyTable(CProject *data, int type,
		   wxFrame *parent, char *title,
		   int x, int y, int width, int height,
		   long style, char *name,int flag)
  :CMyFrame(data,type,parent,title,x,y,width,height,style,name),LineHeight(20)
{
  myPlotArea=NULL;
  if (flag)
    {
      CreateCanvas();
      // Show the Frame
      Show(TRUE);
    }
}

CMyTable::~CMyTable()
{
}

void CMyTable::CreateCanvas()
{
  // creates the Child-frame
  int w,h;
  GetClientSize(&w,&h);
  if (!myPlotArea)
    {
      myPlotArea= new CMyTableArea(this,0,0,w,h);
    }
}


Bool CMyTable::OnClose()
{
  return TRUE;
}

myString CMyTable::GetDocTitle()
{
  return MYTABLE_TITLE;
}

void CMyTable::OnMyMouseEvent(wxMouseEvent& event)
{
  int row,col;
  // get mouse-coordinates
  float x,y;
  x=event.x;
  y=event.y;
  // probably we need to normalize
  // calculate offsets..
  // the x-offset
  {
    int xx=0;
    col=-1;
    for (int i=0;i<NumberOfTabs();i++)
      {
	int width=TabWidth(i);
	if ((x>=xx)&&(x<xx+width))
	  {
	    col=i;
	    break;
	  }
	xx+=width;
      }
  }
  // the y-offset
  {
    row=(int)((float)y/LineHeight)-1;
  }
  
  // now call user-routines
  OnMouseEvent(event,col,row);
}

void CMyTable::OnMouseEvent(wxMouseEvent& , int , int )
{
  // nothing to be done
}

class myTablePrintOut : public wxTextPrintout
{
public:
  myTablePrintOut(char *text,char* title,int border)
    : wxTextPrintout("Printout",text,
		     (float)border,
#ifdef wx_msw
		     8,
		     wxMODERN,wxNORMAL,wxNORMAL,
		     90.0f,
		     8.f,4.f,-1.f,-1.f
#else
		     6,
		     wxMODERN,wxNORMAL,wxNORMAL,
		     70.f,
		     6.f,4.f,-1.f,-1.f
#endif
		     )
  {
    Titlemsg=title;
  }

  virtual void PlotHeader(wxDC *dc,
                          float x, float y, float w, float h,
                          int /*page*/)
  {
    float y0=y;
    PlotLine(dc,x,y,w,h,&y0,Titlemsg,TRUE);
  }

  virtual void NextTab(float *val,int col)
  {
    (*val)=GetScale()*GetTabSize()*col;
  }

private:
  char* Titlemsg;
};

void CMyTable::OnPrint()
{
  int i,j;
  myString TextString;
  // fill in String
  for (i=0;i<NumberOfLines();i++)
    {
      for (j=0;j<NumberOfTabs()-1;j++)
	{
	  TextString+=GetData(i,j)+"\t";
	}
      TextString+=GetData(i,NumberOfTabs()-1)+"\n";
    }
  // Fill in header
  myString Header;
  for (j=0;j<NumberOfTabs()-1;j++)
    {
      Header+=GetHeader(j)+"\t";
    }
  Header+=GetHeader(NumberOfTabs()-1)+char(0);
  // now print it
#ifndef wx_msw
  wxSetPrinterOrientation(PS_LANDSCAPE);
#endif
  wxPrinter printer;
  myTablePrintOut printout(TextString.chars(),Header.chars(),50);
  printer.Print(this,&printout,TRUE);
}

int CMyTable::PaintPlot(wxDC & DC, int x0,int y0,int w0, int h0, int first)
{
  // Create the font to use
  wxFont *myFont=wxTheFontList->FindOrCreateFont(14,wxMODERN,
						 wxNORMAL,
						 wxNORMAL,
						 FALSE);
  // Set Font for DC (Device Context)
  DC.SetFont(myFont);
  // get font extent for 1 character
  // Clean Area
  DC.SetPen(wxWHITE_PEN);
  DC.SetBrush(wxWHITE_BRUSH);
  DC.DrawRectangle((float)x0,(float)y0,(float)w0,(float)h0);
  DC.SetPen(wxBLACK_PEN);
  int last;
  // now Draw Plot...
  {
    int y=y0;
    // Draw Header
    int row,col,tabstart;
    tabstart=0;
    for (col=0;col<NumberOfTabs();col++)
      {
	// Get width
	int width=TabWidth(col);
	// Draw a box
	DC.SetBrush(wxLIGHT_GREY_BRUSH);
	DC.DrawRectangle((float)tabstart,(float)y,(float)width,(float)LineHeight);
	DC.SetBrush(wxTRANSPARENT_BRUSH);
	// Draw text
	DC.SetBackgroundMode(wxTRANSPARENT);
	DrawText(DC,GetHeader(col),tabstart,y,width,LineHeight,0);
	DC.SetBackgroundMode(wxSOLID);
	// Calculate next TabStart
	tabstart+=width;
      }
    // Next Line
    y+=LineHeight;
    // Draw Data
    for (row=first;row<NumberOfLines();row++)
      {
	// TabStart
	tabstart=0;
	// Do all Columnes
	for (col=0;col<NumberOfTabs();col++)
	  {
	    // Get width
	    int width=TabWidth(col);
	    // Draw a box
	    DC.DrawRectangle((float)tabstart,(float)y,
			     (float)width,(float)LineHeight);
	    // Draw text
	    DrawText(DC,GetData(row,col),tabstart+1,y,width,LineHeight,1);
	    // Calculate next TabStart
	    tabstart+=width;
	  }
	// Start of next line
	y+=LineHeight;
	if (y+LineHeight>y0+h0)
	  {
	    // we have allmost reached the bottom...
	    break;
	  }
      }
    last=row;
  }
  // now return the first line not drawn
  return last;
}

void CMyTable::DrawText(wxDC &DC, myString const & text,
			int x0, int y0,
			int w0, int h0,
			int where)
{
  // Find out about Size of Text
  float w,h;
  DC.GetTextExtent(text.chars(),&w,&h);
  // Find Start-coords
  int x,y;
  if (where==0)
    {
      // Centered
      x=int(x0+(w0-w)/2.0);
    }
  else if (where<0)
    {
      // Left Alligned
      x=int(x0+5);
    }
  else
    {
      // Right Alligned
      x=int(x0+w0-w-5);
    }
  y=int(y0+(h0-h)/2.0);
  // Now Draw the Text
  DC.DrawText(text.chars(),(float)x,(float)y);
}

void CMyTable::UpdateDisplay(int flag)
{
  // Get Scrolling-position
  int x=0,y=0;
  if (flag)
    {
      myPlotArea->ViewStart(&x,&y);
    }
  // Get Total Width of Screen
  int LineWidth=0;
  for (int i=0;i<NumberOfTabs();i++)
    {
      LineWidth+=TabWidth(i);
    }

  // Get the Font-size
  {
    // Create the font to use
    wxFont *myFont=wxTheFontList->FindOrCreateFont(14,wxMODERN,
						   wxNORMAL,
						   wxNORMAL,
						   FALSE);
    // Set Font for DC (Device Context)
    wxDC &DC=*(myPlotArea->GetDC());
    DC.SetFont(myFont);
    float w,h;
    DC.GetTextExtent(MYTABLE_DUMMY,&w,&h);
    LineHeight=int(h+10);
    myPlotArea->SetLineHeight(LineHeight);
  }
  myPlotArea->SetScrollbars(100,LineHeight,
			    (LineWidth/100)+1,NumberOfLines()+2,
			    5,5,
			    x,y);
  // repaint plot
  myPlotArea->OnPaint();
}

//////////////////////////////////////////////////////////////////////

CMyTableArea::CMyTableArea(CMyTable* Parent,
			 int x, int y,
			 int w, int h)
  :wxCanvas(Parent,x,y,w,h,0)
{
  SetBackground(wxWHITE_BRUSH);
}

CMyTableArea::~CMyTableArea()
{
}

void CMyTableArea::OnPaint()
{
  Clear();
  // Get the device-context
  wxDC &dc=*GetDC();
  // Get the block to be repainted
  int w,h;
  GetClientSize(&w,&h);
  // Find out about View-start
  int x,y,first;
  ViewStart(&x,&first);
  y=first*LineHeight;
  // Now tell parent to paint the Stuff
  ((CMyTable*)GetParent())->PaintPlot(dc,x,y,w,h,first);
  //That's all folks
}

void CMyTableArea::OnScroll(wxCommandEvent& event)
{
  // Send Message down
  wxCanvas::OnScroll(event);
}

void CMyTableArea::OnSize(int w,int h)
{
  // Send Message down
  wxCanvas::OnSize(w,h);
}

void CMyTableArea::OnEvent(wxMouseEvent& event)
{
  ((CMyTable*)GetParent())->OnMyMouseEvent(event);
  wxCanvas::OnEvent(event);
}




