// generateQuineString.c
/**
 * This code is copied from second half of Ken's code in Figure 1
 * But modified to receive the value from stdin instead
 */

#include <stdio.h>

int main()
{
  int i;

  printf("char compile_attack[] = {");
  int c;
  while ((c = fgetc(stdin)) != EOF)
  {
    printf("%d,", c);
  }
  printf("0};");
  return 0;
}