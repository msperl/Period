/*
 * Program:     Period98
 *
 * File:        xpspecia.cpp
 * Purpose:     implementation-file for
 *              ?
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */
#include <wx.h>
#include "xgprojec.h"
#include "ldialtxt.h"
#include "xpspecia.h"
#include "xgeneral.h"
#include "fperiod.h"

void OnSpecialDialogButton(wxButton& cancel, wxEvent&)
{
  cancel.SetClientData((char*)1);
  (cancel.GetParent())->Show(FALSE); 
}

#define FILLSTRING "0123456789012345"

int SpecialDialog(wxWindow *frame)
{
  int i;

  // create a dialog
  wxDialogBox dialog(frame,SPE_SEL,TRUE,
		     -1,-1,510,350);

  (void) new wxMessage(&dialog,SPE_MSG);
  dialog.NewLine();
  
  int spacing=dialog.GetHorizontalSpacing();
  // List of all names

  wxListBox *mFreq,*mAmp,*mPha;
  
  dialog.SetLabelPosition(wxVERTICAL);
  
  mFreq= new wxListBox(&dialog,NULL,SPE_FRE,wxEXTENDED,
		    -1,-1,150,200,
		    0,NULL,wxEXTENDED);
  int x,y,w,h;
  mFreq->Append(FILLSTRING);
  FitListBox(mFreq);
  mFreq->Clear();
  mFreq->GetPosition(&x,&y);
  mFreq->GetSize(&w,&h);
  //######################
  mAmp=  new wxListBox(&dialog,NULL,SPE_AMP,wxEXTENDED,
		       x+w+spacing,y,150,200,
		       0,NULL,wxEXTENDED);
  mAmp->Append(FILLSTRING);
  FitListBox(mAmp);
  mAmp->Clear();
  mAmp->GetPosition(&x,&y);
  mAmp->GetSize(&w,&h);
  //######################
  mPha=  new wxListBox(&dialog,NULL,SPE_PHA,wxEXTENDED,
		       x+w+spacing,y,150,200,
		       0,NULL,wxEXTENDED);
  mPha->Append(FILLSTRING);
  FitListBox(mPha);
  mPha->Clear();

  dialog.NewLine();
  
  wxButton *OK= new wxButton(&dialog,
			     (wxFunction)&OnSpecialDialogButton,
			     CALCULATE);
  OK->SetDefault();
  wxButton *AmpVar= new wxButton(&dialog,
				 (wxFunction)&OnSpecialDialogButton,
				 SPE_AMPVAR);
  wxButton *Cancel= new wxButton(&dialog,
				 (wxFunction)&OnSpecialDialogButton,
				 CANCEL);

  // Fill the lists...
  CPeriod *Period=&(myProject.GetPeriod());
  myString list;
  for (i=0;i<Period->GetFrequencies();i++)
    {
      if ((*Period)[i].GetActive())
	{
	  // FreqNummer
	  myString base=myProject.GetNumber(i);
	  // Frequency...
	  if (!(*Period)[i].IsComposition())
	    {
	      list=base+":"+myProject.GetFrequency(i);
	      mFreq->Append(list.chars(),(char*)i);
	      mFreq->SetSelection(i,(*Period)[i].IsSelected(Fre));
	    }
	  // Amplitude...
	  list=base+":"+myProject.GetAmplitude(i,-1);
	  mAmp->Append(list.chars(),(char*)i);
	  mAmp->SetSelection(i,(*Period)[i].IsSelected(Amp));
	  // Phase...
	  list=base+":"+myProject.GetPhase(i,-1);
	  mPha->Append(list.chars(),(char*)i);
	  mPha->SetSelection(i,(*Period)[i].IsSelected(Pha));
	}
    }
  // set the selection
  for (i=0;i<Period->GetFrequencies();i++)
    {
      if ((*Period)[i].GetActive())
	{
	  if (!(*Period)[i].IsComposition())
	    {
	      mFreq->SetSelection(i,(*Period)[i].IsSelected(Fre));
	    }
	  mAmp->SetSelection(i,(*Period)[i].IsSelected(Amp));
	  mPha->SetSelection(i,(*Period)[i].IsSelected(Pha));
	}
    }
  
  // Fit the Dialog
  dialog.Fit();
  // resize Buttons
  {
    //get size of Listboxes
    int totw,totx;
    {
      int Fx,Fy,Px,Py,Pw,Ph;
      mFreq->GetPosition(&Fx,&Fy);
      mPha->GetPosition(&Px,&Py);
      mPha->GetSize(&Pw,&Ph);
      totw=Px+Pw;
      totx=Fx;
    }
    // get width of all Buttons
    int Calcw,AmpVarw,Cancelw;
    {
      int dummy;
      OK->GetSize(&Calcw,&dummy);
      AmpVar->GetSize(&AmpVarw,&dummy);
      Cancel->GetSize(&Cancelw,&dummy);
    }
    // now calculate new width
    double scale=1.0*(totw-2.0*spacing)/(Calcw+AmpVarw+Cancelw);
    Calcw  =int(Calcw*scale);
    AmpVarw=int(AmpVarw*scale);
    Cancelw=int(Cancelw*scale);
    // now set size and position
    OK->SetSize(totx,-1,Calcw,-1);
    AmpVar->SetSize(totx+Calcw+spacing,-1,AmpVarw,-1);
    Cancel->SetSize(totx+Calcw+spacing+AmpVarw+spacing,-1,Cancelw,-1);
  }
  // Center it
  dialog.Centre();
  // Show Dialog
  dialog.Show(TRUE);
  
  if ((OK->GetClientData())||(AmpVar->GetClientData()))
    {

      // Deselect all first
      for(i=0;i<Period->GetFrequencies();i++)
	{
	  if ((*Period)[i].GetActive())
	    {
	      (*Period)[i].UnsetSelection(Fre);
	      (*Period)[i].UnsetSelection(Amp);
	      (*Period)[i].UnsetSelection(Pha);
	    }
	}
      
      int num,freq;
      int *Sel;
      // Check for selections in mFreq
      // Get the selected members
      num=mFreq->GetSelections(&Sel);
      //
      for (i=0;i<num;i++)
	{
	  freq=(int)(mFreq->GetClientData(Sel[i]));
	  (*Period)[freq].SetSelection(Fre);
	}
      
      // Check for selections in mAmp
      // Get the selected members
      num=mAmp->GetSelections(&Sel);
      //
      for (i=0;i<num;i++)
	{
	  freq=(int)(mAmp->GetClientData(Sel[i]));
	  (*Period)[freq].SetSelection(Amp);
	}
      
      // Check for selections in mFreq
      // Get the selected members
      num=mPha->GetSelections(&Sel);
      //
      for (i=0;i<num;i++)
	{
	  freq=(int)(mPha->GetClientData(Sel[i]));
	  (*Period)[freq].SetSelection(Pha);
	}
      if (OK->GetClientData())
	{
	  // return with normal calculation
	  return 1;
	}
      else
	{
	  // return for Amplitude variation
	  return 2;
	}
    }
  // Somwhere there was Cancel pressed...
  return 0;
}

