#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
int main (int argc, char *argv[])
{
	struct stat sb;
	off_t len;
	char *p, *d;
    int fd;
	struct timeval t0,t1;
	int tf, numPg, tamPg,i;
    
	if (argc < 2) { 
		fprintf (stderr, "usage: %s <file>\n",
		argv[0]); exit(1); 
	}
	
	fd = open (argv[1], O_RDONLY);
	if (fd == -1) { 
		perror ("open"); 
		exit(1); 
	} 
	if (fstat(fd, &sb)== -1) { 
		perror ("fstat"); 
		exit(1); 
	}
	
	if (!S_ISREG (sb.st_mode)) { 
		fprintf (stderr, "%s is not a file\n", argv[1]); 
		exit(1);
	}

	/*NUMERO DE PAGINAS*/
	
	tamPg= getpagesize();
	if (sb.st_size %tamPg!=0){
		numPg= sb.st_size /tamPg+1;
	}
	printf("Num Pag= %d\n",numPg);
    
     /*TEMPO DISCO*/
    
    d=(char*)malloc(sizeof(char)*sb.st_size);
    
    gettimeofday(&t0,NULL);
    read(fd,d,sizeof(char)*sb.st_size);
    for (i = 0; i < sb.st_size; i++) {
        putchar(d[i]);
    }
    gettimeofday(&t1,NULL);
    tf=(t1.tv_sec-t0.tv_sec) *1e6 + (t1.tv_usec-t0.tv_usec);
    printf("tempo disco: %d us\n",tf);
    
	
	/*TEMPO MEMORIA*/
	gettimeofday(&t0,NULL);
	p = mmap (0, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);
	gettimeofday(&t1,NULL);
	tf=(t1.tv_sec-t0.tv_sec) *1e6 + (t1.tv_usec-t0.tv_usec);
	printf("tempo memoria: %d us\n",tf);
	
	
	if (p == MAP_FAILED) { 
		perror ("mmap"); 
		exit(1);
	}

	if (close (fd) == -1) {
		 perror ("close"); 
		 exit(1);
	}
	
	/*
	for (len = 0; len < sb.st_size; len++) 
		putchar (p[len]);
	*/

	if (munmap (p, sb.st_size) == -1) {
		perror ("munmap");
		exit(1); 
	} 
	return 0;
}
