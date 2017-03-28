/*
 * Implement 3 periodic threads, with periods 50ms, 100ms, and 150ms.
 */

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <stdint.h>
#include <wiringPi.h> // Include WiringPi library!
#include <unistd.h>

#include "periodic_tasks.h"
#include "dispositivos.h"
 
static pthread_mutex_t mutexLuz;
static pthread_mutex_t mutexPot;
static pthread_mutex_t mutexTemp;
static pthread_mutex_t mutexColision;
static pthread_mutex_t mutexMotorDerecho;

//consignas de sensores
int luz_ADC; 	//lectura del sensor de luz
int pot_ADC;	//lectura del sensor de humedad
int temp_ADC;	//ectura del sensor de temperatura de motor

int colision;
//consignas de navegacion
// suponemos que va a seguir recto


//control del motor
int velocidad_dcho;

int peligro_pot;
int peligro_temp;
int peligro_luz;

int v_motor_derecho;

#define TRUE 1
#define FALSE 0


#define PRINT_TIME 0
#define PRINT_TIME_MUTEX_SINGLE  0
#define PRINT_TIME_MUTEX_TCOMM 	 0
#define PRINT_TIME_MUTEX_TRIESGO 0


#define TH_MOTOR 	1
#define TH_NAVEGA 	1
#define TH_COMM		1
#define TH_LUZ		1
#define TH_POT		1
#define TH_TEMP		1
#define TH_RIESGO	1

#define TIME_SCALER		100
#define PERIOD_LUZ		TIME_SCALER * 300
#define PERIOD_POTEN	TIME_SCALER * 500
#define PERIOD_TEMP		TIME_SCALER * 1000
#define PERIOD_COMM		TIME_SCALER * 3000
#define PERIOD_RIESGO	TIME_SCALER * 1000
#define PERIOD_NAVEGA	TIME_SCALER * 100
#define PERIOD_MOTOR	TIME_SCALER * 20

#define BUTTON_PIN 3


/* periodic threads */

void *thread_LUZ(void *param)
{
  struct periodic_task *p_d;

  p_d = start_periodic_timer(0, PERIOD_LUZ);
  while (1) {
    wait_next_activation(p_d);
	t1_LecFotometro();
	
  }
}

void *thread_POT(void *param)
{
  struct periodic_task *p_d;

  p_d = start_periodic_timer(0, PERIOD_POTEN);
  while (1) {
    wait_next_activation(p_d);
    t2_LecPotenciometro();
  }
}

void *thread_TEMP(void *param)
{
  struct periodic_task *p_d;

  p_d = start_periodic_timer(0, PERIOD_TEMP);
  while (1) {
    wait_next_activation(p_d);
    t3_LecTemperatura();
  }
}

void *thread_RIESGO(void *param)
{
  struct periodic_task *p_d;

  p_d = start_periodic_timer(0, PERIOD_RIESGO);
  while (1) {
    wait_next_activation(p_d);
    t5_Riesgo();
  }
}

void *thread_COMM(void *param)
{
  struct periodic_task *p_d;

  p_d = start_periodic_timer(0, PERIOD_COMM);
  while (1) {
    wait_next_activation(p_d);
    t4_Comm();
  }
}

void *thread_NAVEGA(void *param)
{
  struct periodic_task *p_d;

  p_d = start_periodic_timer(0, PERIOD_NAVEGA);
  while (1) {
    wait_next_activation(p_d);
    t7_Navegacion();
  }
}

void *thread_COLI(void *param)
{
    t6_colison();
}

