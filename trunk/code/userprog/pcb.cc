// Cai dat lop PCB
#include "pcb.h"
#include "utility.h"
#include "system.h"
#include "thread.h"
#include "addrspace.h"

extern void StartProcess_ID(int pID);

//constuctor
PCB::PCB()
{
	this->pid = 0;
	this->parentID = -1;
	this->numwait = 0;
	this->exitcode = 0;
	this->thread = NULL;
	//this->boolBG = 0;
	this->joinsem = new Semaphore("joinsem",0);
	this->exitsem = new Semaphore("exitsem",0);
	this->multex = new Semaphore("multex",1);
}

PCB::PCB(int id)
{
	this->parentID = currentThread->processID;
	this->pid = id;
	this->numwait = 0;
	this->exitcode = 0;
	this->thread = NULL;
	//this->boolBG = 0;
	this->joinsem = new Semaphore("joinsem",0);
	this->exitsem = new Semaphore("exitsem",0);
	this->multex = new Semaphore("multex",1);
}

//deconstructor
PCB::~PCB()
{
	if(joinsem != NULL)
		delete this->joinsem;
	if(exitsem != NULL)
		delete this->exitsem;
	if(multex != NULL)
		delete this->multex;
	if(thread != NULL)
	{
		//printf("\n~PCB : free and finish");
		thread->Finish();
	}
	  // (void) interrupt->SetLevel(oldLevel);
}

//////////////////////////////////////////////////////////////////////////

int PCB::GetID() {
	return this->pid;
}

int PCB::GetNumWait()
{
	return this->numwait;
}

int PCB::GetExitCode()
{
	return this->exitcode;
}

void PCB::SetExitCode(int ec)
{
	this->exitcode = ec;
}

//////////////////////////////////////////////////////////////////////////

void PCB::JoinWait()
{
	joinsem->P();
}

void PCB::JoinRelease()
{
	joinsem->V();
}

//////////////////////////////////////////////////////////////////////////

void PCB::ExitWait()
{
	exitsem->P();
}

void PCB::ExitRelease()
{
	exitsem->V();
}

//////////////////////////////////////////////////////////////////////////

void PCB::IncNumWait()
{
	multex->P();
	++numwait;
	multex->V();
}

void PCB::DecNumWait()
{
	multex->P();
	if(numwait > 0)
		--numwait;
	multex->V();
}

//////////////////////////////////////////////////////////////////////////

void PCB::SetFileName(char* fn)
{
	strcpy(FileName,fn);
}

char* PCB::GetFileName()
{
	return FileName;
}

//////////////////////////////////////////////////////////////////////////

//nap chuong trinh co ten luu trong bien file name va ProcessID la id
int PCB::Exec(char* filename, int id)
{
	multex->P();

	//this->pid = id;
	this->thread = new Thread(filename);
	if(this->thread == NULL)
	{
		printf("\nPCB::Exec : Khong the tao 1 mot tien trinh ");
		return -1;
	}

	this->thread->processID = id;
	this->parentID = currentThread->processID;

	this->thread->Fork(StartProcess_ID, id);
	this->SetFileName(filename);

	multex->V();

	return id;

}

