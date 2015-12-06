#include <stdio.h>
#include <cs50.h>    //library provided by cs50
#include <math.h>

int main(void)
{
 float n;
 printf("O hai! How much change is owed?\n");
 n=GetFloat();
 while(n<=0)
 {
 printf("How much change is owed?\n");
 n=GetFloat();
 }
 float m=n*100;
 int x=round(m);
 int p=x/25;
 int q=(x%25)/10;
 int r=((x%25)%10)/5;
 int s=(((x%25)%10)%5);
 printf("%d\n",p+q+r+s);
}
