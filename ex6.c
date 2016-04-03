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
                { 
                  if ( inp[i*4+k] == encoding[j] ) {val = j; break;} 
                }
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

char *num2bin_byte(int num)
{
  char *str = calloc(9, sizeof(char));
  str[8] = '\0';
  //0x1000 = 16^3, 0x0100 = 16^2, 0x0010 = 16^1, 0x0000 = 16^0
  int mask = 0x0100>>1;
  // shift and assign
  for (int i=0;i<8;i++)
    {
      str[i] = !!(mask>>i & num) + '0';
    }
  return str;
}

unsigned int round_div(unsigned int dividend, unsigned int divisor)
{
  if ( (divisor % dividend) == 0 )
    {
     return (dividend / divisor);
       }
  else
    {
  return (dividend / divisor) + 1;
    }
}

float score(char *str)
{
  float acc = 0;
  int len = strlen(str);
  for (int i = 0 ; i < len ; i++)
    {
      switch(str[i])
        {
        case 'e' : acc+=12.7;
        case 't' : acc+=9;
        case 'a' : acc+=8.2;
        case 'o' : acc+=7.5;
        case 'i' : acc+=7;
        case 'n' : acc+=6.7;
        case 's' : acc+=6.3;
        case 'h' : acc+=6;
        case 'r' : acc+=6;
        case 'd' : acc+=4.3;
        case 'l' : acc+=4;
        case 'c' : acc+=2.7;
        case 'u' : acc+=2.7;
        case 'm' : acc+=2.4;
        case 'w' : acc+=2.3;
        case 'f' : acc+=2.2;
        case 'y' : acc+=2;
        case 'p' : acc+=1.9;
        case 'b' : acc+=1.5;
        case 'v' : acc+=1;
        case 'k' : acc+=0.9;
        case 'j' : acc+=0.7;
        case 'x' : acc+=0.2;
        case 'q' : acc+=0.2;
        case 'z' : acc+=0.1;
        case 'E' : acc+=12.7;
        case 'T' : acc+=9;
        case 'A' : acc+=8.2;
        case 'O' : acc+=7.5;
        case 'I' : acc+=7;
        case 'N' : acc+=6.7;
        case 'S' : acc+=6.3;
        case 'H' : acc+=6;
        case 'R' : acc+=6;
        case 'D' : acc+=4.3;
        case 'L' : acc+=4;
        case 'C' : acc+=2.7;
        case 'U' : acc+=2.7;
        case 'M' : acc+=2.4;
        case 'W' : acc+=2.3;
        case 'F' : acc+=2.2;
        case 'Y' : acc+=2;
        case 'P' : acc+=1.9;
        case 'B' : acc+=1.5;
        case 'V' : acc+=1;
        case 'K' : acc+=0.9;
        case 'J' : acc+=0.7;
        case 'X' : acc+=0.2;
        case 'Q' : acc+=0.2;
        case 'Z' : acc+=0.1;
        case ' ' : acc+=5;
        default : acc-=5;
        }
    }
  return acc;
}

struct score
{
  float score;
  int key; // 0 - 255
};

