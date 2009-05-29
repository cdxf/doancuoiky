//stable.h
#ifndef STABLE_H
#define STABLE_H

#include "synch.h"
#include "bitmap.h"

#define MAX_SEMAPHORE 10

class Sem
{
	private:
		char name[50];
		Semaphore *sem;
	public :
		Sem(char* na,int i)
		{
			strcpy(this->name,na);
			sem = new Semaphore(name,i);
		}

		~Sem()
		{
			delete sem;
		}

		void wait()
		{
			sem->P();
		}

		void signal()
		{
			sem->V();
		}

		char* GetName()
		{
			return name;
		}
		
		

};

/////////////////////////////////////////////////////////////

class STable
{
	private:
		BitMap* bm;
		Sem* semTab[MAX_SEMAPHORE];
	public:
		STable();
		~STable();
		int Create(char* name,int init);
		int Wait(char* name);
		int Signal(char* name);
		int FindFreeSlot(int id);
};

#endif 
