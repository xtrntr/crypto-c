#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>

char rcon[256] = {
  0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 
  0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 
  0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 
  0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 
  0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 
  0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 
  0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 
  0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 
  0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 
  0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 
  0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 
  0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 
  0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 
  0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 
  0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 
  0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d
};

char inverse_sbox[256] = {
  0x52, 0x09, 0x6A, 0xD5, 0x30, 0x36, 0xA5, 0x38, 0xBF, 0x40, 0xA3, 0x9E, 0x81, 0xF3, 0xD7, 0xFB,
  0x7C, 0xE3, 0x39, 0x82, 0x9B, 0x2F, 0xFF, 0x87, 0x34, 0x8E, 0x43, 0x44, 0xC4, 0xDE, 0xE9, 0xCB,
  0x54, 0x7B, 0x94, 0x32, 0xA6, 0xC2, 0x23, 0x3D, 0xEE, 0x4C, 0x95, 0x0B, 0x42, 0xFA, 0xC3, 0x4E,
  0x08, 0x2E, 0xA1, 0x66, 0x28, 0xD9, 0x24, 0xB2, 0x76, 0x5B, 0xA2, 0x49, 0x6D, 0x8B, 0xD1, 0x25,
  0x72, 0xF8, 0xF6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xD4, 0xA4, 0x5C, 0xCC, 0x5D, 0x65, 0xB6, 0x92,
  0x6C, 0x70, 0x48, 0x50, 0xFD, 0xED, 0xB9, 0xDA, 0x5E, 0x15, 0x46, 0x57, 0xA7, 0x8D, 0x9D, 0x84,
  0x90, 0xD8, 0xAB, 0x00, 0x8C, 0xBC, 0xD3, 0x0A, 0xF7, 0xE4, 0x58, 0x05, 0xB8, 0xB3, 0x45, 0x06,
  0xD0, 0x2C, 0x1E, 0x8F, 0xCA, 0x3F, 0x0F, 0x02, 0xC1, 0xAF, 0xBD, 0x03, 0x01, 0x13, 0x8A, 0x6B,
  0x3A, 0x91, 0x11, 0x41, 0x4F, 0x67, 0xDC, 0xEA, 0x97, 0xF2, 0xCF, 0xCE, 0xF0, 0xB4, 0xE6, 0x73,
  0x96, 0xAC, 0x74, 0x22, 0xE7, 0xAD, 0x35, 0x85, 0xE2, 0xF9, 0x37, 0xE8, 0x1C, 0x75, 0xDF, 0x6E,
  0x47, 0xF1, 0x1A, 0x71, 0x1D, 0x29, 0xC5, 0x89, 0x6F, 0xB7, 0x62, 0x0E, 0xAA, 0x18, 0xBE, 0x1B,
  0xFC, 0x56, 0x3E, 0x4B, 0xC6, 0xD2, 0x79, 0x20, 0x9A, 0xDB, 0xC0, 0xFE, 0x78, 0xCD, 0x5A, 0xF4,
  0x1F, 0xDD, 0xA8, 0x33, 0x88, 0x07, 0xC7, 0x31, 0xB1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xEC, 0x5F,
  0x60, 0x51, 0x7F, 0xA9, 0x19, 0xB5, 0x4A, 0x0D, 0x2D, 0xE5, 0x7A, 0x9F, 0x93, 0xC9, 0x9C, 0xEF,
  0xA0, 0xE0, 0x3B, 0x4D, 0xAE, 0x2A, 0xF5, 0xB0, 0xC8, 0xEB, 0xBB, 0x3C, 0x83, 0x53, 0x99, 0x61,
  0x17, 0x2B, 0x04, 0x7E, 0xBA, 0x77, 0xD6, 0x26, 0xE1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0C, 0x7D
};

char sbox[256] = {
  0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
  0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
  0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
  0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
  0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
  0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
  0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
  0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
  0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
  0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
  0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
  0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
  0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
  0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
  0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
  0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16
};

