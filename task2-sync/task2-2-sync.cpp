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
  const int pId=1;
  
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
  
  struct sembuf mybuf;
  mybuf.sem_flg=0;
  mybuf.sem_num=0;
  
  int semid = semget(key, 1, 0666|IPC_CREAT|IPC_EXCL);
  if(semid>=0)
  {
    //mybuf.sem_op=1;
    //semop(semid,&mybuf,1);
    //return 2;
    int rtrn = semctl(semid, 0, SETVAL,1);
    //cout<<"rtrn: "<<rtrn<<endl;
  }
  else{
    semid = semget(key, 1, 0);
  }  

  
  int a;
  int sum=0;

  mybuf.sem_op=-1;
  semop(semid,&mybuf,1);
  
  a=mas[pId];
  a=a+1;
  mas[pId]=a;
    
  for(int i=0;i<n-1;i++) sum=sum+mas[i];
  for(int i=0;i<10000000L;i++);
  mas[n-1]=sum;
    
  cout<<"Prog1: "<<mas[0]<<"; prog2: "<<mas[1]<<"; prog3: "<<mas[2]<<"; common: "<<mas[3]<<endl;

  mybuf.sem_op=1;
  semop(semid,&mybuf,1);  
  
  shmdt(mas);
  return 0;
}
