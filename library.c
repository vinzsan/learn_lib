#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>

typedef char ALIGN[16];

typedef union {
  struct{
    void *ptr;
    size_t size;
    unsigned int _flags;
  } OwO;
  ALIGN align;
} NODE;

NODE *alloc_filter(size_t size){
  //NODE *node = NULL;
  size_t n = (size + 8 - 1) & ~(8 - 1);
  if(size < 1024){
    NODE *node = malloc(sizeof(NODE) + n);
    if(node == NULL){
      return NULL;
    }
    //NODE *node = (NODE *)((char *)ptr + 1);
    node->OwO.ptr = (void *)((char *)node + sizeof(NODE));
    node->OwO.size = n;
    node->OwO._flags = 0;
    return node->OwO.ptr;
  }
  else{
    NODE *node = mmap(NULL,sizeof(NODE) + n,PROT_WRITE|PROT_READ,MAP_PRIVATE|MAP_ANONYMOUS,-1,0);
    if(node == (void *)-1){
      return NULL;
    }
    node->OwO.ptr = (void *)((char *)node + sizeof(NODE));
    node->OwO.size = n;
    node->OwO._flags = 1;
    return node->OwO.ptr;
  }
}

void deallocate(void *ptr){
  NODE *node = (NODE *)((char *)ptr - sizeof(NODE));
  if(node->OwO._flags == 0 && node->OwO.size < 1024){
    free(node);
    return;
  }
  else munmap(node,sizeof(NODE) + node->OwO.size);
  return;
}

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

int main(){
  char *ptr = (char *)alloc_filter(10);
  deallocate(ptr);
  return 0;
}