char gmul(char a, char b) {
  char p = 0;
  char counter;
  char carry;
  for(counter = 0; counter < 8; counter++) {
    if((b & 1) == 1) {
      // 1) If the rightmost bit of b is set, XOR p with a.
      p ^= a;
    }
    // 2) Shift b one bit to the right.
    b >>= 1;
    // 3) Keep track of whether the leftmost bit of a is set to one and call this value carry.
    carry = (a & 0x80);
    // 4) Shift a one bit to the left, discarding the leftmost bit, and making the new rightmost bit zero. 
    a <<= 1;
    // 5) If carry had a value of one, XOR a with 0x1b.
    if(carry == 0x80)
      a ^= 0x1b;
  }
  return p;
}
 
void gmix_column(char *column) {
  char a[4];
  for(unsigned int c=0;c<4;c++) {
    a[c] = column[c];
  }
  column[0] = gmul(a[0],14) ^ gmul(a[3],9) ^ gmul(a[2],13) ^ gmul(a[1],11);
  column[1] = gmul(a[1],14) ^ gmul(a[0],9) ^ gmul(a[3],13) ^ gmul(a[2],11);
  column[2] = gmul(a[2],14) ^ gmul(a[1],9) ^ gmul(a[0],13) ^ gmul(a[3],11);
  column[3] = gmul(a[3],14) ^ gmul(a[2],9) ^ gmul(a[1],13) ^ gmul(a[0],11);
}


// a word is 4 bytes.
char *rotWord(char *inp) {
  char a = inp[0];
  char *out = calloc(4,1);
  for(int i=0;i<3;i++)
    {
      out[i] = inp[i+1];
    }
  out[3] = a;
  return out;
}

char *inverseSubBytes(char *inp) {
  char *out = calloc(4,1);
  for(int i=0;i<4;i++)
    {
      int first_4bits = inp[i] >> 4;
      int second_4bits = inp[i] - first_4bits*16;
      out[i] = inverse_sbox[first_4bits*16 + second_4bits];
    }
  return out;
}
// typically applied on a word/4bytes.
char *subBytes(char *inp) {
  char *out = calloc(4,1);
  for(int i=0;i<4;i++)
    {
      int first_4bits = inp[i] >> 4;
      int second_4bits = inp[i] - first_4bits*16;
      out[i] = sbox[first_4bits*16 + second_4bits];
    }
  return out;
}

//for a 4 word block.
void subBytes2(char *state){

  char *col_1  = calloc(16, 1);
  char *col_2  = calloc(16, 1);
  char *col_3  = calloc(16, 1);
  char *col_4  = calloc(16, 1);

  for(int k=0;k<4;k++) {
    col_1[k] = state[k]; 
    col_2[k] = state[4+k];
    col_3[k] = state[8+k];
    col_4[k] = state[12+k];
  }

  col_1 = subBytes(col_1);
  col_2 = subBytes(col_2);
  col_3 = subBytes(col_3);
  col_4 = subBytes(col_4);

  // copy into output
  memcpy(state,      col_1, 4); 
  memcpy(state + 4,  col_2, 4); 
  memcpy(state + 8,  col_3, 4); 
  memcpy(state + 12, col_4, 4); 

}


//for a 4 word block.
void inverseSubBytes2(char *state){

  char *col_1  = calloc(16, 1);
  char *col_2  = calloc(16, 1);
  char *col_3  = calloc(16, 1);
  char *col_4  = calloc(16, 1);

  for(int k=0;k<4;k++) {
    col_1[k] = state[k]; 
    col_2[k] = state[4+k];
    col_3[k] = state[8+k];
    col_4[k] = state[12+k];
  }

  col_1 = inverseSubBytes(col_1);
  col_2 = inverseSubBytes(col_2);
  col_3 = inverseSubBytes(col_3);
  col_4 = inverseSubBytes(col_4);

  // copy into output
  memcpy(state,      col_1, 4); 
  memcpy(state + 4,  col_2, 4); 
  memcpy(state + 8,  col_3, 4); 
  memcpy(state + 12, col_4, 4); 

}

