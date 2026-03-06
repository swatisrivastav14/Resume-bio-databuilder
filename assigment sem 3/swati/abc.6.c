#include<stdio.h>
#include<stdlib.h>
struct node 
{
    int data;
    struct node *next;
};
struct node *start = NULL;
void create()
{
    char ch;
    struct node *new_node, *current;
    do
    {
        
        new_node = (struct node*)malloc(sizeof(struct node));
        if(!new_node)
        {
            printf("Memory allocation failed");
            return;
        }
        printf("\n enter the data :");
        scanf("%d",&new_node->data);
        new_node->next=NULL;
        if(start==NULL)
        {
            start=new_node;
            current=new_node;
        }
        else
        {
            current->next=new_node;
            current=new_node;
        }
        printf("\n do you want to create another        :");
        while ((ch = getchar()) != '\n' && ch != EOF);  
        ch = getchar();
    } while(ch != 'n' && ch != 'N');
}
void display()
{
    struct node *temp = start;
    printf("\n this list is       :\n");
    while(temp != NULL)
    {
        printf("%d-->",temp->data);
        temp=temp->next;
    }
    printf("NULL\n");
}

int main()
{
    create();
    display();
    return 0;
}