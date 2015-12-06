#include <stdio.h>
#include <cs50.h>    //cs50 library

int main(void)
{
   int n;
   printf("Height:");
   n = GetInt();
   while(n<0||n>=24)
   {
   printf("Height:");
   n = GetInt();
   }
   for(int i=0;i<n;i++)
   {
   for(int j=0;j<n-i-1;j++)
   {
   printf(" ");
   }
   for(int k=0;k<i+2;k++)
   {
   printf("#");
   }
   printf("\n");
   }
   
} 
