/*
 * Simple disc I/O generator
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <time.h>


#define BUFFER_LENGTH 512
#define DISK_SZ 1073741824
#define FORKS 8

int main()
{
	int ret, fd, pid, i;
	unsigned int pos;
	char buf[BUFFER_LENGTH];
	clock_t start, end;
    double cpu_time_used;

	start = clock();	

	printf("Starting sector read example...\n");

	printf("Cleaning disk cache...\n");
	system("echo 3 > /proc/sys/vm/drop_caches"); // limpa buffers e caches de disco

	printf("Configuring scheduling queues...\n");
	system("echo 2 > /sys/block/sdb/queue/nomerges");
	system("echo 4 > /sys/block/sdb/queue/max_sectors_kb"); 
	system("echo 0 > /sys/block/sdb/queue/read_ahead_kb");

	printf("Forking processes to put stress on disk scheduler...\n");
	for (int i = 0; i < FORKS; i++)
		fork();

	srand(getpid());

	fd = open("/dev/sdb", O_RDWR);
	if (fd < 0) {
		perror("Failed to open the device...");
		return errno;
	}

	for (i = 0; i < 100; i++) {
		pos = (rand() % (DISK_SZ >> 9));
		/* Set position */
		lseek(fd, pos * 512, SEEK_SET);
		/* Peform read. */
		read(fd, buf, 100);
	}
	close(fd);

	end = clock(); // Stop the timer

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC; // Calculate the execution time

    //printf("Execution Time: %f seconds\n", cpu_time_used);

	return 0;
}