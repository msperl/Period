/*
 * Program:     Period98
 *
 * File:        xtadjust.cpp
 * Purpose:     implementation-file for
 *              ?
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */
#include <wx.h>

#include "xtadjust.h"
#include "xgprojec.h"
#include "ldialtxt.h"
#include "xgeneral.h"
#include "wxtxtprint.h"
#include "xgdial.h"

// ADJUST

wxListBox * AdjustListbox;
wxCheckBox * UseWeightedData;
int AdjustWhat;

void AdjustAdjust(wxButton& adjust,wxEvent &)
{
  // find out what has been selected
  int *selection;
  int selected=AdjustListbox->GetSelections(&selection);
  // find out if anything is selected...
  if (selected==0)
    {
      wxMessageBox(TIME_NOSELECT,WARNING);
    }
  else
    {
      // find out the TRUE id
      int * idlist= new int [selected];
      for (int i=0;i<selected;i++)
	{
	  idlist[i]=(int)AdjustListbox->GetClientData(selection[i]);
	}
      // adjust data
      myProject.Adjust(AdjustWhat,myProject.GetPeriodUseData(),
		       selected,idlist,UseWeightedData->GetValue());
      // delete temporary data
      delete [] idlist;
      // calculate Zeropoints
      myProject.CalcAverage();
      // now Update Display
      AdjustFillBox(AdjustWhat);
    }
  // and hide dialog
  (adjust.GetParent())->Show(FALSE);
}

void AdjustSave(wxButton&,wxEvent &)
{
  // save the textfile
  myProject.SaveStat(AdjustWhat,UseWeightedData->GetValue(),"");
}

void AdjustPrint(wxButton& ,wxEvent &)
{
  myString lines=
    myProject.StatStrings(AdjustWhat,
			   UseWeightedData->GetValue());
  // print file
  {
    wxPrintText(NULL,"",lines.chars(),
		50,
#ifdef wx_msw
		16,
#else
		12,
#endif
		wxMODERN,wxNORMAL,wxNORMAL
		);
  }
}


void AdjustQUIT(wxButton& calc,wxEvent &)
{
  calc.GetParent()->Show(FALSE);
}

int WhatButton;

void AdjustSelection(wxButton &Butt, wxEvent &)
{
  WhatButton=(int)Butt.GetClientData();
  // Update display
  AdjustFillBox(WhatButton);
}

void AdjustSelectionCheck(wxCheckBox &, wxEvent& )
{
  // Update display
  AdjustFillBox(WhatButton);
}

void AdjustFillBox(int id)
{
  int dummy=5;
  AdjustWhat=id;
  AdjustListbox->Clear();
  int ShallWeUseWeights=UseWeightedData->GetValue();
  int identity;
  for (int i=0;i<myProject.NumberOfNames(id);i++)
    {
      myString line=myProject.GetNameStats(id,
					   i,
					   ShallWeUseWeights,
					   &identity
					   );
      if (identity!=-99)
	{
	  AdjustListbox->Append(line.chars(),(char*)identity);
	}
    }
  dummy+=6;
}

