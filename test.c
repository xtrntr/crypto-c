#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

static const char* encoding = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

char *bin2hex(char *bin)
{
  int len = strlen(bin);
  printf("binary length is %d\n", len);
  char *out = calloc(len/4+1, sizeof(char));
  out[len/4] = '0';
  for (int i = 0; i<(len/4); i++)
    {
      int hex_val = 0;
      for (int j = 0; j<4; j++) 
        {
          if (bin[i*4+j] - '0' == 1) { hex_val += pow(2,3-j); }
        }
      switch(hex_val)
        {
        case 10: out[i] = 'a'; break;
        case 11: out[i] = 'b'; break;
        case 12: out[i] = 'c'; break;
        case 13: out[i] = 'd'; break;
        case 14: out[i] = 'e'; break;
        case 15: out[i] = 'f'; break;
        default: out[i] = hex_val + '0';
        }
    }
  out[len/4] = '\0';
  return out;
}

char *base64_to_hex(char* inp)
{
  //base 64 = 6 bits for 1 character, comes in sets of 4 chars. 24 bits / 3 bytes
  //hex = 4 bits for 1 character. 
  int len = strlen(inp);
  char *out = calloc((len*6)/4+1, 1);
  printf("length is %d and size allocated is %d\n", len, (len*6)/4+1);
  for(int i=0;i<len/4;i++)
    {
      char bin[25] = {0};
      printf("binary length is %lu\n", strlen(bin));
      for (int k=0;k<4;k++)
        {
          int val = 0;
          // get base64 val of char
          if ( inp[i] != '=' )
            for(int j=0;j<64;j++)
              { 
                if ( inp[i*4+k] == encoding[j] )
                  {
                    val = j; 
                    break;
                  }
              }
          
          // convert each char into strings of 6 binary bits
          for(int l=0;l<6;l++)
            {
              printf("binary index is %d\n", 5-l+k*6);
              bin[5-l+k*6] = ((val>>l)&1) + '0';
            }
          
        }
      bin[24] = '\0';
      printf("binary content at index %d is %c\n", 23, bin[23]);
      printf("binary length is %lu\n", strlen(bin));
      printf("hex length is %lu\n", strlen(bin2hex(bin)));
      strcat(out, bin2hex(bin));
    }
  return out;
}

int main()
{
  char *test = "6245594466889692012";
  printf("%lu\n", strlen(test));
}
