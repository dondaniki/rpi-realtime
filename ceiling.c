/*
 * Implement 3 periodic threads, with periods 50ms, 100ms, and 150ms.
 * The job bodies are functions named t1_LecFotometro(), t2_LecPotenciometro() and t3_LecTemperatura()
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
int peligro_pot;
int peligro_temp;
int peligro_luz;

#define TRUE 1
#define FALSE 0


#define PRINT_TIME 0
#define PRINT_TIME_MUTEX_SINGLE  0
#define PRINT_TIME_MUTEX_TCOMM 	 0
#define PRINT_TIME_MUTEX_TRIESGO 1

/* periodic threads */

void *thread_LUZ(void *param)
{
  struct periodic_task *p_d;

  p_d = start_periodic_timer(0, 30000);
  while (1) {
    wait_next_activation(p_d);
	t1_LecFotometro();
	
  }
}

void *thread_POT(void *param)
{
  struct periodic_task *p_d;

  p_d = start_periodic_timer(0, 5000);
  while (1) {
    wait_next_activation(p_d);
    t2_LecPotenciometro();
  }
}

void *thread_TEMP(void *param)
{
  struct periodic_task *p_d;

  p_d = start_periodic_timer(0, 10000);
  while (1) {
    wait_next_activation(p_d);
    t3_LecTemperatura();
  }
}
void *thread_RIESGO(void *param)
{
  struct periodic_task *p_d;

  p_d = start_periodic_timer(0, 10000);
  while (1) {
    wait_next_activation(p_d);
    t5_Riesgo();
  }
}

void *thread_COMM(void *param)
{
  struct periodic_task *p_d;

  p_d = start_periodic_timer(0, 3000000);
  while (1) {
    wait_next_activation(p_d);
    t4_Comm();
  }
}

/* activity of tasks */

void t1_LecFotometro(void)
{
#if PRINT_TIME
	struct timespec ts1, ts2;
	clock_gettime( CLOCK_REALTIME, &ts1 );
#endif

    // CUERPO DE LA TAREAS 

	pthread_mutex_lock(&mutexLuz);
#if PRINT_TIME_MUTEX_SINGLE
	struct timespec ts1, ts2;
	clock_gettime( CLOCK_REALTIME, &ts1 );
#endif
	luz_ADC=analogRead(0);
#if PRINT_TIME_MUTEX_SINGLE
	clock_gettime( CLOCK_REALTIME, &ts2);
	printf("demora LecFotometro mutexLuz: %f\n", (float) (1.0 * (1.0 * ts2.tv_nsec - ts1.tv_nsec * 1.0) * 1e-9 + 1.0 * ts2.tv_sec - 1.0 * ts1.tv_sec ));
#endif
	pthread_mutex_unlock(&mutexLuz);

#if PRINT_TIME
	clock_gettime( CLOCK_REALTIME, &ts2);
	printf("demora t1_LecFotometro: %f\n", (float) (1.0 * (1.0 * ts2.tv_nsec - ts1.tv_nsec * 1.0) * 1e-9 + 1.0 * ts2.tv_sec - 1.0 * ts1.tv_sec ));
#endif	
}

void t2_LecPotenciometro(void)
{
#if PRINT_TIME
	struct timespec ts1, ts2;
	clock_gettime( CLOCK_REALTIME, &ts1 );
#endif
    // CUERPO DE LA TAREAS

	pthread_mutex_lock(&mutexPot);
#if PRINT_TIME_MUTEX_SINGLE
	struct timespec ts1, ts2;
	clock_gettime( CLOCK_REALTIME, &ts1 );
#endif
	pot_ADC=analogRead(1);
#if PRINT_TIME_MUTEX_SINGLE
	clock_gettime( CLOCK_REALTIME, &ts2);
	printf("demora LecPotenciometro mutexPot: %f\n", (float) (1.0 * (1.0 * ts2.tv_nsec - ts1.tv_nsec * 1.0) * 1e-9 + 1.0 * ts2.tv_sec - 1.0 * ts1.tv_sec ));
#endif
	pthread_mutex_unlock(&mutexPot);
 
#if PRINT_TIME
	clock_gettime( CLOCK_REALTIME, &ts2);
	printf("demora t2_LecPotenciometro: %f\n", (float) (1.0 * (1.0 * ts2.tv_nsec - ts1.tv_nsec * 1.0) * 1e-9 + 1.0 * ts2.tv_sec - 1.0 * ts1.tv_sec ));
#endif
 
}