void AdjustData(wxWindow *Frame)
{
  // calculate Zeropoints
  myProject.CalcAverage();
  // Create Dialog
  myDialogBox dialog(Frame,ADJ_DAT_TITLE,TRUE,-1,-1,600,340);
  // sets the Button-font
  wxFont *button=wxTheFontList->FindOrCreateFont(
						 12,
						 wxMODERN,
						 wxNORMAL,
						 wxNORMAL);
  dialog.SetButtonFont(button);
  // sets the Labelfont
  wxFont *label=wxTheFontList->FindOrCreateFont(
						12,
						wxMODERN,
						wxBOLD,
						wxNORMAL);  
  dialog.SetLabelFont(label);
  // List-Buttons
  wxButton *HeadBut[4];
  int i;
  for (i=0;i<4;i++)
    {
      HeadBut[i]= new wxButton(&dialog,
			       (wxFunction)&AdjustSelection,
			       myProject.NameSet(i).chars()
			       );
      HeadBut[i]->SetClientData((char*)i);
    }
  dialog.NewLine();
  // create checkbox for weights
  UseWeightedData= new wxCheckBox(&dialog,
				  (wxFunction)&AdjustSelectionCheck,
				  USE_WEIGHTS);
  UseWeightedData->SetValue(myProject.GetPeriodUseWeight());
  //
  dialog.NewLine();
  // create list-box
  wxMessage *Heading;
  if (myProject.GetPeriodUseData()==Observed)
    {
      Heading=new wxMessage(&dialog,ADJ_DAT_HEADING_ORI"  ");
    }
  else
    {
      Heading=new wxMessage(&dialog,ADJ_DAT_HEADING_ADJ"  ");
    }
  dialog.NewLine();
  int xs,ys;
  Heading->GetSize(&xs,&ys);
  AdjustListbox= new wxListBox(&dialog,NULL,"",wxEXTENDED,
			       -1,-1,xs,200,
			       0,NULL,wxEXTENDED);
  // fill List-Box
  WhatButton=0;
  AdjustFillBox(WhatButton);

  //AdjustListbox->Fit();
  FitListBox(AdjustListbox);
  dialog.NewLine();
  wxButton *Adjust,*Save,*Print,*Quit;
  Adjust= new wxButton(&dialog,
		       (wxFunction)&AdjustAdjust,
		       OKAY);
  Adjust->SetDefault();
  Save= new wxButton(&dialog,
		     (wxFunction)&AdjustSave,
		     ADJ_DAT_SAVE);
  Print= new wxButton(&dialog,
		      (wxFunction)&AdjustPrint,
		      ADJ_DAT_PRINT);
  Quit= new wxButton(&dialog,
		     (wxFunction)&AdjustQUIT,
		     CANCEL);
  // now fit the frame
  dialog.Fit();
  // refit Butons
  {
    // get relevant sizes
    int Framew,Frameh;
    dialog.GetSize(&Framew,&Frameh);
    int OKx,OKy,OKw,OKh;
    Adjust->GetPosition(&OKx,&OKy);
    Adjust->GetSize(&OKw,&OKh);
    int spacing=dialog.GetHorizontalSpacing();
    int width=(Framew-OKx-4*spacing)/4;
    // now resize Buttons Adjust and Quit
    Adjust->SetSize(OKx,OKy,width,-1);
    Save  ->SetSize(OKx+1*(width+spacing),-1,width,-1);
    Print ->SetSize(OKx+2*(width+spacing),-1,width,-1);
    Quit  ->SetSize(OKx+3*(width+spacing),-1,width,-1);
    // now all the others
    width=(Framew-OKx-4*spacing)/4;
    int pos=OKx;
    for (i=0;i<4;i++)
      {
	HeadBut[i]->SetSize(pos,-1,width,-1);
	pos+=width+spacing;
      }
  }
  // center Dialog
  dialog.Centre();
  // now show the frame
  dialog.Show(TRUE);
}

// SUBDIVIDE

void SubDivideData(wxWindow *frame)
{
  myDialogBox dialog(frame,ADJ_SUB_TITLE,TRUE);
  // Gapsize
  wxText *Gap= new wxText(&dialog,NULL,ADJ_SUB_GAP,
			  "            ");
  Gap->SetValue(ADJ_SUB_GAP_DEF);
  dialog.NewLine();
  // what column
  myString StrOptions[4];
  char *options[4];
  for (int i=0;i<4;i++)
    {
      StrOptions[i]=myProject.NameSet(i);
      options[i]=StrOptions[i].chars();
    }
  dialog.SetLabelPosition(wxVERTICAL);
  wxChoice *Column= new wxChoice(&dialog,NULL,
				 ADJ_SUB_COLUMN,
				 -1,-1,-1,-1,
				 4,options);
  // Select the first one...
  Column->SetSelection(0);
  dialog.SetLabelPosition(wxHORIZONTAL);

  dialog.NewLine();
  // NamePrefix
  wxText *Name= new wxText(&dialog,NULL,ADJ_SUB_PREFIX,
			   "              ");
  Name->SetValue(ADJ_SUB_PREFIX_DEF);
  dialog.NewLine();
  // Shall we use counter or Time as secondary index
  wxCheckBox *UseCounter= new wxCheckBox(&dialog,NULL,ADJ_SUB_COUNTER);
  dialog.NewLine();
  // OK and Cancel-Buttons...
  wxButton *DoIt,*Quit;
  DoIt= new wxButton(&dialog,
		     (wxFunction)&SubDivideDoIt,
		     OKAY);
  DoIt->SetClientData(NULL);
  DoIt->SetDefault();
  Quit= new wxButton(&dialog,
		  (wxFunction)&SubDivideQUIT,
		  CANCEL);
  // now fit the Frame
  dialog.Fit();
  // now refit Buttons
  {
    // get relevant sizes
    int Framew,Frameh;
    dialog.GetSize(&Framew,&Frameh);
    int OKx,OKy,OKw,OKh;
    DoIt->GetPosition(&OKx,&OKy);
    DoIt->GetSize(&OKw,&OKh);
    int Cancelx,Cancely;
    Quit->GetPosition(&Cancelx,&Cancely);
    // calculate widths
    int spacing=Cancelx-(OKx+OKw);
    int width=(Framew-OKx-2*spacing)/2;
    // now resize Buttons
    DoIt->SetSize(OKx,OKy,width,-1);
    Quit->SetSize(OKx+width+spacing,-1,width,-1);
  }
  // center it on screen
  dialog.Centre();
  // now show this Dialog
  dialog.Show(TRUE);
  // and Shall we go on?
  if (DoIt->GetClientData()!=NULL)
    {
      // Let's Subdivide
      myProject.Subdivide(Gap->GetValue(),
			  Column->GetSelection(),
			  UseCounter->GetValue(),
			  Name->GetValue()
			  );
    }
}

