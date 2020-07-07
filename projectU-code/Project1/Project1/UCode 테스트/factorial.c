/*
  재귀 호출 방식으로 팩토리얼 값 계산
*/

#include <stdio.h>

int factorial (int n) {
	if (n == 1)
		return 1;
	else
		return n*factorial(n-1);
}

void main (void)
{
	int n, f;
	
	scanf("%d", &n);
	f = factorial(n);
	printf("%d ", f);
	printf("\n");
	return;
}

