#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

extern void *memst(void *restrict src,int index,size_t n);
extern void int_sort_arr(int arr[],size_t n);
extern size_t stlen(char *restrict src);

int main(){
  srand(time(NULL));
  int array[10];
  size_t n = sizeof(array)/sizeof(array[0]);
  for(int i = 0;i < n;i++){
    array[i] = rand() % 50;
    printf("%d ",array[i]);
  }
  puts("\n");
  int_sort_arr(array,n);
  for(int i = 0;i < n;printf("%d ",array[i++]));
  memst(&array,0,n * sizeof(int));
  puts("\n");
  for(int i = 0;i < n;printf("%d ",array[i++]));
  printf("\nstrlen : %zu\n",stlen("Hello world\n"));
  return 0;
}
