/*
  Multiplicación de Matrices (Algoritmo Secuencial) en C
  Por: Luis David Arias Manjarrez
  Universidad Tecnologica de Pereira
*/

#include "stdio.h"
#include "stdlib.h"
#include "windows.h"

//Funcion para medir el tiempo
/* retorna "a - b" en segundos */
double performancecounter_diff(LARGE_INTEGER *a, LARGE_INTEGER *b)
{
  LARGE_INTEGER freq;
  QueryPerformanceFrequency(&freq);
  return (double)(a->QuadPart - b->QuadPart) / (double)freq.QuadPart;
}

//Funcion para multiplicar matrices
int multiplyMatrix(int a_rows,
                    int a_cols,
                    int a_matrix[a_rows][a_cols],
                    int b_rows,
                    int b_cols,
                    int b_matrix[b_rows][b_cols],
                    int product[a_rows][b_cols]){

  int a_row,a_col,b_col=0;

  for(a_row=0;a_row<a_rows;a_row++){//Ciclo para moverse entre las filas de la matriz a
    for(b_col=0;b_col<b_cols;b_col++){//Ciclo para moverse entre las columnas de b
      int temp_cell = 0;//Variable para almacenar los datos de la celda en la matriz producto
      for(a_col=0;a_col<a_cols;a_col++){//Ciclo para moverse entre las columnas de a
        temp_cell= temp_cell + (a_matrix[a_row][a_col]*b_matrix[a_col][b_col]);
        //printf("a[%d][%d]xb[%d][%d]=%d\n", a_row,a_col,a_col,b_col,a_matrix[a_row][a_col]*b_matrix[a_col][b_col]);
      }
      //printf("product[%d][%d]= %d\n", a_row,b_col,temp_cell);
      product[a_row][b_col]=temp_cell;
    }
  }
}

//Funcion para llenar matrices
void fillMatrix(FILE *file,int rows,int columns,int matrix[rows][columns]){
  for(int i=0;i<rows;i++){
    for(int j=0;j<columns;j++){
      fscanf(file,"%d,", &matrix[i][j]);
    }
  }
  return;
}
//Funcion para escribir matrices en un fichero
void exportMatrix(FILE *file,int rows,int columns,int matrix[rows][columns]){
  fprintf(file, "%d\n%d\n", rows,columns);
  for(int i=0;i<rows;i++){
    for(int j=0;j<columns;j++){
      fprintf(file,"%d", matrix[i][j]);
      if(j+1 < columns){//Para conservar el formato especificado
        fprintf(file, ",");
      }
    }
    fprintf(file,"\n");
  }
  return;
}

//Funcion para imprimir matrices
void printMatrix(int rows,int columns,int matrix[rows][columns]){
  printf("Matriz %dx%d: \n",rows,columns);
  for(int i=0;i<rows;i++){
    for(int j=0;j<columns;j++){
      printf("[%d]", matrix[i][j]);
    }
    printf("\n");
  }
}

int main(int argc, char **argv) {
  //Variables de tiempo
  LARGE_INTEGER t_ini, t_fin;
  double secs;
  //Declaramos los elementos de la matriz a
  int a_rows, a_columns = 0;
  //Declaramos los elementos de la matriz b
  int b_rows, b_columns = 0;

  //Declaramos los archivos a leer
  //FILE *file1 = fopen(argv[1], "r");
  //FILE *file2 = fopen(argv[2], "r");
  FILE *file1 = fopen("matrix_a.txt", "r");
  FILE *file2 = fopen("matrix_b.txt", "r");

  if(file1==NULL){//Comprobamos la lectura exitosa
    printf("Error, no se encuentra el archivo\n");
  }else{
    printf("Multiplicacion de Matrices\n");
    //printf("Archivos:\n");
    //printf("%s\n",argv[1]);
    //printf("%s\n",argv[2]);

    //Obtenemos el numero de filas y columnas de ambas matrices
    fscanf(file1, "%d\n%d\n",&a_rows,&a_columns);
    fscanf(file2, "%d\n%d\n",&b_rows,&b_columns);

    //Definimos las matrices
    int a_matrix[a_rows][a_columns];
    int b_matrix[b_rows][b_columns];

    //Llenamos las matrices
    fillMatrix(file1,a_rows,a_columns,a_matrix);
    fillMatrix(file2,b_rows,b_columns,b_matrix);

    //Imprimimos las matrices
    printf("\nMatriz A: Filas: %d, Columnas: %d\n",a_rows, a_columns);
    printMatrix(a_rows,a_columns,a_matrix);
    printf("\nMatriz B: Filas: %d, Columnas: %d\n",b_rows, b_columns);
    printMatrix(b_rows,b_columns,b_matrix);

    if(a_columns == b_rows){
      int product[a_rows][b_columns];//Se declara la matriz producto

      QueryPerformanceCounter(&t_ini);//Se establece el tiempo inicial de medicion
      //Realizamos la multiplicacion
      product[a_rows][b_columns] = multiplyMatrix(a_rows,a_columns,a_matrix,b_rows,b_columns,b_matrix,product);
      QueryPerformanceCounter(&t_fin);//Se establece el tiempo final de medicion

      printf("\nMatriz Producto: Filas: %d, Columnas: %d\n",a_rows,b_columns);
      printMatrix(a_rows,b_columns,product);

      //Datos de tiempo
      secs = performancecounter_diff(&t_fin, &t_ini);
      printf("\nLa operacion se realizo en %.16g milisegundos\n", secs * 1000.0);

      //Exportamos la matriz a un fichero de texto
      FILE *export = fopen("matrix_product.txt","w");
      exportMatrix(export,a_rows,b_columns,product);
      fclose(export);
    }else{
      printf("\nNo es posible realizar la multiplicacion\n");
    }
  }
  fclose(file1);
  fclose(file2);

  return 0;
}