void SubDivideQUIT(wxButton& calc,wxEvent &)
{
  calc.GetParent()->Show(FALSE);
}

void SubDivideDoIt(wxButton& calc,wxEvent &)
{
  calc.GetParent()->Show(FALSE);
  calc.SetClientData((char*)1);
}

// RENAME

void RenameSelection(wxWindow *Frame)
{
  myDialogBox dialog(Frame,ADJ_REN_TITLE,TRUE);
  // NamePrefix
  wxText *Name= new wxText(&dialog,NULL,ADJ_REN_WHAT,
			   "              ",-1,-1);
  Name->SetValue(ADJ_REN_WHAT_DEF);
  dialog.NewLine();
  // what column
  myString StrOptions[4];
  char *options[4];
  for (int i=0;i<4;i++)
    {
      StrOptions[i]=myProject.NameSet(i);
      options[i]=StrOptions[i].chars();
    }
  
  dialog.SetLabelPosition(wxVERTICAL);
  wxChoice *Column= new wxChoice(&dialog,NULL,
				 ADJ_SUB_COLUMN,
				 -1,-1,-1,-1,
				 4,options);
  Column->SetSelection(0);
  dialog.SetLabelPosition(wxHORIZONTAL);
  dialog.NewLine();
  // OK and Cancel-Buttons...
  wxButton *DoIt,*Quit;
  DoIt= new wxButton(&dialog,
		     (wxFunction)&RenameDoIt,
		     OKAY);
  DoIt->SetClientData(NULL);
  DoIt->SetDefault();
  Quit=new wxButton(&dialog,
		  (wxFunction)&RenameQUIT,
		  CANCEL);
  // refit the dialog
  dialog.Fit();
  // now refit Buttons
  {
    // get relevant sizes
    int Framew,Frameh;
    dialog.GetSize(&Framew,&Frameh);
    int OKx,OKy,OKw,OKh;
    DoIt->GetPosition(&OKx,&OKy);
    DoIt->GetSize(&OKw,&OKh);
    int Cancelx,Cancely;
    Quit->GetPosition(&Cancelx,&Cancely);
    // calculate widths
    int spacing=Cancelx-(OKx+OKw);
    int width=(Framew-OKx-2*spacing)/2;
    // now resize Buttons
    DoIt->SetSize(OKx,OKy,width,-1);
    Quit->SetSize(OKx+width+spacing,-1,width,-1);
  }
  // center it on screen
  dialog.Centre();
  // now show this Dialog
  dialog.Show(TRUE);
  // and Shall we go on?
  if (DoIt->GetClientData()!=NULL)
    {
      // Let's Subdivide
      myProject.RenameSelection(Column->GetSelection(),
			  Name->GetValue()
			  );
    }
}

void RenameQUIT(wxButton& calc,wxEvent &)
{
  calc.GetParent()->Show(FALSE);
}

void RenameDoIt(wxButton& calc,wxEvent &)
{
  calc.GetParent()->Show(FALSE);
  calc.SetClientData((char*)1);
}



