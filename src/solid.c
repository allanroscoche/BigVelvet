#include <string.h>
#include <stdio.h>
#include "solid.h"

#define READ_SIZE 50
#define MKMER 12

typedef struct Read{
  unsigned int links;
  short min_qual;
} Read;

Read * table;
unsigned int * k_table;

unsigned int errors_counter=0;
int min_qual = 0;
FILE * q1, * q2, *r1, *r2;

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
  loadReads();
  



}

void closeQualFiles(){
  printf("Erros: %d\n",errors_counter);

  //fclose(q1);
  //fclose(q2);
}

void loadReads(){
   
  arquivo.clear();
  arquivo_R3.clear();
  arquivo.seekg(0, ios::beg);
  arquivo_R3.seekg(0, ios::beg);
  for(i=0;i<size;i++){
    if((i % (size/10)) == 0 ){
      cout << ".";
      cout.flush();
    }
    if(i%2){
      do{
	getline(arquivo, linha);
      }while(linha[0] != '>');
      getline(arquivo, linha);
    }
    else {
      do{
	getline(arquivo_R3, linha);
      }while(linha[0] != '>');
      getline(arquivo_R3, linha);
    }
    reads[i].add(READ_TAM,linha);
  }
  arquivo.close();
  arquivo_R3.close();
}

unsigned long countFileSize(){

  string linha;
  unsigned int l_size=0;

  if(arquivo.is_open()){
    while(arquivo.good()){
      getline(arquivo, linha);
      if(linha[0] == '>')
        l_size++;
    }
  }
  else {
    cout << "Arquivo não pode ser aberto" << endl;
  }
  return l_size;
}
