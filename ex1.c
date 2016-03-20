#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char base64_encode_value(uint8_t value) 
{
    static const char* encoding = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    if (value > 63) return '=';
    return encoding[value];
}

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

void bin2base64(char *str, char *res)
{
  for (int i = 0; i < strlen(str); i+=6)
    {
      char bits[7] = {};
      for (int j = 0; j < 6; j++)
        {
          sprintf(bits, "%s%c", bits, str[i+j]);
        }
      char *ptr;
      long ret;
      //recognize string as base 2, and convert it to long value (base 10
      ret = strtol(bits, &ptr, 2);
      sprintf(res, "%s%c", res, base64_encode_value(ret));
    }  
}

int main()
{

  char* msg = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";
    
  char res1[1024] = {};
  char res2[1024] = {};
  hex2bin_str(msg, res1);
  bin2base64(res1, res2); 
  printf("%s \n", res2);
  
  return 0;
}
