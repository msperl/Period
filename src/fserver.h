//
// Fourier Server
//

#include "dfourier.h"
#include "dfnoise.h"
#include "wx.h"
#include "wx_ipc.h"
#include <unistd.h>

#define ERROR_NONE        0
#define ERROR_NO_SUCH_CMD 1
#define ERROR_TS_DATA     2
#define ERROR_FOURIER     3
#define ERROR_NOISE       4
#define ERROR_SPECTRAL    5

#define IPCBUFFERSIZE 1024*16

class P98ServerConnection: public wxConnection
{
 public:
  P98ServerConnection();
  ~P98ServerConnection();
  Bool Error(int error);

  Bool OnExecute(char* topic,char *data, int size, int format);
  Bool TSData(char *data, int size, int format);
  Bool CalcFourier(char *data, int size, int format);
  Bool CalcSpectral(char *data, int size, int format);
  Bool CalcNoise(char *data, int size, int format);

  char* OnRequest(char* topic, char* data, int size, int format);
  char* GetFPoints(char *data, int size, int format);
  char* GetTPoints(char *data, int size, int format);
  char* GetData(char *data, int size, int format);
  char* GetNoise(char *data, int size, int format);
  char* GetStatus(char *data, int size, int format);
 protected:
  CTimeString Timestring;
  CFourier *Fourier;
  double Noise;
  int Status;
  //Buffer Data
      char IPCBuffer[IPCBUFFERSIZE];
  char myTPoints[100];
  char myFPoints[100];
  char myNoise[100];
  char myStatus[100];
  char myGetData[IPCBUFFERSIZE];
};

class P98Server: public wxServer
{
public:
  wxConnection * OnAcceptConnection(char*topic);
};

class MyApp: public wxApp
{
 public:
  wxFrame *OnInit();
};

class MyFrame: public wxFrame
{
 public:
  MyFrame(wxFrame * frame, char * title, 
	  int x, int y, 
	  int w, int h);
  ~MyFrame();
  Bool OnClose();
  void CleanIPC();
 protected:
  wxPanel *panel;
};