// takes 2 characters/strings of equal length and xor them
char *xor(char *part1, char *part2, int len) {
  char *out = calloc(len+1, 1);
  for(int i=0;i<len;i++)
    {
      //printf("xor iteration %d\n", i);
      //printf("xoring %d and %d\n", part1[i], part2[i]);
      out[i] = part1[i] ^ part2[i];
    }
  return out;
}

// each iteration generates a new 16 word block
char *key_schedule_iteration(char *prev_block, int iter_num)
{
  // lots of repetition
  char col_1[4] = {0};
  char col_2[4] = {0};
  char col_3[4] = {0};
  char col_4[4] = {0};
  char *new_block = calloc(16, 1);

  for(int k=0;k<4;k++)
    {
      col_1[k] = prev_block[k]; 
      col_2[k] = prev_block[4+k];
      col_3[k] = prev_block[8+k];
      col_4[k] = prev_block[12+k];
    }

  // rotate and substitute
  char *temp = calloc(4,1);
  temp = subBytes(rotWord(col_4));
  temp[0] ^= rcon[iter_num];

  // generate rest of columns
  char *part1 = xor(col_1, temp, 4); 
  char *part2 = xor(col_2, part1, 4); 
  char *part3 = xor(col_3, part2, 4); 
  char *part4 = xor(col_4, part3, 4); 

  // copy into output
  memcpy(new_block,      part1, 4); 
  memcpy(new_block + 4,  part2, 4); 
  memcpy(new_block + 8,  part3, 4); 
  memcpy(new_block + 12, part4, 4); 

  free(temp);
  return new_block;
}

// from a 16 byte word, use the 1st, 5th
char *key_schedule(char *cipher)
{
  // 177
  char *expanded_key = calloc(177, 1); 
  char *current_block = calloc(16, 1);
  char *prev_block = calloc(16, 1);
  // first part of the key schedule is the cipher key itself
  memcpy(expanded_key, cipher, 16);
  prev_block = cipher;
  // 10 iterations for 128 bit keys
  for(int k=0;k<10;k++)
    { 
      current_block = key_schedule_iteration(prev_block, k+1);
      memcpy(expanded_key + (16 * (1+k)), current_block, 16);
      printf("\n");
      prev_block = current_block;
    }
  return(expanded_key);
}

char *addRoundKey(char *state, char *extended_key, int num_iter)
{
  char *key_part = calloc(16,1);
  memcpy(key_part, extended_key+num_iter*16, 16);
  key_part = xor(state, key_part, 16);
  return key_part;
}

void shiftRows(char *state){
  char temp0, temp1, temp2, temp3;
  // 2nd row
  temp0 = state[1];
  temp1 = state[5];
  temp2 = state[9];
  temp3 = state[13];
  state[1] = temp1;
  state[5] = temp2;
  state[9] = temp3;
  state[13] = temp0;
  // 3rd row
  temp0 = state[2];
  temp1 = state[6];
  temp2 = state[10];
  temp3 = state[14];
  state[2] = temp2;
  state[6] = temp3;
  state[10] = temp0;
  state[14] = temp1;
  // 4th row
  temp0 = state[3];
  temp1 = state[7];
  temp2 = state[11];
  temp3 = state[15];
  state[3] = temp3;
  state[7] = temp0;
  state[11] = temp1;
  state[15] = temp2;
}

