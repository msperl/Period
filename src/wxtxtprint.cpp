/*                                                              -*- C++ -*-
 * $Id: wxtextprint.cpp,v 1.0 1996/06/20 10:35:52 martin Exp $
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

//
// first define the Printout-class
//

#include <ctype.h>
#include "wxtxtprint.h"

wxTextPrintout::wxTextPrintout(char *title, char *text,
			       float border,
			       int pointsize, int family, 
			       int style, int weight,
			       float tab,
			       float header,float headerrule,
			       float footer,float footerrule
			       )
  :wxPrintout(title),TextToPrint(text),
   FontSize(10),FontFamily(family),
   FontStyle(style),FontWeight(weight),
   BorderSize(50.f),
   TabSize(80.f)
{
  // set the Default number of pages
  Pages=32000;
  // if a NULL-pointer, give it an empty one...
  if (!TextToPrint) 
    {
      TextToPrint="(NULL)\n";
    }
  // set the defaults
  SetFontSize(pointsize);
  SetHeaderSize(0.f);
  SetHeaderRuleOff(0.f);
  // acomedate for footer-message by default... 
  SetFooterSize((float)GetFontSize());
  SetFooterRuleOff((float)GetFontSize());
  // Now set the other stuff
  SetBorderSize(border);
  SetHeaderSize(header);
  SetHeaderRuleOff(headerrule);
  SetFooterSize(footer);
  SetFooterRuleOff(footerrule);
  SetTabSize(tab);
}

float wxTextPrintout::GetScale()
{
  // First calculate scale
  int pw=0,ph=0,sw=0,sh=0;
  GetPPIPrinter(&pw,&ph);
  GetPPIScreen(&sw,&sh);
  if ((sw==0)||(sh==0))
    { return float(1);}
  return (float(pw)/sw+float(ph)/sh)/2;
}

void wxTextPrintout::GetPageInfo(int *min, int *max, int *from, int *to)
{ 
  *min=1; 
  *max=Pages; 
  *from=1; 
  *to=Pages; 
}

Bool wxTextPrintout::HasPage(int page)
{
  return (page<=Pages);
}

void wxTextPrintout::OnPreparePrinting()
{
  Pages=32000;
  if (GetDC()) {OnPrintPage(Pages-1);}
}

void wxTextPrintout::PlotHeader(wxDC * /* dc */,
				float /* x */,float /* y */, 
				float /* w */, float /* h */,
				int /* page */)
{
  // nothing to be done...
}

void wxTextPrintout::PlotFooter(wxDC *dc,
				float x,float y, float w, float /*h*/,
				int page)
{
  char txt[256];
  sprintf(txt,"page %i",page);
  float tw,th;
  dc->GetTextExtent(txt,&tw,&th);
  dc->DrawText(txt,x+(w-tw)/2,y);
}

Bool wxTextPrintout::OnPrintPage(int page)
{    
  if (page>Pages)
    {
      return FALSE;
    }
  int i;
  // first get Pagesize
  float x,y,w,h;
  int w0,h0;
  GetPageSizePixels(&w0,&h0);
  // Set sizes
  x=0.f;y=0.f;
  w=float(w0); h=float(h0);
  // now work out the borders
  x=x+BorderSize;
  y=y+GetBorderSize()+(GetHeaderSize()+GetHeaderRuleOff())*GetScale();
  w=w-GetBorderSize()-x;
  h=h-GetBorderSize()-y-(GetFooterSize()+GetFooterRuleOff())*GetScale();
  // get the Device Context
  wxDC *dc=GetDC();
  // now create the font if necessary
  wxFont *myFont=wxTheFontList->FindOrCreateFont(int(GetFontSize()*GetScale()),
						 FontFamily,
						 FontStyle,
						 FontWeight);
  dc->SetFont(myFont);
  // now start drawing
  // protect rest of the page
  dc->SetClippingRegion(x,y,w,h);
  // print the text
  char * StillToPrint=TextToPrint;
  float yplot;
  for (i=1;i<=page;i++)
    {
      yplot=y;
      while (
	     (StillToPrint=PlotLine(dc,x,y,w,h,&yplot,StillToPrint,(i==page)))
	     && (yplot!=-1)
	     )
	{ 
	  // Do nothing - We are still printing...
	}
      if (StillToPrint==0)
	{
	  Pages=i;
	  if ((i!=page))
	    {
	      return FALSE;
	    }
	}
    }
  dc->DestroyClippingRegion();
  // Print Header
  if (GetHeaderSize())
    {
      dc->SetClippingRegion(x,
			    y-(GetHeaderSize()+GetHeaderRuleOff())*GetScale(),
			    w,
			    GetHeaderSize()*GetScale()
			    );
      PlotHeader(dc,
		 x,y-(GetHeaderSize()+GetHeaderRuleOff())*GetScale(),
		 w,GetHeaderSize()*GetScale(),
		 page);
      dc->DestroyClippingRegion();
    }
  // Print footer
  if (GetFooterSize())
    {
      dc->SetClippingRegion(x,
			    y+h+GetFooterRuleOff()*GetScale(),
			    w,
			    GetFooterSize()*GetScale()
			    );
      PlotFooter(dc,
		 x,y+h+GetFooterRuleOff()*GetScale(),
		 w,GetFooterSize()*GetScale(),
		 page);
      dc->DestroyClippingRegion();
    }
  // return TRUE, as the page has been printed
  return TRUE;
}

