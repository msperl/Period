/*
 * Program:     Period98
 *
 * File:        xfdialog.cpp
 * Purpose:     implementation-file for
 *              ?
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */

#include "xgsize.h"
#include "xfdialog.h"
#include "xgprojec.h"

#include "xppredic.h"

#include "ldialtxt.h"
#include "fgeneral.h"

f_File::f_File(CProject *project,wxWindow* parent,int x0, int y0, int w0, int h0)
  :myFile(project,DISPLAY_DIALOG_FOURIER,parent,x0,y0,w0,h0,1)
{
  // First line
  Title=      new wxText(this,NULL,FOU_TITLE,FOU_TITLE_DEF,-1,-1,400);
  NewLine();

  // Second line
  From=       new wxText(this,NULL,FOU_FROM,FOU_FROM_DEF,-1,-1,100);
  To=         new wxText(this,NULL,FOU_TO,FOU_TO_DEF,-1,-1,100);
  Weight =    new wxCheckBox(this,NULL,USE_WEIGHTS);

  NewLine();
  // Third line
  Nyquist=    new wxMessage(this,FOU_NYQUIST);
  char *choices[4];
  choices[0]=FOU_HIGH;
  choices[1]=FOU_MEDIUM;
  choices[2]=FOU_LOW;
  choices[3]=FOU_CUSTOM;
  StepQuality=new wxChoice(this,(wxFunction)FourierSteppingHandler,
			   FOU_STEP,-1,-1,-1,-1,4,choices);
  Stepping=   new wxText(this,NULL,NULL,FOU_STEP_DEF,-1,-1,120);
  NewLine();

  // 4th line
  (void) new wxMessage(this,FOU_HIGHEST);
  (void) new wxMessage(this,FOU_HIGH_F);
  PeakFreq=   new wxMessage(this,"0.0000000     ");
  (void) new wxMessage(this,FOU_HIGH_A);
  PeakAmp=    new wxMessage(this,"0.0000000     ");
  NewLine();

  // fifth line - Radio Buttons
  // here go the definitions...
  char* UseData[5];
  UseData[0]=USE_ORIGDAT;
  UseData[1]=USE_SHIFDAT;
  UseData[2]=USE_SPECTRAL;
  UseData[3]=USE_ORIGRES;
  UseData[4]=USE_SHIFRES;
  SetLabelPosition(wxVERTICAL);
  Use=        new wxRadioBox(this,NULL,USE,
			     -1,-1,-1,-1,
			     5,UseData,3,wxHORIZONTAL);
  SetLabelPosition(wxHORIZONTAL);
  NewLine();

  // sixth Line
  char *CompactData[2];
  CompactData[0]=FOU_ALL;
  CompactData[1]=FOU_PEAK;
  Compact = new wxRadioBox(this,NULL,FOU_COMPACT,
			   -1,-1,-1,-1,
			   2,CompactData,3,wxHORIZONTAL);
  NewLine();
  
  // seventh Line - Now The Calculate Button
  CalcBtn=    new wxButton(this,DefaultButtonHandler(),
			   CALCULATE,-1,-1,-1,-1);
  CalcBtn->SetClientData((char*)BTN_CALC);
  NewLine();

  // eights line - here should come the settings
  SetLabelPosition(wxVERTICAL);
  List= new wxListBox(this,(wxFunction)FourierListHandler,
		      FOU_PREV,wxSINGLE,-1,-1,
		      Fourier_ListWidth,Fourier_ListHeight);
  
  // and now the last line 
  NewLine();
  DeleteBtn=  new wxButton(this,DefaultButtonHandler(),
			   FOU_DELETE,-1,-1,-1,-1);
  DeleteBtn->SetClientData((char*)BTN_DELETE);
  
  RenameBtn=  new wxButton(this,DefaultButtonHandler(),
			   FOU_RENAME,-1,-1,-1,-1);
  RenameBtn->SetClientData((char*)BTN_RENAME);
  NewLine();
  // create basic- buttons
  myFile::CreateButtons();
  Load->Show(0);
  // and update all displays...
  UpdateDisplay();
}

f_File::~f_File()
{
  // nothing to destruct yet !!!
}

void f_File::SetSize(int x, int y, int w, int h, int flags)
{
  // Set general size at first
  myFile::SetSize(x,y,w,h,flags);
  // and now the Buttons
  w=w-20;
  CalcBtn->SetSize(10,-1,w,-1);
  int bw,bh;
  CalcBtn->GetSize(&bw,&bh);
  bw=(w-10)/2;
  y=h-2*(bh+5);
  DeleteBtn->SetSize(10   ,y,bw,-1);
  RenameBtn->SetSize(20+bw,y,bw,-1);
  // now adapt List-box
  int lx,ly;
  List->GetPosition(&lx,&ly);
  int lh=y-5-ly;
  List->SetSize(10,ly,w,lh);
  // now Title
  Title->SetSize(10,-1,w,-1);
}

void f_File::OnUknButton(int i)
{
  switch (i)
    {
    case BTN_CALC:
      OnCalc();
      break;
    case BTN_DELETE:
      OnDelete();
      break;
    case BTN_RENAME:
      OnRename();
      break;
    default:
      myFile::OnUknButton(i);
      break;
    }
}

void f_File::OnSave()
{
  // Is there anything to save?
  if (List->Number()==0)
    {
      // Inform the user
      ::wxMessageBox(FOU_NOSAVE,
		     WARNING);
      // now return early..
      return;
    }
  int i=List->GetSelection();
  myProject.SaveFourier(i);
}

void f_File::OnLoad()
{
  // We cannot do anything...
}