void OnAmpVarButton(wxButton& but, wxEvent&)
{
  but.SetClientData((char*)1);
  (but.GetParent())->Show(FALSE);
}

int *AmplitudeVariation(wxWindow* frame,int *freqs, int* what, CalcMode *mode)
{
  wxDialogBox dialog(frame,AMP_TITLE,TRUE,-1,-1,340,300);
  // Create entries
  // what column
  myString StrOptions[4];
  char *options[4];
  int i;
  for (i=0;i<4;i++)
    {
      StrOptions[i]=myProject.NameSet(i);
      options[i]=StrOptions[i].chars();
    }
  wxChoice *What= new wxChoice(&dialog,NULL,
			       AMP_USE,
			       -1,-1,-1,-1,
			       4,options);
  What->SetSelection(0);
  dialog.NewLine();
  // write message
  (void) new wxMessage(&dialog,
		       AMP_MSG_1"\n"
		       AMP_MSG_2);
  dialog.NewLine();
  // what calculations
  options[0]=AMP_AMPVAR;
  options[1]=AMP_PHAVAR;
  options[2]=AMP_ALLVAR;
  wxChoice *Mode= new wxChoice(&dialog,NULL,
			       AMP_MODE,
			       -1,-1,-1,-1,
			       3,options);
  Mode->SetSelection(0);
  dialog.NewLine();
  // Create the listbox
  wxListBox *mFreqList;
  mFreqList=  new wxListBox(&dialog,NULL,NULL,wxEXTENDED,
			    -1,-1,300,200,
			    0,NULL,wxEXTENDED);

  for (i=0;i<myProject.GetTotalFrequencies();i++)
    {
      if (myProject.GetActive(i))
	{
	  myString tmp=myProject.GetNumber(i)
	    +AMP_FREQ
	    +myProject.GetFrequency(i);
	  mFreqList->Append(tmp.chars(),(char*)i);
	}
    }
  dialog.NewLine();

  // and now the buttons
  wxButton *Calc= new wxButton(&dialog,
			       (wxFunction)&OnAmpVarButton,
			       CALCULATE);
  Calc->SetClientData(0);
  Calc->SetDefault();

  wxButton *Cancel= new wxButton(&dialog,
				 (wxFunction)&OnAmpVarButton,
				 CANCEL);
  Cancel->SetClientData(0);

  // now fit the dialog
  dialog.Fit();
  // now resize stuff
  {
    // general stuff
    int dw,dh;
    dialog.GetSize(&dw,&dh);
    int spacing=dialog.GetHorizontalSpacing();
    // listbox
    int lw,lh,lx,ly;
    mFreqList->GetSize(&lw,&lh);
    mFreqList->GetPosition(&lx,&ly);
    int width=dw-lx-spacing;
    mFreqList->SetSize(lx,ly,width,-1);
    // now Buttons
    int buttonw=(width-spacing)/2;
    Calc->SetSize(lx,-1,buttonw,-1);
    Cancel->SetSize(lx+buttonw+spacing,-1,buttonw,-1);
  }
  // Center it
  dialog.Centre();
  // and now show it...
  dialog.Show(TRUE);

  if (Calc->GetClientData())
    {
      int *Sel;
      // Check for selections in mFreq
      // Get the selected members
      *freqs=mFreqList->GetSelections(&Sel);
      // make a copy
      int* data= new int[*freqs];
      // select specific mode
      switch (Mode->GetSelection())
	{
	case 0:  *mode=AmpVar  ;break;
	case 1:  *mode=PhaseVar;break;
	case 2:  *mode=AllVar  ;break;
	default: *mode=NoVar   ;break;
	}
      // first entry ar the times
      *what=What->GetSelection();
      // now make a copy
      for (i=0;i<*freqs;i++)
	{
	  data[i]=int(mFreqList->GetClientData(Sel[i]));
	}
      // return array
      return data;
    }
  // No calculation
  return NULL;
}

