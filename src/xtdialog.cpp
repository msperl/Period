/*
 * Program:     Period98
 *
 * File:        xtdialog.cpp
 * Purpose:     implementation-file for
 *              ?
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */
#include "xtdialog.h"
#include "ldialtxt.h"
#include "xgsize.h"
#include "xgmenu.h"
#include "xgdial.h"

t_File::t_File(CProject* project,wxWindow* parent,int x0, int y0, int w, int h)
  :myFile(project,DISPLAY_DIALOG_TIMESTRING,parent,x0,y0,w,h,1)
{
  int i;
  // first line
  DataFile= new wxMultiText(this,NULL,TIME_CURRENT,TIME_CURRENT_DEF,
			    -1,-1,100,50,wxREADONLY);
  NewLine();

  // second line
  (void) new wxMessage(this,TIME_SELECTED);
  Selected= new wxMessage(this,TIME_SELECTED_DEF);
  (void) new wxMessage(this,TIME_TOTAL);
  Total= new wxMessage(this,TIME_TOTAL_DEF);
  NewLine();

  // third line
  (void) new wxMessage(this,TIME_START);
  Start= new wxMessage(this,TIME_START_DEF);
  (void) new wxMessage(this,TIME_END);
  End= new wxMessage(this,TIME_END_DEF);
  NewLine();

  // fourth line
  ReverseScale = new wxCheckBox(this,(wxFunction)&ReverseClick,TIME_REVERSE);
  ReverseScale->SetValue(myProject.GetReverseScale());
  NewLine();

  // create Buttons
  for (i=0;i<4;i++)
    {
      Label[i]=  new wxButton(this,
			      DefaultButtonHandler(),
			      myProject.NameSet(i).chars());
    }
  NewLine();

  // create tables
  int height=Timestring_ListHeight;
  int width=100;
  int spacing=10;

  // Create the first listbox
  long style;
#ifdef wx_xt
  style=wxMULTIPLE|wxEXTENDED;
#else
  style=wxEXTENDED;
#endif
  SetLabelPosition(wxVERTICAL);
  Lists[0]= new wxListBox(this,(wxFunction)&ListBoxClick,
			  NULL/*myProject.NameSet(0).chars()*/,
			  style,
			  -1,-1,width,height,
			  0,NULL,
			  style
			  );
  // Now get Starting-positions
  int x=0,y=0;
  Lists[0]->GetPosition(&x,&y);
  // Create the other ones with the relevant coordinates
  for (i=1;i<4;i++)
    {
      // Calculate position
      x+=width+spacing;
      // Create listbox
      Lists[i]= new wxListBox(this, (wxFunction)&ListBoxClick,
			      NULL/*myProject.NameSet(i).chars()*/,
			      style,
			      x,y,width,height,
			      0,NULL,
			      style
			      );
    }
  SetLabelPosition(wxHORIZONTAL);
  NewLine();

  // Create name-Change-buttons
  for (i=0;i<4;i++)
    {
      Edit[i]=  new wxButton(this,DefaultButtonHandler(),TIME_EDIT);
    }
  NewLine();

  // Adapt-Button-positions

  for (i=0;i<4;i++)
    {
      myAllign(Lists[i],Label[i],BTN_LABEL+i);
      myAllign(Lists[i],Edit[i],BTN_EDIT+i);
    }

  // start a new line
  NewLine();

  // now create the other buttons
  myFile::CreateButtons();

  // Update Display - especially the data in it...
  UpdateDisplay();
}

void t_File::SetSize(int x,int y,int w, int h, int flags)
{
  // sets the size of the dialog itself
  myFile::SetSize(x,y,w,h,flags);
  // now adjust coordinates
  x=10;
  w=w-20;
  // int adjust multitext
  int bw,bh;
  DataFile->GetSize(&bw,&bh);
  int bx,by;
  DataFile->GetPosition(&bx,&by);
  DataFile->SetSize(x,by,w,bh);
  // int Buttonsizes
  Edit[0]->GetSize(&bw,&bh);
  // set List-boxes and Buttons
  y=h-2*(bh+5);
  int lw=(w-30)/4;
  int lx,ly;
  Lists[0]->GetPosition(&lx,&ly);
  int lh=y-ly-5;
  for (int i=0;i<4;i++)
    {
      Lists[i]->SetSize(x+i*(lw+10),ly,lw,lh);
      Edit[i] ->SetSize(x+i*(lw+10), y,lw,-1);
      Label[i] ->SetSize(x+i*(lw+10),-1,lw,-1);
    }
}

void t_File::SetReverse()
{
  // set Value
  myProject.SetReverseScale(ReverseScale->GetValue());  
  // Update Displays
  myProject.UpdateTSDisplays();
}

