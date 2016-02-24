/*
 * Program:     Period98
 *
 * File:        dfcalc.cpp
 * Purpose:     implementation-file for
 *              ?
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */
#include "dfcalc.h"

CFourCalc::CFourCalc(int size)
{
  wind=size;
  Array=new CFourCalcData[size];
}

CFourCalc::~CFourCalc()
{
  delete [] Array;
}

void CFourCalc::Power()
{
  for(int i=0;i<wind;i++)
    {
      Array[i].CalcPow();
    }
}

