#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include <dirent.h>
#include "linked_list.h"
#define path_length 200
#define str_length 60

Node *head = NULL; //linked_list keep track of process pid 
pid_t pid;         

char temp_path[path_length];
int p_status;

void func_BG(char **cmd)
{ 
  char order[20] = "";
  strcpy(order,cmd[1]); 
  pid = fork();
    if (pid > 0) //parent process store child process pid and path into linked_list
  {
    getcwd(temp_path, sizeof(temp_path));
    strcat(temp_path,"/pman");
    head = add_newNode(head, pid,temp_path);
  
  }
  else if (pid == 0) // child process run another program  
  { 
    char *temp_file_name = order + 2;
    char *argv_execvp[] = {temp_file_name, NULL};
    if (execvp(order, argv_execvp) < 0)
      perror("Error on execvp");
  }
  else  // fork error exit
  {
    perror("fork error\n");
    exit(EXIT_FAILURE);
  }

}

void func_BGlist(char **cmd) // print all node within the linked list
{
  printList(head);
}

void func_BGkill(char *str_pid)  //check if pid exist -> send SIGTERM to pid process 
{                                //// delet the node with pid from linked_list and recycle the process
  if(str_pid){                              
  int pid = atoi(str_pid);
  if(PifExist(head,pid) == 0){printf("PID does not exist\n"); /* exit(EXIT_FAILURE); */ }
  else{
  int retVal = kill(pid,SIGTERM);
  if (retVal == -1) { 
				perror("Fail at terminate the process\n"); 
				exit(EXIT_FAILURE); 
			}
      head = deleteNode(head,pid);
      pid = waitpid(-1, &p_status, WNOHANG);
      /*    if (wait(&p_status) >= 0)
    {printf("Child process exited with %d status\n", WEXITSTATUS(p_status));}} */
}}else{printf("need a pid\n"); /* exit(EXIT_FAILURE); */}
}

void func_BGstop(char *str_pid)
{
  if(str_pid){
  int pid = atoi(str_pid);
  if(PifExist(head,pid) == 0){printf("PID does not exist\n"); /* exit(EXIT_FAILURE); */ }
  else{
  int retVal = kill(pid,SIGSTOP);
  if (retVal == -1) { 
				perror("Fail at pause a process\n"); 
				exit(EXIT_FAILURE); 
			}
  }}else{printf("need a pid\n"); /* exit(EXIT_FAILURE); */}
}

void func_BGstart(char *str_pid)
{
  if(str_pid){
  int pid = atoi(str_pid);
  if(PifExist(head,pid) == 0){printf("PID does not exist\n"); /* exit(EXIT_FAILURE); */ }
  else{
  int retVal = kill(pid,SIGCONT);
  if (retVal == -1) { 
				perror("Fail at resume a process\n"); 
				exit(EXIT_FAILURE); 
			}
  }}else{printf("need a pid\n"); /* exit(EXIT_FAILURE); */}
}

void func_pstat(char *str_pid)
{
   if(str_pid){
  if(!PifExist(head,atoi(str_pid))){printf("PID does not exist\n"); /* exit(EXIT_FAILURE); */} //test if pid exist
  else{
  DIR *proc_dir; //open dir proc
  FILE *fp_stat, *fp_status;     // file pointer for read stat and status

  char temp[str_length] ="/proc/";
  char read_path[str_length] = "";
  char read_status[str_length] = "";

  char comm[str_length];
  char state[str_length];
  int rss;
  float Utime;
  float Stime;

  int counter = 0;                  //indicate lines of proc/<pid>/status
  char nonvoluntary_ctxt_switches[str_length] ="";
  char voluntary_ctxt_switches[str_length] ="";

  strcat(temp,str_pid);
  strcpy(read_path,temp);
  strcat(read_path,"/stat");     // read_path: /prco/<pid>/stat
  strcpy(read_status,temp);
  strcat(read_status,"/status"); // read_status: /proc/<pid>/status

  proc_dir = opendir(temp);
  if (!proc_dir) {perror("PID does not exist\n");} // open dir
  fp_stat = fopen(read_path, "r"); //open file
  if (!fp_stat){perror("not file exists\n");}
  fp_status = fopen(read_status, "r"); //open file
  if (!fp_status){perror("not file exists\n");}
  fscanf(fp_stat,"%*d %s %s %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %f %f \
  %*d %*d %*d %*d %*d %*d %*d %*d %d", comm, state, &Utime,&Stime,&rss);                //get comm, state,utime,stime, rss from proc/pid/stat
  
while (fgets(nonvoluntary_ctxt_switches, str_length, fp_status) != NULL){ counter ++;} //get last line of proc/pid/status 
counter --;                                                                            //which is nonvoluntary_ctxt_switches
rewind(fp_status);
for(int i = 0; i < counter; i++){fgets(voluntary_ctxt_switches, str_length, fp_status);} //get second last line of proc/pid/status
  comm[strlen(comm)-1] = '\0';                                                           // which is voluntary_ctxt_switches
  printf("\nComm:\t%s\nState\t%s\nUtime:\t%1.f ms\nStime:\t%1.f ms\nRss:\t%d\n%s%s", \
  comm +1, state,((Utime/sysconf(_SC_CLK_TCK)*10))*100,((Stime/sysconf(_SC_CLK_TCK)*10))*100, \
  rss,voluntary_ctxt_switches,nonvoluntary_ctxt_switches);                                //print out needed info for pstat <pid> in a format way
  fclose(fp_stat);
  closedir(proc_dir);
}}else{printf("need a pid\n"); /* exit(EXIT_FAILURE); */} }

int main()                               // almost the same as given code but I delete the line that print out the use input and modify line 171
{
  char user_input_str[str_length];
  while (true)
  {
    printf("Pman: > ");  
    fgets(user_input_str, str_length, stdin);
    char *ptr = strtok(user_input_str, " \n");
    if (ptr == NULL)
    {
      continue;
    }
    char *lst[str_length];
    int index = 0;
    lst[index] = ptr;
    index++;
    while (ptr != NULL)
    {
      ptr = strtok(NULL, " \n");
      lst[index] = ptr;
      index++;
    }
    if (strcmp("bg", lst[0]) == 0 && lst[1])  // just in case someone only input bg as a command
    {
      func_BG(lst);
    }
    else if (strcmp("bglist", lst[0]) == 0)
    {
      func_BGlist(lst);
    }
    else if (strcmp("bgkill", lst[0]) == 0)
    {
      func_BGkill(lst[1]);
    }
    else if (strcmp("bgstop", lst[0]) == 0)
    {
      func_BGstop(lst[1]);
    }
    else if (strcmp("bgstart", lst[0]) == 0)
    {
      func_BGstart(lst[1]);
    }
    else if (strcmp("pstat", lst[0]) == 0)
    {
      func_pstat(lst[1]);
    }
    else if (strcmp("q", lst[0]) == 0)
    {
      printf("Bye Bye \n");
      exit(0);
    }
    else
    {
      printf("Invalid input\n");
    }
  }
  return 0;
}
