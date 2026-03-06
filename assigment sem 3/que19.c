#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX 100

// ------------------ Stack for operators ------------------
char stack[MAX];
int top = -1;

void push(char c) { stack[++top] = c; }
char pop() { return stack[top--]; }
char peek() { return stack[top]; }
int isEmpty() { return top == -1; }

// Operator precedence
int precedence(char c) {
    if(c=='+'||c=='-') return 1;
    if(c=='*'||c=='/') return 2;
    return 0;
}

// ------------------ Infix to Postfix ------------------
void infixToPostfix(char* infix, char* postfix) {
    int i=0,j=0;
    char c;
    top=-1; // reset stack
    while(infix[i]) {
        c=infix[i];
        if(isalnum(c)) postfix[j++]=c; // Operand
        else if(c=='(') push(c);
        else if(c==')') {
            while(!isEmpty() && peek()!='(') postfix[j++]=pop();
            pop(); // remove '('
        }
        else { // operator
            while(!isEmpty() && precedence(peek())>=precedence(c))
                postfix[j++]=pop();
            push(c);
        }
        i++;
    }
    while(!isEmpty()) postfix[j++]=pop();
    postfix[j]='\0';
}

// ------------------ Infix to Prefix ------------------
void reverseString(char* str) {
    int n = strlen(str);
    for(int i=0;i<n/2;i++){
        char temp = str[i];
        str[i] = str[n-1-i];
        str[n-1-i] = temp;
    }
}

void infixToPrefix(char* infix, char* prefix) {
    char revInfix[MAX], revPostfix[MAX];
    strcpy(revInfix, infix);
    reverseString(revInfix);

    // Swap '(' with ')'
    for(int i=0;i<strlen(revInfix);i++){
        if(revInfix[i]=='(') revInfix[i]=')';
        else if(revInfix[i]==')') revInfix[i]='(';
    }

    infixToPostfix(revInfix, revPostfix);
    reverseString(revPostfix);
    strcpy(prefix, revPostfix);
}

// ------------------ Evaluate Postfix ------------------
int evalPostfix(char* postfix) {
    int stackEval[MAX], topEval=-1;
    int i=0;
    char c;
    while(postfix[i]) {
        c=postfix[i];
        if(isdigit(c)) stackEval[++topEval]=c-'0';
        else {
            int b=stackEval[topEval--];
            int a=stackEval[topEval--];
            if(c=='+') stackEval[++topEval]=a+b;
            else if(c=='-') stackEval[++topEval]=a-b;
            else if(c=='*') stackEval[++topEval]=a*b;
            else if(c=='/') stackEval[++topEval]=a/b;
        }
        i++;
    }
    return stackEval[topEval];
}

// ------------------ Evaluate Prefix ------------------
int evalPrefix(char* prefix) {
    int stackEval[MAX], topEval=-1;
    int n = strlen(prefix);
    for(int i=n-1;i>=0;i--){
        char c = prefix[i];
        if(isdigit(c)) stackEval[++topEval]=c-'0';
        else{
            int a = stackEval[topEval--];
            int b = stackEval[topEval--];
            if(c=='+') stackEval[++topEval]=a+b;
            else if(c=='-') stackEval[++topEval]=a-b;
            else if(c=='*') stackEval[++topEval]=a*b;
            else if(c=='/') stackEval[++topEval]=a/b;
        }
    }
    return stackEval[topEval];
}

// ------------------ Main Function ------------------
int main() {
    char infix[MAX], postfix[MAX], prefix[MAX];

    printf("Enter Infix Expression (single-digit operands): ");
    scanf("%s", infix);

    // Infix to Postfix
    infixToPostfix(infix, postfix);
    printf("Postfix: %s\n", postfix);
    printf("Evaluated Result (Postfix): %d\n", evalPostfix(postfix));

    // Infix to Prefix
    infixToPrefix(infix, prefix);
    printf("Prefix: %s\n", prefix);
    printf("Evaluated Result (Prefix): %d\n", evalPrefix(prefix));

    return 0;
}
