#include<stdio.h>
void main()
{
    int arr[100],i,s,c,es;
    printf("Enter the size of the array<100\n ");
    scanf("%d", &s);
    printf("Enter the elements of the array:\n");
    for(i=0;i<s;i++)
    {
        printf("arr[%d]=", i);
        scanf("%d", &arr[i]);
    }
    printf("The elements of the array are:\n");
    for(i=0;i<s;i++)
    {
        printf("%d\n", arr[i]);
    }
    printf("enter how many elements want to be extended            :\n ");
    scanf("%d",&es);
    c=s+es;
    if (c<100)

    {
        printf("Enter the new elements:\n");
        for (i = s; i < c; i++)
        {
            printf("arr[%d]=", i);
            scanf("%d", &arr[i]);
        }
        printf("data after extends:\n");
        for (i = 0; i < c; i++)
        {
            printf("%d\n", arr[i]);
        }
    }
}
