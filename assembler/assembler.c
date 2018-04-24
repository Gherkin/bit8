#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ROM_LENGTH 256
#define LABEL_LENGTH 64
#define LABELS_SIZE 128

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0') 

unsigned char rom[ROM_LENGTH];
unsigned int rp = 0;

struct label {
  char name[LABEL_LENGTH];
  unsigned char rp;
};

struct label labels[LABELS_SIZE];
unsigned int lp = 0;



unsigned char hatoi(char* str) {
  unsigned char out = 0;
  unsigned char i = 0;

  while(*str != '\0' && i < 2) {
    out = out << 4;
    switch(*str++) {
      case '0':
        out = out | 0x00;
        break;
      case '1':
        out = out | 0x01;
        break;
      case '2':
        out = out | 0x02;
        break;
      case '3':
        out = out | 0x03;
        break;
      case '4':
        out = out | 0x04;
        break;
      case '5':
        out = out | 0x05;
        break;
      case '6':
        out = out | 0x06;
        break;
      case '7':
        out = out | 0x07;
        break;
      case '8':
        out = out | 0x08;
        break;
      case '9':
        out = out | 0x09;
        break;
      case 'A':
      case 'a':
        out = out | 0x0A;
        break;
      case 'B':
      case 'b':
        out = out | 0x0B;
        break;
      case 'C':
      case 'c':
        out = out | 0x0C;
        break;
      case 'D':
      case 'd':
        out = out | 0x0D;
        break;
      case 'E':
      case 'e':
        out = out | 0x0E;
        break;
      case 'F':
      case 'f':
        out = out | 0x0F;
        break;
    }
    i++;
  }

  return out;
}

unsigned char batoi(char* str) {
  unsigned char out = 0;
  unsigned char i = 0;

  while(*str !=  '\0' && i < 8) {
    out = out << 1;
    switch(*str++) {
      case '0':
        break;
      case '1':
        out = out | 0x01;
        break;
    }
    i++;
  }

  return out;
}


unsigned char translate_value(char* value) {
  if(*value == '0') {
    if(*(value + 1) == 'x') {
      return hatoi(value + 2);
    } 
    else if(*(value + 1) == 'b') {
      return batoi(value + 2);
    }
  }
}

