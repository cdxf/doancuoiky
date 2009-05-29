#include "schandle.h"
#include "fdtable.h"

// To do
////////////////////////////////////////////////////////////////////////////////////
/*
------------------------------------------------------------------
Input: - User space address (int)
       - Limit of buffer (int)
Output:- Buffer (char*)
Purpose: Copy buffer from User memory space to System memory space
------------------------------------------------------------------
*/

char* User2System(int virtAddr,int limit)
{
    int i;// index
    int oneChar;
    char* kernelBuf = NULL;

    kernelBuf = new char[limit +1];//need one character for terminal string
    if (kernelBuf == NULL)
    {
       return kernelBuf;
    }

    memset(kernelBuf,0,limit+1);

    //printf("\n Filename u2s:");
    for (i = 0 ; i < limit ;i++)
    {
      machine->ReadMem(virtAddr+i, 1, &oneChar);
      kernelBuf[i] = (char)oneChar;
      //printf("%c",kernelBuf[i]);
      if (oneChar == '\0')
      {
	        break;
      }
    }

    return kernelBuf;
}

/*
-----------------------------------------------------------------
Input: - User space address (int)
       - Limit of buffer (int)
       - Buffer (char[])
Output:- Number of bytes copied (int)
Purpose: Copy buffer from System memory space to User  memory space
-----------------------------------------------------------------
*/
int   System2User(int virtAddr,int len,char* buffer)
{
  	if (len < 0) 
		return -1;
  	if (len == 0) 
		return len;
  	int i = 0;
  	int oneChar = 0 ;
  	do
	{
    		oneChar= (int) buffer[i];
    		machine->WriteMem(virtAddr+i,1,oneChar);
    		i ++;
  	}while(i < len && oneChar != 0);

  	return i;
}

/*
-----------------------------------------------------------
intput : name : Tao ra mot file
ouput  : Ket qua
     0 : Neu Thanh cong
     -1: Neu Bi Loi
-----------------------------------------------------------
*/
int  doSC_CreateFile()
{
	int virtAddr;
	char *filename;

	DEBUG(dbgFile,"\n SC_Create call .");
	DEBUG(dbgFile,"\n Reading virtual address of filename .");

	// check for exception
	virtAddr = machine->ReadRegister(4);
	DEBUG(dbgFile,"\n Reading filename \n");

	filename = User2System(virtAddr, MaxFileLength + 1);

	if(filename == NULL)
	{
		printf("\n Not enough memory in system \n");
		DEBUG(dbgFile,"\n Not enough memory in system\n ");
		machine->WriteRegister(2, -1); // Tra ve Loi cho User
		delete filename;
		return -1;
	}

	DEBUG(dbgFile,"\n Finish reading filename.\n");

	if(!fileSystem->Create(filename, 0))
	{
		printf("\n Error Create file : %s", filename);
		machine->WriteRegister(2, -1); // Tra ve cho chuong trinh User bi Loi
		delete filename;
		return -1;
	}

	machine->WriteRegister(2, 0);
	delete filename;
	return 0;
}

/*
------------------------------------------------------------
intput : name : Open file name
output :   rs : ID of file opened
           -1 : Neu Mo bi Loi
-----------------------------------------------------------
*/
int doSC_Open()
{
	int virtAddr = machine->ReadRegister(4);
	int type     = machine->ReadRegister(5);

	if( type < 0 || type > 2)
	{
		printf("\n SC_OpenError : unexpexted file type : %d",type);
		return -1;
	}

	int id = currentThread->fTab->FindFreeSlot();
	if(id < 0 )
	{
		printf("\n SC_OpenError : Not Free Slot.");
		return -1;
	}

	char *filename = User2System(virtAddr, MaxFileLength + 1);
	if( filename == NULL )
	{
		printf("\n Not enough Memory in System.");
		machine->WriteRegister(2, -1);
		delete filename;
		return -1;
	}

	if(strlen(filename) == 0 || strlen(filename) >= MaxFileLength + 1)
	{
		printf("\n Too many characters in filename : %s ",filename);
		machine->WriteRegister(2, -1);
		delete filename;
		return -1;
	}

	OpenFile *of = fileSystem->Open(filename);
	if(of == NULL)
	{
		printf("Not Open file : %s ",filename);
		machine->WriteRegister(2, -1);
		delete filename;
		return -1;
	}

	int rs = currentThread->fTab->fdOpen(virtAddr, type, id, of);
	machine->WriteRegister(2, rs);
	return rs;
}

