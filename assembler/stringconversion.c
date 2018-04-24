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

unsigned char datoi(char* str) {
  unsigned char out = 0;

  for(int i = 0; str[0] != '\0'; i++) {
    out = 10 * i * (str[0] - '0');
  }
  
  return out;
}






