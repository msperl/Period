/*
 * Program:     Period98
 *
 * File:        matrix.cpp
 * Purpose:     implementation-file for
 *              ?
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */

#define MYABS(x) ((x<0)?-(x):(x))

#include "matrix.h"

//
// CVector
//

CVector::CVector()
:entries(0),data(0)
{;}

CVector::CVector(int size, double fil)
:entries(0),data(0)
{
  allocate(size);
  fill(fil);
}

CVector::CVector(CVector const &vec)
:entries(0),data(0)
{
  allocate(vec.entries);
  for(int i=0;i<vec.entries;i++)
    {
      data[i]=vec.data[i];
    }
}

CVector::~CVector()
{
  deallocate();
}

// allocation/deallocation

void CVector::allocate(int size)
{
  if (size<0)
    {
      size=0;
    }
  deallocate();
  if (size!=0)
    {
      entries=size;
      data=new double[size];
    }
}

void CVector::deallocate()
{
  if(entries!=0)
    {
      delete [] data;
    }
  entries=0;
  data=0;
}

// operators

CVector &CVector::operator=(CVector const &vec)
{
  allocate(vec.entries);
  for(int i=0;i<vec.entries;i++)
    {
      data[i]=vec.data[i];
    }
  return *this;
}

CVector &CVector::operator=(double fil)
{
  for(int i=0;i<entries;i++)
    {
      data[i]=fil;
    }
  return *this;
}

CVector &CVector::operator*=(double factor)
{
  for(int i=0;i<entries;i++)
    {
      data[i]*=factor;
    }
  return *this;
}

CVector &CVector::operator/=(double factor)
{
  for(int i=0;i<entries;i++)
    {
      data[i]/=factor;
    }
  return *this;
}

CVector &CVector::operator+=(CVector const &vec)
{
  //  ASSERT(entries!=vec.entries);
  for(int i=0;i<entries;i++)
    {
      data[i]+=vec.data[i];
    }
  return *this;
}

CVector &CVector::operator+=(double val)
{
  for(int i=0;i<entries;i++)
    {
      data[i]+=val;
    }
  return *this;
}

CVector &CVector::operator-=(CVector const &vec)
{
  //  ASSERT(entries!=vec.entries);
  for(int i=0;i<entries;i++)
    {
      data[i]-=vec.data[i];
    }
  return *this;
}

CVector &CVector::operator-=(double val)
{
  for(int i=0;i<entries;i++)
    {
      data[i]-=val;
    }
  return *this;
}

double operator*(CVector const &v1, CVector const &v2)
{
  //ASSERT(v1.entries!=v2.entries);
  double res=0;
  for (int i=0;i<v1.entries;i++)
    {
      res+=v1.data[i]*v2.data[i];
    }
  return res;
}

void CVector::changesize(int size,double fill)
{
  // ASSERT(size<0);
  int to=((size<entries)?size:entries);

  double *tmp=data;
  data=0;
  entries=0;
  allocate(size);
  int i;
  for(i=0;i<to;i++)
    {
      data[i]=tmp[i];
    }
  
  for(i=to;i<size;i++)
    {
      data[i]=fill;
    }

  delete [] tmp;
}

void CVector::fill(int start,int end,double fil)
{
  // ASSERT(start<0);
  // ASSERT(start>entries);
  // ASSERT(end<start);
  // ASSERT(end>entries);
  for(int i=start;i<end;i++)
    {
      data[i]=fil;
    }
}

void CVector::fill(int start, double fil)
{
  fill(start,entries,fil);
}

void CVector::fill(double fil)
{
  fill(0,entries,fil);
}

// file I/O

ostream& operator<<(ostream &ost, CVector const &vec)
{
  for (int i=0;i<vec.entries;i++)
    {
      ost<<' '<<vec.data[i]<<" ";
    }
  ost<<"\n";
  return ost;
}

istream& operator>>(istream &ist, CVector const &)
{
  // ASSERT(0);
  return ist;
}

istream& CVector::readin(istream &ist, int )
{
  // ASSERT(0);
  return ist;
}

