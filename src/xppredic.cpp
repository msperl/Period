/*
 * Program:     Period98
 *
 * File:        xppredic.cpp
 * Purpose:     implementation-file for
 *              ?
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */
#include <wx.h>
#include "xgdial.h"

#include "xgprojec.h"
#include "xppredic.h"
#include "ldialtxt.h"
#include "fgeneral.h"

wxText *TimeToPredict;
wxMessage *PredictResult;

void PredictCalculate(wxButton&,wxEvent &)
{
  myString time=TimeToPredict->GetValue();
  myString Amplitude=myProject.Predict(time);
  PredictResult->SetLabel(Amplitude.chars());
}

void PredictQUIT(wxButton& calc,wxEvent &)
{
  calc.GetParent()->Show(FALSE);
}

void PredictAmplitude(wxWindow *Frame)
{
  myDialogBox dialog(Frame,PREDICT_DATE,TRUE,-1,-1,300,130);
  char tmp[256];
  double start=0;
  if (myProject.GetSelectedPoints()!=0)
    {
      start=(myProject.GetTimeString())[0].GetTime();
    }
  sprintf(tmp,PREDICT_START,start);
  (void) new wxMessage (&dialog,tmp);
  dialog.NewLine();
  TimeToPredict = new wxText (&dialog,NULL,
			      PREDICT_TIME,
			      "0.00000000");
  dialog.NewLine();
  (void) new wxMessage (&dialog,
			PREDICT_AMPL);
  PredictResult = new wxMessage (&dialog,
				 PREDICT_NOTAV);
  dialog.NewLine();
  wxButton *Calc= new wxButton(&dialog,
			       (wxFunction)&PredictCalculate,
			       PREDICT_CALC);
  Calc->SetDefault();
  wxButton *Quit=  new wxButton(&dialog,
				(wxFunction)&PredictQUIT,
				CANCEL);
  // now fit the frame
  dialog.Fit();
  // now resize Butons
  {
    int w,h,x,y;
    dialog.GetSize(&w,&h);
    Calc->GetPosition(&x,&y);
    int spacing=dialog.GetHorizontalSpacing();
    int butw=(w-x-2*spacing)/2;
    Calc->SetSize(x,-1,butw,-1);
    Quit->SetSize(x+butw+spacing,-1,butw,-1);
  }
  // now ceter Frame
  dialog.Centre();
  // and show it finally
  dialog.Show(TRUE);
}

