/*
  Palindrome:
  앞에서나 뒤에서나 같은 수들이 나열된 경우 수를 다시 출력,
  그렇지 않으면 무시.
  12321, -3553 등
*/

#include <stdio.h>

void main (void)
{
	int org, rev;
	int i, j;

	scanf("%d", &org);
	if (org < 0)
		org = (-1) * org;
	i = org;
	rev = 0;
	while (i != 0) {
		j = i % 10;
		rev = rev * 10 + j;
		i /= 10;
	}
	if (rev == org)
		printf("%d ", org);
	printf("\n");
	return;
}
