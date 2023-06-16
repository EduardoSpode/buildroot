#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <linux/sched.h>

//volatile int running = 1;
char *buffer = NULL;
int  pointer =0;

//Semaforo
pthread_mutex_t semaforo;

void *run(void *data)
{
	int id = (int) data;
	char letter = 0x41 + id;
	printf("Inicia thread %d %c \n", id, letter);

	pthread_mutex_lock(&semaforo);
	buffer[pointer] = letter;
	pointer++;
	pthread_mutex_unlock(&semaforo);
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

	int N_THREADS = 0;
	int BUFFER_SIZE = 0;
	int POLICY_TYPE = 0;
	int POLICY_PRIORITY = 0;

	if (argc < 5) {
		printf("usage: ./%s <numero_de_threads> <tamanho_do_buffer_global_em_kilobytes> <politica> <prioridade>\n\n", argv[0]);
		return 0;
	}

	N_THREADS = atoi(argv[1]);
	BUFFER_SIZE = atoi(argv[2]) * 1024;
	POLICY_TYPE = atoi(argv[3]);
	POLICY_PRIORITY = atoi(argv[4]);

	pthread_t thr[N_THREADS];
	buffer = (char*) malloc(sizeof(char) * BUFFER_SIZE) ;

	memset(buffer, 0, sizeof(char) * BUFFER_SIZE);


	printf("Inicio: %d %d %d %d\n", N_THREADS, BUFFER_SIZE, POLICY_TYPE, POLICY_PRIORITY);

	int prioridade = atoi(argv[4]);

	int sched = 0;

	if(strcmp(argv[3],"SCHED_FIFO")==0) sched = SCHED_FIFO;
	else if(strcmp(argv[3],"SCHED_RR")==0) sched = SCHED_RR;

	else if(strcmp(argv[3],"SCHED_DEADLINE")==0) sched = SCHED_DEADLINE;
	else if(strcmp(argv[3],"SCHED_OTHER")==0) sched = SCHED_OTHER;
	else if(strcmp(argv[3],"SCHED_BATCH")==0) sched = SCHED_BATCH;
	else if(strcmp(argv[3],"SCHED_IDLE")==0) sched = SCHED_IDLE;

	for(int i = 0; i < N_THREADS; i++) {
			pthread_create(&thr[i], NULL, run, (void*)i);
			setpriority(&thr, sched, prioridade);
			usleep(5);
	}

	for(int i = 0; i < N_THREADS; i++) {
			pthread_join(thr[i], NULL);
	}

	// for(int i = 0; i < BUFFER_SIZE; i++) {
	// 			printf("Buffer %c \n", buffer[i] );

	// }

	printf("Fim\n");

	pthread_mutex_destroy(&semaforo);
	free(buffer);
	return 0;
}

// #include <pthread.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <string.h>
// #include <linux/sched.h>
// #include <semaphore.h>

// #define BUFFER_SIZE 500

// volatile int running = 1;

// // Buffer global
// char* gbuffer;
// int gbuffer_index = 0;

// // Threads
// pthread_t* threads;
// int tagIndex= -1;
// int bufferSize;

// //Semaforo
// pthread_mutex_t lock;

// // Barreira
// pthread_barrier_t barrier;

// struct thread_data{
//    int  thread_id;
//    char tag;
// };

// void *run(void *data)
// {
// 	struct thread_data *my_data;
// 	my_data = (struct thread_data *) data;
// 	char tag = my_data->tag;
// 	//int id = my_data->thread_id;
// 	//printf("init %d ",my_data->thread_id);
// 	pthread_barrier_wait(&barrier);
// 	while(1)
// 	{	
// 		pthread_mutex_lock(&lock);
// 		//printf("%d-",id);
// 		if(gbuffer_index >= bufferSize){
// 			pthread_mutex_unlock(&lock);
// 			return 0;
// 		}
// 		gbuffer[gbuffer_index] = tag;
// 		gbuffer_index++;
// 		pthread_mutex_unlock(&lock);
// 	}

// 	return 0;
// }

// void print_sched(int policy)
// {
// 	int priority_min, priority_max;

// 	switch(policy){
// 		case SCHED_DEADLINE:
// 			printf("SCHED_DEADLINE");
// 			break;
// 		case SCHED_FIFO:
// 			printf("SCHED_FIFO");
// 			break;
// 		case SCHED_RR:
// 			printf("SCHED_RR");
// 			break;
// 		case SCHED_NORMAL:
// 			printf("SCHED_OTHER");
// 			break;
// 		case SCHED_BATCH:
// 			printf("SCHED_BATCH");
// 			break;
// 		case SCHED_IDLE:
// 			printf("SCHED_IDLE");
// 			break;
// 		default:
// 			printf("unknown\n");
// 	}
// 	priority_min = sched_get_priority_min(policy);
// 	priority_max = sched_get_priority_max(policy);
// 	printf(" PRI_MIN: %d PRI_MAX: %d\n", priority_min, priority_max);
// }

