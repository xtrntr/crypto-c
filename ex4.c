#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char binary[16][5] = { "0000", "0001", "0010", "0011", "0100", "0101", "0110", "0111", "1000", "1001", "1010", "1011", "1100", "1101", "1110", "1111" };

const char * hex2bin_char(int x)
{
  if (x >= '0' && x <= '9') return binary[x - '0'];
  
  if (x >= 'A' && x <= 'F') return binary[x - 'A' + 10];
  
  return binary[x - 'a' + 10];
}

void hex2bin_str(char *str, char *res)
{
  int len = strlen(str);
  for (int i = 0 ; i < len ; i+=2)
    {
      int byte1 = str[i];
      int byte2 = str[i+1];
      //append hex2bin_char(byte1 and byte2) to res string.
      sprintf(res, "%s%s%s", res, hex2bin_char(byte1), hex2bin_char(byte2));
    }
}

void xor(char *str1, char* str2, char *res)
{
  // assume that both str1 and str2 are of equal length
  int len = strlen(str1);
  for(int i = 0 ; i < len; i++)
    {
      int a = str1[i];
      int b  = str2[i];
      int xored = a ^ b;
      sprintf(res, "%s%d", res, xored);
    }
}

//65 to 90, 97 to 122 are alphabetical characters
void single_byte_xor(char *str, char *byte_cipher, char *res)
{
  int len = strlen(str);
  for (int i = 0 ; i < len ; i+=8)
    {
      char encoded[9] = {};
      char decoded[9] = {};
      for (int j = 0; j < 8; j++)
        {
          sprintf(encoded, "%s%c", encoded, str[i+j]);
        }
      //tested up to 0010 0000
      xor(encoded, byte_cipher, decoded);
      sprintf(res, "%s%s", res, decoded);
    }
}

// still not very clear how this bit shifting magic works
void num2bin_byte(int num, char *str)
{
  *(str+9) = '\0';
  //0x1000 = 16^3, 0x0100 = 16^2, 0x0010 = 16^1, 0x0000 = 16^0
  int mask = 0x0100;
  while(mask >>= 1)
    {
      //& is AND, ! is NOT, 
      *str++ = !!(mask & num) + '0';
    }
}

void bin2ascii(char *bin, char *res)
{
  int len = strlen(bin);
  for (int i = 0 ; i < len ; i+=8)
    {
      char buf[9] = {};
      long ret;
      char *ptr;
      for (int j = 0; j < 8; j++)
        {
          buf[j] = bin[i+j]; 
        }
      //recognize string as base 2, 
      ret = strtol(buf, &ptr, 2);
      //and convert it to ascii character 
      sprintf (res, "%s%c", res, ret); 
    }
}

//crappy scoring, but whatever works
float score(char *str)
{
  float acc = 0;
  int len = strlen(str);
  for (int i = 0 ; i < len ; i++)
    {
      switch(str[i])
        {
        case 'e' : 
          acc+=12.7;
        case 't' : 
          acc+=9;
        case 'a' : 
          acc+=8.2;
        case 'o' : 
          acc+=7.5;
        case 'i' : 
          acc+=7;
        case 'n' : 
          acc+=6.7;
        case 's' : 
          acc+=6.3;
        case 'h' : 
          acc+=6;
        case 'r' : 
          acc+=6;
        case 'd' : 
          acc+=4.3;
        case 'l' : 
          acc+=4;
        case 'c' : 
          acc+=2.7;
        case 'u' : 
          acc+=2.7;
        case 'm' : 
          acc+=2.4;
        case 'w' : 
          acc+=2.3;
        case 'f' : 
          acc+=2.2;
        case 'y' : 
          acc+=2;
        case 'p' : 
          acc+=1.9;
        case 'b' : 
          acc+=1.5;
        case 'v' : 
          acc+=1;
        case 'k' : 
          acc+=0.9;
        case 'j' : 
          acc+=0.7;
        case 'x' : 
          acc+=0.2;
        case 'q' : 
          acc+=0.2;
        case 'z' : 
          acc+=0.1;
        case 'E' : 
          acc+=12.7;
        case 'T' : 
          acc+=9;
        case 'A' : 
          acc+=8.2;
        case 'O' : 
          acc+=7.5;
        case 'I' : 
          acc+=7;
        case 'N' : 
          acc+=6.7;
        case 'S' : 
          acc+=6.3;
        case 'H' : 
          acc+=6;
        case 'R' : 
          acc+=6;
        case 'D' : 
          acc+=4.3;
        case 'L' : 
          acc+=4;
        case 'C' : 
          acc+=2.7;
        case 'U' : 
          acc+=2.7;
        case 'M' : 
          acc+=2.4;
        case 'W' : 
          acc+=2.3;
        case 'F' : 
          acc+=2.2;
        case 'Y' : 
          acc+=2;
        case 'P' : 
          acc+=1.9;
        case 'B' : 
          acc+=1.5;
        case 'V' : 
          acc+=1;
        case 'K' : 
          acc+=0.9;
        case 'J' : 
          acc+=0.7;
        case 'X' : 
          acc+=0.2;
        case 'Q' : 
          acc+=0.2;
        case 'Z' : 
          acc+=0.1;
        case ' ' : 
          acc+=5;
        default :
          acc-=25;
        }
    }
  return acc;
}

struct possibility
{
  char *msg;
  float score;
};

int compare(const void *s1, const void *s2)
{
  struct possibility *e1 = (struct possibility *)s1;
  struct possibility *e2 = (struct possibility *)s2;
  int scorecompare  = e1->score - e2->score;
  if (scorecompare == 0)  // same score so heck care
    return -1;
  else
    return -scorecompare;
}

int main()
{
  FILE *ptr_file;
  char buf[1000];
  char bin1[1024] = {};
  char res[1024] = {};
  char byte[9] = {};
  char *msg;
  struct possibility possibilities[83712];

  ptr_file =fopen("ex4.txt","r");
  if (!ptr_file)
    return 1;

  int j = 0;
  msg = (char *)malloc(32);

  while (fgets(buf,60, ptr_file)!=NULL)
    {
      //printf("%s",buf);
      hex2bin_str(buf, bin1);
      for (int i = 0 ; i < 256 ; i++)
        {
          msg = malloc(32);
          num2bin_byte(i, byte);
          single_byte_xor(bin1, byte, res);
          bin2ascii(res, msg);
          possibilities[i+j].score = score(msg);
          possibilities[i+j].msg = msg;
          printf("%d\n", (i+j));
        }
      printf("next line \n");
      j+=256;
      free(msg);
    }

  qsort(possibilities, 83712, sizeof(struct possibility), compare);

  //print the top 250 values only
  for (int i = 0 ; i < 500 ; i++)
    {
      printf("msg = %s, score = %f \n", possibilities[i].msg, possibilities[i].score);
    }

  fclose(ptr_file);
  return 0;
}
