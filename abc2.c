#include<stdio.h>
void main()
{
 int a,b,c;
    printf("enter the value of a", &a);
    scanf("%d", &a);
    printf("enter the value of b", &b);
    scanf("%d", &b);
    c = a + b;
    printf("The sum of a and b is: %d", c);
}