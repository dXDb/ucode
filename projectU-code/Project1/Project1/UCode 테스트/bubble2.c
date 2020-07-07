#include <stdio.h>

int list[100];

int read_element(void);
void bubble_sort(int);

void main (void)
{
	int total, i;
  
  total = read_element();
  bubble_sort(total);
  i = 0;
	while (i < total) 
		printf("%d ", list[i++]);
	printf("\n");
  return;
}

int read_element(void)
{
  int element, i;
  
	i = 0;
	scanf("%d", &element);
	while (element != 0){   // 0: end of input
		list[i] = element;
		++i;
		scanf("%d", &element);
	}
	return i;
}

void bubble_sort(int n)
{
  int i, top, temp;
  
	top = n - 1;
	while (top > 1) {
		for (i = 0; i < top; ++i) 
			if (list[i] > list[i+1]) {
			  temp = list[i];
			  list[i] = list[i+1];
			  list[i+1] = temp;
			}
		top--;
	}
	return;
}
