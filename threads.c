#include <stdio.h>    // Used for printf() statements
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <wiringPi.h> // Include WiringPi library!

#include <pthread.h>

#include "dispositivos.h"

#include <time.h>

#define TRUE 1
#define FALSE 0

#define NUM_THREADS 3

void *t1 (){
int result_ADC;
while (1)
{
	result_ADC=analogRead(0);
  	//printf ("Sensor 1 Luz %d \n",result_ADC);
	if(result_ADC>520){
	Poner_Verde(1);
		}
	else{
	Poner_Verde(0);
	}
	usleep(300);
  
}
 
  pthread_exit (NULL);
}


void *t2 (){  

  int result_ADC;
while (1)
{
	result_ADC=analogRead(1);
  	//printf ("Sensor 2 Potenciometro %d \n",result_ADC);
	if(result_ADC>300){
	Poner_Rojo(1);
		}
	else{
	Poner_Rojo(0);
	}
	usleep(500);
  
}

  pthread_exit (NULL);
}

void *t3 (){

 int pulsado;
 int temperatura;
while (1)
{
	pulsado=Leer_Pulsador();
	temperatura=analogRead(2);
  	//printf ("Sensor 3 Pulsador y temperatura\n");
	if(pulsado==1){
	printf ("¡PULSADO! Temperatura: %d \n",temperatura);
		}
	else{
	printf ("¡NO PULSADO! Temperatura: %d \n",temperatura);
		}
	usleep(1000);
  
}
  pthread_exit (NULL);
}


int main(void)
{

    Inicializar_dispositivos ();

    pthread_t thread [NUM_THREADS]; 

    pthread_create(&thread[0], NULL, t1, NULL);
    pthread_create(&thread[1], NULL, t2, NULL);
    pthread_create(&thread[2], NULL, t3, NULL);

    pthread_join(thread[0], NULL);
    pthread_join(thread[1], NULL);
    pthread_join(thread[2], NULL);

    Cerrar_Dispositivos ();

    return (0);
}