void wxTextPrintout::NextTab(float *x, int /* tabcount */)
{
  float tab=0.0f;
  while (tab<*x)
    {
      tab+=GetTabSize()*GetScale();
    }
  // return the new Value
  *x=tab;
}

char* wxTextPrintout::PlotLine(wxDC *dc,
			       float x0, float y0, float w, float h, 
			       float *y,
			       char *StillToPrint,
			       int PrintIt)
{
  if (!StillToPrint)
    {
      *y=-1.0f;
      return 0;
    }
  // buffer to print currenly
  char tmp[2048];
  tmp[0]=' ';
  tmp[1]=0;
  // number of chars to print
  int numchars;
  // size of message
  float tw,th,x;
  // chck if we are stil within range
  if (*y>y0+h)
    {
      *y=-1.0f;
      // size is to big
      return StillToPrint;
    }
  // now find out what to print
  x=0.0f;
  int quit=0;
  int base=0;
  tw=0.0f;
  th=0.0f;
  // tabcounter
  int tabcount=0;
  for (numchars=0;numchars<2048;numchars++)
    {
      // fill in Buffer
      char c=StillToPrint[numchars];
      switch (c)
	{
	case '\n': // newline
	  // ignore this character
	  numchars++;
	  quit=1;
	  break;
	case '\t': // Tabulated
	  // print text first
	  if (PrintIt) 
	    { 
	      // draw plot
	      dc->DrawText(tmp,x0+x,*y); 
	    }
	  // we are at the next-tab-count
	  tabcount++;
	  // get extent
	  dc->GetTextExtent(tmp,&tw,&th);
	  // calculate new start-position
	  x=x+tw;
	  // and now get next tab
	  NextTab(&x,tabcount);
	  // empty Buffer
	  tmp[0]=' ';
	  tmp[1]=0;
	  base=numchars+1;
	  break;
	default:
	  if (!iscntrl(c))
	    {
	      // copy characters
	      tmp[numchars-base]=c;
	      tmp[numchars-base+1]=0;
	      // get size
	      dc->GetTextExtent(tmp,&tw,&th);
	      if (x+tw>w)
		{
		  numchars--;
		  tmp[numchars-base]=0;
		  quit=1;
		}
	    }
	  break;
	}
      if (quit) { break; }
    }
  // get size
  dc->GetTextExtent(tmp,&tw,&th);
  // does line still fit on Screen?
  StillToPrint=&StillToPrint[numchars];
  // draw text
  if (PrintIt) 
    { dc->DrawText(tmp,x0+x,*y); }
  // calculate new coordinate
  *y+=int(th);
  // check what to return
  if (StillToPrint[0]==0) { return 0;}
  return StillToPrint;
}

void wxPrintText(wxFrame *Parent,
		 char *title,char* text,
		 int border,
		 int pointsize, int family, int style, int weight,
		 float tab,
		 float header,float headerrule,
		 float footer,float footerrule
		)
{
#ifndef wx_msw
  wxSetPrinterOrientation(PS_PORTRAIT);
#endif
  wxPrinter printer;
  wxTextPrintout printout(title,text,
			  (float)border,
			  pointsize,family,style,weight,
			  tab,
			  header,headerrule,
			  footer,footerrule);
  printer.Print(Parent,&printout,TRUE);
}

void wxPrintPreviewText(wxFrame *Parent,
			char *title,char* text,
			int border,
			int pointsize, int family, int style, int weight,
			float tab,
			float header,float headerrule,
			float footer,float footerrule
			)
{
  wxPrintPreview *preview= 
    new wxPrintPreview(
		       new wxTextPrintout(title,text,
					  (float)border,
					  pointsize,family,style,weight,
					  tab,
					  header,headerrule,
					  footer,footerrule),
		       new wxTextPrintout(title,text,
					  (float)border,
					  pointsize,family,style,weight,
					  tab,
					  header,headerrule,
					  footer,footerrule)
		       );
  wxPreviewFrame *frame= new wxPreviewFrame(preview,Parent,title,
					    0,0,500,500);
  frame->Center(wxBOTH);
  frame->Initialize();
  frame->Show(TRUE);
}






