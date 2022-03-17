#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <iostream>

using namespace std;
int main()
{
    
  // File for key generation.
  char filename[]="task1-1.cpp";
  
  // Generate key.
  key_t key = ftok(filename,0);
  
  // Create array of one semophore.
  int semid = semget(key, 1, 0666|IPC_CREAT);
  
  // Struct for semophore condition.
  struct sembuf mybuf;
  mybuf.sem_op=1; //operation
  mybuf.sem_flg=0; //flags
  mybuf.sem_num=0; //semophore number in array
  
  // Set semophore condition. 
  // I.e. semophore condition += 1.
  semop(semid,&mybuf,1);
  cout<< "Sem condition +=1"<<endl;
  
  // Get semophore value.
  int rtn = semctl(semid,/*sem_number=*/0, GETVAL, /*arg=(not using)*/0);
  if(rtn>=0) cout<< "Current sem condition = "<<rtn<<endl;
  else cout<<"Unable to get sem condition"<<endl;
  return 0;
}