//
// CMatrix
//

CMatrix::CMatrix()
{
  clean();
}

CMatrix::CMatrix(int rows, int cols, double fil)
{
  clean();
  allocate(rows,cols,fil);
}

CMatrix::CMatrix(CMatrix const &mat)
{
  clean();
  *this=mat;
}

CMatrix::~CMatrix()
{
  deallocate();
}

// Memory allocation

void CMatrix::clean()
{
  totrows=0;
  totcols=0;
  Rows=0;
  det=0;
}

void CMatrix::allocate(int rows, int cols,double fil)
{
  deallocate();
  if ((rows<=0)|(cols<=0))
    {
      totrows=0;
      totcols=0;
      Rows=0;
      return;
    }
  Rows=new CVector[rows];
  totrows=rows;
  totcols=cols;
  for(int i=0;i<rows;i++)
    {
      Rows[i].changesize(cols,fil);
    }
}

void CMatrix::changesize(int rows, int cols)
{
  if ((rows<0)||(cols<0))
    {
      deallocate();
      return;
    }
  allocate(rows,cols);
}

void CMatrix::deallocate()
{
  if (Rows!=0)
    {
      delete [] Rows;
    }
  clean();
}

// operators

CMatrix &CMatrix::operator=(CMatrix const &mat)
{
  allocate(mat.totrows,mat.totcols);
  for(int i=0;i<totrows;i++)
    {
      Rows[i]=mat.Rows[i];
    }  
  return *this;
}

CMatrix &CMatrix::operator=(double fil)
{
  for(int i=0;i<totrows;i++)
    {
      Rows[i]=fil;
    }  
  return *this;
}

CMatrix &CMatrix::operator*=(double fac)
{
  for(int i=0;i<totrows;i++)
    {
      Rows[i]*=fac;
    }  
  return *this;
}

CMatrix &CMatrix::operator/=(double fac)
{
  for(int i=0;i<totrows;i++)
    {
      Rows[i]/=fac;
    }  
  return *this;
}

CMatrix &CMatrix::operator+=(double val)
{
  for(int i=0;i<totrows;i++)
    {
      Rows[i]+=val;
    }  
  return *this;
}

CMatrix &CMatrix::operator-=(double val)
{
  for(int i=0;i<totrows;i++)
    {
      Rows[i]-=val;
    }  
  return *this;
}

CMatrix &CMatrix::operator+=(CMatrix const & mat)
{
  for(int i=0;i<totrows;i++)
    {
      Rows[i]+=mat.Rows[i];
    }
  return *this;
}

CMatrix &CMatrix::operator-=(CMatrix const & mat)
{
  for(int i=0;i<totrows;i++)
    {
      Rows[i]-=mat.Rows[i];
    }
  return *this;
}

CMatrix &CMatrix::operator*=(CMatrix const & mat)
{
  *this=(*this)*mat;
  return *this;
}

CMatrix operator*(CMatrix const &mat1, CMatrix const &mat2)
{
  //ASSERT(mat1.cols()!=mat2.rows());
  CMatrix Result(mat1.rows(),mat2.cols());
  for (int i=0;i<Result.rows();i++)
    for (int k=0;k<Result.cols();k++)
      {
	double result=0;
	for(int j=0;j<mat1.rows();j++)
	  {
	    result+=mat1[i][j]*mat2[j][k];
	  }
	Result[i][k]=result;
      }
  return Result;
}

CVector operator*(CVector const &, CMatrix const &)
{
  // ASSERT(0==0);
  return CVector();
}

CVector operator*(CMatrix const &, CVector const &)
{
  // ASSERT(0==0);
  return CVector();
} 

CMatrix &CMatrix::operator/=(CMatrix const & mat)
{
  return (*this)*=!mat;
}

CVector operator/(CVector const &, CMatrix const &)
{
  // ASSERT(0==0);
  return CVector();
}

CVector operator/(CMatrix const &, CVector const &)
{
  // ASSERT(0==0);
  return CVector();
} 

