#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*
Let KEYSIZE be the guessed length of the key; try values from 2 to (say) 40.

For each KEYSIZE, take the first KEYSIZE worth of bytes, and the second KEYSIZE worth of bytes, and find the edit distance between them. Normalize this result by dividing by KEYSIZE.

The KEYSIZE with the smallest normalized edit distance is probably the key. You could proceed perhaps with the smallest 2-3 KEYSIZE values. Or take 4 KEYSIZE blocks instead of 2 and average the distances.

Now that you probably know the KEYSIZE: break the ciphertext into blocks of KEYSIZE length.

Now transpose the blocks: make a block that is the first byte of every block, and a block that is the second byte of every block, and so on.

Solve each block as if it was single-character XOR. You already have code to do this.

For each block, the single-byte XOR key that produces the best looking histogram is the repeating-key XOR key byte for that block. Put them together and you have the key.
*/

// str1 should be a hex string
int edit_distance(char*str1, int byte_size)
{
  int out = 0;
  

}

int hamming_distance(char *str1, char *str2)
{
  int len = strlen(str1);
  // str1 len should be equal to str2 len, skip error checking
  int out = 0;
  for(int i=0; i<len; i++)
    {
      uint8_t a = str1[i];
      uint8_t b = str2[i];
      int differing_bits = 0;
      for (int j = 0; j<8; j++) 
        {
          if ((((a>>j)&1) ^ ((b>>j)&1)) == 1) { differing_bits++; }
        }
      out += differing_bits;
    }
  return out;
}

static const char* encoding = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

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

char *base64_to_hex(char* inp)
{
  //base 64 = 6 bits for 1 character, comes in sets of 4 chars. 24 bits / 3 bytes
  //hex = 4 bits for 1 character. 
  printf("%s\n", inp);
  int len = strlen(inp);
  char *out = malloc((len*6)/4);
  for(int i=0;i<len/4;i++)
    {
      char bin[25] = {};
      for (int k=0;k<4;k++)
        {
          int val = 0;
          // get base64 val of char
          if ( inp[i] != '=' )
            {
              for(int j=0;j<64;j++)
                { if ( inp[i*4+k] == encoding[j] ) {val = j; break;} }
            }
          // convert each char into strings of 6 binary bits
          for(int l=0;l<6;l++)
            {
              bin[5-l+k*6] = ((val>>l)&1) + '0';
            }
        }
      bin[24] = 0;
      strcat(out, bin2hex(bin));
    }
  printf("bin is %s\n", out);
  return "";
}

int main()
{

  FILE *ptr_file;
  char *str1 = "this is a test";
  char *str2 = "wokka wokka!!!";

  ptr_file =fopen("ex6.txt","r");
  if (!ptr_file)
    return 1;
  
  char buf[61] = {};
  while (fgets(buf,61, ptr_file)!=NULL)
    {
      if (strcmp(buf, "\n"))
        {
          base64_to_hex(buf);
        }
    }

  printf("%d\n", hamming_distance(str1, str2));

return 0;

}
