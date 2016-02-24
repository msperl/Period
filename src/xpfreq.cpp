/*
 * Program:     Period98
 *
 * File:        xpfreq.cpp
 * Purpose:     implementation-file for
 *              ?
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */

#include "xpfreq.h"
#include "ldialtxt.h"

// general data

DispFreqLine::DispFreqLine()
  :mActive(0),
   mFrequency(0),mAmplitude(0),mPhase(0),
   mLine(-1)
{;}

DispFreqLine::~DispFreqLine()
{;}

#define TEXT_CALLBACK (wxFunction)&FrequencyEntriesChanged

void DispFreqLine::CreateDialogs(wxPanel* parent)
{
  mActive=    new wxCheckBox(parent,(wxFunction)NULL,"F000");
  mFrequency= new wxText(parent,TEXT_CALLBACK,"","0.000000000000");
  mAmplitude= new wxText(parent,TEXT_CALLBACK,"","0.000000000000");
  mPhase=     new wxText(parent,TEXT_CALLBACK,"","0.000000000000");
  mComposit=  new wxMessage(parent,    "(0.000000000000)");
  parent->NewLine();
}

void DispFreqLine::SetData(int i,
			   int active,
			   myString num,
			   myString fre, 
			   myString amp,
			   myString pha,
			   myString com)
{
  mLine=i;
  // number
  mActive->SetLabel(num.chars());
  mActive->SetValue(active);
  // Composit and Frequency
  if (com!="")
    {
      mFrequency->SetValue(com.chars());
      mComposit->SetLabel(fre.chars());
      mComposit->Show(TRUE);
    }
  else
    {
      mFrequency->SetValue(fre.chars());
      mComposit->Show(FALSE);
    }
  mFrequency->SetClientData(0);
  // Amplitude
  mAmplitude->SetValue(amp.chars());
  mAmplitude->SetClientData(0);
  // Phase
  mPhase->SetValue(pha.chars());
  mPhase->SetClientData(0);
}

void FrequencyEntriesChanged(wxText& text,wxCommandEvent& event)
{
  // if something has been edited, then set a flag
  if (event.eventType==wxEVENT_TYPE_TEXT_COMMAND)
    {
      text.SetClientData((char*)1);
    }
}

void DispFreqLine::GetData(int &i,int *active,
			   myString *fre,
			   myString *amp,
			   myString *pha)
{
  i=mLine;
  *active=mActive->GetValue();
  // only set if it has changed
  if (mFrequency->GetClientData())
    {
      *fre=mFrequency->GetValue();
    }
  // only set if it has changed
  if (mAmplitude->GetClientData())
    {
      *amp=mAmplitude->GetValue();
    }
  // only set if it has changed
  if (mPhase->GetClientData())
    {
      *pha=mPhase->GetValue();
    }
}

///////////////////////////////////////////////////////

DispFreqs::DispFreqs(wxPanel *parent, int Lines)
{
  char prev[20],next[20];
  sprintf(prev,PER_PREVX,Lines);
  sprintf(next,PER_NEXTX,Lines);

  btmp1= new wxButton(parent,(wxFunction)&DispFreq_PrevNum,prev);
  btmp1->SetClientData((char*)this);
  btmp2= new wxButton(parent,(wxFunction)&DispFreq_Prev,PER_PREV);
  btmp2->SetClientData((char*)this);
  btmp3= new wxButton(parent,(wxFunction)&DispFreq_Next,PER_NEXT);
  btmp3->SetClientData((char*)this);
  btmp4= new wxButton(parent,(wxFunction)&DispFreq_NextNum,next);
  btmp4->SetClientData((char*)this);
  parent->NewLine();

  // create labels
  wxMessage *tmp1,*tmp2,*tmp3,*tmp4;
  tmp1= new wxMessage(parent,PER_USE);
  tmp2= new wxMessage(parent,PER_FRE);
  tmp3= new wxMessage(parent,PER_AMP);
  tmp4= new wxMessage(parent,PER_PHA);
  
  parent->NewLine();
  
  // create display-lines
  mLines=Lines;
  mTop=0;

  mLinesStruct= new DispFreqLine[Lines];
  for (int i=0;i<Lines;i++)
    {
      mLinesStruct[i].CreateDialogs(parent);
    }
  // now position label
  int xt,yt;
  mLinesStruct[0].mActive->GetPosition(&xt,&yt);
  tmp1->Move(xt,-1);
  mLinesStruct[0].mFrequency->GetPosition(&xt,&yt);
  tmp2->Move(xt,-1);
  mLinesStruct[0].mAmplitude->GetPosition(&xt,&yt);
  tmp3->Move(xt,-1);
  mLinesStruct[0].mPhase->GetPosition(&xt,&yt);
  tmp4->Move(xt,-1);
}

