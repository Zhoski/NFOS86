#ifndef ALIGN_H
#define ALIGN_H

#define LOWER    0
#define HIGHTER  1

static u32 align(u32 x, u32 n, u32 flag) {
  if(flag)
    while(x%n!=0) x++;
  else
    while(x%n!=0) x--;

  return x;
}

#endif
