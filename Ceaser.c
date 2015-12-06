#include <stdio.h>
#include <cs50.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int main(int argc,string argv[])
{ if(argc!=2)
  {
   printf("Incorrect Input\n");
   return 1;
   }
  int k=atoi(argv[1]);
  string x=GetString();
  int y=k%26;
  int m=strlen(x);
  int letter[m],crypt[m],mod[m];
  for(int i=0 ; i<m ; i++)
       {
            if isalpha(x[i])
            {
            letter[i]=x[i];
            crypt[i]=letter[i]+y;
            if((crypt[i]>122)||(crypt[i]>90&&isupper(x[i]))){
            mod[i]=crypt[i]-26;
            printf("%c",mod[i]);
            }
             else
            {
            printf("%c",crypt[i]);
            }
            }
            else
            {
            printf("%c",x[i]);
            }
            
       
       }
       printf("\n");
}
   
