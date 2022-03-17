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
  mybuf.sem_op=-5; //operation
  mybuf.sem_flg=0; //flags
  mybuf.sem_num=0; //semophore number in array
  cout<<"Locked while sem condition != 5"<<endl;
  
  // Set semophore condition.
  semop(semid,&mybuf,1);
  
  // This message will be displayed when it will be possible to make -5 to semophore i.e.semophore condition must be 5. 
  cout<< "Condition set"<<endl;
  
  // Remove semophore array.
  semctl(semid, 0, IPC_RMID, 0);
  return 0;
}
