#include <string.h>
#include <stdio.h>
#include "solid.h"

#define READ_SIZE 50

unsigned int counter, errors_counter=0;
int min_qual = 0;
FILE * q1, * q2;

void convertSequence(char*seq)
{
  int i;
  int size = strlen(seq);

  for(i=2;i<size;i++) {
    switch(seq[i]){
    case '0':
      seq[i-2] = 'A';
      break;
    case '1':
      seq[i-2] = 'C';
      break;
    case '2':
      seq[i-2] = 'G';
      break;
    case '3':
      seq[i-2] = 'T';
      break;
    case '.':
      seq[i-2] = 'N';
      break;
    }
  }
  seq[i-1] = seq[i-2] = 0;

}

int testRead(){

  unsigned int i;
  int quals[60];
  char * line[400];

  fscanf(q1,"%s",line);
  if(line[0]='>'){
    for(i=0;i<READ_SIZE;i++)
      fscanf(q1,"%u",&quals[i]);
    for(i=0;i<READ_SIZE;i++)
      if(quals[i] < min_qual){
        errors_counter++;
        return 0;
        }
  }

  fscanf(q2,"%s",line);
  if(line[0]='>'){
    for(i=0;i<READ_SIZE;i++)
      fscanf(q2,"%u",&quals[i]);
    for(i=0;i<READ_SIZE;i++)
      if(quals[i] < min_qual){
        errors_counter++;
        return 0;
        }
  }

  return 1;
}

//void openQualFiles(char * file1, char * file2, int qual){
void initFiles(char *file1, char* qualfile1, char * file2, char * qualfile2, int qual){
  min_qual = qual;

  q1 = fopen(qualfile1, "r");
  q2 = fopen(qualfile2, "r");

  if(q1 == NULL)
    printf("Cannot open %s.\n",file1);
  if(q2 == NULL)
    printf("cannot open %s.\n",file2);

}

void closeQualFiles(){
  printf("Erros: %d\n",errors_counter);

  //fclose(q1);
  //fclose(q2);
}
