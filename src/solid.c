#include <string.h>
#include <stdio.h>
#include "solid.h"

#define READ_SIZE 50
#define KMER_SIZE 8
#define KTABLE_SIZE 1 << (KMER_SIZE*2)

typedef struct Read{
  unsigned int links;
  short min_qual;
} Read;

Read * table;
unsigned int * k_table;

unsigned int errors_counter=0;
int min_qual = 0;
unsigned long size;
FILE * q1, * q2, *r1, *r2;

void generateAndInsertKmers(char * read);
void tableInput(unsigned long key);

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

unsigned long hash(char * kmer){

  unsigned int i, code;
  unsigned long result=0;

  for(i=0;i<KMER_SIZE;i++){
    switch(kmer[i]){
    case 'A':
      code =0;
      break;
    case 'C':
      code=1;
      break;
    case 'G':
      code=2;
      break;
    case 'T':
      code=3;
      break;
    }
    result |= code << i*2;
  }
  return result;
}

unsigned long countReads(){

  char linha[200];
  unsigned long l_size=0;

  while(fscanf(r1, "%s",linha) != EOF)
    if(linha[0] == '>')
      l_size++;

  return l_size;
}


void loadReads(){

  unsigned int i;
  char linha[200];
  char read[200];

  rewind(r1);

  for(i=0;i<size;i++){
    if((i % (size/10)) == 0 ){
      fflush(r1);
      printf(".");
    }
    if(i%2){
      do{
        fscanf(r1, "%s",linha);
      }while(linha[0] != '>');
      fscanf(r1, "%s",linha);
    }
    else {
      do{
        fscanf(r1, "%s",linha);
      }while(linha[0] != '>');
      fscanf(r1, "%s",linha);
    }
    strcpy(read,linha);
    convertSequence(read);
    generateAndInsertKmers(read);

    //printf(":%s\n",read);
  }
}


int testRead(){

  unsigned int i;
  int quals[60];
  char * line[400];

  for(i=0;i<READ_SIZE;i++)
    quals[i]=0;

  fscanf(q1,"%s",line);
  if(line[0]!='>'){
    for(i=0;i<READ_SIZE;i++)
      fscanf(q1,"%u ",&quals[i]);
    for(i=0;i<READ_SIZE;i++)
      if(quals[i] < min_qual){
        errors_counter+=2;
        return 0;
        }
  }
  else {
    printf("Error1 '%c': %s\n",line[0],line);
    exit(1);
  }

  fscanf(q2,"%s",line);
  if(line[0]!=">"){
    for(i=0;i<READ_SIZE;i++)
      fscanf(q2,"%u ",&quals[i]);
    for(i=0;i<READ_SIZE;i++)
      if(quals[i] < min_qual){
        errors_counter++;
        return 0;
      }
  }
  else {
    printf("Error2 '%c': %s\n",line[0],line);
    exit(1);
  }

  return 1;
}

//void openQualFiles(char * file1, char * file2, int qual){
void initFiles(char *file1, char* qualfile1, char * file2, char * qualfile2, int qual){
  min_qual = qual;

  q1 = fopen(qualfile1, "r");
  q2 = fopen(qualfile2, "r");
  r1 = fopen(file1,"r");
  r2 = fopen(file2,"r");

  if(q1 == NULL)
    printf("Cannot open %s.\n",qualfile1);
  else
    velvetLog("Reading quality file '%s'\n", qualfile1);
  if(q2 == NULL)
    printf("cannot open %s.\n",qualfile2);
  else
    velvetLog("Reading quality file '%s'\n", qualfile2);

  if(r1 == NULL)
    printf("Cannot open %s.\n",file1);
  else
    velvetLog("Reading quality file '%s'\n", file1);
  if(r2 == NULL)
    printf("Cannot open %s.\n",file2);
  else
    velvetLog("Reading quality file '%s'\n", file2);

  size = countReads();
  printf("Size:%lu\n",size);
  initTable();
  loadReads();

}

void closeQualFiles(){
  printf("Erros: %d\n",errors_counter);

  //fclose(q1);
  //fclose(q2);
}

void initTable(){

  unsigned long i;

  printf("Init table with %u entries.\n",KTABLE_SIZE);

  if(k_table == NULL)
    k_table = (unsigned int * ) malloc ( sizeof(unsigned int * )*KTABLE_SIZE) ;

  for(i=0;i<KTABLE_SIZE;i++)
    k_table[i]=0;

}

void generateAndInsertKmers(char * read){

  int i,j;
  char kmer[KMER_SIZE+1];

  for(i=0;i<(READ_SIZE-KMER_SIZE);i++){
    strncpy(kmer,&read[i],KMER_SIZE);
    kmer[KMER_SIZE]=0;

    // inserindo na hash
    k_table[hash(kmer)]++;

    /* Print Kmers
    for(j=0;j<i;j++)
      printf(" ");
    //printf("%s",kmer);
    printf("%d\n",hash(kmer));
    // */
  }
}

