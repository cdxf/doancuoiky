// DUN~G TRUNG TRAN
//stable.cc

#include "stable.h"
//#include "stdlib.h"

STable::STable()
{
  bm = new BitMap(MAX_SEMAPHORE);
  for (int i =0 ; i < MAX_SEMAPHORE;i++)
    semTab[i] = NULL;
}

STable::~STable()
{
  delete bm;
  int i;
  for (i = 0 ; i < MAX_SEMAPHORE ; i++)
    if (semTab[i]!= NULL)
      delete semTab[i];
}

int STable::create(char *name, int init)
{
  int id;
  id = bm->Find();
  if (id == -1)
    {
      printf("\nNo free slot");
      return -1;
    }
  //debug
  // printf("\n STable:create here");
  for (int i = 0 ; i < MAX_SEMAPHORE ; i++)
    {if (semTab[i] != NULL)
      if(strcmp(semTab[i]->GetName(),name) == 0){
	printf("Stable:create error duplicate semaphore name '%s'",name);
	return -1;
      }
    }
  
  semTab[id] = new Sem(name,init);
  bm->Mark(id);

  return 0;
}

int STable::wait(char *name)
{
  int i,index;
  for (i = 0 ; i < MAX_SEMAPHORE ; i++)
    {if (semTab[i] != NULL)
      if(strcmp(semTab[i]->GetName(),name) == 0 ){
	index=i;
	break;
      }
    }
  if (i == MAX_SEMAPHORE )
    {
      printf("Error semaphore '%s' not exist",name);
      return -1;
    }
  semTab[index]->wait();
  return 0;
}

int STable::signal(char *name)
{
  int i,index;
  for (i = 0 ; i < MAX_SEMAPHORE ; i++)
    {if (semTab[i] != NULL)
      if(strcmp(semTab[i]->GetName(),name) == 0){
        index = i;
        break;
      }
    }
  if (i == MAX_SEMAPHORE )
    {
      printf("Error semaphore '%s' not exist",name);
      return -1;
    }
  semTab[index]->signal();
  return 0;
}