unsigned char translate_operator(char* operator, char* operand_a, char* operand_b) {
  unsigned char out = 0;

  if(strcmp(operator, "stu") == 0) {
    out = 0xC0;
#ifdef debug
    printf("0x%02X 0x%02X 0x%02X\n", out, translate_value(operand_a) << 4, translate_value(operand_b));
#endif
    out = out | (translate_value(operand_a) << 4);
    out = out | translate_value(operand_b);
  }
  else if(strcmp(operator, "stl") == 0) {
    out = 0x80;
#ifdef debug
    printf("0x%02X 0x%02X 0x%02X\n", out, translate_value(operand_a) << 4, translate_value(operand_b));
#endif
    out = out | (translate_value(operand_a) << 4);
    out = out | translate_value(operand_b);
  }
  else if(strcmp(operator, "add") == 0) {
    out = 0x10;
#ifdef debug
    printf("0x%02X 0x%02X 0x%02X\n", out, translate_value(operand_a) << 2, translate_value(operand_b));
#endif
    out = out | (translate_value(operand_a) << 2); 
    out = out | translate_value(operand_b); 
  }
  else if(strcmp(operator, "sub") == 0) {
    out = 0x20;
#ifdef debug
    printf("0x%02X 0x%02X 0x%02X\n", out, translate_value(operand_a) << 2, translate_value(operand_b));
#endif
    out = out | (translate_value(operand_a) << 2); 
    out = out | translate_value(operand_b); 
  }
  else if(strcmp(operator, "mup") == 0) {
    out = 0x30;
#ifdef debug
    printf("0x%02X 0x%02X 0x%02X\n", out, translate_value(operand_a) << 2, translate_value(operand_b));
#endif
    out = out | (translate_value(operand_a) << 2); 
    out = out | translate_value(operand_b); 
  }
  else if(strcmp(operator, "beq") == 0) {
    out = 0x40;
#ifdef debug
    printf("0x%02X 0x%02X 0x%02X\n", out, translate_value(operand_a) << 2, translate_value(operand_b));
#endif
    out = out | (translate_value(operand_a) << 2); 
    out = out | translate_value(operand_b); 
  }
  else if(strcmp(operator, "slt") == 0) {
    out = 0x50;
#ifdef debug
    printf("0x%02X 0x%02X 0x%02X\n", out, translate_value(operand_a) << 2, translate_value(operand_b));
#endif
    out = out | (translate_value(operand_a) << 2); 
    out = out | translate_value(operand_b); 
  }
  else if(strcmp(operator, "and") == 0) {
    out = 0x60;
#ifdef debug
    printf("0x%02X 0x%02X 0x%02X\n", out, translate_value(operand_a) << 2, translate_value(operand_b));
#endif
    out = out | (translate_value(operand_a) << 2); 
    out = out | translate_value(operand_b); 
  }
  else if(strcmp(operator, "lor") == 0) {
    out = 0x70;
#ifdef debug
    printf("0x%02X 0x%02X 0x%02X\n", out, translate_value(operand_a) << 2, translate_value(operand_b));
#endif
    out = out | (translate_value(operand_a) << 2); 
    out = out | translate_value(operand_b); 
  }
  else if(strcmp(operator, "out") == 0) {
    out = 0x00;
#ifdef debug
    printf("0x%02X 0x%02X\n", out, translate_value(operand_a));
#endif
    out = out | translate_value(operand_a); 
  }
  else if(strcmp(operator, "pus") == 0) {
    out = 0x04;
#ifdef debug
    printf("0x%02X 0x%02X\n", out, translate_value(operand_a));
#endif
    out = out | translate_value(operand_a); 
  }
  else if(strcmp(operator, "pop") == 0) {
    out = 0x08;
#ifdef debug
    printf("0x%02X 0x%02X\n", out, translate_value(operand_a));
#endif
    out = out | translate_value(operand_a); 
  }
  else if(strcmp(operator, "jmp") == 0) {
    out = 0x0C;
#ifdef debug
    printf("0x%02X 0x%02X\n", out, translate_value(operand_a));
#endif
  out = out | translate_value(operand_a); 
  }
  else {
    fprintf(stderr, "unrecognized operator %s\n", operator);
    exit(1);
  }

  return out;
}

int get_word(char** word_ptr, char** line) {
 int i = 0;
  while(**line != '\0' && **line != '\n' && **line != ' ') {
    if(i + 1 >= LABEL_LENGTH) {
      fprintf(stderr, "too long word!\n");
      exit(1);
    }
    *((*word_ptr) + i) = *(*line)++;
    i++;
  }
  (*line)++;

  return i;
}

int parse(unsigned char* ins, char* line) {
  char* ptr = line;
  int is_label = false;
  char* operator = malloc(LABEL_LENGTH * sizeof(char));
  memset(operator, 0, LABEL_LENGTH);
  char* operand_a = malloc(LABEL_LENGTH * sizeof(char));
  memset(operand_a, 0, LABEL_LENGTH);
  char* operand_b = malloc(LABEL_LENGTH * sizeof(char));
  memset(operand_b, 0, LABEL_LENGTH);
  
  get_word(&operator, &ptr);
  
  get_word(&operand_a, &ptr);
  get_word(&operand_b, &ptr);

#ifdef debug
  printf("%s %s %s\n", operator, operand_a, operand_b);
#endif
  *ins = translate_operator(operator, operand_a, operand_b);
  
  free(operator);
  free(operand_a);
  free(operand_b);

  rp++;
  return 0;
}


int main(int argc, char* argv[]) {
  char* line = malloc(256 * sizeof(char));
  size_t len = 256;
  unsigned char ins;

  while(getline(&line, &len, stdin) != -1) {
    if(*line == '\n') {
      continue;
    }
    if(parse(&ins, line) == -1) {
      continue;
    }
    printf(BYTE_TO_BINARY_PATTERN"\n", BYTE_TO_BINARY(ins));
  }
  free(line);
}
