#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <iostream>

using namespace std;
int main()
{
  char filename[]="task1-1.cpp";
  key_t key = ftok(filename,0);
  int semid = semget(key, 1, 0666|IPC_CREAT);
  struct sembuf mybuf;
  mybuf.sem_op=-5;
  mybuf.sem_flg=0;
  mybuf.sem_num=0;
  cout<<"Locked while sem condition != 5"<<endl;
  semop(semid,&mybuf,1);
  cout<< "Condition set"<<endl;
  semctl(semid, 0, IPC_RMID, 0);
  return 0;
}
