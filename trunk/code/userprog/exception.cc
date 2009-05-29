// exception.cc 
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.  
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"
#include "schandle.h"

//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2. 
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//	"which" is the kind of exception.  The list of possible exceptions 
//	are in machine.h.
//----------------------------------------------------------------------

void
ExceptionHandler(ExceptionType which)
{
   	int type = machine->ReadRegister(2);
	switch(which)
	{
	case NoException: printf("NoException\n");
		return;
	case PageFaultException: printf("PageFaultException\n");
		break;
	case ReadOnlyException: printf("ReadOnlyException\n");
		break;
	case BusErrorException: printf("BusErrorException\n");
		break;
	case AddressErrorException: printf("AddressErrorException\n");
		break;
	case OverflowException: printf("OverflowException\n");
		break;
	case IllegalInstrException: printf("IllegalInstrException\n");
		break;
	case NumExceptionTypes: printf("NumExceptionTypes\n");
		break;
	case SyscallException:
		switch(type)
		{
		case SC_Halt:
			DEBUG('a', "Shutdown, initiated by user program.\n");
   			interrupt->Halt();
      		
    case SC_Open:
      DEBUG('f', "Open a file.\n");
	    doSC_Open();
	    break;

    case SC_Close:
      DEBUG('f', "Close the file.\n");
	    doSC_Close();
	    break;

    case SC_Read:
      DEBUG('f', "Read from file or console input.\n");
	    doSC_Read();
	    break;

    case SC_Write:
      DEBUG('f', "Write to memory or console output.\n");
	    doSC_Write();
	    break;

    case SC_CreateFile:
      DEBUG('f', "Create a new file.\n");
	    doSC_CreateFile();
	    break;
    
    case SC_Exec:
			DEBUG('a', "Execute program.\n");
			doSC_Exec();
      machine->WriteRegister(PCReg, machine->ReadRegister(PCReg) + 4);
			break;
    case SC_Join:
      DEBUG('a', "Join to parent process.\n");
      doSC_Join();
      machine->WriteRegister(PCReg, machine->ReadRegister(PCReg) + 4);
			break;

    case SC_Exit:
	    DEBUG('a', "\n SC_Exit, initiated by user program.!");
	    doSC_Exit();
	    break;

   case SC_CreateSemaphore :
	    doSC_CreateSemaphore();
	    break;
      
   case SC_Signal:
	    doSC_Signal();
	    break;
      
   case SC_Wait:
	    doSC_Wait();
    	break;

    	default:
			printf("Unexpected user mode exception %d %d\n", which, type);
			ASSERT(FALSE);
  	}
	}
}