void t3_LecTemperatura(void)
{
#if PRINT_TIME
	struct timespec ts1, ts2;
	clock_gettime( CLOCK_REALTIME, &ts1 );
#endif

	pthread_mutex_lock(&mutexTemp);
#if PRINT_TIME_MUTEX_SINGLE
	struct timespec ts1, ts2;
	clock_gettime( CLOCK_REALTIME, &ts1 );
#endif
	temperatura=analogRead(2);
#if PRINT_TIME_MUTEX_SINGLE
	clock_gettime( CLOCK_REALTIME, &ts2);
	printf("demora LecTemperatura mutexTemp: %f\n", (float) (1.0 * (1.0 * ts2.tv_nsec - ts1.tv_nsec * 1.0) * 1e-9 + 1.0 * ts2.tv_sec - 1.0 * ts1.tv_sec ));
#endif
	pthread_mutex_unlock(&mutexTemp);  

#if PRINT_TIME
	clock_gettime( CLOCK_REALTIME, &ts2);
	printf("demora t3_LecTemperatura: %f\n", (float) (1.0 * (1.0 * ts2.tv_nsec - ts1.tv_nsec * 1.0) * 1e-9 + 1.0 * ts2.tv_sec - 1.0 * ts1.tv_sec ));
#endif

}

void t4_Comm(void)
{
#if PRINT_TIME
	struct timespec ts1, ts2;
	clock_gettime( CLOCK_REALTIME, &ts1 );
#endif
	pthread_mutex_lock(&mutexTemp);
#if PRINT_TIME_MUTEX_TCOMM
	struct timespec ts1, ts2;
	clock_gettime( CLOCK_REALTIME, &ts1 );
#endif
	printf("\n¡temperatura! %d",temperatura);
#if PRINT_TIME_MUTEX_TCOMM
	clock_gettime( CLOCK_REALTIME, &ts2);
	printf("demora Comm mutexTemp: %f\n", (float) (1.0 * (1.0 * ts2.tv_nsec - ts1.tv_nsec * 1.0) * 1e-9 + 1.0 * ts2.tv_sec - 1.0 * ts1.tv_sec ));
#endif
	pthread_mutex_unlock(&mutexTemp);  
	
	
	pthread_mutex_lock(&mutexPot);
#if PRINT_TIME_MUTEX_TCOMM
	//struct timespec ts1, ts2;
	clock_gettime( CLOCK_REALTIME, &ts1 );
#endif
	printf("\n¡pot_ADC! %d",pot_ADC);
#if PRINT_TIME_MUTEX_TCOMM
	clock_gettime( CLOCK_REALTIME, &ts2);
	printf("demora Comm mutexPot: %f\n", (float) (1.0 * (1.0 * ts2.tv_nsec - ts1.tv_nsec * 1.0) * 1e-9 + 1.0 * ts2.tv_sec - 1.0 * ts1.tv_sec ));
#endif
	pthread_mutex_unlock(&mutexPot); 
	
	pthread_mutex_lock(&mutexLuz);
#if PRINT_TIME_MUTEX_TCOMM
	//struct timespec ts1, ts2;
	clock_gettime( CLOCK_REALTIME, &ts1 );
#endif
	printf("\n¡LUZ! %d",luz_ADC);
#if PRINT_TIME_MUTEX_TCOMM
	clock_gettime( CLOCK_REALTIME, &ts2);
	printf("demora Comm mutexLuz: %f\n", (float) (1.0 * (1.0 * ts2.tv_nsec - ts1.tv_nsec * 1.0) * 1e-9 + 1.0 * ts2.tv_sec - 1.0 * ts1.tv_sec ));
#endif
	pthread_mutex_unlock(&mutexLuz); 
#if PRINT_TIME
	clock_gettime( CLOCK_REALTIME, &ts2);
	printf("demora t4_Comm: %f\n", (float) (1.0 * (1.0 * ts2.tv_nsec - ts1.tv_nsec * 1.0) * 1e-9 + 1.0 * ts2.tv_sec - 1.0 * ts1.tv_sec ));
#endif

}

