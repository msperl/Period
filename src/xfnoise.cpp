/*
 * Program:     Period98
 *
 * File:        xfnoise.cpp
 * Purpose:     implementation-file for
 *              ?
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */

#include <wx.h>
#include "xgprojec.h"
#include "ldialtxt.h"

#include "fgeneral.h"

#include "xgdial.h"

void NoiseButton(wxButton& button,wxEvent&)
{
  // close the parent frame
  button.GetParent()->Show(FALSE);
  // Set Pressed flag
  button.SetClientData((char*)1);
}

wxText * noisestep;
wxChoice * noisestepqual;

void UpdateNoiseStepping(CFourier::StepQuality stepq, double step)
{
  double steprate=myProject.GetStepRate(stepq,step);
  // select the right one
  switch (stepq)
    {
    case CFourier::Low :
      noisestepqual->SetSelection(2);
      noisestep->SetEditable(0);
      break;
    case CFourier::Medium :
      noisestepqual->SetSelection(1);
      noisestep->SetEditable(0);
      break;
    case CFourier::High :
      noisestepqual->SetSelection(0);
      noisestep->SetEditable(0);
      break;
    default:
      noisestepqual->SetSelection(3);
      noisestep->SetEditable(1);
      break;
    }
  // fill in the value
  char txt[32];
  sprintf(txt,FORMAT_FREQUENCY,steprate);
  noisestep->SetValue(txt);
}

void NoiseSteppingHandler(wxChoice &choice, wxCommandEvent&)
{
  int type=choice.GetSelection();
  double step=0;
  CFourier::StepQuality stepq=CFourier::High;
  switch (type)
    {
    case 0: stepq=CFourier::High; break;
    case 1: stepq=CFourier::Medium; break;
    case 2: stepq=CFourier::Low; break;
    case 3: stepq=CFourier::Custom; break;
    }
  sscanf((noisestep)->GetValue(),"%lg",&step);
  UpdateNoiseStepping(stepq,step);
}

