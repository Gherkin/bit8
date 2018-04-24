#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stringconversion.h"
#include <ctype.h>

#define ROM_LENGTH 256
#define INST_LENGTH 128
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

char** rom[ROM_LENGTH];
unsigned int rp = 0;

int complexi[ROM_LENGTH];
unsigned int cp = 0;

int label_loc[ROM_LENGTH];
unsigned int llp = 0;

struct label {
  char* name;
  unsigned char rp;
};

struct label labels[LABELS_SIZE];
unsigned int lp = 0;

unsigned char translate_value(char* value) {
  if(*value == '0') {
    if(*(value + 1) == 'x') {
      return hatoi(value + 2);
    } 
    else if(*(value + 1) == 'b') {
      return batoi(value + 2);
    }
    else {
      return datoi(value + 2);
    }
  }
}

unsigned int find_label(char* lbl) {
  for(int i = 0; i < lp; i++) {
    if(strcmp(labels[i].name, lbl) == 0) {
#ifdef debug
      printf("label: %s %d\n", lbl, labels[i].rp);
#endif
      return labels[i].rp;
    }
  }
  fprintf(stderr, "couldn't find label '%s'\n", lbl);
  exit(1);
}

unsigned char parse_operator(char* op) {
  if(strcmp(op, "stu") == 0) {
    return 0xC0;
  }
  else if(strcmp(op, "stl") == 0) {
    return 0x80;
  }
  else if(strcmp(op, "add") == 0) {
    return 0x10;
  }
  else if(strcmp(op, "sub") == 0) {
    return 0x20;
  }
  else if(strcmp(op, "mup") == 0) {
    return 0x30;
  }
  else if(strcmp(op, "beq") == 0) {
    return 0x40;
  }
  else if(strcmp(op, "slt") == 0) {
    return 0x50;
  }
  else if(strcmp(op, "and") == 0) {
    return 0x60;
  }
  else if(strcmp(op, "lor") == 0) {
    return 0x70;
  }
  else if(strcmp(op, "out") == 0) {
    return 0x00;
  }
  else if(strcmp(op, "pus") == 0) {
    return 0x04;
  }
  else if(strcmp(op, "pop") == 0) {
    return 0x08;
  }
  else if(strcmp(op, "jmp") == 0) {
    return 0x0C;
  }
  else {
    fprintf(stderr, "unrecognized operator '%s'\n", op);
    exit(1);
  }
}

unsigned char encode_instruction(unsigned char op, unsigned char op1, unsigned char op2) {
  unsigned char out = op;
  switch (op) {
    case 0xC0:
    case 0x80:
      out = out | (op1 << 4);
      out = out | op2;
      break;
    case 0x10:
    case 0x20:
    case 0x30:
    case 0x40:
    case 0x50:
    case 0x60:
    case 0x70:
      out = out | (op1 << 2);
      out = out | op2;
      break;
    case 0x00:
    case 0x04:
    case 0x08:
    case 0x0C:
      out = out | op1;
      break;
  }

  return out;
}

unsigned char parse_instruction(char* inst[3]) {
  unsigned char op = parse_operator(inst[0]);
  unsigned char op1 = translate_value(inst[1]);
  unsigned char op2 = translate_value(inst[2]);

  return encode_instruction(op, op1, op2);
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
  *((*word_ptr) + i) = '\0';
  (*line)++;

#ifdef debug
  printf("%s\n", *word_ptr);
#endif

  return i;
}


int create_label(char* line) {
  struct label lbl;
  lbl.name = malloc(LABEL_LENGTH * sizeof(char));
  *lbl.name = 'a';
  int i = 0;

  lbl.rp = rp;
  while(isalpha(line[i])) {
    lbl.name[i] = line[i];
    i++;
  }
  lbl.name[i] = '\0';

#ifdef debug
    printf("lbl: %s %d\n", lbl.name, lbl.rp);
#endif

  labels[lp++] = lbl;

  return lbl.rp;
}

