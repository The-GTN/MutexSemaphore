#include <stdio.h>
#include "try.h"

static struct ctx_s pctx;

static int mul(int depth){
  int i;

  switch (scanf("%d", &i)) {
    case EOF :
      return 1; /* neutral element */
    case 0 :
      return mul(depth+1); /* erroneous read */
    case 1 :
      if(i){
        return i * mul(depth+1);
      }else{
        //return 0;
        return throw(&pctx,0);
      }
  }
  return -1;
}

int main(){
  int product;

  printf("A list of int, please\n");

  //product = mul(0);
  product = try(&pctx, mul, 0);


  printf("product = %d\n", product);
}
