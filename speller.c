Dictionary Spell Checker using Hash Tables
compile using makefile keeping all the below files in same directory.

*************************** dictionary.h *********************************************

#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stdbool.h>

// maximum length for a word
// (e.g., pneumonoultramicroscopicsilicovolcanoconiosis)
#define LENGTH 45

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char* word);

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char* dictionary);
typedef struct node
{
   char word[46];
   struct node* next;
}
node;

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void);

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void);

#endif // DICTIONARY_H

************************************* end of dictionary.h **************************************

*************************** dictionary.c ********************************************

#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "dictionary.h"
#include <ctype.h>

node* hashtable[784];
int r=0;

int hash(char* key)
{
 int l;
 if(key[1]=='\0')
 {
 l=0;
 }
 else if(isalpha(key[1]))
 {
 l=key[1]-'a'+2;
 }
 else
 {
 l=1;
 }
 int t=(key[0]-'a')*28+l;
 return t;
}
/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char* word)
{
    // TODO    
    int l=strlen(word);
    char input[l];
    for(int i=0;i<l+1;i++)
    {
    input[i]=tolower(word[i]);
    }    
    int k=hash(input);
    node* ptr=hashtable[k];
    if(input[1]=='\0')
    {
    if(ptr!=NULL)
    {
    return true;
    }
    return false;
    }
    while(ptr!=NULL)
    {       
      if((ptr->word[2]==input[2])&&(strlen(ptr->word)==l))
      {
      if(l==3||l==2)
      {
      return true;
      }
      else
      {
      if(strcmp(ptr->word,input)==0)
      {
      return true;
      }
      }
      }
      ptr=ptr->next;
    }
    return false;
}

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char* dictionary)
{
    // TODO
    FILE* fp=fopen(dictionary,"r");
    int load=0;
    char output[46];
    for(int i=0;!feof(fp);i++)
    {
    node* nodei=malloc(sizeof(node));
    fscanf(fp,"%s",output);
    strcpy(nodei->word,output);
    int k = hash(output);
    nodei->next=hashtable[k];
    hashtable[k]=nodei;
    load++;
    }
    fclose(fp);  
    r=load-1; 
    if(r>0)
    {
    return true;
    }
    else
    {
    return false;
    }
}
/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    // TODO
    int t=r;
    return t;
}

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void)
{
    // TODO
    int n=0;
    for(int p=0;p<784;p++)
    {
    node* cursor=hashtable[p];
    while(cursor!=NULL)
    {
      node*temp=cursor;
      cursor=cursor->next;
      free(temp);
    }
    n=p+1;
    }
    if(n==784)
      return true;
    else
      return false;
}
******************************** end of dictionary.c **************************************

************************* speller.c **********************************************
#include <ctype.h>
#include <stdio.h>
#include <sys/resource.h>
#include <sys/time.h>

#include "dictionary.h"
#undef calculate
#undef getrusage

// default dictionary
#define DICTIONARY "/home/cs50/pset6/dictionaries/large"

// prototype
double calculate(const struct rusage* b, const struct rusage* a);

