//
// period
// ======
//
// written by: Martin Sperl 1995 (c)
//
/////////////////////////////////////////////////
//
// period.cc
// =========
//
// Purpose: period datastructure implementation
//
/////////////////////////////////////////////////

#include "dperiod.h"
#include "dgtimein.h"

#define PRCINT2 1e-12
#define PRCEXT2 1e-14
#define CH_INIT 99999

int CPeriod::Calc(CTimeString &ti)
{
  // now calculate all and return with error-code
  return CalcLoop(ti);
}

int CPeriod::CalcLoop(CTimeString &ti)
{
  // Initialises additional data
  Prepare(ti);
  // Recalc Frequency-combinations
  Recalc();
  // find ChiSqr
  mChisqr=ChiSqr(ti);
  // calculation loops
  double ch=CH_INIT,chdiff;
  mIterations=0;
  int result=0;
  char txt[256]; 
  int factor=1,quit=1;
  while(quit)
    {
      // fill in percentage
      sprintf(txt,"%i Iterations of possible %i done...",
	      mIterations,mMaxIterations);
      InformUserOfProgress(txt);
      // do Message-handling
      ProcessMessageQueue();
      // make a fit of the current data
      result=CurFit();
      if (result!=0)
	{
	  // The determinant could not be determined
	  // or cancel was pressed, so let us terminate...
	  quit=0;
	}
      // check for conditions to quit loop
      chdiff=ch-mChisqr;
      ch=mChisqr;
      mIterations++;
      if(mIterations>=factor*mMaxIterations)
	{
	  char text[1024];
	  sprintf(text,
		  "Maximum number of iterations reached\n"
		  "Do you want to continue?");
	  if (AskUserToContinue(text)==0)
	    {
	      quit=0;
	    }
	  else
	    {
	      // Continue - enlage factor!
	      factor++;
	    }
	}
      if (chdiff<PRCEXT2)
	{quit=0;}
    }
  // remove allocated memory, etc...
  Terminate();
  // adapt phases and frequencies to fit sensible parameters
  Adapt();
  // write out the residuals
  SetResiduals(ti);
  // return without an error
  return result;
}

