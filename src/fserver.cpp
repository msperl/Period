//
// Fourier Server
//

#include "fserver.h"

P98ServerConnection::P98ServerConnection()
  :wxConnection(IPCBuffer,IPCBUFFERSIZE),
   Fourier(NULL),
   Noise(0.0),
   Status(0)
{
}

P98ServerConnection::~P98ServerConnection()
{
  if (Fourier) { delete Fourier;}
  Fourier=NULL;
}

Bool P98ServerConnection::Error(int error)
{
  Status=error;
  if (Status==0)
    {return TRUE;}
  return FALSE;
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
// Command Execution Queue
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

Bool P98ServerConnection::OnExecute(char* topic, char* data, int size, int format)
{
  if (strcmp(topic,"TSdata"  )==0) return TSData      (data,size,format);
  if (strcmp(topic,"Fourier" )==0) return CalcFourier (data,size,format);
  if (strcmp(topic,"Spectral")==0) return CalcSpectral(data,size,format);
  if (strcmp(topic,"Noise"   )==0) return CalcNoise   (data,size,format);
  
  return Error(ERROR_NO_SUCH_CMD);
}

Bool P98ServerConnection::TSData(char *data, int /*size*/, int /*format*/)
{
  double time,amplitude,weight;
  int read=sscanf(data,
		  "%lg %lg %lg",
		  &time,
		  &amplitude,
		  &weight
		  );
  if (read==3)
    {
      Timestring.AddData(CTimePoint(time,amplitude,0,0,weight));
      Timestring.SelectAll();
      return Error(ERROR_NONE);
    }
  return Error(ERROR_TS_DATA);
}

Bool P98ServerConnection::CalcFourier(char *data, int /*size*/, int /*format*/)
{
  double from,to,step;
  int compact;
  int read=sscanf(data,
		  "%lg %lg %lg %i",
		  &from,&to,&step,&compact
		  );
  if (read==4)
    {
      CompactMode comp=(CompactMode) compact;
      myString dummy("Dummy");
      CFourier tmp(dummy,from,to,CFourier::Custom,
		   step,Observed,comp,1);
      tmp.Transform(Timestring,0);
      return Error(ERROR_NONE);
    }
  return Error(ERROR_FOURIER);
}

Bool P98ServerConnection::CalcSpectral(char *data, int /*size*/, int /*format*/)
{
  double from,to,step;
  int compact;
  int read=sscanf(data,
		  "%lg %lg %lg %i",
		  &from,&to,&step,&compact
		  );
  if (read==4)
    {
      myString dummy("Dummy");
      CompactMode comp=(CompactMode) compact;
      CFourier tmp(dummy,from,to,CFourier::Custom,
		   step,SpectralWindow,comp,1);
      tmp.Transform(Timestring,0);
      return Error(ERROR_NONE);
    }
  return Error(ERROR_SPECTRAL);
}

Bool P98ServerConnection::CalcNoise(char *data, int /*size*/, int /*format*/)
{
  double from,to,step;
  int read=sscanf(data,
		  "%lg %lg %lg",
		  &from,&to,&step
		  );
  if (read==3)
    {
      
      Noise=NoiseCalc(Timestring,0.0,from,to,step,1,Observed);
      return Error(ERROR_NONE);
    }
  return Error(ERROR_NOISE);
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
// Request Queue
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

char* P98ServerConnection::OnRequest(char* topic, char* data, int size, int format)
{
  if (strcmp(topic,"TPoints" )==0) return GetTPoints(data,size,format);
  if (strcmp(topic,"FPoints" )==0) return GetFPoints(data,size,format);
  if (strcmp(topic,"Data"    )==0) return GetData   (data,size,format);
  if (strcmp(topic,"Noise"   )==0) return GetNoise  (data,size,format);
  if (strcmp(topic,"Status"  )==0) return GetStatus (data,size,format);
  return NULL;
}
char* P98ServerConnection::GetTPoints(char */*data*/, int /*size*/, int /*format*/)
{
  sprintf(myTPoints,"%i",Timestring.GetSelectedPoints());
  Error(ERROR_NONE);
  return myTPoints;
}

char* P98ServerConnection::GetFPoints(char */*data*/, int /*size*/, int /*format*/)
{
  if (Fourier!=NULL)
    {
      sprintf(myFPoints,"%i",Fourier->Points());
      Error(ERROR_NONE);
      return myFPoints;
    }
  Error(ERROR_FOURIER);
  return NULL;
}

char* P98ServerConnection::GetData(char */*data*/, int /*size*/, int /*format*/)
{
  return "NODATA";
}

char* P98ServerConnection::GetNoise(char */*data*/, int /*size*/, int /*format*/)
{
  sprintf(myNoise,"%.16g",Noise);
  Error(ERROR_NONE);
  return myNoise;
}

char* P98ServerConnection::GetStatus(char */*data*/, int /*size*/, int /*format*/)
{
  sprintf(myStatus,"%i",Status);
  Error(ERROR_NONE);
  return myStatus;
}

wxConnection * P98Server::OnAcceptConnection(char*topic)
{
  if (strcmp(topic,"STDIO")!=0 && strcmp(topic,"P98C/S")==0)
    {
      return new P98ServerConnection;
    }
  return NULL;
}

MyFrame * frame=NULL;
P98Server *my_server=NULL;
MyApp myApp;

wxFrame* MyApp::OnInit()
{
  // create frame
  frame=new MyFrame(NULL,"FourierServer",0,0,400,500);

  // create Icons

  // create Frame content

  // Start IPC
  wxIPCInitialize();
  my_server=new P98Server;
  my_server->Create("4711");

  // Show the frame
  frame->Show(TRUE);
  return frame;
}

#define GENERAL_BUTTON_HANDLER (wxFunction)&myGeneralButtonHandler
void myGeneralButtonHandler(wxButton& but,wxEvent &)
{
  (but.GetParent()->GetParent())->OnClose();
  (but.GetParent()->GetParent())->Show(FALSE);
  delete (MyFrame*) but.GetParent()->GetParent();
}

MyFrame::MyFrame(wxFrame * frame, char * title, 
		 int x, int y, 
		 int w, int h)
  :wxFrame(frame, title,x,y,w,h)
{
  panel=new wxPanel(this,0,0,w,h);
  new wxButton(panel,
	       GENERAL_BUTTON_HANDLER,
	       "Close the Fourier-server"
	       );
  panel->Fit();
  Fit();
}


MyFrame::~MyFrame()
{
}

void MyFrame::CleanIPC()
{
  if (my_server)
    {delete my_server;}
  wxIPCCleanUp();
}

Bool MyFrame::OnClose()
{
  CleanIPC();
  return TRUE;
}

int ProcessMessageQueue()
{
  return 0;
}

void InformUserOfProgress(char *)
{
}