void AmplitudeVariationDisplay(wxWindow * frame,char *txt)
{
  // create the dialog-box
  wxDialogBox dialog(frame,AMP_RESULT,TRUE,
		     -1,-1,520,290);
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
  // now the listbox
  int width=
    int(dialog.GetPanelDC()->GetCharWidth()*PROTOCOL_AMPVAR_WIDTH);
  wxListBox *list= new wxListBox(&dialog,NULL,NULL,wxSINGLE,-1,-1,500,250);

  // need to fill in the data from txt into the list
  char line[256];
  int i=0,j=0;
  for (j=0;j<256;j++) { line[j]=0; }
  j=0;
  while (txt[i])
    {
      if (txt[i]=='\n')
	{
	  line[j]=0;
	  list->Append(line);
	  for (j=0;j<256;j++) { line[j]=0; }
	  j=0;
	}
      else
	{
	  line[j]=txt[i];
	  j++;
	}
      i++;
    }
  if (line[0]!=0) { list->Append(line); }

  //list->Fit();
  FitListBox(list);

  dialog.NewLine();
  
  wxButton *tmp= new wxButton(&dialog,
		      (wxFunction)&OnAmpVarButton,
		      OKAY,
		      -1,-1,width-20,-1);
  tmp->SetClientData(NULL);
  // now fit the display
  dialog.Fit();
  // Center it
  dialog.Centre();
  // and show it
  dialog.Show(TRUE);
  // and return the result
}
      
