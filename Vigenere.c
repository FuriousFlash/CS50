#include <stdio.h>
#include <cs50.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int main(int argc,string argv[])
{ if (argc!=2)
  {
  printf("Improper Input");
  return 1;
  }
  int l=strlen(argv[1]);
  int k[l],c[l];
  int g=-1;
  for(int z=0 ; z<l ; z++)
  {
  k[z]=argv[1][z];
  if isalpha(argv[1][z])
  {
  if isupper(argv[1][z])
  {
  c[z]=k[z]-65;
  }
  else  
  {
  c[z]=k[z]-97;
  }
  }
  else
  {
  printf("Invalid keyword");
  return 1;
  }
  }
  string x=GetString();
  int m=strlen(x);
  int letter[m],crypt[m],mod[m];
  for(int i=0 ; i<m ; i++)
       {                  
            if isalpha(x[i])
            {
            g=g+1;
            letter[i]=x[i];
            crypt[i]=letter[i]+c[g%l];
            if(((crypt[i]>90)&&isupper(x[i]))||(crypt[i]>122&&islower(x[i])))
            {
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
            g=g+0;
            printf("%c",x[i]);
            }
       }
   printf("\n");  
}
