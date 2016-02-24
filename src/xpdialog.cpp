/*
 * Program:     Period98
 *
 * File:        xpdialog.cpp
 * Purpose:     implementation-file for
 *              ?
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */
#include "xgsize.h"
#include "xpdialog.h"
#include "ldialtxt.h"

#include "xmain.h"

p_File::p_File(CProject *project,wxWindow* parent,int x, int y, int w, int h)
  :myFile(project,DISPLAY_DIALOG_PERIOD,parent,x,y,w,h,1)
{
  // first line:
  (void) new wxMessage(this,PER_ACTIVE);
  Active= new wxMessage(this,"00000");
  //(void) new wxMessage(this,PER_LOOP); 
  //Loops= new wxMessage(this,"000",-1,-1,70);
  Weight = new wxCheckBox(this,NULL,USE_WEIGHTS);
  NewLine();

  // second line
  (void) new wxMessage(this,PER_ZERO); 
  Zero= new wxMessage(this,"0.0000000000     ",-1,-1,130);
  (void) new wxMessage(this,PER_RESIDUALS); 
  Residuals= new wxMessage(this,"0.000000000",-1,-1,130);
  NewLine();

  // third line
  char * BasedData[2];
  BasedData[0]=USE_ORIGDAT;
  BasedData[1]=USE_SHIFDAT;
  Based= new wxRadioBox(this,NULL,USE,
			-1,-1,-1,-1,
			2,BasedData,2,wxHORIZONTAL);
  NewLine();
  // now estimate the number of lines for different screensizes
  int lines=0;
  {
   // find start of first line
 	int xactive,yactive;
    Active->GetPosition(&xactive,&yactive);
   // find start of second line
	int xzero,yzero;
    Zero->GetPosition(&xzero,&yzero);
	int yspace=yzero-yactive;
    // get the Display size of the screen
    int xsize,ysize;
    ::wxDisplaySize(&xsize,&ysize);
    ysize=ysize-(
	  +150 // menu and handler
      +50       // footer
      +8*yspace  // 8 other lines
      );
    lines=(int)(ysize/yspace);
    // set a lower limit
    if (lines<4)
      { lines=4; }
  }
  // check for the number of lines wished by the user
  if (myApp.FrequencyRows!=-1) 
    {
      if (myApp.FrequencyRows<lines)
	{
	  lines=myApp.FrequencyRows;
	}
    }
    
  // Now, here should go the-frequency-list-box
  //Freqs= new DispFreqs(this,Period_FreqShown);
  Freqs= new DispFreqs(this,lines);

  // here goes the Calculation-Buttons...
  CalcFixed=new wxButton(this,DefaultButtonHandler(),
			 CALCULATE);
  CalcFixed->SetClientData((char*)BTN_CALC_FIXED);

  CalcAll=new wxButton(this,DefaultButtonHandler(),
		       PER_IMPROVE);
  CalcAll->SetClientData((char*)BTN_CALC_ALL);

  CalcSelect=new wxButton(this,DefaultButtonHandler(),
			  PER_SPECIAL);
  CalcSelect->SetClientData((char*)BTN_CALC_SELECT);

  // and now the final button-line
  NewLine();
  myFile::CreateButtons();

  // And finally update display
  UpdateDisplay();
  // and deactivate Buttons
  DispData->Show(FALSE);
  //DispGraph->Show(FALSE);
  DispGraph->SetLabel(PER_PHASEDIA);
}

p_File::~p_File()
{
  // nothing to destruct yet !!!
}

void p_File::SetSize(int x, int y, int w, int h, int flags)
{
  // Set general size at first
  myFile::SetSize(x,y,w,h,flags);
  // and now the Buttons
  w=w-20;
  int bw,bh;
  CalcFixed->GetSize(&bw,&bh);
  y=h-2*(bh+5);
  bw=(w-20)/3;
  CalcFixed ->SetSize(10     ,y,bw,-1);
  CalcAll   ->SetSize(20+bw  ,y,bw,-1);
  CalcSelect->SetSize(30+2*bw,y,bw,-1);
  // now set the next/prev buttons...
  Freqs->ResizeButtons(10,w);
}

void p_File::UpdateDisplay(int)
{
  // active Frequencies
  Active->SetLabel(myProject.GetActiveFrequenciesString().chars());
  
  // Zeropoint
  Zero->SetLabel(myProject.GetZeropoint().chars());
  
  // Residuals
  Residuals->SetLabel(myProject.GetResiduals().chars());

  // Set Weight
  Weight->SetValue(myProject.GetPeriodUseWeight());
  
  // Set Data:
  Freqs->SetData();
}

Bool p_File::Show(Bool show)
{
  if (show)
    {
      // if we get shown update the display
      UpdateDisplay();
    }
  else
    {
      // if we get hidden store the settings
      UpdateData();
    }
  // call the actual functions
  return myFile::Show(show);
}

void p_File::OnSave()
{
  myProject.SavePeriod();
}

void p_File::OnLoad()
{
  myProject.LoadPeriod();
}

void p_File::OnDispGraph()
{
  myProject.DisplayPeriodGraph();
}

void p_File::OnUknButton(int button)
{
  DataMode mode;
  if (Based->GetSelection()==0)
    {mode=Observed;}
  else
    {mode=Adjusted;}
  switch (button)
    {
    case BTN_CALC_FIXED:
      myProject.SetPeriodUseWeight(Weight->GetValue());
      myProject.CalculatePeriod(mode);
      break;
    case BTN_CALC_ALL:
      myProject.SetPeriodUseWeight(Weight->GetValue());
      myProject.ImprovePeriod(mode);
      break;
    case BTN_CALC_SELECT:
      myProject.SetPeriodUseWeight(Weight->GetValue());
      myProject.ImproveSpecialPeriod(mode);
      break;
    default:
      myFile::OnUknButton(button);
    }
}









