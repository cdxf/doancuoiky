#include "schandle.h"

// To do
////////////////////////////////////////////////////////////////////////////////////
/*
Input: - User space address (int)
       - Limit of buffer (int)
Output:- Buffer (char*)
Purpose: Copy buffer from User memory space to System memory space
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
Input: - User space address (int)
       - Limit of buffer (int)
       - Buffer (char[])
Output:- Number of bytes copied (int)
Purpose: Copy buffer from System memory space to User  memory space
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
}

/*
------------------------------------------------------------
Tao semaphore
input : name : ten cua bien semaphore
	    semval : gia tri ban dau
output :   0 : thanh cong
	        -1 : loi
------------------------------------------------------------
*/
void doSC_CreateSemaphore()
{ 	
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
}
