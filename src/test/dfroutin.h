//
// HEADER
//


#ifdef HEADER
#undef MAINCODE

  int wind=int(200000/time.GetSelectedPoints())+1;
  if (wind<2) { wind=2; }
  if (wind>2000) { wind=3000; }
  int times=wind;
  CFourCalc calc(wind);
  
  double p2; 
  // should we use weighted data?
  if (UseWeight)
    {
      // use the sum of weight instead
      p2=2.0/time.GetWeightSum();
    }
  else
    {
      p2=2.0/time.GetSelectedPoints();
    }

  // find out Mode of calculations
  if (mode==SpectralWindow)
    {
      // for scaling purposes...
      p2/=2.0;
    }

#endif /* HEADER */

//
// MAINCODE
//

#ifdef MAINCODE
  int quit=0;

  int i,j;
  double start=from;
  do
    {
      //
      // calculate part of the spectra
      //

      // initialisation
      for (i=0;i<wind;i++)
	{
	  calc[i].Comp()=0;
	  if (from>to)
	    {
	      calc[i].Freq()=END;
	      break;
	    }
	  else
	    {
	      calc[i].Freq()=from;
	      from+=step;
	    }
	}
	// main loop
	double ti,am;
	for (i=0;i<time.GetSelectedPoints();i++)
	  {
#ifdef USEWEIGHTS
	    double weight=(time.Point(i,mode,&ti,&am)).GetWeight();
#else /* USEWEIGHTS */
	    time.Point(i,mode,&ti,&am);
#endif /* USEWEIGHTS */
	    // subtract zeropoint
	    am-=zero;
	    // prepare SIN-Approx
	    double ThisSin,LastSin,SDF,ThisCos,LastCos,CDF;
	    {
	      double XX=2*MYPI*ti;
	      
	      double XF0=XX*(calc[0].Freq()-step);
	      XF0=fmod(XF0,MY2PI);
	      
	      double XDF=XX*step;
	      XDF=fmod(XDF,MY2PI);
	      
	      LastSin=sin(XF0);
	      LastCos=cos(XF0);
	      SDF=sin(XDF);
	      CDF=cos(XDF);
	    }

#ifdef SPECTRALWINDOW
#  ifdef USEWEIGHTS
	    am=p2*weight;
#  else
	    am=p2;
#  endif
#else
#  ifdef USEWEIGHTS
	    am*=p2*weight;
#  else /* USEWEIGHTS */
	    am*=p2;
#  endif /* USEWEIGHTS */
#endif

	    for(j=0;j<times;j++)
	      {
		ThisSin=LastSin*CDF+LastCos*SDF;
		ThisCos=LastCos*CDF-LastSin*SDF;
		calc[j].Comp()+=am*mycomplex(ThisSin,ThisCos);
		LastSin=ThisSin;
		LastCos=ThisCos;
	      }
	  }

	// calculate power
	calc.Power();

	// Process Message Queue
	// if returns non zero cancel has been pressed...
	quit=ProcessMessageQueue();
	// write out data
	for (int loc=0;loc<wind;loc++)
	  {

	    if (calc[loc].Freq()==END)
	      {
		quit=1;
		break;
	      } 
	    // should I write this out?
	    // this is a macro from the main-programm
	    EXECUTE_CODE;
	  }
	// calculate % done
	double Done1=(from-start)*PercentageStep/(to-start);
	if (Done1>PercentageStep) Done1=PercentageStep;
	int Done=(int)(Done1+PercentageStart);
	SetDone(Done);
    }
  while (quit==0);
#endif /* MAINCODE */

#undef SPECTRALWINDOW
#undef USEWEIGHTS