void CPeriod::Prepare(CTimeString &ti)
{
  // Work on Queue
  ProcessMessageQueue();
  // Now do other stuff
  mTimeString=&ti;
  // Here for Amplitude-Variations
  int ActiveNames=ti.GetActiveNames(mWhat);
  int HighestID=ti.GetHighestActiveIDName(mWhat)+1;
  // now prepare Periods for Amplitude-Variations
  for (int i=0;i<GetFrequencies();i++)
    {
      mData[i].CreateAmplitudeVariationData(HighestID);
    }
  // if we do not use Frequencies, 
  mVariables=CountDegreesOfFreedome(ActiveNames);
  // calculate the maximum number of iterations
  mMaxIterations=3*mVariables;
  if (mMaxIterations<20)
    { mMaxIterations=20; }
  // reset Number of iterations
  mIterations=0;
  // prepare variables-dependent variables  
  mWorkMatrix.changesize(mVariables,mVariables);
  mBaseMatrix.changesize(mVariables,mVariables);
  mWork= new derive_data[mVariables+5];
  // Zero-point
  mWork[0].Result=&mZeropoint;
  mWork[0].Start=mZeropoint;

  // all other variables;
  int where=0;
  int ID=-1;// starts with default-frequency
  for(int fr=0;fr<GetFrequencies();fr++)
    {
      // by default valid for all...
      mWork[where].ID=-1;
      // For the Frequencies
      if (mData[fr].IsSelected(Fre))
	{
	  where++;
	  mWork[where].Result=mData[fr].GetFrequencyRef();
	  mWork[where].Start=mData[fr].GetFrequency();
	  mWork[where].freq=fr;
	  mWork[where].what=Fre;
	  mWork[where].ID=-1;
	}
      // For the Amplitudes
      if (mData[fr].IsSelected(Amp))
	{
	  // if amplitude =0 use a default of 0.006
	  if (mData[fr].GetAmplitude(-1)==0.0)
	  {
	    mData[fr].SetAmplitude(0.006,-1);
	  }
	  // get The Variation-mode
	  CalcMode Var=mData[fr].GetAmpVariation();
	  // now fill in the data
	  switch (Var)
	    {
	    case NoVar:
	    case PhaseVar: 
	      {
		// No Amplitude Variations, so ID=-1
		ID=-1;
		where++;
		mWork[where].Result=mData[fr].GetAmplitudeRef(ID);
		mWork[where].Start=mData[fr].GetAmplitude(ID);
		mWork[where].freq=fr;
		mWork[where].what=Amp;
		mWork[where].ID=ID;
		break;
	      }
	    case AmpVar:
	    case AllVar:
	      {
		// Amplitude Variations
		for (int i=0;i<ti.NumberOfNames(mWhat);i++)
		  {
		    CName &tmp=ti.GetIndexName(mWhat,i);
		    if (tmp.GetPoints()!=0)
		      {
			// this is an active Frequency, so include it...
			ID=tmp.GetID();
			// now add it...
			where++;
			mWork[where].Result=mData[fr].GetAmplitudeRef(ID);
			mWork[where].Start=mData[fr].GetAmplitude(ID);
			mWork[where].freq=fr;
			mWork[where].what=Amp;
			mWork[where].ID=ID;
		      }
		  }
		break;
	      }
	    }
	}
      // For the Phases
      if (mData[fr].IsSelected(Pha))
	{
	  // get The Variation-mode
	  CalcMode Var=mData[fr].GetAmpVariation();
	  // now fill in the data
	  switch (Var)
	    {
	    case NoVar:
	    case AmpVar:
	      {
		// No Phase Variations, so ID=-1
		ID=-1;
		where++;
		mWork[where].Result=mData[fr].GetPhaseRef(ID);
		mWork[where].Start=mData[fr].GetPhase(ID);
		mWork[where].freq=fr;
		mWork[where].what=Pha;
		mWork[where].ID=ID;
		break;
	      }
	    case PhaseVar:
	    case AllVar:
	      {
		// Phase Variations
		for (int i=0;i<ti.NumberOfNames(mWhat);i++)
		  {
		    CName &tmp=ti.GetIndexName(mWhat,i);
		    if (tmp.GetPoints()!=0)
		      {
			// this is an active Frequency, so include it...
			ID=tmp.GetID();
			// now add it...
			where++;
			mWork[where].Result=mData[fr].GetPhaseRef(ID);
			mWork[where].Start=mData[fr].GetPhase(ID);
			mWork[where].freq=fr;
			mWork[where].what=Pha;
			mWork[where].ID=ID;
		      }
		  }
		break;
	      }
	    }
	}

    }
  SetActiveIndex();

}

void CPeriod::Terminate()
{
  mVariables=0;
  mTimeString=0;

  delete [] mWork;
  mWork=0;

  mWorkMatrix.changesize(0,0);
  mBaseMatrix.changesize(0,0);
}

void CPeriod::PrepareMatrices()
{
  for(int j=0;j<mVariables;j++)
    {
      mWork[j].Weighted=0;
      mWork[j].Start=*(mWork[j].Result);
      for(int k=0;k<=j;k++)
	{
	  mBaseMatrix[j][k]=0;
	}
    }
}

void CPeriod::PrepareDerive()
{
  double time,res;
  for(int co=0;co<mTimeString->GetSelectedPoints();co++)
    {
      CTimePoint &tmp=mTimeString->Point(co,&time,&res);
      double weight=tmp.GetWeight();
      int id=tmp.GetIDName(mWhat);
      res-=ActivePredict(time,id);
      Derive(time,id);
      for(int j=0;j<mVariables;j++)
	{
	  if (GetUseWeight())
	    {
	      // use weighted version
	      mWork[j].Weighted+=res*mWork[j].Derived*weight;
	      for(int k=0;k<=j;k++)
		{
		  mBaseMatrix[j][k]+=mWork[j].Derived*mWork[k].Derived*weight;
		}
	    }
	  else
	    {
	      // use normal version
	      mWork[j].Weighted+=res*mWork[j].Derived;
	      for(int k=0;k<=j;k++)
		{
		  mBaseMatrix[j][k]+=mWork[j].Derived*mWork[k].Derived;
		}
	    }
	}
    }
}

