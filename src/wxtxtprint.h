/*                                                              -*- C++ -*-
 * $Id: wxtextprint.h,v 1.0 1996/06/20 10:35:52 martin Exp $
 *
 * Purpose: base event handler of windows etc.
 *
 * Authors: Martin Sperl
 *
 * Copyright: (C) 1995, GNU (Martin)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * Additionally everyone using this library has to announce it with:
 *
 *   This software uses the wxWindows-Xt GUI library
 *   (C) Markus Holzem, available via
 *       ftp://ftp.aiai.ed.ac.uk/pub/packages/wxwin/ports/xt
 */
#ifndef wxtextprint_h
#define wxtextprint_h

//#ifdef __GNUG__
//#pragma interface
//#endif

#include <wx.h>
#include <wx_print.h>
class wxTextPrintout : public wxPrintout
{
public:
  wxTextPrintout(char *title, char *text,
		 float border=50.,
		 int pointsize=10, 
		 int family=wxDEFAULT, 
		 int style=wxNORMAL, 
		 int weight=wxNORMAL,
		 float tab=80.,
		 float header=0.,float headerrule=0.,
		 float footer=-1.,float footerrule=-1.
		 );

  virtual void OnPreparePrinting();
  virtual Bool OnPrintPage(int page);
  virtual void GetPageInfo(int *min, int *max, int *from, int *to);
  virtual Bool HasPage(int page);

  // Stuff relevant for a header

  float GetHeaderSize()              { return HeaderSize; }
  void  SetHeaderSize(float size)    { if (size>=0) {HeaderSize=size;} }
  float GetHeaderRuleOff()           { return HeaderRuleOff; }
  void  SetHeaderRuleOff(float size) { if (size>=0) {HeaderRuleOff=size;} }
  virtual void PlotHeader(wxDC *dc,
			  float x, float y, float w, float h,
			  int page);

  // Stuff relevant for the footer

  float GetFooterSize()              { return FooterSize; }
  void  SetFooterSize(float size)    { if (size>=0) {FooterSize=size;} }
  float GetFooterRuleOff()           { return FooterRuleOff; }
  void  SetFooterRuleOff(float size) { if (size>=0) {FooterRuleOff=size;} }
  virtual void PlotFooter(wxDC *dc,
			  float x0,float y0, float w0, float h0,
			  int page);

  // gives the next tab-position to use
  float GetTabSize()              { return TabSize; }
  void  SetTabSize(float size)    { if (size>=0) {TabSize=size;} }
  virtual void NextTab(float *val, int tab);

  // Borders
  float GetBorderSize()           { return BorderSize; }
  void  SetBorderSize(float size) { if (size>=0) {BorderSize=size;} }

  // Fonts
  int GetFontSize()           { return FontSize; }
  void  SetFontSize(int size) { if (size>=0) {FontSize=size;} }

  // Gets the scale in which to display...
  virtual float GetScale();
  // Plots a certain line
  virtual char* PlotLine(wxDC *dc,
			 float x0,float y0, float w0, float h0,
			 float *y,
			 char *StillToPrint,
			 int PrintIt );
protected:
  // the text to print
  char* TextToPrint;
  // Font-definitions
  int FontSize;
  int FontFamily;
  int FontStyle;
  int FontWeight;
  // Page-count
  int Pages;
  // the some ruleoffes defined
  float BorderSize;
  float HeaderSize;
  float HeaderRuleOff;
  float FooterSize;
  float FooterRuleOff;
  // Tabs
  float TabSize;
};

void wxPrintText(wxFrame *Parent,
		 char *title,char* text,
		 int border=50,
		 int pointsize=10, 
		 int family=wxDEFAULT, 
		 int style=wxNORMAL, 
		 int weight=wxNORMAL,
		 float tabs=80.,
		 float header=0.,float headerrule=0.,
		 float footer=-1.,float footerrule=-1.
		);

void wxPrintPreviewText(wxFrame *Parent,
			char *title,char* text,
			int border=50,
			int pointsize=10, 
			int family=wxDEFAULT, 
			int style=wxNORMAL, 
			int weight=wxNORMAL,
			float tabs=80.,
			float header=0.,float headerrule=0.,
			float footer=-1.,float footerrule=-1.
			);

#endif // wxtextprint_h