void inverseShiftRows(char *state){
  char temp0, temp1, temp2, temp3;
  // 2nd row
  temp0 = state[1];
  temp1 = state[5];
  temp2 = state[9];
  temp3 = state[13];
  state[1] = temp3;
  state[5] = temp0;
  state[9] = temp1;
  state[13] = temp2;
  // 3rd row
  temp0 = state[2];
  temp1 = state[6];
  temp2 = state[10];
  temp3 = state[14];
  state[2] = temp2;
  state[6] = temp3;
  state[10] = temp0;
  state[14] = temp1;
  // 4th row
  temp0 = state[3];
  temp1 = state[7];
  temp2 = state[11];
  temp3 = state[15];
  state[3] = temp1;
  state[7] = temp2;
  state[11] = temp3;
  state[15] = temp0;
}

void mixColumns(char *state){
  
  char col_1[4] = {0};
  char col_2[4] = {0};
  char col_3[4] = {0};
  char col_4[4] = {0};

  for(int k=0;k<4;k++)
    {
      col_1[k] = state[k];
      col_2[k] = state[4+k];
      col_3[k] = state[8+k];
      col_4[k] = state[12+k];
    }
  
  gmix_column(col_1);
  gmix_column(col_2);
  gmix_column(col_3);
  gmix_column(col_4);

  memcpy(state,      col_1, 4); 
  memcpy(state + 4,  col_2, 4); 
  memcpy(state + 8,  col_3, 4); 
  memcpy(state + 12, col_4, 4); 
}

void inverseMixColumns(char *state){
  
  char col_1[4] = {0};
  char col_2[4] = {0};
  char col_3[4] = {0};
  char col_4[4] = {0};

  for(int k=0;k<4;k++)
    {
      col_1[k] = state[k];
      col_2[k] = state[4+k];
      col_3[k] = state[8+k];
      col_4[k] = state[12+k];
    }
  
  gmix_column(col_1);
  gmix_column(col_2);
  gmix_column(col_3);
  gmix_column(col_4);

  memcpy(state,      col_1, 4); 
  memcpy(state + 4,  col_2, 4); 
  memcpy(state + 8,  col_3, 4); 
  memcpy(state + 12, col_4, 4); 
}

char *aes_128_encrypt(char *plain_text, char *cipher_key){
  char *state = calloc(17,1);
  char *extended_key = key_schedule(cipher_key);

  state = plain_text;
  state = addRoundKey(state,extended_key,0);

  for (int i=0;i<9;i++){
    subBytes2(state);
    shiftRows(state);
    mixColumns(state);
    state = addRoundKey(state,extended_key,i+1);
  }

  subBytes2(state);
  shiftRows(state);
  state = addRoundKey(state,extended_key,10);
  state[16] = '\0';
  return state;
}

char *aes_128_decrypt(char *plain_text, char *cipher_key, int len){
  char *out = calloc(len,1);
  char *extended_key = key_schedule(cipher_key);
  char *state = calloc(16,1);

  for (int i=0;i<len;i+=16) {
    // if the length is not a factor of 16, some garbage might get into the last block.
    state = plain_text+i;
    state = addRoundKey(state,extended_key,10);
    inverseShiftRows(state);
    inverseSubBytes2(state);

    for (int j=9;j>=1;j--){
      state = addRoundKey(state,extended_key,j);
      inverseMixColumns(state);
      inverseShiftRows(state);
      inverseSubBytes2(state);
    }
    
    state = addRoundKey(state,extended_key,0);
    memcpy(out+i, state, 16);
  }

  return out;
}

static const char* encoding = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

