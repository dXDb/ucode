#include <stdio.h>

const int MAX = 10;

int sub(int, int[]); 

void main(void)
{
	int i;
	int a[10];
	int j;
	i = j = 0;
	while (i < MAX) {
		a[i] = j;
		a[i] = sub(i, a);
		j = j + a[i];
		++i;
	}
	printf("%d ", j);
	printf("\n");
	return;
}

int sub(int i, int a[]) {
	int j;
	
	scanf("%d", &j);
	j = j + a[i];
	return j;
}

