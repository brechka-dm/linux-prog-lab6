#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <stdio.h>

using namespace std;
int main()
{
  const int n=4;
  
  // Process ID.
  const int pId=2;
  
  key_t key;
  int shmid;
  
  // Array of n int.
  int* mas;
  
  char pathname1[] = "task2-1-async.cpp";
  
  key = ftok(pathname1, 0);
  
  // Create shared memory of n int.
  shmid = shmget(key, sizeof(int)*n,0666|IPC_CREAT|IPC_EXCL);
  if(shmid>=0)
  {
    // If memory was created attach it and initialize mas to 0.
    mas = (int*) shmat(shmid,NULL,0);
    for(int i=0;i<n;i++) mas[i]=0;
  }
  else
  {
    // If memory was not created it means it's already exists.
    // Get memory and attach without mas initialization.
    shmid = shmget(key, sizeof(int)*n, 0);
    mas = (int*) shmat(shmid,NULL,0);
  }
  
  int a;
  int sum=0;

  // Increment given mas element.
  a=mas[pId];
  a=a+1;
  mas[pId]=a;
  
  // Calculating mas sum.
  for(int i=0;i<n-1;i++) sum=sum+mas[i];
  
  // Artificaly slow down process.
  for(int i=0;i<10000000L;i++);
  
  // Write sum to last mas element.
  mas[n-1]=sum;
    
  cout<<"Prog1: "<<mas[0]<<"; prog2: "<<mas[1]<<"; prog3: "<<mas[2]<<"; common: "<<mas[3]<<endl;
  
  // Detach shared memory.
  shmdt(mas);
  return 0;
}
