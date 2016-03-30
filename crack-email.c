#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

/* 

TODO:
apply repeating-key xor on his stupid email

*/

char *base64_to_hex(char* inp)
{
  //base 64 = 6 bits for 1 character, comes in sets of 4 chars. 24 bits / 3 bytes
  //hex = 4 bits for 1 character. 
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
  return out;
}

const char binary[16][5] = { "0000", "0001", "0010", "0011", "0100", "0101", "0110", "0111", "1000", "1001", "1010", "1011", "1100", "1101", "1110", "1111" };

const char * hex2bin_char(int x)
{
  if (x >= '0' && x <= '9') return binary[x - '0'];
  
  if (x >= 'A' && x <= 'F') return binary[x - 'A' + 10];
  
  return binary[x - 'a' + 10];
}

int main()
{
  char *email = base64_to_hex("k6bIi6jDmZ2VuaOZvZ+zsKvDwJmQpoaEqMPIhQ==");
  int len = strlen(email);
  char *email_decrypted = malloc(len/2+1);
  printf("%s\n", email);
  for (int i=0;i<len;i+=2)
    {
      char *byte1 = malloc(9);
      int val = 0;
      strcat(byte1, hex2bin_char(email[i]));
      strcat(byte1, hex2bin_char(email[i+1]));
      strcpy(email_decrypted, byte1);
      printf("%s\n", email_decrypted);
      for (int j=0;j<8;j++)
        {
          if (email_decrypted[j] == '1') { val+= pow(2,7-j); }
        }
      printf("%d -> %c\n", val, (val + '0'));
    }

return 0;

}
