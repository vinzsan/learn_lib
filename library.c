#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <time.h>

typedef char ALIGN[16];

typedef union {
  struct{
    void *ptr;
    size_t size;
    unsigned int _flags;
  } OwO;
  ALIGN align;
} NODE;

void *_mmap(void *ptr,size_t size,uint32_t prot,uint32_t mode,uint32_t fd,uint32_t offst){
  void *p = NULL;
  __asm__ __volatile__(
    "movq $9,%%rax\n\t"
    "movq %1,%%rdi\n\t"
    "movq %2,%%rsi\n\t"
    "movq %3,%%rdx\n\t"
    "movq %4,%%r10\n\t" 
    "movq %5,%%r8\n\t"
    "movq %6,%%r9\n\t"
    "syscall\n\t"
    "movq %%rax,%0"
    :"=r"(p)
    :"r"(ptr),"r"(size),"r"((ulong)prot),"r"((ulong)mode),"r"((ulong)fd),"r"((ulong)offst)
    :"rax","rdi","rsi","rdx","r10","r8","r9","memory"
  );
  return p;
}

float fast_div(float a,float b){
  float ret = 0;
  __asm__ __volatile__(
    "movss %1,%%xmm0\n\t"
    "movss %2,%%xmm1\n\t"
    "divss %%xmm1,%%xmm0\n\t"
    "movss %%xmm0,%0"
    :"=m"(ret)
    :"m"(a),"m"(b)
    :"xmm0","xmm1","memory"
  );
  return ret;
}

int _munmap(void *ptr,size_t size){
  if(ptr == (void *)-1){
    return -1;
  }
  long retval = 0;
  __asm__ __volatile__(
    "movq $11,%%rax\n\t"
    "movq %1,%%rdi\n\t"
    "movq %2,%%rsi\n\t"
    "syscall\n\t"
    "movq %%rax,%0"
    :"=r"(retval)
    :"r"(ptr),"r"(size)
    :"rax","rdi","rsi","memory"
  );
  return (int)retval;
}

int _write(int fd,char *restrict src,size_t size){
  long retval = 0;
  __asm__ __volatile__(
    "movq $1,%%rax\n\t"
    "movq %1,%%rdi\n\t"
    "movq %2,%%rsi\n\t"
    "movq %3,%%rdx\n\t"
    "syscall\n\t"
    "movq %%rax,%0"
    :"=r"(retval)
    :"r"((long)fd),"r"(src),"r"(size)
    :"rax","rdi","rsi","rdx","memory"
  );
  return (int)retval;
}

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
    NODE *node = _mmap(NULL,sizeof(NODE) + n,PROT_WRITE|PROT_READ,MAP_PRIVATE|MAP_ANONYMOUS,-1,0);
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
  else _munmap(node,sizeof(NODE) + node->OwO.size);
  return;
}

size_t stlen(char *restrict src){
  if(!src) return 0;
  size_t n = 0;
  for(;n[src];n++);
  return n;
}

int print(char *restrict str){
  return _write(STDOUT_FILENO,str,stlen(str));
}

void *memst(void *restrict ptr,int s,size_t n){
  unsigned char *p = (unsigned char *)ptr;
  unsigned char dl = (unsigned char)s;
  for(int i = 0;i < n;i++){
    *(p + i) = dl;
  }
  return p;
}

void *memcopy(void *restrict dest,void *restrict src,size_t n){
  if(!dest || !src) return NULL;
  unsigned char *p1 = (unsigned char *)dest;
  unsigned char *p2 = (unsigned char *)src;
  for(int i = 0;i < n;i++){
    *(p1 + i) = *(p2 + i);
  }
  return dest;
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

void ptr_sort_arr(int *arr,size_t n){
  for(int i = 0;i < n - 1;i++){
    for(int j = 0;j <n - 1;j++){
      if(*(arr + j) > *(arr + j + 1)){
        int temp = *(arr + j);
        //*(arr + j) = *(arr + j);
        *(arr + j) = *(arr + j + 1);
        *(arr + j + 1) = temp;
      }
    }
  }
}

//void 
__attribute__((visibility("hidden"),used))
int main(){
  srand(time(NULL));
  char *ptr = (char *)alloc_filter(1024);
  char *header = "Hello world\n";
  memcopy(ptr,header,stlen(header));
  printf("%s",ptr);

  printf("%f\n",fast_div(50.0,5.0));
  print("Hello world\n");

  int arr[10];
  int size = sizeof(arr)/sizeof(arr[0]);
  for(int i = 0;i < size;arr[i++] = rand() % 50);
  for(int i = 0;i < size;printf("%d ",arr[i++]));

  int_sort_arr(arr,size);

  puts("\n");
  for(int i = 0;i < size;printf("%d ",arr[i++]));
  deallocate(ptr);
  return 0;
}
