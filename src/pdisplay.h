/*
 * Program:     Period98
 *
 * File:        project.h
 * Purpose:     header-file for
 *              the general parts of the project
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */

  /// DisplayManager
  void UpdateAllData();
  void UpdateDataID(int ID);
  void UpdateTSData() { UpdateDataID(DISPLAY_DIALOG_TIMESTRING); }
  void UpdatePEData() { UpdateDataID(DISPLAY_DIALOG_PERIOD); }

  void UpdateAllDisplays();
  void UpdateTSDisplays();
  void UpdatePEDisplays();
  void UpdateDisplayID(int ID);

  void CloseDisplayID(int ID);

  void CloseAllDisplays();

  int FindDisplay(CMyDisplayP disp);

  void RegisterDisplay(CMyDisplayP disp);
  void DeregisterDisplay(CMyDisplayP disp);

