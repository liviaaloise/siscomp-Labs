#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#define MAX 20
int main (int argc, char *argv[])
{
	struct stat sb;
	off_t len;
	char *p;
    int fd;
    int i,ini;
    
	if (argc < 2) { 
		fprintf (stderr, "usage: %s <file>\n",
		argv[0]); exit(1); 
	}
	
	fd = open (argv[1], O_RDWR);
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


	p = mmap (0, sb.st_size+MAX, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	
	if (p == MAP_FAILED) { 
		perror ("mmap"); 
		exit(1);
	}

	if (close (fd) == -1) {
		 perror ("close"); 
		 exit(1);
	}
	
    ini=sb.st_size;
    
    for(i=0;i<MAX;i++){
        p[ini+i]='F'+i;
    }
    
	
	for (len = 0; len < sb.st_size+MAX; len++)
		putchar (p[len]);
    printf("\n");

	if (munmap (p, sb.st_size) == -1) {
		perror ("munmap");
		exit(1); 
	} 
	return 0;
}

/*
 
 gcc -o ex5 ex5.c
 MBP-de-Livia:LAB8 liviaaloise$ ./ex5 a.txt
 ABCDEFGHIJKLMNOPQRSTUVWXY
 
*/
