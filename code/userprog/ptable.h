#ifndef PTABLE_H
#define PTABLE_H

#include "bitmap.h"
#include "pcb.h"
#include "synch.h"

#define MAXPROCESS 10

class PTable 
{
 private:
  BitMap *bm;
  PCB    *pcb[MAXPROCESS];
  int     psize;
  Semaphore *bmsem;
 public:
  PTable(int size);
  ~PTable();
  int ExecUpdate(char *filename);// return PID
  int ExitUpdate(int ec);
  int JoinUpdate(int id);
  int GetFreeSlot();
  bool IsExist(int pid);
  //int GetMax();
  void Remove(int pid);
};

#endif