CMatrix CMatrix::operator!() const
{
  CMatrix tmp(*this);
  if (tmp.invert()==0)
    {
      // no inversion done...
      tmp.changesize(0,0);
    }
  return tmp;
}

double CMatrix::invert()
{
  // is matrix square ?
  if (totrows!=totcols)
    {
      return 0;
    }

  // create temporary data
  int *colsubmat=new int[totcols];
  int *rowsubmat=new int[totrows];
  // determinant
  double det=1;

  {
    // highest element in matrix
    double amax;
    // loop variables
    int col,row,submat;
    
    for(submat=0;submat<totcols;submat++)
      {
	amax=0;
	// find largest element in rest of Matrix
	for(row=submat;row<totrows;row++)
	  {
	    // to speed up lookup
	    double *tmp=Rows[row].data;
	    for(col=submat;col<totrows;col++)
	      {
		double tmp1=tmp[col];
		if (MYABS(amax)<=MYABS(tmp1))
		  {
		    amax=tmp1;
		    colsubmat[submat]=col;
		    rowsubmat[submat]=row;
		  }
	      }
	  }

        // is the matrix from [submat,submat] on empty ?
	if (amax==0)
	  {
	    // the matrix from [submat,submat] on is empty (i.e 0)
            // so not invertible
	    MYERROR("Matrix can not be inverted !!!");
            return 0;
	  }

	
	// create temporary amax for speed (multiply is normaly faster then divide !!!)
	
	double invamax=1/amax;
	double ninvamax=-invamax;

	// interchange rows and colunmes to put amax in [submat,submat]
                
        // exchange colums: colsubmat[submat] with submat
        col=colsubmat[submat];
        if (col>submat)
          {
	    exchangecolsint(submat,col);
          }
        
	// exchange rows: rowsubmat[submat] with submat
	row=rowsubmat[submat];
	if (row>submat)
	  {
	    exchangerowsint(submat,row);
	  }
	
	// accumulate elements of inverse matrix
        double *tmpsub=Rows[submat].data;
	for(col=0;col<totcols;col++)
	  {
	    if (col!=submat)
              {
		//Rows[col][submat]*=ninvamax;
		//Rows[submat].data[col]*=ninvamax;
		tmpsub[col]*=ninvamax;
              }
	  }
	for(row=0;row<totrows;row++)
	  {
	    double *tmprow=Rows[row].data;
	    if (row!=submat)
	      {
		for(col=0;col<totcols;col++)
		  {
		    if (col!=submat)
		      {
			//Rows[col][row]+=Rows[col][submat]*Rows[submat][row];
			//Rows[row].data[col]+=
			//  Rows[submat].data[col]*
			//  Rows[row].data[submat];
			tmprow[col]+=
			  tmpsub[col]*
			  tmprow[submat];
		      }
		  }
	      }
	  }
	for(row=0;row<totrows;row++)
	  {
	    if (row!=submat)
	      {
		// Rows[submat][row]*=invamax;
		Rows[row].data[submat]*=invamax;
	      }
	  }
	//Rows[submat].data[submat]=invamax;
	tmpsub[submat]=invamax;
	det*=amax;
      }

    //restore Ordering of Matrix
    for(submat=totcols-1;submat!=-1;submat--)
      {
	// exchange rows to previous state
	row=colsubmat[submat];
	if (row>submat)
	  {
	    exchangerowsint(row,submat);
	  }
	// exchange colums to previous state
	col=rowsubmat[submat];
	if (col>submat)
	  {
	    exchangecolsint(col,submat);
	  }
      }
  }
  
  // delete temporary data
  delete [] colsubmat;
  delete [] rowsubmat;

  // return determinant
  return det;
}

ostream &operator<<(ostream &ost, CMatrix const &mat)
{
  for (int i=0;i<mat.totrows;i++)
    {
      ost<<mat.Rows[i];
    }
  return ost;
}

istream &operator>>(istream & ist, CMatrix const &)
{
  // ASSERT(0);
  return ist;
}

istream &CMatrix::readsize(istream & ist, int, int)
{
  // ASSERT(0);
  return ist;
}








