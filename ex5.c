#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

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

char *repeating_key_xor(char *key, char* inp)
{
  // assume key is a factor of inp, skip error checking
  int len = strlen(inp);
  int key_len = strlen(key)/8;
  int curr_index = 0;
  char *out = malloc(len+1);
  out[len*8] = 0;
  for (int i = 0; i<(len/8); i++)
    {
      for (int j = 0; j<8; j++) 
        {
          //xor it and then make it an ASCII digit with '0'
          out[i*8+j] = (inp[i*8+j] ^ key[j+8*curr_index]) + '0';
        }
      curr_index += 1;
      if (curr_index > key_len - 1) { curr_index = 0; }
    }
  return out;
}

char *bin2hex(char *bin)
{
  int len = strlen(bin);
  char *out = malloc(len/4+1);
  out[len/4] = 0;
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
  return out;
}
 
int main ()
{
  
  char* ascii_str = "Burning 'em, if you ain't quick and nimble\nI go crazy when I hear a cymbal";
  char* bin_str = malloc(strlen(ascii_str)*8+1);
  char* ascii_key = "ICE";
  char* bin_key = malloc(strlen(ascii_key)*8+1);
  char* bin_encrypted = malloc(strlen(ascii_str)*8+1);
  char* hex_encrypted = malloc(strlen(ascii_str)*4+1);
  bin_key = ascii2bin(ascii_key);
  bin_str = ascii2bin(ascii_str);
  bin_encrypted = repeating_key_xor(bin_key, bin_str);
  hex_encrypted = bin2hex(bin_encrypted);
  
  printf("ENCRYPT\n\n%s\n\nTO\n\n%s\n", ascii_str, hex_encrypted);

  return 0;
}
