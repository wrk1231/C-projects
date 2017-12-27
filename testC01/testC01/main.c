//
//  main.c
//  testC01
//
//  Created by 王瑞康
//  Copyright © 2017年 Jerry. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <math.h>

void reverse()
{
    char c;
    if((c = getchar())!='\n')
        reverse();
     printf("%c", c);
}
void swap(int *p1, int *p2)
{
    int *p;
    p = p1;
    p1 = p2;
    p2 = p;
}

int main()

{
    
    char *a;
    a= "I love China!";
    printf("%s\n",a);
}
