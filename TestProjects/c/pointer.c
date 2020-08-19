
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    int c;
    int d;
} B;

typedef struct {
    int a;
    int b;
    B* bStu;
} A;

void CopyStuA(A *p, A **pp)   
{   
    *pp = (A*)malloc(sizeof(A));
    (*pp)->a = p->a;
    (*pp)->b = p->b;
    (*pp)->bStu = (B*)malloc(sizeof(B));
    (*pp)->bStu->c = p->bStu->c;
    (*pp)->bStu->d = p->bStu->d;
}

void Test (void)   
{   
    A from;
    from.a = 1;
    from.b = 2;
    from.bStu = (B*)malloc(sizeof(B));
    from.bStu->c = 3;
    from.bStu->d = 4;

    A* to = NULL;
    CopyStuA(&from, &to);
    printf("after copy to.a = %d, to.b = %d, to.bStu->c = %d, to.bStu->d = %d\n",
            to->a, to->b, to->bStu->c, to->bStu->d);
}

int main()  
{   
    Test();
    return 0;  
}  