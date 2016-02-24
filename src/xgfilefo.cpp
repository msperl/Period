/*
 * Program:     Period98
 *
 * File:        xgfilefor.cpp
 * Purpose:     implementation-file for
 *              the FileFormat-Dialogs
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */
#include <ctype.h>
#include <string.h>

#include "xgeneral.h"
#include "xgprojec.h"

#include "xpspecia.h"

#include "xgmainfr.h"
#include "ldialtxt.h"
#include "xgdial.h"

myString wxPeriProject::FileFormat(myString format, int Phase)
{
  char *text[12];
  int select[12];
  int selected=0;
  myString Names[4];
  Names[0]=NameSet(0);
  Names[1]=NameSet(1);
  Names[2]=NameSet(2);
  Names[3]=NameSet(3);
  if (Phase) { text[0]=TIMET_PHASE; } else { text[0]=TIMET_TIME; }
  if (format.Find('t')!=-1) { select[selected]=0; selected++;}
  text[1]=TIMET_OBS;
  if (format.Find('o')!=-1) { select[selected]=1; selected++;}
  text[2]=TIMET_ADJ;
  if (format.Find('a')!=-1) { select[selected]=2; selected++;}
  text[3]=TIMET_CALC;
  if (format.Find('c')!=-1) { select[selected]=3; selected++;}
  text[4]=TIMET_RESOBS;
  if (format.Find('p')!=-1) { select[selected]=4; selected++;}
  text[5]=TIMET_RESADJ;
  if (format.Find('b')!=-1) { select[selected]=5; selected++;}
  text[6]=TIMET_PNT_WEIGHT;
  if (format.Find('g')!=-1) { select[selected]=6; selected++;}
  text[7]=TIMET_WEIGHT;
  if (format.Find('w')!=-1) { select[selected]=7; selected++;}
  text[8]=Names[0].chars();
  if (format.Find('1')!=-1) { select[selected]=8; selected++;}
  text[9]=Names[1].chars();
  if (format.Find('2')!=-1) { select[selected]=9; selected++;}
  text[10]=Names[2].chars();
  if (format.Find('3')!=-1) { select[selected]=10; selected++;}
  text[11]=Names[3].chars();
  if (format.Find('4')!=-1) { select[selected]=11; selected++;}
  
  // create dialog
  int result= wxGetMultipleChoice(
				  WXPROJ_FILEFOR,
				  WXPROJ_FILESELECT,
				  12,text,
				  selected,select,
				  NULL,-1,-1,TRUE,
				  150,250
				  );
  if (result==-1) { return ""; }
  // parse result
  myString tmp="";
  for (int i=0;i<result;i++)
    {
      switch (select[i])
	{
	case 0: tmp+='t';break;
	case 1: tmp+='o';break;
	case 2: tmp+='a';break;
	case 3: tmp+='c';break;
	case 4: tmp+='p';break;
	case 5: tmp+='b';break;
	case 6: tmp+='g';break;
	case 7: tmp+='w';break;
	case 8: tmp+='1';break;
	case 9: tmp+='2';break;
	case 10: tmp+='3';break;
	case 11: tmp+='4';break;
	default:break;
	}
    }
  return tmp;
}

#define FIFOLINES 30
#define FIFOCOLUMNS 4
myString fifoLines[FIFOLINES];
char fifoformat[FIFOLINES+2];
typedef  wxChoice * myfifochoice;
myfifochoice fifochoice[FIFOCOLUMNS];
int fifotabs[FIFOCOLUMNS];
wxListBox * fifolist;
int fifoposition;
char *fifochoicemap="toacgpb1234n";
int fifostrformat=17;