/*
----------------------------------------------
Input   : OpenfileID = reg4
Output  : 0- success , -1 - fail
Purpose : close file
----------------------------------------------
*/
int doSC_Close()
{
  int id = machine->ReadRegister(4);
  if (id < 0 || id >= currentThread->fTab->GetMax())
    {
      printf("\n CloseError: Unexpected file id: %d",id);
      return -1;
    }
  if (!currentThread->fTab->IsExist(id)){
    printf("\n CloseError: closing file id %d is not opened",id);
    return -1;
  }

  currentThread->fTab->fdClose(id);
  return 0;
}

/*
------------------------------------------------------------------
Input: offset = reg4, OpenFileID = reg5
Output: currentOffset, or -1 - fail
Purpose: do seek the pointer of a file. If seek to offset -1 mean SEEK_END
------------------------------------------------------------------
*/
int doSC_Seek()
{
  int id = machine->ReadRegister(5);
  if (id < 0 || id >= currentThread->fTab->GetMax())
    {
      printf("\n SC_SeekError: Unexpected file id: %d",id);
      return -1;
    }
  if (!currentThread->fTab->IsExist(id)){
    printf("\n SC_SeekError: seeking file id %d is not opened",id);
    return -1;
  }

  int offset = machine->ReadRegister(4);
  currentThread->fTab->fdSeek(offset,id);

  return 0;
}

/*
-----------------------------------------------------------------------
Input   : User space address = reg4, buffer size = reg5, OpenfileID = reg6
Output  :               -1: error
        numbytes were read: success
Purpose : do read from file or console
-----------------------------------------------------------------------
*/
int doSC_Read()
{
 //  printf("\n Calling SC_Read.");
  int virtAddr = machine->ReadRegister(4);

  int size = machine->ReadRegister(5);
  int id = machine->ReadRegister(6);
  //char* name = User2System(virtAddr,size);

  if (size <= 0)
    {
      printf("\nSC_ReadError: unexpected buffer size: %d",size);
      return -1;
    }

  if (id < 0 || id >= currentThread->fTab->GetMax())
    {
      printf("\n ReadError: Unexpected file id: %d",id);
      return -1;
    }
  if (!currentThread->fTab->IsExist(id)){
    printf("\n ReadError: reading file id %d is not opened",id);
    return -1;
  }

  int rs = currentThread->fTab->fdRead(virtAddr,size,id);

  machine->WriteRegister(2,rs);

  return rs;   
}

/*
--------------------------------------------------------------------------
Input: User space address = reg4, buffer size= reg5, OpenFileID = reg6
Output: = -1 - error
        or = numbytes were writen
Purpose: do write to file or console
--------------------------------------------------------------------------
*/
int doSC_Write()
{
  //  printf("\n Calling SC_Write.");
  int virtAddr = machine->ReadRegister(4);
  int size = machine->ReadRegister(5);
  int id = machine->ReadRegister(6);
  //char* name = User2System(virtAddr,size);

  if (size < 0)
    {
      printf("\nSC_WriteError: unexpected buffer size: %d",size);
      return -1;
    }
  else if (size == 0)
    return 0;

  if (id < 0 || id >= currentThread->fTab->GetMax())
    {
      printf("\n WriteError: Unexpected file id: %d",id);
      return -1;
    }
  if (!currentThread->fTab->IsExist(id)){
    printf("\n WriteError: writing file id %d is not opened",id);
    return -1;
  }

  int rs = currentThread->fTab->fdWrite(virtAddr,size,id);

  machine->WriteRegister(2,rs);

  return rs;
}

