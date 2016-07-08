#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <inttypes.h>

/*
char *find_most_frequent_file(char *inp, int len){

}
*/
 
uint64_t hex_string_to_uint64( char *s )
{
  int i, shift;
  uint64_t r;
  for( i = 0, r = 0, shift = 60 ; i < 16 ; i++, shift -= 4 )
  {
    if( ( s[i] >= '0' ) && ( s[i] <= '9' ) )
      r |= ( (uint64_t)( s[i] - '0' ) ) << shift;
    else if( ( s[i] >= 'a' ) && ( s[i] <= 'f' ) )
      r |= ( (uint64_t)( s[i] - ( 'a' - 10 ) ) ) << shift;
    else if( ( s[i] >= 'A' ) && ( s[i] <= 'F' ) )
      r |= ( (uint64_t)( s[i] - ( 'A' - 10 ) ) ) << shift;
    else
      return 0;
  }
  return r;
}

int compare(const void * a, const void * b)
{
  uint64_t x =  *(uint64_t*)a;
  uint64_t y = *(uint64_t*)b;
  //printf("x =%" PRIu64 " and y =%" PRIu64 ", x-y =%" PRIu64 "\n", x, y, x-y);
  if (x > y)
    return -1;
  else
    return 1;
}

int main()
{
  FILE *ptr_file;
  
  ptr_file =fopen("ex8.txt","r");
  if (!ptr_file)
    return 1;
  
  char buf[320] = {0};
  int file_len = 204*320;
  char *full_file = calloc(file_len+1, 1);
  //3903
  int counter = 0; 
  full_file[file_len] = 0;
  while (fgets(buf, 321, ptr_file)!=NULL)
    {
      if (strcmp(buf, "\n")) {
        memcpy(full_file+counter, buf, 320);
        counter += 320;
      }
    }
  fclose(ptr_file);
  full_file[file_len] = '\0';
  
  char test[16] ={0};
  char *most_frequent = calloc(17,1);
  int list_of_terms = 4080;
  uint64_t *list = calloc(list_of_terms, sizeof(uint64_t));
  for (int i=0;i<file_len;i+=16){
    memcpy(test, full_file+i, 16);
    list[i/16] = hex_string_to_uint64(test);
    //printf("%lu", strlen(test));
    //printf("%" PRIu64 "\n", list[i/16]);
  }
  qsort(list, list_of_terms, sizeof(uint64_t), compare);

  for (int i=0;i<4080;i++){
    printf("%" PRIu64 "\n", list[i]);
  }

  //printf("\n\n\n\n%" PRIu64 "\n", hex_string_to_uint64("06609df433d587c"));
}