void fifoFillInData(int lines=FIFOLINES)
{
  int i;
  // first fill in selection
  for (i=0;i<FIFOCOLUMNS;i++)
    {
      int j;
      // none is the default
      char typ=fifoformat[fifoposition+i];
      // now find the true
      for (j=0;(j<12)&&(fifochoicemap[j]!=typ);j++)
	{;}
      if (j==12) { j=11;}
      fifochoice[i]->SetSelection(j);
    }
  // clean fifolists and hide
  fifolist->Show(FALSE);
  fifolist->Clear();
  // now fill the entries
  for (i=0;i<lines;i++)
    {
      char *line=fifoLines[i].chars();
      int j,k=0;
      // skip first fields
      for (j=0;j<fifoposition;j++)
	{
	  while ( (!isspace(line[k]))&&(line[k]!=0) )
	    { k++; }
	  if (isspace(line[k]))
	    { k++; }
	}
      char tmp[FIFOCOLUMNS][256];
      for (j=0;j<FIFOCOLUMNS;j++)
	{
	  int l=0;
	  while ( (!isspace(line[k]))&&(line[k]!=0) )
	    { tmp[j][l]=line[k]; k++;l++; }
	  if (isspace(line[k]))
	    { k++; }
	  tmp[j][l]=0;
	  tmp[j][fifostrformat]=0;
	}
      char format[256];
      myString Final="";
      sprintf(format,"%%%is",fifostrformat);
      fifotabs[0]=0;
      for (j=0;j<FIFOCOLUMNS;j++)
	{
	  char string[256];
	  sprintf(string,format,tmp[j]);
	  if (j!=FIFOCOLUMNS-1)
	    {
	      float w,h;
	      Final+=string;
	      Final+=" |";
	      fifolist->GetTextExtent(Final.chars(),&w,&h);
	      Final+=" ";
	      fifotabs[j+1]=int(w);
	    }
	  else
	    {
	      Final+=string;
	    }
	}
      fifolist->Append(Final.chars());
    }
  // show fifolists
  fifolist->Show(TRUE);
}

void fifoRestoreFormat()
{
  // get values and set it
  for (int i=0;i<FIFOCOLUMNS;i++)
    {
      fifoformat[fifoposition+i]=fifochoicemap[fifochoice[i]->GetSelection()];
    }
}

void fifoLeft(wxButton& ,wxEvent &)
{
  fifoRestoreFormat();
  fifoposition--;
  if (fifoposition<0) { fifoposition=0; }
  fifoFillInData();
}

void fifoRight(wxButton& ,wxEvent &)
{
  fifoRestoreFormat();
  fifoposition++;
  if (fifoposition>26) { fifoposition=26; }
  fifoFillInData();
}

