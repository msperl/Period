/*
 * Program:     Period98
 *
 * File:        matrix.h
 * Purpose:     header-file for
 *              matrix-class
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */

#include <iostream.h>
#include "error.h"

class CMatrix;

///
class CVector
{
  ///
  friend class CMatrix;
  
protected:
  //@ManMemo: number of entries
  int entries;
  //@ManMemo: data entries
  double * data;

public:
  //@ManMemo: constructors/destructors

  CVector();
  ///
  CVector(int size, double fill=0);
  ///
  CVector(CVector const & vec);

  ///
  ~CVector();

  //@ManMemo: allocate/deallocate
protected:
  void allocate(int size);
  ///
  void deallocate();

public:
  //@ManMemo: operators

  CVector &operator=(CVector const &tmp);
  ///
  CVector &operator=(double fil);
    
  ///
  CVector &operator*=(double factor);
  ///
  CVector &operator/=(double factor);

  ///
  CVector &operator+=(CVector const & vec);
  ///
  CVector &operator+=(double val);

  ///
  CVector &operator-=(CVector const & vec);
  ///
  CVector &operator-=(double val);

  ///
  friend double operator*(CVector const &v1, CVector const &v2);

  ///
  inline double &operator[](int i) const
    {
      // ASSERT(i>entries);
      // ASSERT(i<0);
      return data[i];
    }

  //@ManMemo: size of vector
  inline int size()
    {return entries;}
  ///
  void changesize(int size, double fill=0);

  //@ManMemo: filling option
    
  void fill(double fil=0);
  ///
  void fill(int start,double fil=0);
  ///
  void fill(int start,int end,double fil=0);

  //@ManMemo: file i/o

  friend ostream& operator<<(ostream &ost, CVector const &vec);
  ///
  friend istream& operator>>(istream &ist, CVector const &vec);

  ///
  istream& readin(istream &ist, int size);

};

// other operators

#define SIMPLEOPS(cmd) {CVector tmp(vec);cmd;return tmp;}

///
inline CVector operator*(CVector const &vec,double factor)
  SIMPLEOPS(tmp*=factor)
inline CVector operator*(double factor,CVector const &vec)
  SIMPLEOPS(tmp*=factor)

inline CVector operator/(CVector const &vec,double factor)
  SIMPLEOPS(tmp/=factor)
inline CVector operator/(double factor,CVector const &vec)
  SIMPLEOPS(tmp/=factor)

inline CVector operator+(CVector const &vec,double factor)
  SIMPLEOPS(tmp+=factor)
inline CVector operator+(double factor,CVector const &vec)
  SIMPLEOPS(tmp+=factor)
inline CVector operator+(CVector const & vec,CVector const &vec1)
  SIMPLEOPS(tmp+=vec1)

inline CVector operator-(CVector const &vec,double factor)
  SIMPLEOPS(tmp-=factor)
inline CVector operator-(double factor,CVector const &vec)
  SIMPLEOPS(tmp-=factor)
inline CVector operator-(CVector const & vec,CVector const &vec1)
  SIMPLEOPS(tmp-=vec1)

//
// CMatrix
//

///     
class CMatrix
{
public:

  // constructors/destructors

  CMatrix();
  CMatrix(int rows, int cols, double fill=0);
  CMatrix(CMatrix const &mat);
  
  ~CMatrix();

  // memory allocoation functions

protected:

  void clean();// cleans constants (for constructors only !!!)
  void allocate(int rows,int cols, double fil=0);
  void deallocate();

public:

  // operators

  CMatrix &operator=(CMatrix const& mat);
  CMatrix &operator=(double fil);

  CMatrix &operator*=(double fac);
  CMatrix &operator/=(double fac);

  CMatrix &operator+=(double val);
  CMatrix &operator-=(double val);

  CMatrix &operator*=(CMatrix const &mat);
  friend CMatrix operator*(CMatrix const &mat1, CMatrix const &mat2);
  friend CVector operator*(CVector const &vec1, CMatrix const &mat2);
  friend CVector operator*(CMatrix const &mat1, CVector const &vec2);
  
  CMatrix &operator/=(CMatrix const &mat);
  friend CMatrix operator/(CMatrix const &mat1, CMatrix const &mat2);
  friend CVector operator/(CVector const &vec1, CMatrix const &mat2);
  friend CVector operator/(CMatrix const &mat1, CVector const &vec2);
  
  CMatrix &operator+=(CMatrix const &mat);
  CMatrix &operator-=(CMatrix const &mat);

  // mirror Matrix

  void mirror()
  {
    if (totrows!=totcols)
      {
	MYERROREXIT("Cannot mirror this Matrix - wrong dimensions!!!");
      }
    for(int j=0;j<totrows;j++)
      {
	for(int k=0;k<j;k++)
	  {
	    Rows[k].data[j]=Rows[j].data[k];
	  }
      }

  }
  // invert
  
  CMatrix operator!() const;
  double invert();

  // swap rows and columes
  inline void exchangerows(int from, int to)
  {
    // ASSERT(from<0);ASSERT(from>=totrows);ASSERT(to<0);ASSERT(to>=totrows);
    exchangerowsint(from, to);
  }

  inline void exchangecols(int from, int to)
  {
    // ASSERT(from<0);ASSERT(from>=totcols);ASSERT(to<0);ASSERT(to>=totcols);
    exchangecolsint(from, to);
  }
protected:
  inline void exchangerowsint(int from, int to)
  {
    double tmp;
    double *fr=Rows[from].data;
    double *de=Rows[to].data;
    for(int col=0;col<totcols;col++)
      {
	tmp=fr[col];
	fr[col]=de[col];
	de[col]=-tmp;
      }
  }
  
  inline void exchangecolsint(int from, int to)
  {
    double tmp;
    double *tmprow;
    for(int row=0;row<totrows;row++)
      {
	tmprow=Rows[row].data;
	tmp=tmprow[from];
	tmprow[from]=tmprow[to];
	tmprow[to]=-tmp;
      }
  }
  
 public:
  // size of the matrix

  inline int rows() const
    { return totrows; }
  inline int cols() const
    { return totcols; }
  void changesize(int row,int col);

  // adress operators

  inline CVector &operator[](int i) const
  {
    // ASSERT(i<0);ASSERT(i>=totrows);
    return Rows[i];
  }
  
  // file I/O

  friend ostream &operator<<(ostream &ost, CMatrix const &mat);
  friend istream &operator>>(istream &ist, CMatrix const &mat);

  istream &readsize(istream &ist, int rows, int cols);

protected:

  int totrows;
  int totcols;
  CVector * Rows;
  double det;

};
