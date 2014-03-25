#include <string.h>
#include "solid.h"

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

int testRead(char file, int minQual){

  
  return 1;
}

void openQualFile(char * file){
  

}
