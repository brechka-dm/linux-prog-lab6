#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <stdio.h>

using namespace std;
int main()
{
  const int n=4;
  const int pId=2;
  
  key_t key;
  int shmid;
  int* mas;
  
  char pathname1[] = "task2-1-sync.cpp";
  
  key = ftok(pathname1, 0);
  
  shmid = shmget(key, sizeof(int)*n,0666|IPC_CREAT|IPC_EXCL);
  if(shmid>=0)
  {
    mas = (int*) shmat(shmid,NULL,0);
    for(int i=0;i<n;i++) mas[i]=0;
  }
  else
  {
    shmid = shmget(key, sizeof(int)*n, 0);
    mas = (int*) shmat(shmid,NULL,0);
  }
  
  // Struct for semophore condition.
  struct sembuf mybuf;
  mybuf.sem_flg=0;
  mybuf.sem_num=0;
  
  // Trying to create semophore.
  int semid = semget(key, 1, 0666|IPC_CREAT|IPC_EXCL);
  if(semid>=0)
  {
    // If semophore was created set it to 1.
    int rtrn = semctl(semid, 0, SETVAL,1);
  }
  else
  {
    // If semophore was not created it means it's already exists.
    // Get semaphore.
    semid = semget(key, 1, 0);
  }  

  
  int a;
  int sum=0;

  // Make semophore -1.
  // If semophore condition ==1 than we entering critical region
  // otherwise block.
  mybuf.sem_op=-1;
  semop(semid,&mybuf,1);
  
  a=mas[pId];
  a=a+1;
  mas[pId]=a;
    
  for(int i=0;i<n-1;i++) sum=sum+mas[i];
  for(int i=0;i<10000000L;i++);
  mas[n-1]=sum;
    
  cout<<"Prog1: "<<mas[0]<<"; prog2: "<<mas[1]<<"; prog3: "<<mas[2]<<"; common: "<<mas[3]<<endl;

  // Set semophore +=1 while leaving critical region.
  mybuf.sem_op=1;
  semop(semid,&mybuf,1);  
  
  shmdt(mas);
  return 0;
}