/*
------------------------------------------------------------
purpose : Thuc thi 1 chuong trinh moi trong system thread
input :  name : ten cua file chuong trinh
output :       -1 : error
          SpaceID : success
------------------------------------------------------------
*/
void doSC_Exec()
{
	int virtAdd = machine->ReadRegister(4);
	char *FileName = User2System(virtAdd, MaxFileLength + 1);
	OpenFile *ob = fileSystem->Open(FileName);

	if (ob == NULL)
	{
		printf("Khong mo duoc file %s\n", FileName);
		machine->WriteRegister(2, -1);
		return;
	}

	delete ob;

	int pID = pTab->ExecUpdate(FileName);
	machine->WriteRegister(2, pID);
}

/*
------------------------------------------------------------
int Join(SpaceID id)
input : id cua tien trinh con can join
output :  fail : -1
		   success : exit code cua tien trinh no block
------------------------------------------------------------
*/
void doSC_Join()
{
    //debug
    //printf("Entering syscall join.\n");
    int pID = machine->ReadRegister(4);

    int ExitCode = pTab->JoinUpdate(pID);

    //tra ve exit code
    machine->WriteRegister(2, ExitCode);
}

/*
------------------------------------------------------------
void Exit(int exitCode)
purpose: exit parent process
intput : exitcode = 0 : thread run successfully
		    exitcode != 0 : thread not finish
------------------------------------------------------------
*/
void doSC_Exit()
{

//	printf("\n\n Calling SC_Exit.");
	DEBUG(dbgFile, "\n\n Calling SC_Exit.");

  	int exitStatus = machine->ReadRegister(4);
  	//int pid = currentThread->processID; //pid la cua tien trinh can thoat

  	// if process exited with error, print error
	if (exitStatus != 0)
    		printf("\nProcess %s exited with error level %d",currentThread->getName(),exitStatus);

	pTab->ExitUpdate(exitStatus);

	currentThread->FreeSpace();
	currentThread->Finish();
}

/*
------------------------------------------------------------
Create semaphore
input : name : semaphore name
	    semval : start value
output :   0 : success
	        -1 : fail
------------------------------------------------------------
*/
void doSC_CreateSemaphore()
{
  int addr = machine->ReadRegister(4);
	int semval = machine->ReadRegister(5);
	char *name = User2System(addr,32);
	if(name == NULL)
	{
		printf("\nSC_CreateSemaphore :: name = NULL");
		machine->WriteRegister(2,-1);
		return;
	}
	int rs = sTab->Create(name,semval);
	if(rs < 0)
	{
		printf("\nSC_CreateSemaphore :: Can't create semaphore : %s",name);
		machine->WriteRegister(2,-1);
		return;
	}
	machine->WriteRegister(2,0);	
}

/*
--------------------------------------------------------
input : name : ten cua semaphore can wait
output :   0 : thanh cong
          -1 : loi
--------------------------------------------------------
*/
void doSC_Wait()
{
  int addr = machine->ReadRegister(4);
  char *name = User2System(addr,32);
	if(name == NULL)
	{
		printf("\nSC_Wait :: name = NULL");
		machine->WriteRegister(2,-1);
		return;
	}
	int kq = sTab->Wait(name);
	if(kq < 0)
	{
		printf("\nSC_Wait :: Can't wait semaphore : %s",name);
		machine->WriteRegister(2,-1);
		return;
	}
	machine->WriteRegister(2,0);
}

/*
---------------------------------------------------------
input : name : ten cua semaphore can signal
output : 0 : thanh cong
	      -1 : loi
---------------------------------------------------------
*/  
void doSC_Signal()
{
  int addr = machine->ReadRegister(4);
	char *name = User2System(addr,32);
	if(name == NULL)
	{
		printf("\nSC_Signal :: name = NULL");
		machine->WriteRegister(2,-1);
		return;
	}
	int kq = sTab->Signal(name);
	if(kq < 0)
	{
		printf("\nSC_Signal :: Can't signal semaphore : %s",name);
		machine->WriteRegister(2,-1);
		return;
	}
	machine->WriteRegister(2,0);
}