void CPeriod::PrepareWorkMatrix()
{
  for(int j=0;j<mVariables;j++)
    {
      for(int k=0;k<mVariables;k++)
	{
	  mWorkMatrix[j][k]=mBaseMatrix[j][k]/
	    sqrt(mBaseMatrix[j][j]*mBaseMatrix[k][k]);
	}
      mWorkMatrix[j][j]=1+mFlamda;
    }
}

void CPeriod::DeconvoluteMatrix()
{
  for(int j=0;j<mVariables;j++)
    {
      derive_data &rev=mWork[j];
      *(rev.Result)=rev.Start;
      for(int k=0;k<mVariables;k++)
	{
	  *(rev.Result)+=mWork[k].Weighted*mWorkMatrix[j][k]/
	    sqrt(mBaseMatrix[j][j]*mBaseMatrix[k][k]);
	}
    }
}

int CPeriod::CurFit()
{
  double tmp=0,chisqrcp=mChisqr;
  // do Message-handling
  if (ProcessMessageQueue())
    {
      // calculation aborted...
      return 2;
    }
  // prepare Matrices
  PrepareMatrices();
  // prepare Derive
  PrepareDerive();
  // mirror Matrix
  mBaseMatrix.mirror();
  // main loop
  do
    {
      // prepare Work matrix
      PrepareWorkMatrix();
      // invert Matrix
      double det=mWorkMatrix.invert();
      if (det==0)
	{
	  // Matrix could not be inverted, so let's return early..
	  return 1;
	}
      // deconvolute Matrix
      DeconvoluteMatrix();
      // Recalc Frequency-combinations
      Recalc();
      // find out about the best fit
      mChisqr=ChiSqr(*mTimeString);
      tmp=chisqrcp+PRCINT2-mChisqr;
      if (tmp<0)
	{
	  mFlamda*=10;
	}
      // do Message-handling
      if (ProcessMessageQueue())
	{
	  // calculation aborted...
	  return 2;
	}
    }
  while(tmp<0);

  mFlamda*=.1;
  return 0;
}

double CPeriod::ChiSqr(CTimeString &timestring)
{
  
  double time,amplitude,chi=0,tmp,weight=1;
  int id=0;
  if (GetUseWeight())
    {
      // use weighted version
      for(int i=0;i<timestring.GetSelectedPoints();i++)
	{
	  CTimePoint &tpoint=timestring.Point(i,&time,&amplitude);
	  weight=tpoint.GetWeight();
	  id=tpoint.GetIDName(mWhat);
	  tmp=(amplitude-ActivePredict(time,id));
	  chi+=weight*tmp*tmp;
	}
      return (chi/timestring.GetWeightSum());
    }
  else
    {
      // use normal version      
      for(int i=0;i<timestring.GetSelectedPoints();i++)
	{
	  CTimePoint &tpoint=timestring.Point(i,&time,&amplitude);
	  id=tpoint.GetIDName(mWhat);
	  tmp=(amplitude-ActivePredict(time,id));
	  chi+=tmp*tmp;
	}
      return (chi/timestring.GetSelectedPoints());
    }
}

void CPeriod::Derive(double t,int id)
{
  mWork[0].Derived=1; // zeropoint
  for(int i=1;i<mVariables;i++)
    {
      if (mWork[i].ID==-1)
	{
	  // No amplitude-variations, so this setting is valid for all
	  mWork[i].Derived=mData[mWork[i].freq].Derive(mWork[i].what,t,id);
	}
      else
	{
	  // We are working on an amplitude-variation-array
	  if (id==mWork[i].ID)
	    {
	      // this is only for a special ID
	      mWork[i].Derived=mData[mWork[i].freq].Derive(mWork[i].what,t,id);
	    }
	  else
	    {
	      // the derived is 0 otherwise
	      mWork[i].Derived=0;
	    }
	}
    }
}

int CPeriod::CountDegreesOfFreedome(int actnames)
{
  int v=0;
  for(int fr=0;fr<GetFrequencies();fr++)
    {
      v+=mData[fr].GetDegreesOfFreedome(actnames);
    }
  return v+1;
}






