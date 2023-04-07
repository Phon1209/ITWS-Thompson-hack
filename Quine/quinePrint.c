#include <stdio.h>
// This code is basically the same as the figure 1 of Ken's

int main()
{
  int i;

  // To print the entire quine string first
  printf("char\ts[] = {\n");
  for (i = 0; s[i]; i++)
    printf("\t%d,\n", s[i]);
  printf("\t0\n};\n\n");

  // print the main function part
  printf("%s", s);
  return 0;
}