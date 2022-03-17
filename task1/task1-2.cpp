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

  mybuf.sem_op=1;
  mybuf.sem_flg=0;
  mybuf.sem_num=0;
  semop(semid,&mybuf,1);
  cout<< "Sem condition +=1"<<endl;
  
  int rtn = semctl(semid, 0, GETVAL, 0);
  if(rtn>=0) cout<< "Current sem condition = "<<rtn<<endl;
  else cout<<"Unable to get sem condition"<<endl;
  return 0;
}
