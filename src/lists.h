/*
 * Program:     Period98
 *
 * File:        lists.h
 * Purpose:     header-file for
 *              list-templates
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */

#ifndef __lists_h__
#define __lists_h__

#include <iostream.h>
#include <stdlib.h>
#include "error.h"

//
// here go the Data Methodes
//

//@ManMemo: single Linked Data item

template <class type> 

class TLinked
{
public:
  //@ManMemo: these are the variables
  TLinked* Next;
  ///
  type Data;

public:
  //@ManMemo: here goes the implementation
  TLinked(type const & dta, TLinked* nxt=0)
    :Next(nxt),Data(dta)
      { ; }
  ///
  ~TLinked()
    {
      if (Next!=0)
	{
	  delete Next;
	}
      Next=0;
    }
};

//@ManMemo: Double linked data item

template <class type>

class TDLinked
{
public:
  //@ManMemo: these are the variables
  TDLinked* Next;
  ///
  TDLinked* Prev;
  ///
  type Data;
  
public:
  //@ManMemo: here goes the implementation
  TDLinked(type const & dta, TDLinked* nxt=0, TDLinked* prv=0)
    :Next(nxt),Prev(prv),Data(dta)
      {
	if (Next!=0)
	  {
	    Next->Prev=this;
	  }
      }

  ///
  ~TDLinked()
    {
      // delete all the things befor
	if (Prev!=0)
	  {
	    Prev->Next=0;
	    delete Prev;
	    Prev=0;
	  }
      // delete all the things after
	if (Next!=0)
	  {
	    Next->Prev=0;
	    delete Next;
	    Next=0;
	  }
    }
};

//
// here go the Structures
//

//@ManMemo: here goes the Pipe

template <class type>

class TPipe
{
protected:
  //@ManMemo: these are the variables
  TDLinked<type>* Head;
  ///
  TDLinked<type>* Tail;
  ///
  int Count;
  
public:
  //@ManMemo: here goes the implementation
  TPipe()
    :Head(0),Tail(0),Count(0)
      { ; }

  ///
  ~TPipe()
    {  
      if (Head!=0)
	{
	  delete Head;
	}
      Head=0;
      Tail=0;
      Count=0;
    }

  ///
  void in(type dta)
    {
      Tail=new TDLinked<type>(dta,Tail);
      if (Head==0)
	{
	  Head=Tail;
	}
      Count++;
    }

  ///
  type out()
    {
      TDLinked<type>* tmp=Head;
      if (tmp==0)
	{
	  MYERROREXIT("Pipe:no Data entries when executing out()\n");
	}
      Head=tmp->Prev;
      if (Head!=0)
	{
	  Head->Next=0;
	}
      else
	{
	  Tail=0;
	}
      tmp->Prev=0;
      type tmpl (tmp->Data);
      delete tmp;
      Count--;
      return tmpl;
    }

  ///
  type const & peekhead() const
    {
      if (Head==0)
	{
	  MYERROREXIT("Pipe:no Data entries when executing peekhead()\n");
	}
      return (Head->Data);
    }

  ///
  type const & peektail() const
    {
      if (Tail==0)
	{
	  MYERROREXIT("Pipe:no Data entries when executing peektail()\n");
	}
      return (Tail->Data);
    }
  
  ///
  inline int entries() const
    {
      return Count;
    }
};

//@ManMemo: here goes the stack

template <class type>

class TStack
{
protected:
  //@ManMemo: these are the variables
  TLinked<type>* Top;
  ///
  int Count;
  
public:
  //@ManMemo: here goes the implementation
  TStack()
    :Top(0),Count(0)
      {;}
  ///
  ~TStack()
    {
      if (Top!=0)
	{
	  delete Top;
	}
      Top=0;
      Count=0;
    }

  ///
  void push(type const &dta)
    {
      Top=new TLinked<type>(dta,Top);
      Count++;
    }
  
  ///
  type pop()
    {
      TLinked<type>* tmp=Top;
      if (tmp==0)
	{
	  MYERROREXIT("Stack:no Data entries when executing pop()\n");
	}
      Top=tmp->Next;
      tmp->Next=0;
      type tmpl(tmp->Data);
      delete tmp;
      Count--;
      return tmpl;
    }

  ///
  type const & peek() const
    {
      if (Top==0)
	{
	  MYERROREXIT("Stack:no Data entries when executing peek()\n");
	}
      return (Top->Data);
    }
  
///
inline int entries() const
  {
    return Count;
  }
};

//@ManMemo: simple list

template <class type>

class TList
{
protected:
  //@ManMemo: these are the variables
  TLinked<type>* Head;
  ///
  TLinked<type>* Tail;
  ///
  TLinked<type>* iter;
  ///
  int Count;
    
  public:
  ///
  TList()
    :Head(0),Tail(0),iter(0),Count(0)
      {; }
  
  ///
  virtual ~TList()
    { clean(); }

  ///
  virtual void clean()
    {
      while (Head!=0)
	{
	  removehead();
	}
      Head=0;
      Tail=0;
      iter=0;
      Count=0;
    }