myString wxPeriProject::FileFormat(myString fifoformatstr,myString filename)
{
  int i;
  // fill the Fifoformat
  for (i=0;i<FIFOLINES+2;i++) { fifoformat[i]='n';}
  // clean fifo-lines
  for (i=0;i<FIFOLINES;i++)
    {
      fifoLines[i]="";
    }
  // read first view lines in from file
  for (i=0;i<fifoformatstr.length();i++) { fifoformat[i]=fifoformatstr[i];}
  // an array of string
  {
    char tmp[1024];
    ifstream file(filename.chars());
    for (i=0;(i<FIFOLINES)&&(!file.eof());i++)
      {
	file.getline(tmp,1024);
	if (file.good())
	  {
	    int len=strlen(tmp);
	    int k=0;
	    for (int j=0;j<len;j++)
	      {
		if (isspace(tmp[j]))
		  {
		    if (!isspace(tmp[j+1]))
		      {
			if (k)
			  {
			    tmp[k]=tmp[j];
			    k++;
			  }
		      }
		  }
		else
		  {
		    tmp[k]=tmp[j];
		    k++;
		  }
	      }
	    tmp[k]=0;
	    fifoLines[i]=tmp;
	  }
      }
  }
    
  // create Dialog
  myDialogBox dialog(NULL,WXPROJ_FILEFOR,TRUE);
  // Set Dialog Fonts
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
  
  // Create message
  (void) new wxMessage(&dialog,WXPROJ_FILEFORMAT);
  dialog.NewLine();
  // define fifopositionsible choices
  myString Names[FIFOCOLUMNS];
  for (i=0;i<FIFOCOLUMNS;i++)
    {
      Names[i]=NameSet(i);
    }
  char *choices[12];
  choices[0]=TIMET_TIME;
  choices[1]=TIMET_OBS;
  choices[2]=TIMET_ADJ;
  choices[3]=TIMET_CALC;
  choices[4]=TIMET_PNT_WEIGHT;
  choices[5]=TIMET_RESOBS;
  choices[6]=TIMET_RESADJ;
  choices[7]=Names[0].chars();
  choices[8]=Names[1].chars();
  choices[9]=Names[2].chars();
  choices[10]=Names[3].chars();
  choices[11]=TIMET_NONE;

  // at first create the left Button
  wxButton *Left=new wxButton(&dialog,(wxFunction)&fifoLeft," < ");
  // now create the choices
  for (i=0;i<FIFOCOLUMNS;i++)
    {
      fifochoice[i]=new wxChoice(&dialog,NULL,NULL,
			     -1,-1,-1,-1,
			     12,choices);
      //fifochoice[i]->Fit();
    }
  // then create the right Button
  wxButton *Right=new wxButton(&dialog,(wxFunction)&fifoRight," > ");
  dialog.NewLine();
  // now the Listbox
  fifolist=new wxListBox(&dialog,NULL,NULL,
			    wxSINGLE,-1,-1,300,-1);
  // calculate spacing
  {
    // first get minimum size for Choices
    int cw,ch;
    fifochoice[0]->GetSize(&cw,&ch);
	cw=cw+40;
    // now find size
    char text[256];
    float w,h;
    for (int ii=1;ii<40;ii++)
      {
	fifostrformat=ii;
	text[ii-1]='_';
	text[ii]=0;
	fifolist->GetTextExtent(text,&w,&h,NULL,NULL,button);
	if (w>cw)
	  {
	    break;
	  }
      }
  }
  // now fill the data in...
  fifoFillInData(1);
  // refit listbox
  FitListBox(fifolist,0);
  dialog.NewLine();
  // now add OK and CANCEL Button
  wxButton *Ok=new wxButton(&dialog,GENERAL_BUTTON_HANDLER,OKAY);
  Ok->SetDefault();
  wxButton *Cancel=new wxButton(&dialog,GENERAL_BUTTON_HANDLER,CANCEL);
  // now resize the Buttons
  {
    int xxl,yyl,xxr,yyr;
    int lx,ly,lw,lh;
    // first set list to the correct position
    fifochoice[0]->GetPosition(&lx,&ly);
    fifolist->SetSize(lx,-1,-1,-1,wxSIZE_USE_EXISTING);
    // now resize the choices
    int width=fifotabs[2]-fifotabs[1]-dialog.GetHorizontalSpacing();
    for (int j=0;j<FIFOCOLUMNS;j++)
      {
	fifochoice[j]->
	  SetSize(lx+fifotabs[j]+dialog.GetHorizontalSpacing()/2,
		  -1,
		  width,
		  -1,wxSIZE_USE_EXISTING);
      }
#ifdef wx_msw
    fifostrformat=fifostrformat>>1;
#endif
    fifoFillInData();
    // first the left/right buttons
    fifolist->GetPosition(&lx,&ly);
    fifolist->GetSize(&lw,&lh);
    fifochoice[0]->GetPosition(&xxr,&yyr);
    Left ->SetSize(-1,yyr,-1,lh+ly-yyr,wxSIZE_USE_EXISTING);
    Right->SetSize(lx+lw+dialog.GetHorizontalSpacing(),
		   yyr,
		   -1,
		   lh+ly-yyr,wxSIZE_USE_EXISTING);
    // now OK and cancel
    Right->GetSize(&lw,&lh);
    Right->GetPosition(&xxr,&yyr);
    Left->GetPosition(&xxl,&yyl);
    int xxmax=xxr+lw;
    width=(xxmax-xxl-dialog.GetHorizontalSpacing())/2;
    Ok->GetPosition(&lx,&ly);
    Ok->SetSize(xxl,
		-1,
		width,
		-1,wxSIZE_USE_EXISTING);
    Cancel->SetSize(xxl+width+dialog.GetHorizontalSpacing(),
		    -1,
		    width,
		    -1,wxSIZE_USE_EXISTING);
    // now size window to fit
    dialog.Fit();
    // resize to fit
    dialog.SetSize(-1,-1,
		   xxl+2*width+3*dialog.GetHorizontalSpacing(),-1,
		   wxSIZE_USE_EXISTING);
    }
  fifoposition=0;
  // set buttons inactive
  Ok->SetClientData(0);
  Cancel->SetClientData(0);
  // center Dialog
  dialog.Centre();
  // and show it
  dialog.Show(TRUE);
  // restore fifoformat
  fifoRestoreFormat();
  // remove 'n' from file-format
  for (i=FIFOLINES;i>-1;i--)
    {
      if (fifoformat[i]=='n')
	{
	  fifoformat[i]=0;
	}
      else
	{
	  break;
	}
    }
  // is Cancel Pressed?
  if (Cancel->GetClientData())
    {
      // OK, abort
      return "";
    }
  // return File-format
  return fifoformat;
}