void NoiseCalculate(wxWindow *frame)
{
  // Create Dialog-box
  myDialogBox dialog(frame,NOISE_CALCNOISE,TRUE,-1,-1,500,200);
  // Create entries
  wxText *point= new wxText(&dialog,NULL,
			    NOISE_AT,
			    "10.000000");
  point->SetValue(NOISE_AT_DEF);
  wxText *box= new wxText(&dialog,NULL,
			  NOISE_BOX,
			  "10.000000");
  box->SetValue(NOISE_BOX_DEF);
  dialog.NewLine();

  char *choices[4];
  choices[0]=FOU_HIGH;
  choices[1]=FOU_MEDIUM;
  choices[2]=FOU_LOW;
  choices[3]=FOU_CUSTOM;
  noisestepqual=new wxChoice(&dialog,(wxFunction)NoiseSteppingHandler,
			     FOU_STEP,-1,-1,-1,-1,4,choices);
  noisestep=   new wxText(&dialog,NULL,NULL,FOU_STEP_DEF,-1,-1,120);
  UpdateNoiseStepping(CFourier::High,0.001);

  wxCheckBox * Weight;
  Weight = new wxCheckBox(&dialog,NULL,USE_WEIGHTS);
  
  dialog.NewLine();
  
  char* UseData[5];
  UseData[0]=USE_ORIGDAT;
  UseData[1]=USE_SHIFDAT;
  UseData[2]=USE_SPECTRAL;
  UseData[3]=USE_ORIGRES;
  UseData[4]=USE_SHIFRES;
  dialog.SetLabelPosition(wxVERTICAL);
  wxRadioBox *Use= new wxRadioBox(&dialog,NULL,USE,
				  -1,-1,-1,-1,
				  5,UseData,3,wxHORIZONTAL);
  dialog.SetLabelPosition(wxHORIZONTAL);
  dialog.NewLine();

  // now come the Buttons...
  wxButton *Calculate = new wxButton(&dialog,
				     (wxFunction)&NoiseButton,
				     OKAY);
  Calculate->SetClientData(NULL);
  Calculate->SetDefault();
  wxButton * Quit= new wxButton(&dialog,
				(wxFunction)&NoiseButton,
				CANCEL);
  Quit->SetClientData(NULL);

  // now fit the Frame
  dialog.Fit();
  // fit the buttons
  {
    int w,h,x,y;
    dialog.GetSize(&w,&h);
    Calculate->GetPosition(&x,&y);
    int spacing=dialog.GetHorizontalSpacing();
    int width=(w-x-2*spacing)/2;
    Calculate->SetSize(x,y,width,-1);
    Quit->SetSize(x+width+spacing,y,width,-1);
  }

  // now center it
  dialog.Centre();
  int quit=0;
  while (!quit)
    {
      quit=1;
      dialog.Show(TRUE);
      // should we calculate ?
      if (Calculate->GetClientData()!=NULL)
	{
	  quit=0;
	  // Get values
	  double valpoint,valbox;
	  sscanf(point->GetValue(),"%lf",&valpoint);
	  sscanf(box->GetValue(),"%lf",&valbox);
	  if (valbox<0) { valbox=-valbox; }
	  // work on steps...
	  CFourier::StepQuality stepq=CFourier::High;
	  switch (noisestepqual->GetSelection())
	    {
	    case 0: stepq=CFourier::High; break;
	    case 1: stepq=CFourier::Medium; break;
	    case 2: stepq=CFourier::Low; break;
	    case 3: stepq=CFourier::Custom; break;
	    }
	  double valstep;
	  sscanf(noisestep->GetValue(),"%lf",&valstep);
	  // Calculate weighted data?
	  int valweight=Weight->GetValue();
	  DataMode valselect=(DataMode) Use->GetSelection();
	  // now do Calculation
	  double noise=myProject.CalculateNoise(valpoint,valbox,
						stepq,valstep,
						valselect,valweight);
	  // cancel has not been pressed
	  if (noise!=-1)
	    {
		  char tmp[256],*tmpptr=tmp;
		  sprintf(tmp,NOISE_NOISE,valpoint,noise);
		  ::wxGetSingleChoice(NOISE_MESSAGE,NOISE_CAPTION,1,&tmpptr);
		  Calculate->SetClientData(NULL);
	    }
	}
    }
}