void t5_Riesgo(void)
{
#if PRINT_TIME
	struct timespec ts1, ts2;
	clock_gettime( CLOCK_REALTIME, &ts1 );
#endif    
	pthread_mutex_lock(&mutexTemp);
#if PRINT_TIME_MUTEX_TRIESGO
	struct timespec ts1, ts2;
	clock_gettime( CLOCK_REALTIME, &ts1 );
#endif
	if(temperatura>200)
	{
	   	peligro_temp=TRUE;
		
	}
	else
	{
	   peligro_temp=FALSE;
	}
#if PRINT_TIME_MUTEX_TRIESGO
	clock_gettime( CLOCK_REALTIME, &ts2);
	printf("demora Riesgo mutexTemp: %f\n", (float) (1.0 * (1.0 * ts2.tv_nsec - ts1.tv_nsec * 1.0) * 1e-9 + 1.0 * ts2.tv_sec - 1.0 * ts1.tv_sec ));
#endif
	pthread_mutex_unlock(&mutexTemp);  
	
	
	pthread_mutex_lock(&mutexPot);
#if PRINT_TIME_MUTEX_TRIESGO
	//struct timespec ts1, ts2;
	clock_gettime( CLOCK_REALTIME, &ts1 );
#endif
	if(pot_ADC>300)
	{
	   	peligro_pot=TRUE;
	}
	else
	{
	   	peligro_pot=FALSE;
	}
#if PRINT_TIME_MUTEX_TRIESGO
	clock_gettime( CLOCK_REALTIME, &ts2);
	printf("demora Riesgo mutexPot: %f\n", (float) (1.0 * (1.0 * ts2.tv_nsec - ts1.tv_nsec * 1.0) * 1e-9 + 1.0 * ts2.tv_sec - 1.0 * ts1.tv_sec ));
#endif
	pthread_mutex_unlock(&mutexPot); 
	
	pthread_mutex_lock(&mutexLuz);

#if PRINT_TIME_MUTEX_TRIESGO
	//struct timespec ts1, ts2;
	clock_gettime( CLOCK_REALTIME, &ts1 );
#endif
	if(luz_ADC>520)
	{
	   	peligro_luz=TRUE;
	}
	else
	{
	   	peligro_luz=FALSE;
	}
#if PRINT_TIME_MUTEX_TRIESGO
	clock_gettime( CLOCK_REALTIME, &ts2);
	printf("demora Riesgo mutexLuz: %f\n", (float) (1.0 * (1.0 * ts2.tv_nsec - ts1.tv_nsec * 1.0) * 1e-9 + 1.0 * ts2.tv_sec - 1.0 * ts1.tv_sec ));
#endif
	pthread_mutex_unlock(&mutexLuz); 
	
	
	if(peligro_temp==1)
		printf("\n¡PELIGRO TEMP!");
	if(peligro_luz==1)
		printf("\n¡PELIGRO LUZ!");
	if(peligro_pot==1)
		printf("\n ¡PELIGRO POT!"); 

#if PRINT_TIME
	clock_gettime( CLOCK_REALTIME, &ts2);
	printf("demora t5_Riesgo: %f\n", (float) (1.0 * (1.0 * ts2.tv_nsec - ts1.tv_nsec * 1.0) * 1e-9 + 1.0 * ts2.tv_sec - 1.0 * ts1.tv_sec ));
#endif


}

