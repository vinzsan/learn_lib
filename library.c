#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

size_t stlen(char *restrict src){
  if(!src) return 0;
  size_t n = 0;
  for(;n[src];n++);
  return n;
}

void *memst(void *restrict ptr,int s,size_t n){
  unsigned char *p = (unsigned char *)ptr;
  unsigned char dl = (unsigned char)s;
  for(int i = 0;i < n;i++){
    *(p + i) = dl;
  }
  return p;
}

// ssize_t arr_len(int arr[]){
//   return sizeof(arr)/sizeof(arr[0]);
// }

void int_sort_arr(int arr[],size_t n){
  for(int i = 0;i < n - 1;i++){
    for(int j = 0;j < n - 1;j++){
      if(arr[j] > arr[j + 1]){
        int temp = arr[j];
        arr[j] = arr[j + 1];
        arr[j + 1] = temp;
      }
    }
  }
}