void NoiseSpectraCalculate(wxWindow *frame)
{
  // Create Dialog-box
  wxDialogBox dialog(frame,NOISE_CALCNOISE,TRUE,-1,-1,550,400);
  // Create entries
  wxText *from= new wxText(&dialog,NULL,
			    NOISE_FROM,
			    "10.0000000");
  from->SetValue(NOISE_FROM_DEF);
  wxText *to= new wxText(&dialog,NULL,
			 NOISE_TO,
			    "10.0000000");
  to->SetValue(NOISE_TO_DEF);

  dialog.NewLine();

  wxText *Spacing = new wxText(&dialog,NULL,
                               NOISE_SPACE,
                               "10.0000000");
  Spacing->SetValue(NOISE_SPACE_DEF);

  wxText *box= new wxText(&dialog,NULL,
			  NOISE_BOX,
			  "10.0000000");
  box->SetValue(NOISE_BOX_DEF);

  dialog.NewLine();
  
  char *choices[4];
  choices[0]=FOU_HIGH;
  choices[1]=FOU_MEDIUM;
  choices[2]=FOU_LOW;
  choices[3]=FOU_CUSTOM;
  noisestepqual=new wxChoice(&dialog,(wxFunction)NoiseSteppingHandler,
			     FOU_STEP,-1,-1,-1,-1,4,choices);
  noisestep=   new wxText(&dialog,NULL,NULL,FOU_STEP_DEF,-1,-1,120);
  UpdateNoiseStepping(CFourier::High,0.001);

  wxCheckBox * Weight;
  Weight = new wxCheckBox(&dialog,NULL,USE_WEIGHTS);

  dialog.NewLine();
  
  char* UseData[5];
  UseData[0]=USE_ORIGDAT;
  UseData[1]=USE_SHIFDAT;
  UseData[2]=USE_SPECTRAL;
  UseData[3]=USE_ORIGRES;
  UseData[4]=USE_SHIFRES;
  dialog.SetLabelPosition(wxVERTICAL);
  wxRadioBox *Use= new wxRadioBox(&dialog,NULL,USE,
				  -1,-1,-1,-1,
				  5,UseData,3,wxHORIZONTAL);
  dialog.SetLabelPosition(wxHORIZONTAL);
  dialog.NewLine();
  
  // now come the Buttons...
  wxButton *Calculate = new wxButton(&dialog,
				     (wxFunction)&NoiseButton,
				     OKAY);
  Calculate->SetClientData(NULL);
  Calculate->SetDefault();
  wxButton * Quit= new wxButton(&dialog,
				(wxFunction)&NoiseButton,
				CANCEL);
  Quit->SetClientData(NULL);

  dialog.NewLine();

  // now fit the frame
  dialog.Fit();

  // fit the buttons
  {
    int w,h,x,y;
    dialog.GetSize(&w,&h);
    Calculate->GetPosition(&x,&y);
    int spacing=dialog.GetHorizontalSpacing();
    int width=(w-x-2*spacing)/2;
    Calculate->SetSize(x,y,width,-1);
    Quit->SetSize(x+width+spacing,y,width,-1);
  }
  // center Frame
  dialog.Centre();
  // ok final loop
  int quit=0;
  while (!quit)
    {
      quit=1;
      dialog.Show(TRUE);
      // should we calculate ?
      if (Calculate->GetClientData()!=NULL)
	{
	  //result->Show(TRUE);
	  quit=0;
	  // Get values
	  double valfrom,valto,valspacing,valbox;
	  sscanf(from->GetValue(),"%lf",&valfrom);
	  sscanf(to->GetValue(),"%lf",&valto);
	  sscanf(Spacing->GetValue(),"%lf",&valspacing);
	  sscanf(box->GetValue(),"%lf",&valbox);
	  if (valbox<0) { valbox=-valbox; }

	  CFourier::StepQuality stepq=CFourier::High;
	  switch (noisestepqual->GetSelection())
	    {
	    case 0: stepq=CFourier::High; break;
	    case 1: stepq=CFourier::Medium; break;
	    case 2: stepq=CFourier::Low; break;
	    case 3: stepq=CFourier::Custom; break;
	    }
	  int valstep=1;
	  sscanf(noisestep->GetValue(),"%i",&valstep);

	  int valweight=Weight->GetValue();
	  DataMode valselect=(DataMode) Use->GetSelection();
	  // now do Calculation
	  // these are the results:
	  double noise[1024],freq[1024];
	  int points=myProject.CalculateNoiseSpectra
	    (valfrom, valto, valspacing,
	     valbox,
	     stepq,valstep,
	     valselect,valweight,
	     freq,noise);
	  // calculation was not interrupted?
	  if (points!=-1)
	    {
		  int i;
		  typedef char * charptr;
		  char **array=new charptr[points];
	      // now write out result
	      for (i=0;i<points;i++)
		{

		  char *tmp=new char[256];
		  sprintf(tmp,NOISE_NOISE,freq[i],noise[i]);
		  array[i]=tmp;
		}
		  ::wxGetSingleChoice(NOISE_MESSAGE,NOISE_CAPTION,points,array);
		  for (i=0;i<points;i++)
		  {
			  delete [] array[i];
		  }
		  delete [] array;
		  // Now, that we have done calculation
	      // reset Flag
	      Calculate->SetClientData(NULL);
	    }
	}
    }
}







