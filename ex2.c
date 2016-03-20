#include <stdlib.h>
#include <string.h>
#include <stdio.h>

const char binary[16][5] = { "0000", "0001", "0010", "0011", "0100", "0101", "0110", "0111", "1000", "1001", "1010", "1011", "1100", "1101", "1110", "1111" };

const char * hex2bin_char(int x)
{
  if (x >= '0' && x <= '9') return binary[x - '0'];
  
  if (x >= 'A' && x <= 'F') return binary[x - 'A' + 10];
  
  return binary[x - 'a' + 10];
}

void hex2bin_str(char *str, char *res)
{
  for (int i = 0; i < strlen(str); i+=2)
    {
      int byte1 = str[i];
      int byte2 = str[i+1];
      sprintf(res, "%s%s%s", res, hex2bin_char(byte1), hex2bin_char(byte2));
    }
}

void xor(char *str1, char *str2, char *res)
{
  // assume that both str1 and str2 are of equal length
  for(int i = 0; i < strlen(str1); i++)
    {
      int a = str1[i];
      int b  = str2[i];
      int xor = a ^ b;
      sprintf(res, "%s%d", res, xor);
    }
}

char hex_encode_value(uint8_t value) 
{
    static const char* encoding = "0123456789abcdef";
    return encoding[value];
}

void bin2hex_str(char *str, char *res)
{
  for (int i = 0; i < strlen(str); i+=4)
    {
      char bits[5] = {};
      for (int j = 0; j < 4; j++)
        {
          sprintf(bits, "%s%c", bits, str[i+j]);
        }
      char *ptr;
      long ret;
      //recognize string as base 2, and convert it to long value (base 16)
      ret = strtol(bits, &ptr, 2);
      sprintf(res, "%s%c", res, hex_encode_value(ret));
    }  
}

int main()
{
  char* hex1 = "1c0111001f010100061a024b53535009181c";
  char* hex2 = "686974207468652062756c6c277320657965";
  
  char bin1[1024] = {};
  char bin2[1024] = {};
  char bin3[1024] = {};
  char res[1024] = {};
  
  hex2bin_str(hex1, bin1);
  hex2bin_str(hex2, bin2);
  xor(bin1, bin2, bin3);
  bin2hex_str(bin3, res);
  
  printf("%s \n", res);
  
}