void f_File::OnDelete()
{
  myProject.DeleteActiveFourier();
  UpdateDisplay();
}

void f_File::OnRename()
{
  char *name=wxGetTextFromUser(FOU_RELABEL,NULL,
			       Title->GetValue());
  if (name)
    {
      myProject.RenameActiveFourier(name);
      UpdateDisplay();
    }
}

void f_File::OnCalc()
{
  myString title=Title->GetValue();
  double from=0.0,to=50.0;
  sscanf(From->GetValue(),"%lf",&from);
  sscanf(To->GetValue(),"%lf",&to);
  double step=0;
  CFourier::StepQuality stepq=CFourier::High;
  switch(StepQuality->GetSelection())
    {
    case 0:  stepq=CFourier::High; break;
    case 1:  stepq=CFourier::Medium; break;
    case 2:  stepq=CFourier::Low; break;
    default:
             stepq=CFourier::Custom; 
	     sscanf(Stepping->GetValue(),"%lf",&step);
	     break;
    }
  DataMode dm;
  int weight;
  switch (Use->GetSelection())
    {
    case 0: dm=Observed; break;
    case 1: dm=Adjusted; break;
    case 2: dm=SpectralWindow; break;
    case 3: dm=DataResiduals; break;
    case 4: dm=AdjustedResiduals; break;
    default:
      return;
    }
  CompactMode cm;
  switch (Compact->GetSelection())
    {
    case 0: cm=AllData; break;
    case 1: cm=PeaksOnly; break;
    default:
      return;
    }
  // Check for weight
  weight=Weight->GetValue();
  myProject.CalculateFourier(title,from,to,stepq,step,dm,cm,weight);
  UpdateDisplay();
}

Bool f_File::Show(Bool i)
{
  if (i)
    { UpdateDisplay(); }
  return myFile::Show(i);
}

void f_File::UpdateData()
{
}

void f_File::UpdateDisplay(int i)
{
  const CFourier *tmp=GetDisplayData();
  char txt[256];
  if (tmp==NULL)
    return;
  Title->SetValue(tmp->GetTitle().chars());
  sprintf(txt,FORMAT_FREQUENCY,tmp->GetFrom());
  From->SetValue(txt);
  sprintf(txt,FORMAT_FREQUENCY,tmp->GetTo());
  To->SetValue(txt);
  // fill in nyquist
  sprintf(txt,FOU_NYQUIST_P,myProject.GetNyquist());
  Nyquist->SetLabel(txt);
  // fill in Steping/steping/quality
  UpdateStepping(tmp->GetStepQuality(),tmp->GetStepping());
  // set the used modes
  Use->SetSelection(tmp->GetMode());
  Compact->SetSelection(tmp->GetCompact());

  // fill in the peak found
  CPeriPoint peak=tmp->GetPeak();
  sprintf(txt,FORMAT_FREQUENCY,peak.GetFrequency());
  PeakFreq->SetLabel(txt);
  sprintf(txt,FORMAT_AMPLITUDE,peak.GetAmplitude(-1));
  PeakAmp->SetLabel(txt);
  // fill in the list
  if (i==0)
    {
      UpdateList();
    }
  // fill in the weight
  Weight->SetValue(tmp->GetUseWeight());
}

void f_File::UpdateStepping(CFourier::StepQuality stepq, double step)
{
  double steprate=myProject.GetStepRate(stepq,step);
  // select the right one
  switch (stepq)
    {
    case CFourier::Low :
      StepQuality->SetSelection(2);
      Stepping->SetEditable(0);
      break;
    case CFourier::Medium :
      StepQuality->SetSelection(1);
      Stepping->SetEditable(0);
      break;
    case CFourier::High :
      StepQuality->SetSelection(0);
      Stepping->SetEditable(0);
      break;
    default:
      StepQuality->SetSelection(3);
      Stepping->SetEditable(1);
      break;
    }
  // fill in the value
  char txt[32];
  sprintf(txt,FORMAT_FREQUENCY,steprate);
  Stepping->SetValue(txt);
}

void f_File::UpdateList()
{
  List->Clear();
  for (int i=0;i<myProject.GetFourierEntries();i++)
    {
      List->Append(myProject.GetFourierTitle(i).chars(),(char*)i);
    }
  List->SetStringSelection(myProject.GetFourierActive()->GetTitle().chars());
}

const CFourier *f_File::GetDisplayData() const
{
  return myProject.GetFourierActive();
}

myString f_File::GetName(int i) const
{
  return myProject.GetFourierTitle(i);
}

int f_File::GetNames() const
{
  return myProject.GetFourierEntries();
}

void FourierListHandler(wxListBox &list,wxCommandEvent&)
{
  int selected=list.GetSelection();
  f_File * tmp=(f_File*)(list.GetParent());
  selected=(int)(list.GetClientData(selected));
  myProject.ActivateFourier(selected);
  tmp->UpdateDisplay(1);
}

void FourierSteppingHandler(wxChoice &choice, wxCommandEvent&)
{
  int type=choice.GetSelection();
  CFourier::StepQuality stepq=CFourier::High;
  f_File *tmp=(f_File*)choice.GetParent();
  double step=0;
  sscanf((tmp->Stepping)->GetValue(),"%lf",&step);
  switch (type)
    {
    case 0: stepq=CFourier::High; break;
    case 1: stepq=CFourier::Medium; break;
    case 2: stepq=CFourier::Low; break;
    case 3: stepq=CFourier::Custom; break;
    }
  tmp->UpdateStepping(stepq,step);
}

void f_File::OnDispGraph()
{
  myProject.DisplayFourierGraph();
}

void f_File::OnDispData()
{
  myProject.DisplayFourierTable();
}













