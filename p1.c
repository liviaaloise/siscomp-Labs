#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdlib.h>
int main (int argc, char *argv[])
{
    struct stat sb;
    off_t len;
    char *p; int fd,i;
    
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
        fprintf (stderr, "%s is nota file\n", argv[1]);
        exit(1);
    }
    
    p = mmap (0, sb.st_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    if (p == MAP_FAILED) {
        perror ("mmap");
        exit(1);
    }
    
    if (close (fd) == -1) {
        perror ("close");
        exit(1);
    }
    
    while(1){
        for (i=0;i<sb.st_size;i++){
            putchar(p[i]);
            fflush(stdout);
        }
        sleep(1);
    }
    
    if (munmap (p, sb.st_size) == -1) {
        perror ("munmap");
        exit(1);
    }
    return 0;
}

/*
 ./p1 a.txt
 tam: 5
 123451234512345123451234512345123451234512345123451234512345ABCDEABCDEABCDEABCDEABCDEABCDEABCDEABCDEABCDEABCDEABCDEABCDE
 
*/

