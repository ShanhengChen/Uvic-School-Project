#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include "queue.h"

struct customer_info{ /// use this struct to record the customer information read from customers.txt
  int user_id;
	int class_type;
  int arrival_time;
	int service_time;
};
//------------------------------------------------- declear function 
void fread_file( char *);
double getCurrentSimulationTime();
void * customer_come(void * cus_info);
void * customer_entry(void * cus_info);
//------------------------------------------------- golbal variable
Node *queue_esc = NULL;
Node *queue_BUS = NULL;

static struct timeval start_time; // simulation start time
struct customer_info events[60];// storing all custom info in a array
pthread_mutex_t lock,c_lock,start_time_mtex; 
pthread_cond_t convar = PTHREAD_COND_INITIALIZER;

int total_customer;
int Bus_customer_number;
int esc_customer_number;
int NClerks = 5;  // number of Nclerks
int counter = 0; // track number of customer in queue

int Clerk_id = 0;// track how many clerk is running

double st; // starting time of a program
double cur_simulation_secs; //current time that the customer arrive
double end_time; // the time when customer is finish
double overall_waiting_time ;  // over all avg time for all customer
double ecs_wait_time = 0; // The average waiting time for all economy-class customers
double BUS_wait_time = 0; //The average waiting time for all business-class customers
int queue_status[5] = {1,1,1,1,1};

int main(int argc, char *argv[])
{   
    fread_file(argv[1]); // read file and store stuffs into events 
    overall_waiting_time = 0;
    pthread_t customId[total_customer],customer[total_customer];
    st = getCurrentSimulationTime();
    if (pthread_mutex_init(&lock, NULL) != 0){ //mutex initialization
        printf("\n mutex init failed\n");
        return 1; }
    if (pthread_mutex_init(&c_lock, NULL) != 0){ //mutex initialization
        printf("\n mutex init failed\n");
        return 1; }
    if (pthread_mutex_init(&start_time_mtex, NULL) != 0){ //mutex initialization
        printf("\n mutex init failed\n");
        return 1; }
    
    for(int i = 0; i < total_customer ; i++){ 
		  if(pthread_create(&customer[i], NULL, customer_come, (void *)&events[i]) != 0)
      {printf("can't create customer_thread \n");}
    }   

    for(int j = 0; j < total_customer ; j++){ // number of customers
		  if(pthread_create(&customId[j], NULL, customer_entry, (void *)&events[j]) != 0)
      {printf("can't create customer_thread \n");}
    }  

    for(int l = 0; l < total_customer; l++){ //wait for completion of all threads
      pthread_join(customer[l], NULL);
    }
    for(int k = 0; k < total_customer; k++){ //wait for completion of all threads
      pthread_join(customId[k], NULL);
    }
    printf("All jobs done...\n");
    printf("\nThe average waiting time for all customers in system is: %.2f seconds\n", overall_waiting_time/total_customer);
    printf("\nThe average waiting time for all business-class in system is: %.2f seconds\n", BUS_wait_time/Bus_customer_number);
    printf("\nThe average waiting time for all economy-class in system is: %.2f seconds\n", ecs_wait_time/esc_customer_number);
	  pthread_mutex_destroy(&lock);
    pthread_mutex_destroy(&start_time_mtex);
    pthread_mutex_destroy(&c_lock);
	  return 0;
    }

void * customer_come(void * cus_info){ // control the print of customer come in 
  struct customer_info * p_myInfo = (struct customer_info *) cus_info;
  usleep(p_myInfo->arrival_time*100000);;
  double arrive_time = p_myInfo->arrival_time;
  double s_time = arrive_time*10 /100;
  printf("customer %d arrive at %.2f\n", p_myInfo->user_id,s_time);
   if (p_myInfo->class_type == 0){
    printf("A customer enters a queue: the queue ID %d, and length of the queue %d\n",p_myInfo->class_type+1,counter + 1);
   }
   else{
    printf("A customer enters a queue: the queue ID %d, and length of the queue %d\n",p_myInfo->class_type+1,counter + 1);}
  pthread_exit(NULL);
	return((void *)0);
}
//A customer enters a queue: the queue ID 1, and length of the queue  1.
// queue (Queue 0) for economy class and the other12 (Queue 1) for business class

