To recover jpegs from a raw file.


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
typedef uint8_t BYTE;
#define size 512

int main(int argc, char* argv[])
{
    // TODO
    FILE* file = fopen("card.raw", "r");   //use filename instead of card.raw in which the recoverable jpegs are available.
    BYTE block[size];
    int n=0;
    char s[8];
    int l=0;
    while(!feof(file))
    {
    if(n==50)
    {
    break;
    }
    if(l!=1)
    {
    if(fread(&block, sizeof(block), 1, file)!=1)
    {
    break;
    }
    }
    if((block[0]==0xff)&&(block[1]==0xd8)&&(block[2]==0xff)&&((block[3]==0xe0)||(block[3]==0xe1)))
    {
    sprintf(s,"%03d.jpg",n);
    FILE* img = fopen( s, "a");
    fwrite(&block, sizeof(block), 1,img);
    fread(&block, sizeof(block), 1, file);
    while (!((block[0]==0xff)&&(block[1]==0xd8)&&(block[2]==0xff)&&((block[3]==0xe0)||(block[3]==0xe1))))
    {   
    fwrite(&block, sizeof(block), 1,img);
    int y=fread(&block, sizeof(block), 1, file);
    if(n==49)
    {
    if(y!=1)
    {
    break;
    }
    }
    }
    l=1;
    fclose(img);
    n++;
    }
    }
    fclose(file);
    return 0;
}