DispFreqs::~DispFreqs()
{
  delete [] mLinesStruct;
}

void DispFreqs::ResizeButtons(int x, int w)
{
  // resize Buttons
  int bw=(w-30)/4;
  btmp1->SetSize(x        ,-1,bw,-1);
  btmp2->SetSize(x+10+  bw,-1,bw,-1);
  btmp3->SetSize(x+20+2*bw,-1,bw,-1);
  btmp4->SetSize(x+30+3*bw,-1,bw,-1);
}

void DispFreqs::SetData()
{
  CProject &project=myProject;
  for (int i=0;i<mLines;i++)
    {
      mLinesStruct[i].SetData(mTop+i,
			      project.GetActive(mTop+i),
			      project.GetNumber(mTop+i),
			      project.GetFrequency(mTop+i),
			      project.GetAmplitude(mTop+i),
			      project.GetPhase(mTop+i),
			      project.GetComposite(mTop+i)
			      );
    }
}

void DispFreqs::ShiftBy(int i)
{

  // Read old Data first
  GetData();
  // Now make the shift
  CProject &project=myProject;
  mTop+=i;
  if (mTop<0)
    {
      mTop=0;
    }
  if (mTop>=(project.GetTotalFrequencies()-mLines))
    {
      mTop=project.GetTotalFrequencies()-mLines;
    }

  // Now display the new ones...
  SetData();
}

void DispFreq_PrevNum(wxButton &but,wxCommandEvent&)
{
  DispFreqs *tmp=(DispFreqs*)(but.GetClientData());
  tmp->ShiftBy(-tmp->GetLines());
}

void DispFreq_Prev(wxButton &but,wxCommandEvent&)
{
  ((DispFreqs*)(but.GetClientData()))->ShiftBy(-1);
}

void DispFreq_Next(wxButton &but,wxCommandEvent&)
{
  ((DispFreqs*)(but.GetClientData()))->ShiftBy(1);
}

void DispFreq_NextNum(wxButton &but,wxCommandEvent&)
{
  DispFreqs *tmp=(DispFreqs*)(but.GetClientData());
  tmp->ShiftBy(tmp->GetLines());
}

#ifdef wx_msw
#pragma optimize("",off)
#endif

void DispFreqs::GetData()
{
  CProject &project=myProject;
  int num=0,act=0,i;
  myString freq,amp,pha;
  for (i=0;i<mLines;i++)
    {
      // initialize values as not changed....
      freq="Default";
      amp="Default";
      pha="Default";
      mLinesStruct[i].GetData(num,&act,&freq,&amp,&pha);
      if (num!=-1)
	{
	  //we can set the data...
	  if (freq!="Default")
	    { project.SetFrequency(num,freq); }
	  if (amp!="Default")
	    { project.SetAmplitude(num,amp); }
	  if (pha!="Default")
	    { project.SetPhase(num,pha); }
	}
    }
  // now set the frequencies active
  for (i=0;i<mLines;i++)
    {
      mLinesStruct[i].GetData(num,&act,&freq,&amp,&pha);
      if (num!=-1)
	{
	  (void) project.SetActive(num,act);  
	}
    }
  project.CheckActive();
}
#ifdef wx_msw
#pragma optimize("",on)
#endif









