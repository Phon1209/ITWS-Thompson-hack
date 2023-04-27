// generateQuineString.c
/**
 * This code is copied from second half of Ken's code in Figure 1
 * But modified to receive the value from stdin instead
 */

#include <stdio.h>

int main()
{
  int i;

  printf("char compile_attack[] = \"");
  int c;
  while ((c = fgetc(stdin)) != EOF)
  {
    if(c == '\"')
      printf("\\\"");
    else if(c == '\\')
      printf("\\\\");
    else if(c == '\n')
      continue;
    else printf("%c", c);
  }
  printf("\";");
  return 0;
}