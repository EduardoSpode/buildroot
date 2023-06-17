#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <linux/sched.h>
#include <semaphore.h>

// Buffer global
char* buffer;
int ponter = 0;

int buffer_size;

//Semaforo
pthread_mutex_t semaforo;

// Barreira
pthread_barrier_t semaforo_wait;

void *run(void *data)
{
	int id = (int) data;
	char letter = 0x41 + id;
	pthread_barrier_wait(&semaforo_wait);
	while(1)
	{	
		pthread_mutex_lock(&semaforo);
		if(ponter >= buffer_size){
			pthread_mutex_unlock(&semaforo);
			return 0;
		}
		buffer[ponter] = letter;
		ponter++;
		pthread_mutex_unlock(&semaforo);
	}

	return 0;
}

void print_sched(int policy)
{
	int priority_min, priority_max;

	switch(policy){
		case SCHED_DEADLINE:
			printf("SCHED_DEADLINE");
			break;
		case SCHED_FIFO:
			printf("SCHED_FIFO");
			break;
		case SCHED_RR:
			printf("SCHED_RR");
			break;
		case SCHED_NORMAL:
			printf("SCHED_OTHER");
			break;
		case SCHED_BATCH:
			printf("SCHED_BATCH");
			break;
		case SCHED_IDLE:
			printf("SCHED_IDLE");
			break;
		default:
			printf("unknown\n");
	}
	priority_min = sched_get_priority_min(policy);
	priority_max = sched_get_priority_max(policy);
	printf(" PRI_MIN: %d PRI_MAX: %d\n", priority_min, priority_max);
}

int setpriority(pthread_t *thr, int newpolicy, int newpriority)
{
	int policy, ret;
	struct sched_param param;

	if (newpriority > sched_get_priority_max(newpolicy) || newpriority < sched_get_priority_min(newpolicy)){
		printf("Invalid priority: MIN: %d, MAX: %d", sched_get_priority_min(newpolicy), sched_get_priority_max(newpolicy));

		return -1;
	}

	pthread_getschedparam(*thr, &policy, &param);
	printf("current: ");
	print_sched(policy);

	param.sched_priority = newpriority;
	
	ret = pthread_setschedparam(*thr, newpolicy, &param);
	if (ret != 0)
		perror("perror(): ");

	pthread_getschedparam(*thr, &policy, &param);
	printf("new: ");
	print_sched(policy);

	return 0;
}

int main(int argc, char **argv)
{

	pthread_mutex_init(&semaforo, NULL);
	pthread_barrier_init(&semaforo_wait, NULL, atoi(argv[1]));

	int N_THREADS = 0;
	int POLICY_PRIORITY = 0;

	if (argc < 4){
		printf("usage: %s <número_de_threads> <tamanho_do_buffer_global_em_kilobytes> <politica> <prioridade>\n\n", argv[0]);
		return 0;
	}

	N_THREADS = atoi(argv[1]);
	buffer_size = atoi(argv[2]) * 1024;
	POLICY_PRIORITY = atoi(argv[4]);

	int sched;
	if(strcmp(argv[3],"SCHED_FIFO")==0) sched = SCHED_FIFO;
	else if(strcmp(argv[3],"SCHED_RR")==0) sched = SCHED_RR;
	else if(strcmp(argv[3],"SCHED_IDLE")==0) sched = SCHED_IDLE;
	else if(strcmp(argv[3],"SCHED_OTHER")==0) sched = SCHED_OTHER;
	else if(strcmp(argv[3],"SCHED_BATCH")==0) sched = SCHED_BATCH;
	else if(strcmp(argv[3],"SCHED_DEADLINE")==0) sched = SCHED_DEADLINE;
	

	pthread_t thr[N_THREADS];

	// buffer init 
	buffer = (char *) malloc(buffer_size);
	memset(buffer, 0, buffer_size);

	for(int i = 0; i < N_THREADS; i++){
		pthread_create(&thr[i], NULL, run, (void *) i);
		setpriority(&thr[i], sched, POLICY_PRIORITY);
	}

	for(int i = 0; i < N_THREADS; i++){
			pthread_join(thr[i], NULL);
	}

	pthread_barrier_destroy(&semaforo_wait);
	pthread_mutex_destroy(&semaforo);

	//Mostra a sequencia
	// for(int i = 0; i < buffer_size; i++){
	// 	printf("%c", buffer[i]);
	// }


	int* thread_rep_counter = (int*) malloc(N_THREADS * sizeof(int));
	memset(thread_rep_counter,0,N_THREADS);

	//Mostra a quantidade sem repeticoes
	for(int i = 0; i < buffer_size; i++){
		int index = (int)buffer[i]-0x41;
		if(i==0){
			thread_rep_counter[index] = thread_rep_counter[index] + 1;
		} 

		if(i > 0 && buffer[i] != buffer[i-1]){
			thread_rep_counter[index] = thread_rep_counter[index] + 1;
		}
	}
	
	printf("\n\n");
	for(int i = 0; i < N_THREADS; i++){
		printf("%c = %d\n", i+0x41 ,thread_rep_counter[i]);
	}

	return 0;
}

