/*
 * Program:     Period98
 *
 * File:        dgtimein.h
 * Purpose:     header-file for
 *              ?
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */
//
//
// this header defines a simple function,
// which should keep X aborting the programm
// when some Messages are not responded to
// this should return nonzero for abort 

int ProcessMessageQueue();

// this function should be defined with the main
//@ManMemo: application for wxWindows...

void InformUserOfProgress(char * text);

// this function informs the user about the status

int AskUserToContinue(char * text);
