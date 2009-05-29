// process control block
#ifndef PCB_H
#define PCB_H

#include "thread.h"
#include "synch.h"

class PCB
{
	private:
		Semaphore* joinsem; //semaphore cho qua trinh joim
		Semaphore* exitsem; //semaphore cho qua trinh exit
		Semaphore* multex;  //semaphore truy xuat doc quyen
		int exitcode;
		int numwait;

		char FileName[32];

		Thread* thread;
		int pid;

	public:
		int parentID;//ID cua tien trinh cha
		//char boolBG;
		PCB();
		PCB(int id);//contructor
		~PCB();//deconstructor

		// nap tien trinh con
		int Exec(char *filename,int pid);
		int GetID();
		int GetNumWait();

		void JoinWait(); //tien trinh cha doi tien trinh con ket thuc
		void ExitWait(); //la tien trinh con xin tien trinh cha ket thuc

		void JoinRelease(); //signal bao cho tien trinh cha thuc thi tiep
		void ExitRelease(); //cho phep tien trinh con ket thuc

		void IncNumWait();
		void DecNumWait();

		void SetExitCode(int ec);
		int  GetExitCode();

		void SetFileName(char* filename);
		char* GetFileName();

};

#endif