// takes a hex string, returns an ascii string
char *xor_string(char *str, char *key)
{
  int len = strlen(str);
  char *out = calloc(len/2+1, sizeof(char));
  for(int i=0;i<len;i+=2)
    {
      char *temp = malloc(9);
      strcat(temp, hex2bin_char(str[i]));
      strcat(temp, hex2bin_char(str[i+1]));
      temp[8] = '\0';
      char xored[9] = {};
      int count = 0;
      for (int j=0;j<8;j++)
        {
          if ( ((temp[j] - '0') ^ (key[j] - '0')) == 1)
            {
              count += pow(2, 7-j);
            }
          xored[j] = ((temp[j] - '0') ^ (key[j] - '0')) + '0';
        }
      xored[8] = '\0';
      //printf("%d\nstr %s\nkey %s\nxor %s\n", i, temp, key, xored);
      out[i/2] = count;
    }
  out[len/2] = '\0';
  //printf("\norig %s\nafter %s\n\n", str, out);
  return out;
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
  int file_len = 54*61;
  char *full_file = calloc(file_len, sizeof(char));
  while (fgets(buf,61, ptr_file)!=NULL)
    {
      if (strcmp(buf, "\n"))
        { strcat(full_file, base64_to_hex(buf)); }
    }
  fclose(ptr_file);

  //score possibilities by the lowest edit distance
  for(int i = 0;i<key_permutations;i++)
    {
      possibilities[i].keysize = i+2;
      possibilities[i].score = edit_distance(full_file, i+2);
    }

  qsort(possibilities, key_permutations, sizeof(struct possibility), compare);

  int keysize_results[3];
  //print the top 3 values only; the smaller the edit distance, the better.
  for (int i = 0 ; i < 3 ; i++)
    {
      keysize_results[i] = possibilities[i].keysize;
      //printf("keysize : %d\n", keysize_results[i]);
    }

  //transpose the blocks
  int keysize = keysize_results[0]; // 29
  int file_hex_len = strlen(full_file); // 5754
  char *part1 = calloc(keysize+1, sizeof(char));
  int len_transposed = round_div(file_hex_len, keysize)+1; // 199->200
  char transposed[keysize][len_transposed+1];
  int byte_num = 0;
  int odd_num = 0;
  if ( (keysize % 2) == 1 ) { odd_num = 1; }
  
  memset(transposed, 0, keysize*(len_transposed+1)*sizeof(char));

  //transpose
  for(int i=0;i<file_hex_len;i+=keysize*2)
    {
      char* chunk = calloc(keysize*2+1, sizeof(char));
      chunk[keysize] = '\0';
      strncpy(chunk, full_file+i, keysize*2);
      //printf("chunk %s index %d\n", chunk, i/(keysize*2));
      // byte by byte is 2 hex chars at a time.
      for(int j=0;j<keysize*2;j+=2)
        {
          if ( strlen(chunk) == keysize*2 ) 
            {
              transposed[j/2][byte_num] = chunk[j];
              //printf("1 transposed[%d][%d] = %c\n", j/2, byte_num, chunk[j]);
              transposed[j/2][byte_num+1] = chunk[j+1];
              //printf("2 transposed[%d][%d] = %c\n", j/2, byte_num+1, chunk[j+1]);
            }
          else
            {
              if ( chunk[j] == 0 ) 
                { 
                  transposed[j/2][byte_num] = '\0';
                  //printf("3 transposed[%d][%d] end\n", j/2, byte_num);
                }
              else if ( chunk[j+1] == 0 )
                {
                  transposed[j/2][byte_num] = chunk[j];
                  transposed[j/2][byte_num+1] = '\0';
                  //printf("4 transposed[%d][%d] = %c\n", j/2, byte_num, chunk[j]);
                  //printf("3 transposed[%d][%d] end\n", j/2, byte_num+1);
                }
              else
                {
                  transposed[j/2][byte_num] = chunk[j];
                  transposed[j/2][byte_num+1] = chunk[j+1];
                  transposed[j/2][byte_num+2] = '\0';
                  //printf("5 transposed[%d][%d] = %c\n", j/2, byte_num, chunk[j]);
                  //printf("5 transposed[%d][%d] = %c\n", j/2, byte_num+1, chunk[j+1]);
                  //printf("3 transposed[%d][%d] end\n", j/2, byte_num+2);
                }
            }
        }
      byte_num+=2;
    }
  
  //printf("%d %d %d\n", file_hex_len, len_transposed, keysize);
   
  // solve each block as if it were single key XOR
  // find the key that yields the highest score (we use score to determine how similar the distribution of chars is to the english language
  char *key = calloc(keysize+1, sizeof(char));
  for(int j=0;j<keysize;j++)
    {
      struct score store;
      store.score = -999999;
      for (int i=0;i<256;i++)
        {
          char *byte_key = num2bin_byte(i);
          float temp_score = score(xor_string(transposed[j], byte_key));
          //printf("ori %s\nxor %s\n\n", transposed[j], xor_string(transposed[j], byte_key));
          if ( temp_score > store.score ) 
            {
              //printf("new score: %f\n", temp_score);
              store.key = i;
              store.score = temp_score;
            } 
          else
            {
              //printf("%d %f\n", temp_score, store.score);
            }
        }
      //printf("index %d, strlen %lu, score %f, key %c\n", j, strlen(transposed[j]), store.score, store.key); 
      key[j] = store.key;
    }
  key[keysize] = '\0';
  //printf("key %s , len %lu \n", key, strlen(key));
  
  int key_bin_len = keysize*8;
  char *key_bin = calloc(key_bin_len+1, sizeof(char));
  key_bin[key_bin_len] = '\0';
  for(int i=0;i<key_bin_len;i++)
    {
      strcat(key_bin, num2bin_byte(key[i]));
    }
  //printf("%s\n", key_bin);
  char *decrypted = calloc(file_hex_len+1, sizeof(char));
  decrypted[file_hex_len] = '\0';
  
  printf("%s\n", full_file);
  for(int i=0;i<file_hex_len;i+=keysize*2)
    {
      char *chunk = calloc(keysize*2+1, sizeof(char));
      chunk[keysize*2] = '\0';
      strncpy(chunk, full_file+i, keysize*2);
      for(int j=0;j<keysize*2;j+=2)
        {
          char *byte = calloc(3, sizeof(char));
          byte[0] = chunk[j];
          byte[1] = chunk[j+1];
          byte[2] = '\0';
          strcat(decrypted, xor_string(byte, num2bin_byte(key[j/2])));
        }
     }
  printf("%s\n", decrypted);

  return 0;

}