void * customer_entry(void * cus_info){ // control start serving time and queue
	struct customer_info * p_myInfo = (struct customer_info *) cus_info;
  usleep(p_myInfo->arrival_time*100000);
  pthread_mutex_lock(&lock);
  if (p_myInfo->class_type == 0)
  {queue_esc = enqueue(queue_esc,p_myInfo->user_id, p_myInfo->class_type);
    esc_customer_number ++;
  //printf("A customer enters a queue: the queue ID %d, and length of the queue%d",p_myInfo->class_type+1,counter + 1);
  }
  else{
    queue_BUS = enqueue(queue_BUS,p_myInfo->user_id, p_myInfo->class_type);
   // printf("A customer enters a queue: the queue ID %d, and length of the queue%d",p_myInfo->class_type+1,counter + 1);
    Bus_customer_number ++;
    }
  counter ++;
  Clerk_id ++;
  int check = 0;

  while(Clerk_id >5){ // wait until someone serving customer is done
    usleep(1);
    if(Clerk_id<= 5){break;}}//
  

  while(true){
    if(queue_BUS != NULL){  // let business class go first 
      cur_simulation_secs = getCurrentSimulationTime(); // line 128-131 get business class watting time 
      double temp_time = p_myInfo->arrival_time;
      double temp_timediv10 = temp_time/10;
      double b_waiting_time = (cur_simulation_secs - st) -temp_timediv10;
      BUS_wait_time += b_waiting_time; 
      overall_waiting_time += b_waiting_time; 
      for(int i = 0; i <NClerks; i++){ //turn a clerk into use state
      if(queue_status[i] == 1){queue_status[i] = -1; check = i;break;}}
      printf("A clerk starts serving a customer: start time %.2f and  the customer ID is %d, the clerk ID %d.\n",cur_simulation_secs - st ,p_myInfo->user_id, check);
      
      queue_BUS = dequeue(queue_BUS); 
      counter --; // queue length

      break;
    }else if(p_myInfo->user_id == queue_esc->id && queue_BUS == NULL){
      cur_simulation_secs = getCurrentSimulationTime();// line 141-144 get business class watting time 
      double temp_time = p_myInfo->arrival_time;
      double temp_timediv10 = temp_time/10;
      double e_waiting_time = (cur_simulation_secs - st) -temp_timediv10;
      ecs_wait_time += e_waiting_time;
      overall_waiting_time += e_waiting_time;
      for(int i = 0; i <NClerks; i++){ //turn a clerk into use state
      if(queue_status[i] == 1){queue_status[i] = -1; check = i;break;}} //check var record which clerk is serving this customer
      printf("A clerk starts serving a customer: start time %.2f and  the customer ID is %d, the clerk ID %d.\n",cur_simulation_secs - st ,p_myInfo->user_id, check);
      queue_esc = dequeue(queue_esc); 
      counter --; // queue length
      break;
    }
      }

            
      end_time = getCurrentSimulationTime();
      double service_time = p_myInfo->service_time;
      double service_time_div10 = service_time* 100 /1000;
      double start_service_time = cur_simulation_secs - st;
      double final_time = service_time_div10 + start_service_time;
      pthread_mutex_unlock(&lock);
      usleep(p_myInfo->service_time*100000);

      pthread_mutex_lock(&c_lock); // once a customer is finished release the clerk and back to serve
      queue_status[check] = 1;
      Clerk_id --;
       pthread_mutex_unlock(&c_lock);
       //pthread_cond_signal(&convar);
      printf("-->>> A clerk finishes serving a customer: end time %.2f,the customer %d the clerk ID %d\n",final_time, p_myInfo->user_id, check);
  pthread_exit(NULL);
	return((void *)0);
  
}

void fread_file( char * argv){
    FILE * fp;
    char input[20];
    char temp_array[20];
    int temp_record = 0;
    int value_checker = 0; 
  fp = fopen (argv, "r");
    if(fp == NULL)
    {printf("Could not open %s\n", argv); exit(EXIT_FAILURE);} // exit if we cant open file
    fgets(input, sizeof(input), fp); // get total number
    sscanf(input, "%d", &total_customer);
    while(fgets(input, sizeof(input), fp)){ //printf("%s\t\n",input);
      strcpy(temp_array,input);
      sscanf( temp_array, "%d:%d,%d,%d", &events[temp_record].user_id, &events[temp_record].class_type,&events[temp_record].arrival_time, &events[temp_record].service_time);
        value_checker = events[temp_record].service_time;
      if( value_checker < 1) // if checker <= 0 which means sscanf does not read a line in a proper way or there is some illegal values comes in 
      {printf("\n wrong format of data\n"); // exit if we cant read correct date
      exit(EXIT_FAILURE);break;}
      temp_record +=1; }
      fclose(fp);
}

double getCurrentSimulationTime(){
	
	struct timeval cur_time;
	double cur_secs, init_secs;
  pthread_mutex_lock(&start_time_mtex); 
	init_secs = (start_time.tv_sec + (double) start_time.tv_usec / 1000000);
	pthread_mutex_unlock(&start_time_mtex);
	gettimeofday(&cur_time, NULL);
	cur_secs = (cur_time.tv_sec + (double) cur_time.tv_usec / 1000000);
	return cur_secs - init_secs;
}