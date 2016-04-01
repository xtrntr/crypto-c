#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

/*
Let KEYSIZE be the guessed length of the key; try values from 2 to (say) 40.

For each KEYSIZE, take the first KEYSIZE worth of bytes, and the second KEYSIZE worth of bytes, and find the edit distance between them. Normalize this result by dividing by KEYSIZE.

The KEYSIZE with the smallest normalized edit distance is probably the key. You could proceed perhaps with the smallest 2-3 KEYSIZE values. Or take 4 KEYSIZE blocks instead of 2 and average the distances.

Now that you probably know the KEYSIZE: break the ciphertext into blocks of KEYSIZE length.

Now transpose the blocks: make a block that is the first byte of every block, and a block that is the second byte of every block, and so on.

Solve each block as if it was single-character XOR. You already have code to do this.

For each block, the single-byte XOR key that produces the best looking histogram is the repeating-key XOR key byte for that block. Put them together and you have the key.
*/


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

// str1 should be a hex string
float edit_distance(char *str1, int byte_size)
{
  float out = 0;
  int len = strlen(str1);
  int num_chars = byte_size*2;
  char *part1 = calloc(num_chars+1, sizeof(char));
  char *part2 = calloc(num_chars+1, sizeof(char));
  //printf("str1 %s, len %d, \n", str1, len);
  for(int i=0;i<len;i+=num_chars*2)
    {
      strncpy(part1, str1+i, num_chars+1);
      strncpy(part2, str1+i+num_chars, num_chars+1);
      //printf("part1 %s\n", part1);
      //printf("part2 %s\n", part2);
      float disp = hamming_distance(part1, part2);
      //printf("dist  %f\n", disp / byte_size);
      out += disp;
    }
  free(part1);
  free(part2);
  return out;
}

static const char* encoding = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

char *bin2hex(char *bin)
{
  int len = strlen(bin);
  char *out = calloc(len/4+1, sizeof(char));
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
  int len = strlen(inp);
  char *out = calloc((len*6)/4, sizeof(char));
  
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
  return out;
}

const char binary[16][5] = { "0000", "0001", "0010", "0011", "0100", "0101", "0110", "0111", "1000", "1001", "1010", "1011", "1100", "1101", "1110", "1111" };

const char * hex2bin_char(int x)
{
  if (x >= '0' && x <= '9') return binary[x - '0'];
  
  if (x >= 'A' && x <= 'F') return binary[x - 'A' + 10];
  
  return binary[x - 'a' + 10];
}

struct possibility
{
  int keysize;
  float score;
};

int compare(const void *s1, const void *s2)
{
  struct possibility *e1 = (struct possibility *)s1;
  struct possibility *e2 = (struct possibility *)s2;
  int scorecompare  = e1->score - e2->score;
  if (scorecompare == 0)  // same score so heck care
    return 1; 
  else
    return scorecompare;
}

int main()
{

  FILE *ptr_file;
  char *str1 = "this is a test";
  char *str2 = "wokka wokka!!!";
  int key_permutations = 39;
  struct possibility possibilities[key_permutations];

  ptr_file =fopen("ex6.txt","r");
  if (!ptr_file)
    return 1;
  
  char buf[61] = {};
  char *full_file = calloc(54 * 61, sizeof(char));
  while (fgets(buf,61, ptr_file)!=NULL)
    {
      if (strcmp(buf, "\n"))
        {
          //printf("%d\n", edit_distance(base64_to_hex(buf), 29));
          strcat(full_file, base64_to_hex(buf));
        }
    }
  fclose(ptr_file);
  int x = 0;
  for(int i = 0;i<key_permutations;i++)
    {
      possibilities[i].keysize = i+2;
      possibilities[i].score = edit_distance(full_file, i+2);
      if ( edit_distance(full_file, i+2) != possibilities[i].score ) 
        {
          printf("for i of %d, edit dist is : %f and %f \n", i+2, edit_distance(full_file, i+2), possibilities[i].score);
        }
      x ++;
    }

  qsort(possibilities, key_permutations, sizeof(struct possibility), compare);

  int keysize_results[3];
  //print the top 5 values only
  for (int i = 0 ; i < 3 ; i++)
    {
      //printf("score = %f, keysize = %d \n", possibilities[i].score, possibilities[i].keysize);
      keysize_results[i] = possibilities[i].keysize;
    }

  //transpose the blocks

  int keysize = keysize_results[0];
  int file_hex_length = strlen(full_file);
  printf("full_text %s\n", full_file);
  char *part1 = calloc(keysize+1, sizeof(char));
  printf("num iterations : %d\n", file_hex_length/keysize);
  for(int i = 0;i<file_hex_length;i+=keysize)
    {
      printf("part %d, strlen %lu, str %s\n", i/keysize, strlen(strncpy(part1, full_file+i, keysize)), strncpy(part1, full_file+i, keysize)); 
    }
  
  return 0;

}