void EpochDialog(wxWindow *Frame)
{
  // create the dialog-box
  wxDialogBox dialog(Frame,EPO_TITLE,TRUE,
		     -1,-1,520,290);
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
  // first the Dialog for times
  
  wxText *EpochTime=new wxText(&dialog,NULL,EPO_TIME,"0.00000000");
  dialog.NewLine();

  //wxCheckBox *EpochIntensity= new wxCheckBox(&dialog,NULL,EPO_INT);
  //EpochIntensity->SetValue(myProject.GetReverseScale());
  //dialog.NewLine();

  // now the listbox
  dialog.SetLabelPosition(wxVERTICAL);
  wxListBox *EpochList= new wxListBox(&dialog,
				      NULL,
				      PROTOCOL_EPOCH_FORMAT_HEADER,
				      wxSINGLE,
				      -1,
				      -1,
				      400,
				      250);
  dialog.SetLabelPosition(wxHORIZONTAL);
  dialog.NewLine();
  // now the Buttons
  wxButton *Calc= new wxButton(&dialog,
			       GENERAL_BUTTON_HANDLER,
			       CALCULATE);
  Calc->SetClientData(0);
  Calc->SetDefault();
  
  wxButton *Cancel= new wxButton(&dialog,
				 GENERAL_BUTTON_HANDLER,
				 CANCEL);
  Cancel->SetClientData(0);
  
  // now fit the dialog
  dialog.Fit();

  // now set quit
  int quit=FALSE;
  // repeat until the user presses cancel
  while (!quit)
    {
      // reset values at first
      Calc->SetClientData(0);
      Cancel->SetClientData(0);
      // fill listbox
      {
	// calculate the epochs
	myString txt=myProject.ShowEpoch(EpochTime->GetValue(),
					 myProject.GetReverseScale());
	// clear list
	EpochList->Clear();
	// fill in list
	{
	  char line[256];
	  int i=0,j=0;
	  for (j=0;j<256;j++) { line[j]=0; }
	  j=0;
	  while (txt[i])
	    {
	      if (txt[i]=='\n')
		{
		  line[j]=0;
		  EpochList->Append(line);
		  for (j=0;j<256;j++) { line[j]=0; }
		  j=0;
		}
	      else
		{
		  line[j]=txt[i];
		  j++;
		}
	      i++;
	    }
	  if (line[0]!=0) { EpochList->Append(line); }
	}
      }
      // then rescale stuff
      {
	// get Listbox size
	//EpochList->Fit();
	FitListBox(EpochList);
	dialog.Fit();
	int dw,dh;
	EpochList->GetSize(&dw,&dh);
	int Calcw,Calch,Calcx,Calcy;
	Calc->GetSize(&Calcw,&Calch);
	Calc->GetPosition(&Calcx,&Calcy);
	int Cancelx,Cancely;
	Cancel->GetPosition(&Cancelx,&Cancely);
	int spacing=Cancelx-(Calcx+Calcw);
	int width=(dw-spacing)/2;
	Calc->SetSize(Calcx,-1,width,-1);
	Cancel->SetSize(Calcx+spacing+width,-1,width,-1);
	dialog.Fit();
      }
      // and now show it...
      dialog.Centre();
      dialog.Show(TRUE);
      // check if calc has been pressed
      if (!Calc->GetClientData())
	{
	  // cancel has been pressed, so let's exit
	  quit=TRUE;
	}      
    }
}











