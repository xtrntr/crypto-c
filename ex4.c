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
  int len = strlen(str2);
  for(int i = 0 ; i < len; i++)
    {
      int a = str1[i];
      int b  = str2[i];
      int xored = a ^ b;
      sprintf(res, "%s%d", res, xored);
    }
}

//65 to 90, 97 to 122 are alphabetical characters
void single_byte_xor(char *bin, char *byte_cipher, char *res)
{
  int len = strlen(bin);
  char encoded[8];
  char decoded[8];
  char temp[len];
  for (int i = 0 ; i < len ; i+=8)
    {
      for (int j = 0; j < 8; j++)
        { 
          encoded[j] = bin[i+j];
        }
      xor(encoded, byte_cipher, decoded);
      for (int j = 0; j < 8; j++)
        { 
          temp[i+j] = decoded[j];
        }
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
  char *buf;
  for (int i = 0 ; i < len ; i+=8)
    {
      long ret;
      char *ptr;
      buf = calloc(1,8);
      for (int j = 0; j < 8; j++)
        {
          buf[j] = bin[i+j]; 
        }
      //recognize string as base 2, 
      ret = strtol(buf, &ptr, 2);
      //and convert it to ascii character 
      sprintf (res, "%s%c", res, ret); 
    }
  //free(buf);
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
  //should be 83712
  struct possibility possibilities[83712];

  ptr_file =fopen("ex4.txt","r");
  if (!ptr_file)
    return 1;

  int j = 0;
  char *msg;
  char *res;
  char *buf;
  char *bin1;
    
  buf = malloc(61);
  while (fgets(buf,61, ptr_file)!=NULL)
    {
      if (strcmp(buf, "\n"))
        {
          bin1 = calloc(1,240);
          hex2bin_str(buf, bin1);
          printf("line: %d, bin1len: %lu \n", (j/256),  strlen(bin1));
          printf("bin1: %s \n", bin1);
          for (int i = 0 ; i < 256 ; i++)
            { 
              char byte[8] = {};
              res = calloc(1,240);
              msg = calloc(1,30);
              num2bin_byte(i, byte);
              printf("before: bin1len: %lu, reslen: %lu \n", strlen(bin1), strlen(res));
              single_byte_xor(bin1, byte, res);
              printf("before: bin1len: %lu, reslen: %lu \n", strlen(bin1), strlen(res));
              bin2ascii(res, msg);
              //possibilities[i+j].msg = msg;
              //possibilities[i+j].score = score(msg);
              if ( i+j > 0 )
                {
                  //printf("%lu\n", strlen(possibilities[i+j-1].msg));
                }
              
            }
          j+=256;
        }
      buf = calloc(1,70);
    }


    qsort(possibilities, 83712, sizeof(struct possibility), compare);

    //print the top 250 values only
    for (int i = 0 ; i < 50 ; i++)
    {
    printf("len = %lu, msg = %f \n", strlen(possibilities[i].msg), possibilities[i].score);
    }

  fclose(ptr_file);
  return 0;
}
