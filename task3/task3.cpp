#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

using namespace std;
void parent(int* pd1, int semid, sembuf& mybuf)
{
  // Write message.
  int size = write(pd1[1], "Parent to child", 15);
  
  // Wait until semophore condition !=1.
  mybuf.sem_num=0;
  mybuf.sem_op=-1;
  semop(semid,&mybuf,1);
  
  char resstring[20]="\0";
  
  // Read and output message.
  read(pd1[0], resstring, 15);
  cout<<resstring<<endl;
}

void child(int* pd1, int semid, sembuf& mybuf)
{
  char resstring[20]="\0";
  
  // Read and output message.
  read(pd1[0], resstring, 15);
  cout<<resstring<<endl;
  
  // Write message.
  int size = write(pd1[1],"Child to parent", 15);
  
  // Set semaphore to 1.
  mybuf.sem_num=0;
  mybuf.sem_op=1;
  semop(semid,&mybuf,1);
}

int main()
{
  
  // Creating one pipe.
  int fd1[2];
  if(pipe(fd1)<0) {
    cout<<"Pipe error"<<endl;
    return -1;
  }
  
  char pathname1[] = "task3.cpp";
  key_t key = ftok(pathname1, 0);
  
  // Creating one semophore and set in to 0.
  int semid = semget(key, 1, 0666|IPC_CREAT|IPC_EXCL);
  semctl(semid, 0, SETVAL,0);
  struct sembuf mybuf;
  mybuf.sem_flg=0;
  
  // Create new process.
  pid_t pid = fork();
  if(pid==-1) cout<<"Fork error"<<endl;
  else if(pid==0) child(fd1, semid, mybuf);
  else {
    parent(fd1, semid, mybuf);
    wait(0);
  }
  
  // Close pipe in both directions.
  close(fd1[0]);
  close(fd1[1]);
  
  // Remove semaphore.
  semctl(semid, 0, IPC_RMID, 0);
  return 0; 
}