/* -------------------- */
/* body of main program */
/* -------------------- */

int main(int argc, char *argv[])

{
  int err;
  void *returnvalue;
  pthread_t second_id, third_id, fourth_id,fifth_id,sixth_id;
  int p_min;
  int p_luz=0;
  int p_comm=0;
  int p_temp=3;
  int p_pot=3;
  int p_riesgo=5;

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

  p_min = sched_get_priority_min(SCHED_FIFO);
  p_luz+=p_min;
  p_temp+=p_min;
  p_pot=p_min;
  p_riesgo+=p_min;
  p_comm+=p_min;

  //-------- creation of mutex 

  pthread_mutexattr_init(&mymutexattr);

  if (pthread_mutexattr_setprotocol (&mymutexattr, PTHREAD_PRIO_PROTECT) != 0) {
    perror ("Error in mutex attribute setprotocol \n");
  }

  //-------- ceiling priority of mutex 

  if (pthread_mutexattr_setprioceiling (&mymutexattr, p_riesgo) != 0) {
    perror ("Error in mutex attribute setprotocol \n");
  }

  //-------- init mutex 
  if (pthread_mutex_init (&mutexLuz,&mymutexattr) != 0) {
    perror ("Error in mutex init");
  }


  //-------- init mutex 
  if (pthread_mutex_init (&mutexPot,&mymutexattr) != 0) {
    perror ("Error in mutex init");
  }


  //-------- init mutex 
  if (pthread_mutex_init (&mutexTemp,&mymutexattr) != 0) {
    perror ("Error in mutex init");
  }

  pthread_mutexattr_destroy(&mymutexattr);

  //--------  creation and activation of the new thread 
  sp.sched_priority =  p_comm;
  err = pthread_attr_setschedparam(&attrs, &sp);
  if (err != 0) {
    perror("setschedparam");
  }
  err = pthread_create(&sixth_id, &attrs, thread_COMM, (void *)NULL);
  if (err != 0) {
    fprintf(stderr, "Cannot create thread_COMM");
  }


  //--------  creation and activation of the new thread 
  sp.sched_priority =  p_luz;
  err = pthread_attr_setschedparam(&attrs, &sp);
  if (err != 0) {
    perror("setschedparam");
  }
  err = pthread_create(&second_id, &attrs, thread_LUZ, (void *)NULL);
  if (err != 0) {
    fprintf(stderr, "Cannot create thread_LUZ");
  }


  sp.sched_priority = p_pot;
  err = pthread_attr_setschedparam(&attrs, &sp);
  if (err != 0) {
    perror("setschedparam");
  }
  err = pthread_create(&third_id, &attrs, thread_POT, (void *)NULL);
  if (err != 0) {
    fprintf(stderr, "Cannot create thread_POT");
  }


  sp.sched_priority = p_temp;
  err = pthread_attr_setschedparam(&attrs, &sp);
  if (err != 0) {
    perror("setschedparam");
  }
  err = pthread_create(&fourth_id, &attrs, thread_TEMP, (void *)NULL);
  if (err != 0) {
    fprintf(stderr, "Cannot create thread_TEMP");
  }

 sp.sched_priority = p_riesgo;
  err = pthread_attr_setschedparam(&attrs, &sp);
  if (err != 0) {
    perror("setschedparam");
  }
  err = pthread_create(&fifth_id, &attrs, thread_RIESGO, (void *)NULL);
  if (err != 0) {
    fprintf(stderr, "Cannot create thread_RIESGO");
  }



  //-------- We wait the end of the threads we just created. 
  pthread_join(second_id, &returnvalue);
  pthread_join(third_id, &returnvalue);
  pthread_join(fourth_id, &returnvalue);
  pthread_join(fifth_id, &returnvalue);


  printf("main: returnvalue is %d\n", (int)returnvalue);

  return 0;
}

