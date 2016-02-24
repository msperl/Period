#include <fstream>
#include <iomanip>
#include "dfourier.h"
#include "dtimestr.h"

int ProcessMessageQueue()
{
  return 0;
}

void InformUserOfProgress(char * text)
{
  static char compare[256]={0};
  if (strcmp(compare, text)) {
    cerr << "Progress: " << text << endl;
    strncpy(compare, text, sizeof(compare)-1);
  }
}

double parse_arg(char* process, char *field, char *sval)
{
  double val;
  char *end;

  val = strtod(sval, &end);
  if (end == sval) {
    cerr << process << ": Could not parse " << field << " argument " << sval << endl;
    exit(4);
  }
  
  return val;
}

int main(int argc, char **argv)
{
  CTimeString *ts;
  CFourier *fourier;
  CPeriPoint peak;
  double start, end, step = 0, nyqu, avg;
  const char *ofile = NULL;
  ostream *out;

  if (argc < 4) {
    cerr << "Usage: " << argv[0] << " <timesting> <from> <step> <to> [<output>]" << endl;
    exit(1);
  }

  /* read timestring */
  ts = new CTimeString(argv[1], "to");
  if (!ts)
    exit(3);

  /* get nyquist */
  nyqu = ts->Nyquist();
  avg = ts->Average();

  /* check parameters */
  start = parse_arg(argv[0], "start", argv[2]);
  step  = parse_arg(argv[0], "step",  argv[3]);
  end   = parse_arg(argv[0], "end",   argv[4]);
  ofile= (argc < 4) ? "-" : (argv[5] ? argv[5] : "-");
  ofile= (argc < 4) ? "-" : argv[5];

  if (start > end) {
    cerr << argv[0] << ": start (" << start << ") > end (" << end << ")" << endl;
    exit(5);
  }
  if (step <= 0) {
    cerr << argv[0] << ": step (" << step << ") < 0" << endl;
    exit(6);
  }

  if (end > nyqu)
    cerr << argv[0] << ": WARNING: end frequency  (" << nyqu << ") > nyquist (" << end << ")" << endl;

  /* create fourier object */
  fourier = new CFourier("xxx", start, end, CFourier::StepQuality::Custom, step, Observed, AllData, avg);
  if (!fourier)
    exit(2);

  /* report */
  cerr << "Points:\t"  << ts->GetSelectedPoints() << endl;
  cerr << "Start:\t"   << std::setprecision(8) << start << endl;
  cerr << "Step:\t"    << std::setprecision(8) << step << endl;
  cerr << "End:\t"     << std::setprecision(8) << end << endl;
  cerr << "Nyquist:\t" << std::setprecision(8) << nyqu << endl;
  cerr << "Average:\t" << std::setprecision(8) << avg << endl;

  /* run fourier */
  fourier->Transform(*ts, 0);
  peak = fourier->GetPeak();

  /* report on peak */
  cerr << "Peak:\t" << peak.GetFrequency() << "\t" << peak.GetAmplitude(0) << endl;

  /* and dump fourier */
  if (!ofile || (strcmp(ofile, "-") == 0)) {
    out = &cout;
  } else {
    out = new ofstream(ofile);
    if (!out) {
      cerr << argv[0] << ": Could not write to " << ofile << endl;
      exit(7);
    }
  }
    
  /* and dump it */
  fourier->WriteData(*out);

  return 0;
}
