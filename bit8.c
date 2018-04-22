#include <stdio.h>
#include <unistd.h>

unsigned char rom[256];
unsigned char rp = 0;
unsigned char reg[4];

unsigned char stack[32];
unsigned char sp = 0;

void push(unsigned char c) {
  if(sp > 31) {
    printf("no puish, no stack");
    return;
  }
  stack[sp++] = c;
}

unsigned char pop() {
  if(sp < 1) {
    printf("no more porpopy");
    return stack[sp];
  }
  return stack[--sp];
}

void handleBtype(unsigned char i) {
  unsigned char op = i >> 2;
  unsigned char r = i & 0x03;
  switch(op) {
    case 0:
      printf("%d\n", reg[r]);
      //printf("0x%02X\n", reg[r]);
      break;
    case 1:
      push(reg[r]);
      break;
    case 2:
      reg[r] = pop();
      break;
    case 3:
      rp = reg[r];
      break;
  }
}

void handleEtype(unsigned char i) {
  unsigned char op = i >> 4;
  unsigned a = (i & 0x0f) >> 2; 
  unsigned b = i & 0x03; 
  switch(op) {
    case 0:
      handleBtype(i);
      break;
    case 1:
      reg[b] = reg[b] + reg[a];
      break;
    case 2:
      reg[b] = reg[b] - reg[a];
      break;
    case 3:
      reg[b] = reg[b] * reg[a];
      break;
    case 4: //beq
      if(reg[a] == reg[b]) {
        rp = reg[3];
      }
      break;
    case 5:
      reg[b] = reg[b] < reg[a];
      break;
    case 6:
      reg[b] = reg[b] & reg[a];
      break;
    case 7:
      reg[b] = reg[b] | reg[a];
      break;
  }
}

void handleItype(unsigned char i) {
  unsigned char a = (i >> 4) & 0x03;
  unsigned char v = i & 0x0f;
  if(i & 0x40) { //upper bits
    reg[a] = reg[a] & 0x0f;
    reg[a] = reg[a] | (v << 4);
  }
  else {
    reg[a] = reg[a] & 0xf0;
    reg[a] = reg[a] | v;
  }
}

void doInstruction(unsigned char i) {
  if(i & 0x80) {
    handleItype(i);
  }
  else if(i >> 4) {
    handleEtype(i);
  }
  else {
    handleBtype(i);
  }
}

void printReg() {
  for(int i = 0; i < 4; i++) {
    printf("0x%02X\n", reg[i]);
  }
    printf("%d\n", sp);
}

char parsec(unsigned char* p) {
  unsigned char out = 0x00;
  char c;
  char i = 0;
  while(i < 8) {
    int a = read(0, &c, 1);
    if(a < 1) {
      printf("%s\n", "efo");
      return -1;
    }
    if(c == 0x30) {// zero
      out = out << 1;
      i++;
      //printf("%s\n", "zero");
    }
    else if(c == 0x31) {
      out = (out << 1) | 0x01;
      i++;
      //printf("%s\n", "one");
    }
  }
  *p = out;
  return 0;
}

void readRom() {
  unsigned char bf = 0;
  unsigned char i = 0;
  while(parsec(&bf) != -1) {
    rom[i++] = bf;
    printf("0x%02X\n", bf);
  }
}

void run() {
  while(rp < 255) {
    doInstruction(rom[rp++]);
  }
}

int main() {

  readRom();
  run();
  printf("\n");
  printReg(); 
  return 0;
}
