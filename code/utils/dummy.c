#include <stdio.h>
#include <unistd.h>

int main()
{
double x = 56.5;
int y = 8;
long z = 330;

printf("pid: %d\n", (int) getpid());
printf("addr: %lu\n", (long) &x);
printf("addr: %lu\n", (long) &y);
printf("addr: %lu\n", (long) &z);


while(1)
{	
	getchar();
	y++;
	x+=0.15;
	printf("x:%lf, y:%d, z:%lu", x, y, z);
}
}
