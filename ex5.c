#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char *ascii2bin(char *ascii)
{
  int len = strlen(ascii);
  char *out = malloc(len*8+1);
  out[len*8] = 0;
  for (int i = 0; i<len; i++) 
    {
      uint8_t b = ascii[i];
      for (int j = 0; j<8; j++) 
        {
          //shift it, find the least significant bit and then make it an ASCII digit with '0'
          out[i*8+(7-j)] = ((b>>j)&1) + '0';
        }
    }
  return out;
}
 
int main ()
{
  
  char* str = "Burning 'em, if you ain't quick and nimble\nI go crazy when I hear a cymbal";
  char* key = "ICE";
  char* bin = malloc(1000);
  bin = ascii2bin(key);
  printf("%s\n", bin);

  return 0;
}