void t_File::SetSelection()
{
  // set Selection
  for (int id=0;id<4;id++)
    {
      // ClearSelection
      for (int j=0;j<myProject.NumberOfNames(id);j++)
	{
	  int WhichOne=(int)Lists[id]->GetClientData(j);
	  int Selected=Lists[id]->Selected(j);
	  myProject.SetIDNameSelect(id,WhichOne,Selected);
	}
    }
  // Select Data
  myProject.Select();
  UpdateSelected();
}

void t_File::OnUknButton(int i)
{
  switch (i)
    {
    case BTN_EDIT+0:
    case BTN_EDIT+1:
    case BTN_EDIT+2:
    case BTN_EDIT+3:
      EditSettings(i-BTN_EDIT);
      break;
    case BTN_LABEL+0:
    case BTN_LABEL+1:
    case BTN_LABEL+2:
    case BTN_LABEL+3:
      EditLabel(i-BTN_LABEL);
      break;
    default:
      myFile::OnUknButton(i);
      break;
    }
}

void t_File::OnDispGraph()
{
  myProject.DisplayTSGraph();
}

void t_File::OnDispData()
{
  myProject.DisplayTSTable();
}

void t_File::EditLabel(int i)
{
  myString oldtitle=Label[i]->GetLabel();
  char * NewTitle=::wxGetTextFromUser(HEADING_MESSAGE,
				      HEADING_CAPTION,
				      oldtitle.chars(),
				      this);
  if (NewTitle!=NULL)
    {
      myProject.ChangeHeading(i,NewTitle);
    }
}

void t_File::EditSettings(int i)
{
  // find active settings...
  int *selection;
  int selected=Lists[i]->GetSelections(&selection);
  if (selected==0)
    {
      wxMessageBox(TIME_NOSELECT);
      return;
    }
  // loop through all selected entries...
  for (int j=0;j<selected;j++)
    {
      // What is the ID of the string
      int ID=(int)(Lists[i]->GetClientData(selection[j]));
      // create strings
      myString namestr=myProject.GetIDNameStr(i,ID);
      myString weightstr=myProject.GetIDNameWeight(i,ID);
      char* colorstr=myProject.GetIDNameColor(i,ID);
      // Create a Dialog-box
      myDialogBox dialog(this->GetGrandParent(),
		  TIME_CHANGE,TRUE,-1,-1,300,90);
      // Create a name-display
      (void) new wxMessage(&dialog,TIME_NAME);
      wxText *name= new wxText(&dialog,NULL,NULL,
			       "____________");
      dialog.NewLine();
      // Create a Weight-display
      (void) new wxMessage(&dialog,TIME_WEIGHT);
      wxText *weight= new wxText(&dialog,NULL,
				 NULL,
				 "__________");
      dialog.NewLine();
      // Create a Color-display
      (void) new wxMessage(&dialog,TIME_COLOR);
      wxChoice *color= new wxChoice(&dialog,NULL,
				    NULL,
				    -1,-1,-1,-1,
				    myProject.GetMaxColors(),
				    myProject.GetColors()
				    );
      dialog.NewLine();
      // Create a OK-Button
      wxButton *DoIt,*Quit; 
      DoIt= new wxButton(&dialog,
			 (wxFunction)&ChangeDialogOK,
			 OKAY);
      DoIt->SetDefault();
      // Create a Close-Button
      Quit= new wxButton(&dialog,
			 (wxFunction)&ChangeDialogCancel,
			 CANCEL);
      // Insert Data
      name->SetValue(namestr.chars());
      weight->SetValue(weightstr.chars());
      color->SetStringSelection(colorstr);
      // make the Dialog fit
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
	// now find out about the size of all of these
	int xmax=0,wmax=0;
	int x,y,w,h;
	name->GetPosition(&xmax,&y);
	name->GetSize(&wmax,&h);
	weight->GetPosition(&x,&y);
	weight->GetSize(&w,&h);	
	if (w>wmax) { wmax=w; }
	if (x>xmax) { xmax=x; }
	color->GetPosition(&x,&y);
	color->GetSize(&w,&h);	
	if (w>wmax) { wmax=w; }
	if (x>xmax) { xmax=x; }
	// now check size of Buttons
	if (width<OKw) 
	  { 
	    wmax+=2*(OKw-width);
	    width=OKw;
	  }
	name->SetSize(xmax,-1,wmax,-1);
	weight->SetSize(xmax,-1,wmax,-1);
	color->SetSize(xmax,-1,wmax,-1);
	// now resize Buttons
	DoIt->SetSize(OKx,OKy,width,-1);
	Quit->SetSize(OKx+width+spacing,-1,width,-1);
      }      
      // refit the dialog
      dialog.Fit();
      dialog.Centre();
      // open Dialogbox
      dialog.Show(TRUE);
      // check if we should change this set
      if ((int)dialog.GetClientData()==1)
	{
	  // Now read settings and change these...
	  myProject.ChangeName(i,ID,
			       name->GetValue(),
			       weight->GetValue(),
			       color->GetStringSelection()
			       );
	}
    }
  myProject.CalcWeights();
  myProject.UpdateAllDisplays();
}

