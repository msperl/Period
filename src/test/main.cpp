//
// Fourier Server
//

#include "dfourier.h"
#include "dfnoise.h"

#define IN cin
#define OUT cout
#define ERR cerr

CTimeString Timestring;

int Fourier();
int Spectral();
int Noise();

int main()
{
  char c;
  double time,amp,calc,weight;
  int i;
  int quit=0;
  while (!quit)
    {
      // read Line
      IN>>c;
      switch (c)
	{
	case 'd': // data      (time, amp, weight)
	  IN>>time>>amp>>weight;
	  Timestring.AddData(CTimePoint(time,amp,0,0,weight));
	  break;
	case 'f': // fourier   (from,to,step,compact)
	  Timestring.SelectAll();
	  ERR<<Timestring.GetSelectedPoints()<<endl;
	  Fourier();
          quit=1;
	  break;
	case 's': // spectral  (from,to,step,compact)
	  Timestring.SelectAll();
	  Spectral();
          quit=1;
	  break;
	case 'n': // noise     (from,to,step)
	  Timestring.SelectAll();
	  Noise();
          quit=1;
	  break;
        case 'q': // quit
          quit=1;
	  break;
	default:
	  ERR<<"Command not known:"<<c<<endl;
	  break;
	}
    }
  flush(OUT);
}

int Fourier()
{
  double from,to,step;
  int compact;
  // Read in
  IN>>from>>to>>step>>compact;
  CompactMode comp=(CompactMode)compact;
  // Start Calculate
  myString dummy("Dummy");
  CFourier tmp(dummy,from,to,CFourier::Custom,step,Observed,
	       comp,1);
  ERR<<"Calc"<<endl;
  tmp.Transform(Timestring,0);
  ERR<<"Calc done"<<endl;
  // Output
  int i,points=tmp.entries();
  for (i=0;i<points;i++)
    { OUT<<tmp[i]<<endl;}
  return 0;
}

int Spectral()
{
  double from,to,step;
  int compact;
  // Read in
  IN>>from>>to>>step>>compact;
  CompactMode comp=(CompactMode)compact;
  // Start Calculate
  myString dummy("Dummy");
  CFourier tmp(dummy,from,to,CFourier::Custom,step,SpectralWindow,
	       comp,1);
  tmp.Transform(Timestring,0);
  // Output
  tmp.startiterate();
  CFourierPoint *pnt;
  while (pnt=tmp.iterate())
    {
      OUT<<(*pnt)<<endl;
    }
  return 0;
}

int Noise()
{
  double from,to,step;
  int compact;
  IN>>from>>to>>step>>compact;
  OUT<<NoiseCalc(Timestring,0.0,from,to,step,1,Observed)<<endl;
  return 0;
}

int ProcessMessageQueue()
{
  return 0;
}

void InformUserOfProgress(char *)
{}