int main(int argc, char* argv[])
{
    // check for correct number of args
    if (argc != 2 && argc != 3)
    {
        printf("Usage: speller [dictionary] text\n");
        return 1;
    }

    // structs for timing data
    struct rusage before, after;

    // benchmarks
    double ti_load = 0.0, ti_check = 0.0, ti_size = 0.0, ti_unload = 0.0;

    // determine dictionary to use
    char* dictionary = (argc == 3) ? argv[1] : DICTIONARY;

    // load dictionary
    getrusage(RUSAGE_SELF, &before);
    bool loaded = load(dictionary);
    getrusage(RUSAGE_SELF, &after);

    // abort if dictionary not loaded
    if (!loaded)
    {
        printf("Could not load %s.\n", dictionary);
        return 1;
    }

    // calculate time to load dictionary
    ti_load = calculate(&before, &after);
    
    // try to open text
    char* text = (argc == 3) ? argv[2] : argv[1];
    FILE* fp = fopen(text, "r");
    if (fp == NULL)
    {
        printf("Could not open %s.\n", text);
        unload();
        return 1;
    }

    // prepare to report misspellings
    printf("\nMISSPELLED WORDS\n\n");

    // prepare to spell-check
    int index = 0, misspellings = 0, words = 0;
    char word[LENGTH+1];

    // spell-check each word in text
    for (int c = fgetc(fp); c != EOF; c = fgetc(fp))
    {
        // allow only alphabetical characters and apostrophes
        if (isalpha(c) || (c == '\'' && index > 0))
        {
            // append character to word
            word[index] = c;
            index++;

            // ignore alphabetical strings too long to be words
            if (index > LENGTH)
            {
                // consume remainder of alphabetical string
                while ((c = fgetc(fp)) != EOF && isalpha(c));

                // prepare for new word
                index = 0;
            }
        }

        // ignore words with numbers (like MS Word can)
        else if (isdigit(c))
        {
            // consume remainder of alphanumeric string
            while ((c = fgetc(fp)) != EOF && isalnum(c));

            // prepare for new word
            index = 0;
        }

        // we must have found a whole word
        else if (index > 0)
        {
            // terminate current word
            word[index] = '\0';

            // update counter
            words++;

            // check word's spelling
            getrusage(RUSAGE_SELF, &before);
            bool misspelled = !check(word);
            getrusage(RUSAGE_SELF, &after);

            // update benchmark
            ti_check += calculate(&before, &after);

            // print word if misspelled
            if (misspelled)
            {
                printf("%s\n", word);
                misspellings++;
            }

            // prepare for next word
            index = 0;
        }
    }

    // check whether there was an error
    if (ferror(fp))
    {
        fclose(fp);
        printf("Error reading %s.\n", text);
        unload();
        return 1;
    }

    // close text
    fclose(fp);

    // determine dictionary's size
    getrusage(RUSAGE_SELF, &before);
    unsigned int n = size();
    getrusage(RUSAGE_SELF, &after);

    // calculate time to determine dictionary's size
    ti_size = calculate(&before, &after);

    // unload dictionary
    getrusage(RUSAGE_SELF, &before);
    bool unloaded = unload();
    getrusage(RUSAGE_SELF, &after);

    // abort if dictionary not unloaded
    if (!unloaded)
    {
        printf("Could not unload %s.\n", dictionary);
        return 1;
    }

    // calculate time to unload dictionary
    ti_unload = calculate(&before, &after);

    // report benchmarks
    printf("\nWORDS MISSPELLED:     %d\n", misspellings);
    printf("WORDS IN DICTIONARY:  %d\n", n);
    printf("WORDS IN TEXT:        %d\n", words);
    printf("TIME IN load:         %.2f\n", ti_load);
    printf("TIME IN check:        %.2f\n", ti_check);
    printf("TIME IN size:         %.2f\n", ti_size);
    printf("TIME IN unload:       %.2f\n", ti_unload);
    printf("TIME IN TOTAL:        %.2f\n\n", 
     ti_load + ti_check + ti_size + ti_unload);

    // that's all folks
    return 0;
}

/**
 * Returns number of seconds between b and a.
 */
double calculate(const struct rusage* b, const struct rusage* a)
{
    if (b == NULL || a == NULL)
    {
        return 0.0;
    }
    else
    {
        return ((((a->ru_utime.tv_sec * 1000000 + a->ru_utime.tv_usec) -
                 (b->ru_utime.tv_sec * 1000000 + b->ru_utime.tv_usec)) +
                ((a->ru_stime.tv_sec * 1000000 + a->ru_stime.tv_usec) -
                 (b->ru_stime.tv_sec * 1000000 + b->ru_stime.tv_usec)))
                / 1000000.0);
    }
}

********************* end of speller.c ********************************************

********************** MakeFile *************************************************

# compiler to use
CC = clang

# flags to pass compiler
CFLAGS = -ggdb3 -O0 -Qunused-arguments -std=c99 -Wall -Werror

# name for executable
EXE = speller

# space-separated list of header files
HDRS = dictionary.h

# space-separated list of libraries, if any,
# each of which should be prefixed with -l
LIBS =

# space-separated list of source files
SRCS = speller.c dictionary.c

# automatically generated list of object files
OBJS = $(SRCS:.c=.o)


# default target
$(EXE): $(OBJS) $(HDRS) Makefile
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)

# dependencies 
$(OBJS): $(HDRS) Makefile

# housekeeping
clean:
	rm -f core $(EXE) *.o
	
***************************************** end of MakeFile ********************************************