  ///
  inline int entries() const
  {
    return Count;
  }

  ///
  void addhead(type const &D)
    {
      Head=new TLinked<type>(D,Head);
      if (Tail==0)
	{
	  Tail=Head;
	}
      Count++;
    }
  
  ///
  void removehead()
    {
      if (Head==0)
	{
	  MYERROREXIT("List: nothing in the list when calling removehead()\n");
	}
      TLinked<type>* tmp=Head;
      Head=Head->Next;
      tmp->Next=0;
      delete tmp;
      if ((--Count)==0)
	{
	  Tail=0;
	}
    }

  ///
  void addtail(type const &D)
    {
      TLinked<type>*tmp=new TLinked<type>(D);
      if (Count==0)
	{
	  Head=tmp;
	  Tail=tmp;
	  Count++;
	}
      else
	{
	  Tail->Next=tmp;
	  Tail=tmp;
	  Count++;
	}
    }

  ///
  void removetail()
    {
      if (Head==0)
	{
	  MYERROREXIT("List: nothing in the list when calling removetail()\n");
	}
      TLinked<type>* tmp=Head;
	// find last but one !!!
	while(tmp->Next!=Tail)
	  {
	    tmp=tmp->Next;
	  }
      tmp->Next=0;
      delete Tail;
      Tail=tmp;
      Count--;
    }

  ///
  TLinked<type>* entry(int in) const
    {
      if ((in>Count)||(in<0))
	{
	  MYERROREXIT("List: "<<in<<" is not in range of:[O,"
		    <<Count<<"] when calling entry()\n");
	}
      TLinked<type>* tmp=Head;
      int i;
      for(i=0;i<in;i++)
	{
	  tmp=tmp->Next;
	}
      return tmp;
    }

  ///
  void addat(int i, type const & D)
    {
      if (i==0)
	{
	  addhead(D);
	  return;
	}
      if (i==Count)
	{
	  addtail(D);
	  return;
	}
      if ((i>Count)||(i<0))
	{
	  MYERROREXIT("List: "<<i<<" is not in range of:[O,"
		    <<Count<<"] when calling addat()\n");
	}
      TLinked<type>* tmp= entry(i-1);
      addafterptr(tmp,D);
    }

protected:
  ///
  void addafterptr(TLinked<type>* tmp,type const &D)
    {
      tmp->Next=new TLinked<type>(D, tmp->Next);
      Count++;
    }

public:
  ///
  void removeat(int i)
    {
      if ((i>Count)||(i<0))
	{
	  MYERROREXIT("List: "<<i<<" is not in range of:[O,"
		    <<Count<<"] when calling removeat()\n");
	}
      if (i==0)
	{
	  removehead();
	  return;
	}
      if (i==Count-1)
	{
	  removetail();
	  return;
	}
      TLinked<type>* last=entry(i-1);
      TLinked<type>* tmp=last->Next;
      last->Next=tmp->Next;
      tmp->Next=0;
      delete tmp;
      Count--;
    }

  ///
  type& operator[](int i) const
    {
      return (entry(i)->Data);
    }

  ///
  void startiterate(int i=0)
    {
      iter=entry(i);
    }

  ///
  type *iterate()
    {
      type *tmp=0;
      if (iter!=0)
	{
	  tmp=&iter->Data;
	}
      iter=iter->Next;
      return tmp;
    }
};

///
template <class type>

class TSortUpList: public TList<type>
{
protected:
  TLinked<type>* lastadded;
public:
  ///
  TSortUpList()
    :lastadded(0)
    {; }

  ///
  virtual ~TSortUpList()
    { clean(); }

  virtual void clean()
    {
      lastadded=0;
      TList<type>::clean();
    }

protected:
  
  ///
  void addat(int i, type const & d)
    {
      TList<type>::addat(i,d);
    }
  ///
  void addafterptr(TLinked<type>* tmp, type const & d)
    {
      TList<type>::addafterptr(tmp,d);
      lastadded=tmp->Next;
    }
  
  ///
  void addtail(type const & d)
    {
      TList<type>::addtail(d);
      lastadded=TList<type>::Tail;
    }
  
  ///
  void addhead(type const & d)
    {
      TList<type>::addhead(d);
      lastadded=TList<type>::Head;
    }
  
public:
  ///
  void add(type const &d)
    {
      if (TList<type>::Count==0)
	{
	  addhead(d);
	  return;
	}
      if (TList<type>::Tail->Data<=d)
	{
	  addtail(d);
	  return;
	}
      if (d<TList<type>::Head->Data)
	{
	  addhead(d);
	  return;
	}
      TLinked<type>*tmp=TList<type>::Head;
      if (lastadded!=0)
	if (lastadded->Data<=d)
	{
	  tmp=lastadded;
	}
      TLinked<type>*last=tmp;
      while (tmp->Data<=d)
        {
	  last=tmp;
          tmp=tmp->Next;
          if (tmp==0)
            {
              addtail(d);
              return;
            }
        }
      addafterptr(last,d);
    }
};

#endif













