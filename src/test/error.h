/*
 * Program:     Period98
 *
 * File:        error.h
 * Purpose:     header-file for
 *              redirecting error-outputs
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */

#ifndef __error_h__
#define __error_h__
#include <stdlib.h>


#ifndef _NDEBUG

#define MYERROR1           cerr<<__FILE__<<":"<<__LINE__<<": "

#define MYERROR(a)         MYERROR1<<a<<endl;
#define MYERROREXIT(a)     MYERROR1<<a<<endl;abort();exit(255);

#else

#define MYERROR(a)
#define MYERROREXIT(a)

#endif


#endif