int complex_inst(char* inst[3], unsigned int p) {
  if(strcmp(inst[0], "sto") == 0) {
    unsigned char op = translate_value(inst[2]);
    strcpy(rom[p][0], "stu");
    strcpy(rom[p][1], inst[1]);
    sprintf(rom[p][2], "0x%02X", op >> 4);
    strcpy(rom[p + 1][0], "stl");
    strcpy(rom[p + 1][1], inst[1]);
    sprintf(rom[p + 1][2], "0x%02X", op & 0x0f);
  }
  else {
    return 0;
  }

  return 1;
}

int split_words(char* inst[3], char* line) {

  for(int i = 0; i < 3; i++) {
    get_word(&inst[i], &line);
  } 

#ifdef debug
  printf("%s %s %s\n", inst[0], inst[1], inst[2]);
#endif

  return 0;
}


int is_complex(char* op) {
  int ops = 0;
  if(strcmp("sto", op) == 0) {
    return 2;
  } 

  return 0;
}

void parse_input(char* line) {
  int ops = 0;
  if(*line == ':') {
    create_label(line + 1);
    return;
  } 
  char** inst;
  inst = malloc(3 * sizeof(char*));
  for(int i = 0; i < 3; i++) {
    inst[i] = malloc(LABEL_LENGTH * sizeof(char));
  }
  split_words(inst, line);

  if(ops = is_complex(inst[0])) {
    complexi[cp++] = rp;
    for(int n = 1; n < ops; n++) {
      rom[rp + n] = malloc(3 * sizeof(char*));
      for(int i = 0; i < 3; i++) {
        rom[rp + n][i] = malloc(LABEL_LENGTH * sizeof(char));
      }
    }

  } else {
    ops = 1;
  }

  if(*inst[1] == ':' || *inst[2] == ':') {
    label_loc[llp++] = rp;
  }

  rom[rp] = inst;
  rp += ops;

#ifdef debug
  printf("%d: %s %s %s\n", rp - ops, rom[rp - ops][0], rom[rp - ops][1], rom[rp - ops][2]);
#endif
}

void replace_labels() {
  unsigned int p, p2;
  for(int i = 0; i < llp; i++) {
    p = label_loc[i];

    if(*rom[p][1] == ':') {
      p2 = find_label(rom[p][1] + 1);
      sprintf(rom[p][1], "0x%02X", p2);
    }
    if(*rom[p][2] == ':') {
      p2 = find_label(rom[p][2] + 1);
      sprintf(rom[p][2], "0x%02X", p2);
    }
  }
}


void unpack_complex() {
  int p;
  for(int i = 0; i < cp; i++) {
    int p = complexi[i];
    complex_inst(rom[p], p);
  }
}

int main(int argc, char* argv[]) {
  char* line = malloc(256 * sizeof(char));
  size_t len = 256;
  unsigned char ins;

  while(getline(&line, &len, stdin) != -1) {
    if(*line == '\n') {
      continue;
    }
    parse_input(line);
  }
  free(line);
#ifdef debug
  for(int i = 0; i < rp; i++) {
    printf("%d: %s %s %s\n", i, rom[i][0], rom[i][1], rom[i][2]);
  }
#endif
  replace_labels();
#ifdef debug
  for(int i = 0; i < rp; i++) {
  printf("%d: %s %s %s\n", i, rom[i][0], rom[i][1], rom[i][2]);
  }
#endif
  unpack_complex();

  for(int i = 0; i < rp; i++) {
#ifdef debug
  printf("%d: %s %s %s\n", i, rom[i][0], rom[i][1], rom[i][2]);
#endif
    ins = parse_instruction(rom[i]);
    printf(BYTE_TO_BINARY_PATTERN"\n", BYTE_TO_BINARY(ins));
    for(int n = 0; n < 3; n++) {
      free(rom[i][n]);
    }
    free(rom[i]);
  }
}

