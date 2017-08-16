#include "stdio.h"
#include "stdlib.h"

int main(int argc, char **argv) {
  int a_rows, a_columns = 0;
  FILE *f1 = fopen(argv[1], "r");
  printf("%s\n",argv[1]);
  fscanf( f1, "%d\n%d\n",&a_rows,&a_columns);
  printf("Filas: %d, Columnas: %d\n",&a_rows, &a_columns);
  return 0;
}
