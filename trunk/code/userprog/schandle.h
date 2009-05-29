#ifndef SCHANDLE_H
#define SCHADLE_H

#include "system.h"
//#include "machine.h"
#include "../userprog/syscall.h"

char* User2System(int virtAddr,int maxfilelen);
int   System2User(int virtAddr,int len,char* buffer);

int doSC_Open();
int doSC_Close();
int doSC_CreateFile();
int doSC_Read();
int doSC_Write();
int doSC_Seek();

void doSC_Exit();
void doSC_Exec();
void doSC_Join();
void doSC_CreateSemaphore();
void doSC_Wait();
void doSC_Signal();

#endif