void *thread_MOTOR(void *param)
{
  struct periodic_task *p_d;

  p_d = start_periodic_timer(0, PERIOD_MOTOR);
  while (1) {
    wait_next_activation(p_d);
    t8_SetMotor();
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
	printf("\ndemora LecFotometro mutexLuz: %f\n", (float) (1.0 * (1.0 * ts2.tv_nsec - ts1.tv_nsec * 1.0) * 1e-9 + 1.0 * ts2.tv_sec - 1.0 * ts1.tv_sec ));
#endif
	pthread_mutex_unlock(&mutexLuz);

#if PRINT_TIME
	clock_gettime( CLOCK_REALTIME, &ts2);
	printf("\ndemora t1_LecFotometro: %f\n", (float) (1.0 * (1.0 * ts2.tv_nsec - ts1.tv_nsec * 1.0) * 1e-9 + 1.0 * ts2.tv_sec - 1.0 * ts1.tv_sec ));
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
	printf("\ndemora LecPotenciometro mutexPot: %f\n", (float) (1.0 * (1.0 * ts2.tv_nsec - ts1.tv_nsec * 1.0) * 1e-9 + 1.0 * ts2.tv_sec - 1.0 * ts1.tv_sec ));
#endif
	pthread_mutex_unlock(&mutexPot);
 
#if PRINT_TIME
	clock_gettime( CLOCK_REALTIME, &ts2);
	printf("\ndemora t2_LecPotenciometro: %f\n", (float) (1.0 * (1.0 * ts2.tv_nsec - ts1.tv_nsec * 1.0) * 1e-9 + 1.0 * ts2.tv_sec - 1.0 * ts1.tv_sec ));
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
	temp_ADC=analogRead(2);
#if PRINT_TIME_MUTEX_SINGLE
	clock_gettime( CLOCK_REALTIME, &ts2);
	printf("\ndemora LecTemperatura mutexTemp: %f\n", (float) (1.0 * (1.0 * ts2.tv_nsec - ts1.tv_nsec * 1.0) * 1e-9 + 1.0 * ts2.tv_sec - 1.0 * ts1.tv_sec ));
#endif
	pthread_mutex_unlock(&mutexTemp);  

#if PRINT_TIME
	clock_gettime( CLOCK_REALTIME, &ts2);
	printf("\ndemora t3_LecTemperatura: %f\n", (float) (1.0 * (1.0 * ts2.tv_nsec - ts1.tv_nsec * 1.0) * 1e-9 + 1.0 * ts2.tv_sec - 1.0 * ts1.tv_sec ));
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
	printf("\n¡temp_ADC! %d",temp_ADC);
#if PRINT_TIME_MUTEX_TCOMM
	clock_gettime( CLOCK_REALTIME, &ts2);
	printf("\ndemora Comm mutexTemp: %f\n", (float) (1.0 * (1.0 * ts2.tv_nsec - ts1.tv_nsec * 1.0) * 1e-9 + 1.0 * ts2.tv_sec - 1.0 * ts1.tv_sec ));
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
	printf("\ndemora Comm mutexPot: %f\n", (float) (1.0 * (1.0 * ts2.tv_nsec - ts1.tv_nsec * 1.0) * 1e-9 + 1.0 * ts2.tv_sec - 1.0 * ts1.tv_sec ));
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
	printf("\ndemora Comm mutexLuz: %f\n", (float) (1.0 * (1.0 * ts2.tv_nsec - ts1.tv_nsec * 1.0) * 1e-9 + 1.0 * ts2.tv_sec - 1.0 * ts1.tv_sec ));
#endif
	pthread_mutex_unlock(&mutexLuz); 
#if PRINT_TIME
	clock_gettime( CLOCK_REALTIME, &ts2);
	printf("\ndemora t4_Comm: %f\n", (float) (1.0 * (1.0 * ts2.tv_nsec - ts1.tv_nsec * 1.0) * 1e-9 + 1.0 * ts2.tv_sec - 1.0 * ts1.tv_sec ));
#endif	

	pthread_mutex_lock(&mutexColision);
#if PRINT_TIME_MUTEX_TCOMM
	//struct timespec ts1, ts2;
	clock_gettime( CLOCK_REALTIME, &ts1 );
#endif
	printf("\n¡Colision! %d",colision);
#if PRINT_TIME_MUTEX_TCOMM
	clock_gettime( CLOCK_REALTIME, &ts2);
	printf("\ndemora Comm mutexColision: %f\n", (float) (1.0 * (1.0 * ts2.tv_nsec - ts1.tv_nsec * 1.0) * 1e-9 + 1.0 * ts2.tv_sec - 1.0 * ts1.tv_sec ));
#endif
	pthread_mutex_unlock(&mutexColision); 
#if PRINT_TIME
	clock_gettime( CLOCK_REALTIME, &ts2);
	printf("\ndemora t4_Comm: %f\n", (float) (1.0 * (1.0 * ts2.tv_nsec - ts1.tv_nsec * 1.0) * 1e-9 + 1.0 * ts2.tv_sec - 1.0 * ts1.tv_sec ));
#endif

	pthread_mutex_lock(&mutexMotorDerecho);
#if PRINT_TIME_MUTEX_SINGLE
	struct timespec ts1, ts2;
	clock_gettime( CLOCK_REALTIME, &ts1 );
#endif
	printf("\n¡v_motor_derecho! %d",v_motor_derecho);
#if PRINT_TIME_MUTEX_SINGLE
	clock_gettime( CLOCK_REALTIME, &ts2);
	printf("\ndemora t4_Comm mutexMotorDerecho: %f\n", (float) (1.0 * (1.0 * ts2.tv_nsec - ts1.tv_nsec * 1.0) * 1e-9 + 1.0 * ts2.tv_sec - 1.0 * ts1.tv_sec ));
#endif
	pthread_mutex_unlock(&mutexMotorDerecho);  

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
	if( temp_ADC>200 || temp_ADC<75)
	{
	   	peligro_temp=TRUE;

	}
	else
	{
	   peligro_temp=FALSE;
	}
#if PRINT_TIME_MUTEX_TRIESGO
	clock_gettime( CLOCK_REALTIME, &ts2);
	printf("\ndemora Riesgo mutexTemp: %f\n", (float) (1.0 * (1.0 * ts2.tv_nsec - ts1.tv_nsec * 1.0) * 1e-9 + 1.0 * ts2.tv_sec - 1.0 * ts1.tv_sec ));
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
	printf("\ndemora Riesgo mutexPot: %f\n", (float) (1.0 * (1.0 * ts2.tv_nsec - ts1.tv_nsec * 1.0) * 1e-9 + 1.0 * ts2.tv_sec - 1.0 * ts1.tv_sec ));
#endif
	pthread_mutex_unlock(&mutexPot); 
	
	pthread_mutex_lock(&mutexLuz);

#if PRINT_TIME_MUTEX_TRIESGO
	//struct timespec ts1, ts2;
	clock_gettime( CLOCK_REALTIME, &ts1 );
#endif
	if(luz_ADC<200)
	{
	   	peligro_luz=TRUE;
	}
	else
	{
	   	peligro_luz=FALSE;
	}
#if PRINT_TIME_MUTEX_TRIESGO
	clock_gettime( CLOCK_REALTIME, &ts2);
	printf("\ndemora Riesgo mutexLuz: %f\n", (float) (1.0 * (1.0 * ts2.tv_nsec - ts1.tv_nsec * 1.0) * 1e-9 + 1.0 * ts2.tv_sec - 1.0 * ts1.tv_sec ));
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

void t6_colison ()
{
#if PRINT_TIME
	struct timespec ts1, ts2;
	clock_gettime( CLOCK_REALTIME, &ts1 );
#endif

	pthread_mutex_lock(&mutexColision);
#if PRINT_TIME_MUTEX_SINGLE
	struct timespec ts1, ts2;
	clock_gettime( CLOCK_REALTIME, &ts1 );
#endif
	colision=!colision;
	//printf("\n¡Colision! %d",colision);
#if PRINT_TIME_MUTEX_SINGLE
	clock_gettime( CLOCK_REALTIME, &ts2);
	printf("\ndemora t6_colison mutexColision: %f\n", (float) (1.0 * (1.0 * ts2.tv_nsec - ts1.tv_nsec * 1.0) * 1e-9 + 1.0 * ts2.tv_sec - 1.0 * ts1.tv_sec ));
#endif
	pthread_mutex_unlock(&mutexColision);  

#if PRINT_TIME
	clock_gettime( CLOCK_REALTIME, &ts2);
	printf("\ndemora t6_colison : %f\n", (float) (1.0 * (1.0 * ts2.tv_nsec - ts1.tv_nsec * 1.0) * 1e-9 + 1.0 * ts2.tv_sec - 1.0 * ts1.tv_sec ));
#endif
}

void t7_Navegacion ()
{

	int local_peligro_pot;
	int local_peligro_luz;
	int local_peligro_temp;
	int local_peligro_colision;
	
	int local_peligro;
	int local_velocidad_motor_derecho;
//---------------------
#if PRINT_TIME
	struct timespec ts1, ts2;
	clock_gettime( CLOCK_REALTIME, &ts1 );
#endif
    
	pthread_mutex_lock(&mutexTemp);
#if PRINT_TIME_MUTEX_NAVEGACION
	struct timespec ts1, ts2;
	clock_gettime( CLOCK_REALTIME, &ts1 );
#endif

	local_peligro_temp=peligro_temp;
	
#if PRINT_TIME_MUTEX_NAVEGACION
	clock_gettime( CLOCK_REALTIME, &ts2);
	printf("\ndemora t7_Navegacion mutexTemp: %f\n", (float) (1.0 * (1.0 * ts2.tv_nsec - ts1.tv_nsec * 1.0) * 1e-9 + 1.0 * ts2.tv_sec - 1.0 * ts1.tv_sec ));
#endif
	pthread_mutex_unlock(&mutexTemp);  
//---------------------
	pthread_mutex_lock(&mutexPot);
#if PRINT_TIME_MUTEX_NAVEGACION
	//struct timespec ts1, ts2;
	clock_gettime( CLOCK_REALTIME, &ts1 );
#endif

	local_peligro_pot=peligro_pot;
	
#if PRINT_TIME_MUTEX_NAVEGACION
	clock_gettime( CLOCK_REALTIME, &ts2);
	printf("\ndemora t7_Navegacion mutexPot: %f\n", (float) (1.0 * (1.0 * ts2.tv_nsec - ts1.tv_nsec * 1.0) * 1e-9 + 1.0 * ts2.tv_sec - 1.0 * ts1.tv_sec ));
#endif
	pthread_mutex_unlock(&mutexPot); 
//---------------------
	pthread_mutex_lock(&mutexLuz);

#if PRINT_TIME_MUTEX_NAVEGACION
	//struct timespec ts1, ts2;
	clock_gettime( CLOCK_REALTIME, &ts1 );
#endif

	local_peligro_luz=peligro_luz;
	
#if PRINT_TIME_MUTEX_TRIESGO
	clock_gettime( CLOCK_REALTIME, &ts2);
	printf("\ndemora t7_Navegacion mutexLuz: %f\n", (float) (1.0 * (1.0 * ts2.tv_nsec - ts1.tv_nsec * 1.0) * 1e-9 + 1.0 * ts2.tv_sec - 1.0 * ts1.tv_sec ));
#endif
	pthread_mutex_unlock(&mutexLuz); 

//--------------------- 
	pthread_mutex_lock(&mutexColision);
#if PRINT_TIME_MUTEX_SINGLE
	struct timespec ts1, ts2;
	clock_gettime( CLOCK_REALTIME, &ts1 );
#endif
	local_peligro_colision=colision;
#if PRINT_TIME_MUTEX_SINGLE
	clock_gettime( CLOCK_REALTIME, &ts2);
	printf("\ndemora t7_Navegacion mutexColision: %f\n", (float) (1.0 * (1.0 * ts2.tv_nsec - ts1.tv_nsec * 1.0) * 1e-9 + 1.0 * ts2.tv_sec - 1.0 * ts1.tv_sec ));
#endif
	pthread_mutex_unlock(&mutexColision);  
//--------------------- 


	//printf("\n local_peligro_colision! %d",local_peligro_colision);
	//printf("\n local_peligro_luz! %d",local_peligro_luz);
	//printf("\n local_peligro_pot! %d",local_peligro_pot);
	//printf("\n local_peligro_temp! %d",local_peligro_temp);
	//printf("\n local_peligro! %d",local_peligro);

	local_peligro=local_peligro_temp + local_peligro_pot + local_peligro_luz;
	
	if(local_peligro_colision)
	{
		local_velocidad_motor_derecho=0;
	}
	else
	{
		switch (local_peligro)
		{
			case 0:
				local_velocidad_motor_derecho=100;
				break;
			case 1:
				local_velocidad_motor_derecho=70;
				break;
			case 2:
				local_velocidad_motor_derecho=40;
				break;
			case 3:
				local_velocidad_motor_derecho=10;
				break;
		}
	}


	pthread_mutex_lock(&mutexMotorDerecho);
#if PRINT_TIME_MUTEX_SINGLE
	struct timespec ts1, ts2;
	clock_gettime( CLOCK_REALTIME, &ts1 );
#endif
	v_motor_derecho=local_velocidad_motor_derecho;
	//printf("\n¡Colision! %d",colision);
#if PRINT_TIME_MUTEX_SINGLE
	clock_gettime( CLOCK_REALTIME, &ts2);
	printf("\ndemora t7_Navegacion mutexMotorDerecho: %f\n", (float) (1.0 * (1.0 * ts2.tv_nsec - ts1.tv_nsec * 1.0) * 1e-9 + 1.0 * ts2.tv_sec - 1.0 * ts1.tv_sec ));
#endif
	pthread_mutex_unlock(&mutexMotorDerecho);  

#if PRINT_TIME
	clock_gettime( CLOCK_REALTIME, &ts2);
	printf("\ndemora t7_Navegacion : %f\n", (float) (1.0 * (1.0 * ts2.tv_nsec - ts1.tv_nsec * 1.0) * 1e-9 + 1.0 * ts2.tv_sec - 1.0 * ts1.tv_sec ));
#endif
}

void t8_SetMotor(void)
{
	int local_sleep;
	int local_colision;
	unsigned int usecs; 
	
	//while (1)	
	//{

	#if PRINT_TIME
		struct timespec ts1, ts2;
		clock_gettime( CLOCK_REALTIME, &ts1 );
	#endif

		pthread_mutex_lock(&mutexMotorDerecho);
	#if PRINT_TIME_MUTEX_SINGLE
		struct timespec ts1, ts2;
		clock_gettime( CLOCK_REALTIME, &ts1 );
	#endif		
		local_sleep = v_motor_derecho;
		
	#if PRINT_TIME_MUTEX_SINGLE
		clock_gettime( CLOCK_REALTIME, &ts2);
		printf("\ndemora t8_SetMotor mutexTemp: %f\n", (float) (1.0 * (1.0 * ts2.tv_nsec - ts1.tv_nsec * 1.0) * 1e-9 + 1.0 * ts2.tv_sec - 1.0 * ts1.tv_sec ));
	#endif
		pthread_mutex_unlock(&mutexMotorDerecho);  

	#if PRINT_TIME
		clock_gettime( CLOCK_REALTIME, &ts2);
		printf("\ndemora t8_SetMotor: %f\n", (float) (1.0 * (1.0 * ts2.tv_nsec - ts1.tv_nsec * 1.0) * 1e-9 + 1.0 * ts2.tv_sec - 1.0 * ts1.tv_sec ));
	#endif
	
	
		pthread_mutex_lock(&mutexColision);
#if PRINT_TIME_MUTEX_SINGLE
	struct timespec ts1, ts2;
	clock_gettime( CLOCK_REALTIME, &ts1 );
#endif
	local_colision=colision;
	//printf("\n¡Colision! %d",colision);
#if PRINT_TIME_MUTEX_SINGLE
	clock_gettime( CLOCK_REALTIME, &ts2);
	printf("\ndemora t8_SetMotor mutexColision: %f\n", (float) (1.0 * (1.0 * ts2.tv_nsec - ts1.tv_nsec * 1.0) * 1e-9 + 1.0 * ts2.tv_sec - 1.0 * ts1.tv_sec ));
#endif
	pthread_mutex_unlock(&mutexColision); 
	
	
	if(local_colision)
	{
		Poner_Rojo (1);
		Poner_Verde (0);
		return;
	}
	//else 
	//{
		Poner_Rojo (0);
		Poner_Verde (1);
		if(local_sleep != 100)
		{
			usecs = (100 - local_sleep)*10000;
			
			Poner_Verde (0);
			//printf("\n motor OFF");
			usleep(usecs);
			
			Poner_Verde (1);
			//printf("\n motor ON");
			
			usleep(usecs);
		}
		else
		{
			Poner_Verde (1);
		}
#if PRINT_TIME
	clock_gettime( CLOCK_REALTIME, &ts2);
	printf("\ndemora t8_SetMotor : %f\n", (float) (1.0 * (1.0 * ts2.tv_nsec - ts1.tv_nsec * 1.0) * 1e-9 + 1.0 * ts2.tv_sec - 1.0 * ts1.tv_sec ));
#endif
}

void miTareaEsporadica (void)
{
  int err;
  void *returnvalue;
  struct sched_param sp;
  int p_coli=0;
  pthread_attr_t attrs;
  pthread_mutexattr_t mymutexattr;
  pthread_t thread_COLI_id;
  int prioridad_min;
  
  prioridad_min = sched_get_priority_min(SCHED_FIFO);

  //-------- init attributes of tasks 

  pthread_attr_init(&attrs);  
	  
  err = pthread_attr_setinheritsched(&attrs, PTHREAD_EXPLICIT_SCHED);
  if (err != 0) {
    perror("setineheritsched");
  }

  err = pthread_attr_setschedpolicy(&attrs, SCHED_FIFO);
  if (err != 0) {
    perror("setschedpolicy");
  }


	//--------  creation and activation of the new thread 
	sp.sched_priority =  p_coli;
	err = pthread_attr_setschedparam(&attrs, &sp);
	if (err != 0) {
		perror("setschedparam");
	}
	err = pthread_create(&thread_COLI_id, &attrs, thread_COLI, (void *)NULL);
	if (err != 0) {
		fprintf(stderr, "Cannot create thread_COLI");
	}
	
	
	pthread_join(thread_COLI_id, &returnvalue);
}


/* -------------------- */
/* body of main program */
/* -------------------- */

int main(int argc, char *argv[])

{
  int err;
  void *returnvalue;
  pthread_t thread_LUZ_id, thread_POT_id, thread_TEMP_id, thread_RIESGO_id, thread_COMM_id, thread_NAVEGA_id, thread_MOTOR_id;
  int prioridad_min;
  int p_luz=5;
  int p_pot=5;
  int p_temp=5;
  int p_comm=10;
  int p_riesgo=0;
  int p_coli=0;
  int p_navega=0;
  int p_motor=0;
  
  colision=FALSE;
  
  struct sched_param sp;
  pthread_attr_t attrs;
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

	//registro de la interrupcion
	  
	//if (wiringPiISR (BUTTON_PIN, INT_EDGE_FALLING, &miTareaEsporadica) < 0)
	if (wiringPiISR (BUTTON_PIN, INT_EDGE_BOTH , &miTareaEsporadica) < 0)
	{
		fprintf (stderr, "Unable to setup ISR: %s\n", strerror (errno)) ;
		return 1 ;
	}
  
  prioridad_min = sched_get_priority_min(SCHED_FIFO);
  p_luz+=prioridad_min;  	//t1
  p_pot=prioridad_min;		//t2
  p_temp+=prioridad_min;	//t3
  p_comm+=prioridad_min;	//t4
  p_riesgo+=prioridad_min;	//t5
  p_navega+=prioridad_min;	//t7
  p_motor+=prioridad_min;	//t8

  //-------- creation of mutex 

  pthread_mutexattr_init(&mymutexattr);

  if (pthread_mutexattr_setprotocol (&mymutexattr, PTHREAD_PRIO_PROTECT) != 0) {
    perror ("Error in mutex attribute setprotocol \n");
  }

  //-------- ceiling priority of mutex attr

  if (pthread_mutexattr_setprioceiling (&mymutexattr, p_riesgo) != 0) {
    perror ("Error in mutex attribute setprotocol \n");
  }

  //-------- init mutex mutexLuz
  if (pthread_mutex_init (&mutexLuz,&mymutexattr) != 0) {
    perror ("Error in mutex init");
  }


  //-------- init mutex mutexPot
  if (pthread_mutex_init (&mutexPot,&mymutexattr) != 0) {
    perror ("Error in mutex init");
  }


  //-------- init mutex mutexTemp
  if (pthread_mutex_init (&mutexTemp,&mymutexattr) != 0) {
    perror ("Error in mutex init");
  }
  //-------- init mutex mutexColision
  if (pthread_mutex_init (&mutexColision,&mymutexattr) != 0) {
    perror ("Error in mutex init");
  }
  //-------- init mutex mutexMotorDerecho
  if (pthread_mutex_init (&mutexMotorDerecho,&mymutexattr) != 0) {
    perror ("Error in mutex init");
  }

  pthread_mutexattr_destroy(&mymutexattr);
 
#if TH_MOTOR 
  //--------  creation and activation of the new thread 
  sp.sched_priority =  p_motor;
  err = pthread_attr_setschedparam(&attrs, &sp);
  if (err != 0) {
    perror("setschedparam");
  }
  err = pthread_create(&thread_MOTOR_id, &attrs, thread_MOTOR, (void *)NULL);
  if (err != 0) {
    fprintf(stderr, "Cannot create thread_MOTOR");
  } 
#endif

#if TH_NAVEGA  
  //--------  creation and activation of the new thread 
  sp.sched_priority =  p_navega;
  err = pthread_attr_setschedparam(&attrs, &sp);
  if (err != 0) {
    perror("setschedparam");
  }
  err = pthread_create(&thread_NAVEGA_id, &attrs, thread_NAVEGA, (void *)NULL);
  if (err != 0) {
    fprintf(stderr, "Cannot create thread_NAVEGA");
  }  
#endif  
  
#if TH_COMM  
  //--------  creation and activation of the new thread 
  sp.sched_priority =  p_comm;
  err = pthread_attr_setschedparam(&attrs, &sp);
  if (err != 0) {
    perror("setschedparam");
  }
  err = pthread_create(&thread_COMM_id, &attrs, thread_COMM, (void *)NULL);
  if (err != 0) {
    fprintf(stderr, "Cannot create thread_COMM");
  }
#endif

#if TH_LUZ  
  //--------  creation and activation of the new thread 
  sp.sched_priority =  p_luz;
  err = pthread_attr_setschedparam(&attrs, &sp);
  if (err != 0) {
    perror("setschedparam");
  }
  err = pthread_create(&thread_LUZ_id, &attrs, thread_LUZ, (void *)NULL);
  if (err != 0) {
    fprintf(stderr, "Cannot create thread_LUZ");
  }
#endif

#if TH_POT  
  //--------  creation and activation of the new thread 
  sp.sched_priority = p_pot;
  err = pthread_attr_setschedparam(&attrs, &sp);
  if (err != 0) {
    perror("setschedparam");
  }
  err = pthread_create(&thread_POT_id, &attrs, thread_POT, (void *)NULL);
  if (err != 0) {
    fprintf(stderr, "Cannot create thread_POT");
  }
#endif

#if TH_TEMP  
  //--------  creation and activation of the new thread 
  sp.sched_priority = p_temp;
  err = pthread_attr_setschedparam(&attrs, &sp);
  if (err != 0) {
    perror("setschedparam");
  }
  err = pthread_create(&thread_TEMP_id, &attrs, thread_TEMP, (void *)NULL);
  if (err != 0) {
    fprintf(stderr, "Cannot create thread_TEMP");
  }
#endif

#if TH_RIESGO  
  //--------  creation and activation of the new thread 
 sp.sched_priority = p_riesgo;
  err = pthread_attr_setschedparam(&attrs, &sp);
  if (err != 0) {
    perror("setschedparam");
  }
  err = pthread_create(&thread_RIESGO_id, &attrs, thread_RIESGO, (void *)NULL);
  if (err != 0) {
    fprintf(stderr, "Cannot create thread_RIESGO");
  }
#endif


  //-------- We wait the end of the threads we just created. 
  pthread_join(thread_LUZ_id, &returnvalue);
  pthread_join(thread_POT_id, &returnvalue);
  pthread_join(thread_TEMP_id, &returnvalue);
  pthread_join(thread_RIESGO_id, &returnvalue);
  pthread_join(thread_COMM_id, &returnvalue);
  pthread_join(thread_MOTOR_id, &returnvalue);


  printf("main: returnvalue is %d\n", (int)returnvalue);

  return 0;
}