void ChangeDialogCancel(wxButton& button,wxEvent &)
{
  button.GetParent()->SetClientData((char*)0);
  button.GetParent()->Show(FALSE);
}

void ChangeDialogOK(wxButton& button,wxEvent &)
{
  button.GetParent()->SetClientData((char*)1);
  button.GetParent()->Show(FALSE);
}

void t_File::myAllign(wxListBox* lb, wxButton *bu, int id)
{
  int xb,yb,wb,hb;
  int xl,yl,wl,hl;

  // Get the Position of the two objects
  lb->GetPosition(&xl,&yl);
  bu->GetPosition(&xb,&yb);

  // get the size of the two objects
  lb->GetSize(&wl,&hl);
  bu->GetSize(&wb,&hb);

  // set button-position correctly
  bu->SetSize(xl,yb,wl,hb);

  // set identification
  bu->SetClientData((char*) id);
}

t_File::~t_File()
{
  // nothing to destruct yet !!!
}

void t_File::UpdateData()
{
}

void t_File::UpdateDisplay(int)
{
  char tmp[256];

  // write Filename
  myString name=myProject.GetTSFileName();
  if (name=="")
    {
      name=TIME_NODATA;
    }
  DataFile->SetValue(name.chars());

  // write selected points
  UpdateSelected();
  
  // write total Points
  sprintf(tmp,"%i",myProject.GetTotalPoints());
  Total->SetLabel(tmp);

  // check Reverse scale
  ReverseScale->SetValue(myProject.GetReverseScale());

  // 
  wxMenuBar *menu=((wxFrame*)GetGrandParent())->GetMenuBar();
  menu->Check(M_TIMESTRING_NAME_WEIGHT,myProject.GetUseNameWeight());
  menu->Check(M_TIMESTRING_POINT_WEIGHT,myProject.GetUsePointWeight());

  // now set the lists
  int i;
  for (i=0;i<4;i++)
    {
      // fill the lists
      FillList(Lists[i],i);
      // set the label
      Label[i]->SetLabel(myProject.NameSet(i).chars());
    }

}

void t_File::UpdateSelected()
{
  // write selected points
  int points=myProject.GetSelectedPoints();
  char tmp[256];
  sprintf(tmp,"%i",points);
  Selected->SetLabel(tmp);
  // write start time

  if (points>0)
    {
      Start->SetLabel(myProject.GetStartTime().chars());
      End->SetLabel(myProject.GetEndTime().chars());
    }
  else
    {
      Start->SetLabel(TIME_NO_POINTS);
      End->SetLabel(TIME_NO_POINTS);
    }
}

struct myentry
{
public:
int id;
int select;
myString string;
};

int SortMyEntry(const void *t1,const void *t2)
{
  myString tmp1=((myentry *)t1)->string;
  myString tmp2=((myentry *)t2)->string;
  if (tmp1>tmp2) return 1;
  if (tmp1<tmp2) return -11;
  return 0;
}

void t_File::FillList(wxListBox * lbox, int id)
{
  int i;
  lbox->Clear();
  int entries=myProject.NumberOfNames(id);
  // create sorted table
  myentry* array=new myentry[entries];
  for (i=0;i<entries;i++)
    {
      array[i].string=myProject.GetIndexNameStr(id,i);
      array[i].id    =myProject.GetIndexNameID(id,i);
      array[i].select=myProject.GetIndexNameSelect(id,i);
    }
  // sort array
  qsort(array,entries,sizeof(myentry),SortMyEntry);
  // fill list
  for(i=0;i<entries;i++)
    {
      lbox->Append(array[i].string.chars(),
		   (char*)array[i].id);
    }
  // select list
  for(i=0;i<entries;i++)
    {
      lbox->SetSelection(i,array[i].select);
    }
}

void t_File::OnLoad()
{
  // ask for filename and load it
  if (myProject.GetTotalPoints()==0)
    {
      myProject.LoadTimeString();      
    }
  else 
    {
      if (wxMessageBox(TIMET_REMOVEOLDTS,TIMET_REMOVEOLDTS_H,wxYES_NO)!=0)
	{
	  myProject.LoadTimeString();
	}
    }
}

void t_File::OnSave()
{
  // ask for filename and save to it
  myProject.SaveTimeString();
  UpdateDisplay();
}

void ListBoxClick(wxListBox &list,wxEvent &)
{
  ((t_File*)list.GetParent())->SetSelection();
}

void ReverseClick(wxCheckBox &list,wxEvent &)
{
  ((t_File*)list.GetParent())->SetReverse();
}