char *bin2hex(char *bin, int len)
{
  char *out = calloc(len/4, 1);
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

static const char* hex_encoding = "0123456789abcdef";

char hex2int(char inp) {
  // no error checking, assumes whatever you feeds inside is a char within hex_encoding
  int temp = 0;
  for (int j=0;j<16;j++){
    if (inp == hex_encoding[j]){
      temp = j; 
      break;
    }
  }
  return temp;
}

//takes a character array of n values and returns a character array of n/2 values
char* hex2char(char *inp, int len){
  char *out = calloc(len/2, 1);
  for(int i=0;i<len;i+=2){
    out[i/2] = hex2int(inp[i]) * 16 + hex2int(inp[i+1]);
    //printf("out[%d] = %d from %d and %d\n", i/2, out[i/2], hex2int(inp[i]), hex2int(inp[i+1]));
  }
  return out;
}

char *base64_to_char(char *inp, int len)
{
  //base 64 = 6 bits for 1 character, comes in sets of 4 chars. 24 bits / 3 bytes 
  //hex = 4 bits for 1 character. 
  char *hex = calloc(len*6/4, 1);
  //1 char = 2 hex characters.
  char *out = calloc(len*3/4, 1);
  for(int i=0;i<len/4;i++)
    {
      char bin[24] = {0};
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
              bin[5-l+k*6] = ((val>>l)&1) + '0';
            }
        }
      /* to confirm whether bin and hex values are okay
      for (int i=0;i<24;i++){
        printf("%c", bin[i]);
        if (i%8 == 0)
          printf("\n");
      }
      */
      for (int i=0;i<6;i++){
        printf("%c", bin2hex(bin, 24)[i]);
      }
      
      memcpy(hex+(i*6), bin2hex(bin, 24), 6);
    }
  out = hex2char(hex, 90);
  return out;
}

int main()
{
  FILE *ptr_file;
  char *given_key = "YELLOW SUBMARINE";   
  //char test_key[16] = {0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6,0xab,0xf7,0x15,0x88,0x09,0xcf,0x4f,0x3c};
  //char test_plaintext[16] = {0x6b,0xc1,0xbe,0xe2,0x2e,0x40,0x9f,0x96,0xe9,0x3d,0x7e,0x11,0x73,0x93,0x17,0x2a};
  //char *test_ciphertext = aes_128_encrypt(test_plaintext, test_key);
  //char test_ciphertext[16] = {0x3a,0xd7,0x7b,0xb4,0x0d,0x7a,0x36,0x60,0xa8,0x9e,0xca,0xf3,0x24,0x66,0xef,0x97};
  //char *test_plaintext = aes_128_decrypt(test_ciphertext, test_key);

  //facilitates testing
  //test vectors found at http://csrc.nist.gov/publications/nistpubs/800-38a/sp800-38a.pdf
  //char expected[16] = {0x3a,0xd7,0x7b,0xb4,0x0d,0x7a,0x36,0x60,0xa8,0x9e,0xca,0xf3,0x24,0x66,0xef,0x97};
  //char expected[16] = {0x6b,0xc1,0xbe,0xe2,0x2e,0x40,0x9f,0x96,0xe9,0x3d,0x7e,0x11,0x73,0x93,0x17,0x2a};

  /*
  for (int j=0;j<16;j++){
    printf("state[%d] = %d\n", j, test_plaintext[j]);
    printf("expected = %d\n", expected[j]);
  }
  */

  ptr_file =fopen("ex7.txt","r");
  if (!ptr_file)
    return 1;
  
  char buf[61] = {0};
  int file_len = 64*45;
  char *full_file = calloc(file_len+1, 1);
  //3903
  int counter = 0; 
  char *temp = calloc(90,1);
  full_file[file_len] = 0;
  while (fgets(buf,61, ptr_file)!=NULL)
    {
      if (strcmp(buf, "\n")) {
        /*
        for (int i=0;i<60;i++){
          printf("%c", buf[i]);
        }
        printf("\nend of base64!\n");
        */
        temp = base64_to_char(buf, 60);
        memcpy(full_file+counter, temp, 45);
        counter+=45;
      }
    }
  fclose(ptr_file);
  full_file[file_len] = '\0';

  printf("\nend of full file!\n");
  char *answer = aes_128_decrypt(full_file, given_key, file_len);
  printf("%s\n", answer);
  return 0;
  
}

//unit tests