void CreateArtificialData(wxWindow *Frame)
{
  myDialogBox dialog(Frame,PREDICT_ART,TRUE,-1,-1,300,220);

  // Create Dialogs
  (void) new wxMessage(&dialog,PREDICT_BEG);
  wxText *StartTime = new wxText (&dialog,NULL,
				  NULL,
				  "0.00000000");
  dialog.NewLine();
  (void) new wxMessage(&dialog,PREDICT_END);
  wxText *EndTime = new wxText (&dialog,NULL,
				NULL,
				"0.00000000");
  dialog.NewLine();
  (void) new wxMessage(&dialog,PREDICT_STEP);
  wxText *TimeStep = new wxText (&dialog,NULL,
				 NULL,
				 "0.00000000");
  dialog.NewLine();
  (void) new wxMessage(&dialog,PREDICT_LEAD);
  wxText *Leading = new wxText (&dialog,NULL,
			      NULL,
			      "0.00000000");
  dialog.NewLine();

  // Create Buttons
  
  wxButton *create,*append,*cancel;
  append= new wxButton(&dialog,
		       (wxFunction)&CreateArtificialDataOK,
		       PREDICT_APP);
  append->SetClientData(0);
  append->SetDefault();

  create= new wxButton(&dialog,
		       (wxFunction)&CreateArtificialDataOK,
		       PREDICT_NEW);
  create->SetClientData(0);

  cancel= new wxButton(&dialog,
		       (wxFunction)&CreateArtificialDataCancel,
		       PREDICT_CANC);
  cancel->SetClientData(0);

  // Now set values
  if (myProject.GetSelectedPoints()!=0)
  {
    StartTime->SetValue(myProject.GetStartTime().chars());
    EndTime->SetValue(myProject.GetEndTime().chars());
  }

  // now find out about the highest Frequency:
  CPeriod *Period=&(myProject.GetPeriod());

  if (Period->GetFrequencies()==0)
    {
	  myProject.InformUser(PREDICT_NOFRE);
	  return;
    }
  double highest=0;
  for (int i=0;i<Period->GetFrequencies();i++)
    {
      if ((*Period)[i].GetActive())
        {
	  double freq=(*Period)[i].GetFrequency();
	  if (freq>highest)
	    {
	      highest=freq;
	    }
	}
    }

  // Calculate Stepping
  if (highest==0)
    {
	  highest=1;
    }
  else
  {
    highest=(1./20.)/highest;
  }
  char tmp[32];
  sprintf(tmp,FORMAT_FREQUENCY,highest);

  TimeStep->SetValue(tmp);

  // now fit the frame
  dialog.Fit();
  // adjust Buttons and TextInputs
  {
    // get relevant sizes
    int Framew,Frameh;
    dialog.GetSize(&Framew,&Frameh);
    int Appendx,Appendy,Appendw,Appendh;
    append->GetPosition(&Appendx,&Appendy);
    append->GetSize(&Appendw,&Appendh);
    int Createx,Createy,Createw,Createh;
    create->GetPosition(&Createx,&Createy);
    create->GetSize(&Createw,&Createh);
    // calculate widths
    int spacing=Createx-(Appendx+Appendw);
    int width=(Framew-Appendx-2*spacing)/2;
    // now find out about the size of all of these
    int xmax=0,wmax=0;
    int x,y,w,h;
    StartTime->GetPosition(&xmax,&y);
    StartTime->GetSize(&wmax,&h);
    EndTime->GetPosition(&x,&y);
    EndTime->GetSize(&w,&h);	
    if (w>wmax) { wmax=w; }
    if (x>xmax) { xmax=x; }
    TimeStep->GetPosition(&x,&y);
    TimeStep->GetSize(&w,&h);	
    if (w>wmax) { wmax=w; }
    if (x>xmax) { xmax=x; }
    Leading->GetPosition(&x,&y);
    Leading->GetSize(&w,&h);	
    if (w>wmax) { wmax=w; }
    if (x>xmax) { xmax=x; }
    // now check size of Buttons
    if (width<Appendw) 
      { 
	width=Appendw;
      }
    if (width<Createw) 
      { 
	width=Createw;
      }
    wmax =Appendx+2*(width+spacing)+width-xmax;
    StartTime->SetSize(xmax,-1,wmax,-1);
    EndTime->SetSize(xmax,-1,wmax,-1);
    TimeStep->SetSize(xmax,-1,wmax,-1);
    Leading->SetSize(xmax,-1,wmax,-1);
    // now resize Buttons
    append->SetSize(Appendx,-1,width,-1);
    create->SetSize(Appendx+width+spacing,-1,width,-1);
    cancel->SetSize(Appendx+2*(width+spacing),-1,width,-1);
  }      
  // Refit dialog
  dialog.Fit();
  // Center it
  dialog.Centre();
  // Now Show data
  dialog.Show(TRUE);

  // Are we Creating?
  if (create->GetClientData()!=0)
    {
      char *name=wxFileSelector(PREDICT_SAVE,
				NULL,NULL,PREDICT_EXT,PREDICT_PAT,
				wxSAVE|wxOVERWRITE_PROMPT);
      // Lets write it out...
      if (name)
	{
	  myProject.
	    CreateArtificialData(name,
				 StartTime->GetValue(),
				 EndTime->GetValue(),
				 TimeStep->GetValue(),
				 Leading->GetValue(),
				 FALSE
				 );
	}
    }
  // Are we appending?
  if (append->GetClientData()!=0)
    {
      char *name=wxFileSelector(PREDICT_SAVAP,
				NULL,NULL,PREDICT_EXT,PREDICT_PAT,
				wxSAVE);
      // Lets append it...
      if (name)
	{
	  myProject.
	    CreateArtificialData(name,
				 StartTime->GetValue(),
				 EndTime->GetValue(),
				 TimeStep->GetValue(),
				 Leading->GetValue(),
				 TRUE
				 );
	}
    }
}


void CreateArtificialDataOK(wxButton& calc,wxEvent &)
{
  calc.SetClientData((char*)1);
  calc.GetParent()->Show(FALSE);
}

void CreateArtificialDataCancel(wxButton& calc,wxEvent &)
{
  calc.GetParent()->Show(FALSE);
}




