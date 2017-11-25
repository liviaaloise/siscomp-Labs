#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#define _GNU_SOURCE

void *memorypool;

void *myallocate( int n){
	return sbrk(n);
}

void initmemorypool(void) {
	memorypool = sbrk(0);
}

void resetmemorypool(void){
	brk(memorypool);
}

int main(){
	void *p1,*p2;
	initmemorypool();
	p1=myallocate(0);
	p2=myallocate(10);
	printf("%d\n", (int)p1);
	printf("%d\n", (int)p2);
	p2=myallocate(10);
	printf("%d\n", (int)p2);
	p2=myallocate(10);
	printf("%d\n", (int)p2);
	resetmemorypool();
	p1=myallocate(0);
	printf("%d\n", (int)p1);
	
	return 0;
}