// int setpriority(pthread_t *thr, int newpolicy, int newpriority)
// {
// 	int policy, ret;
// 	struct sched_param param;

// 	if (newpriority > sched_get_priority_max(newpolicy) || newpriority < sched_get_priority_min(newpolicy)){
// 		printf("Invalid priority: MIN: %d, MAX: %d", sched_get_priority_min(newpolicy), sched_get_priority_max(newpolicy));

// 		return -1;
// 	}

// 	pthread_getschedparam(*thr, &policy, &param);
// 	printf("current: ");
// 	print_sched(policy);

// 	param.sched_priority = newpriority;
	
// 	printf("AQUI %d", newpolicy);
// 	ret = pthread_setschedparam(*thr, newpolicy, &param);
// 	if (ret != 0)
// 		perror("perror(): ");

// 	pthread_getschedparam(*thr, &policy, &param);
// 	printf("new: ");
// 	print_sched(policy);

// 	return 0;
// }

// int main(int argc, char **argv)
// {

// 	pthread_mutex_init(&lock, NULL);
// 	pthread_barrier_init(&barrier, NULL, atoi(argv[1]));

// 	if (argc < 4){
// 		printf("usage: %s <número_de_threads> <tamanho_do_buffer_global_em_kilobytes> <politica> <prioridade>\n\n", argv[0]);
// 		return 0;
// 	}

// 	int priority = atoi(argv[4]);

// 	int p;
// 	if(strcmp(argv[3],"SCHED_DEADLINE")==0) p = SCHED_DEADLINE;
// 	else if(strcmp(argv[3],"SCHED_FIFO")==0) p = SCHED_FIFO;
// 	else if(strcmp(argv[3],"SCHED_RR")==0) p = SCHED_RR;
// 	else if(strcmp(argv[3],"SCHED_OTHER")==0) p = SCHED_OTHER;
// 	else if(strcmp(argv[3],"SCHED_BATCH")==0) p = SCHED_BATCH;
// 	else if(strcmp(argv[3],"SCHED_IDLE")==0) p = SCHED_IDLE;
	
// 	//printf("%s - %d\n",argv[3],p);

// 	// buffer init
// 	bufferSize = atoi(argv[2]) * 1000;
// 	gbuffer = (char *) malloc(bufferSize);
// 	memset(gbuffer,0,bufferSize);

// 	// threads array init
// 	int nThreads = atoi(argv[1]);
// 	threads = (pthread_t *) malloc(nThreads * sizeof(pthread_t));
// 	memset(threads,0,nThreads);

// 	for(int i = 0; i < nThreads; i++){
// 		struct thread_data *t = malloc(sizeof(struct thread_data));
// 		int j = i;
// 		t->thread_id = j+1;
// 		t->tag = 0x41 + j;
// 		pthread_create(&threads[i], NULL, run, (void *) t);
// 		setpriority(&threads[i], p, priority);
// 	}

//     printf("\nAll threads set, go!\n");
// 	//pthread_barrier_wait(&barrier);

// 	for(int i = 0; i < nThreads; i++){
// 			pthread_join(threads[i], NULL);
// 	}

// 	printf("%d\n",gbuffer_index);
// 	printf("%s\n", gbuffer);
// 	for(int i = 0; i < bufferSize; i++){
// 		printf("%c", gbuffer[i]);
// 	}
	
// 	printf("\n\n");

// 	int* counter = (int*) malloc(nThreads * sizeof(int));
// 	memset(counter,0,nThreads);

// 	for(int i = 0; i < bufferSize; i++){
// 		int index = (int)gbuffer[i]-0x41;
// 		if(i==0){
// 			printf("%c", gbuffer[i]);
// 			counter[index] = counter[index] + 1;
// 		} 

// 		if(i > 0 && gbuffer[i] != gbuffer[i-1]){
// 			printf("%c", gbuffer[i]);
// 			counter[index] = counter[index] + 1;
// 		}
// 	}
	
// 	printf("\n\n");
// 	for(int i = 0; i < nThreads; i++){
// 		printf("%c = %d\n", i+0x41 ,counter[i]);
// 	}

// 	// free(gbuffer);
// 	// free(threads);
// 	// free(counter);

//     pthread_barrier_destroy(&barrier);
// 	pthread_mutex_destroy(&lock);

// 	return 0;
// }

