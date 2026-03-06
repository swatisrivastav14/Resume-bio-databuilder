#include<stdio.h>
int main()
{
    int arr1[50],arr2[50],arr3[100],s1,s2,i;
    printf("enter the size of both arrays      \n");
    scanf("%d%d",&s1,&s2);
    printf("enter the elements of first array   \n");
    for(i=0;i<s1;i++)
    {
        scanf("%d",&arr1[i]);
    }
    printf("enter the elements of second array  \n");
    for(i=0;i<s2;i++)
    {
        scanf("%d",&arr2[i]);
    }
    printf("1st array elements are       \n");
    for(i=0;i<s1;i++)
    {
        printf("%d ",arr1[i]);
    }
     for(i=0;i<s2;i++)
    {
        printf("%d",arr2[i]);
    }
    for(i=0;i<(s1+s2);i++)
    {
        if(i<s1)
        {
            arr3[i]=arr1[i];
        }
        else
        {
            arr3[i]=arr2[i-s1];
        }
    }
    printf("Data after merging:     \n");
    for(i=0;i<(s1+s2);i++)
    {
        printf("%d\n",arr3[i]);
    }
}