/*
 * Implement 3 periodic threads, with periods 50ms, 100ms, and 150ms.
 * The job bodies are functions named task1(), task2() and task3()
 */

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <stdint.h>
#include <wiringPi.h> // Include WiringPi library!

#include "periodic_tasks.h"
#include "dispositivos.h"


static pthread_mutex_t mutexLuz;
static pthread_mutex_t mutexPot;
static pthread_mutex_t mutexTemp;
static int th_cnt;
int luz_ADC;
int pot_ADC;
int temperatura;
int peligro;

#define TRUE 1
#define FALSE 0

/* periodic threads */

void *thread1(void *param)
{
  struct periodic_task *p_d;

  p_d = start_periodic_timer(0, 30000);
  while (1) {
    wait_next_activation(p_d);
	task1();
	
  }
}

void *thread2(void *param)
{
  struct periodic_task *p_d;

  p_d = start_periodic_timer(0, 5000);
  while (1) {
    wait_next_activation(p_d);
    task2();
  }
}

void *thread3(void *param)
{
  struct periodic_task *p_d;

  p_d = start_periodic_timer(0, 10000);
  while (1) {
    wait_next_activation(p_d);
    task3();
  }
}

/* activity of tasks */

void task1(void)
{
    // CUERPO DE LA TAREAS 

   //   pthread_mutex_lock(&mutexLuz);
    // REGION CRITICA 
	luz_ADC=analogRead(0);
   //   pthread_mutex_unlock(&mutexLuz);
if(luz_ADC>520){
	Poner_Verde(1);
		}
	else{
	Poner_Verde(0);
	}
}

void task2(void)
{
    // CUERPO DE LA TAREAS
 
    //  pthread_mutex_lock(&mutexPot);
    // REGION CRITICA
	pot_ADC=analogRead(1);
   //   pthread_mutex_unlock(&mutexPot); 
if(pot_ADC>300){
	Poner_Rojo(1);
		}
	else{
	Poner_Rojo(0);
	}  
}

void task3(void)
{
    // CUERPO DE LA TAREA 
 // pthread_mutex_lock(&mutexTemp);
    // REGION CRITICA
	temperatura=analogRead(2);
    //  pthread_mutex_unlock(&mutexTemp);  
printf("\n Temperatura: %d", temperatura); 
}

/*
void task4(void)
{
    
 // pthread_mutex_lock(&mutexTemp);
    

if(temperatura>200){
 // pthread_mutex_lock(&mutexPeligro);
   	peligro=TRUE;
    //  pthread_mutex_unlock(&mutexPeligro); 
}
    //  pthread_mutex_unlock(&mutexTemp);  


 //pthread_mutex_lock(&mutexPot);
	if(pot_ADC>300){
 // pthread_mutex_lock(&mutexPeligro);
   	peligro=TRUE;
    //  pthread_mutex_unlock(&mutexPeligro); 
	}
 //  pthread_mutex_unlock(&mutexPot); 

 //pthread_mutex_lock(&mutexLuz);
	if(luz_ADC>520){
 // pthread_mutex_lock(&mutexPeligro);
   	peligro=TRUE;
    //  pthread_mutex_unlock(&mutexPeligro); 
	}
 //  pthread_mutex_unlock(&mutexLuz); 

}
*/
/* -------------------- */
/* body of main program */
/* -------------------- */

int main(int argc, char *argv[])

{
  int err;
  void *returnvalue;
  pthread_t second_id, third_id, fourth_id;
  int pmin;
  pthread_attr_t attrs;
  struct sched_param sp;

  pthread_mutexattr_t mymutexattr;

  //-------- init attributes of tasks 

  pthread_attr_init(&attrs);
	  Inicializar_dispositivos ();
  err = pthread_attr_setinheritsched(&attrs, PTHREAD_EXPLICIT_SCHED);
  if (err != 0) {
    perror("setineheritsched");
  }

  err = pthread_attr_setschedpolicy(&attrs, SCHED_FIFO);
  if (err != 0) {
    perror("setschedpolicy");
  }

  pmin = sched_get_priority_min(SCHED_FIFO);

  //-------- creation of mutex 

  pthread_mutexattr_init(&mymutexattr);

  if (pthread_mutexattr_setprotocol (&mymutexattr, PTHREAD_PRIO_PROTECT) != 0) {
    perror ("Error in mutex attribute setprotocol \n");
  }

  //-------- ceiling priority of mutex 

  if (pthread_mutexattr_setprioceiling (&mymutexattr, pmin) != 0) {
    perror ("Error in mutex attribute setprotocol \n");
  }

  //-------- init mutex 
  if (pthread_mutex_init (&mutexLuz,&mymutexattr) != 0) {
    perror ("Error in mutex init");
  }

  pthread_mutexattr_destroy(&mymutexattr);

  //--------  creation and activation of the new thread 
  sp.sched_priority = pmin;
  err = pthread_attr_setschedparam(&attrs, &sp);
  if (err != 0) {
    perror("setschedparam");
  }
  err = pthread_create(&second_id, &attrs, thread1, (void *)NULL);
  if (err != 0) {
    fprintf(stderr, "Cannot create pthread 1");
  }


  sp.sched_priority = pmin;
  err = pthread_attr_setschedparam(&attrs, &sp);
  if (err != 0) {
    perror("setschedparam");
  }
  err = pthread_create(&third_id, &attrs, thread2, (void *)NULL);
  if (err != 0) {
    fprintf(stderr, "Cannot create pthread 2");
  }


  sp.sched_priority = pmin;
  err = pthread_attr_setschedparam(&attrs, &sp);
  if (err != 0) {
    perror("setschedparam");
  }
  err = pthread_create(&fourth_id, &attrs, thread3, (void *)NULL);
  if (err != 0) {
    fprintf(stderr, "Cannot create pthread 3");
  }

  //-------- We wait the end of the threads we just created. 
  pthread_join(second_id, &returnvalue);
  pthread_join(third_id, &returnvalue);
  pthread_join(fourth_id, &returnvalue);


  printf("main: returnvalue is %d\n", (int)returnvalue);

  return 0;
}

