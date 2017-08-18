/*
  Multiplicación de Matrices (Algoritmo Paralelo) en C
  Por: Luis David Arias Manjarrez
  Universidad Tecnologica de Pereira
*/

#include "omp.h"
#include "stdio.h"
#include "stdlib.h"
#include "time.h"

#define NRA 62                 /* number of rows in matrix A */
#define NCA 15                 /* number of columns in matrix A */
#define NRB 15                 /* number of columns in matrix A */
#define NCB 7                  /* number of columns in matrix B */

//Funcion para multiplicar matrices
int multiplyMatrix(int a_rows,
                    int a_cols,
                    int a_matrix[a_rows][a_cols],
                    int b_rows,
                    int b_cols,
                    int b_matrix[b_rows][b_cols],
                    int product[a_rows][b_cols]){

  int a_row,a_col,b_col=0;
  // #pragma omp parallel for private(tid)
  // for(a_row=0;a_row<a_rows;a_row++){
  //   tid = omp_get_thread_num();
  //   //printf("Operacion desde el hilo %d\n", tid);
  //   #pragma omp parallel for private(tid)
  //   for(b_col=0;b_col<b_cols;b_col++){
  //     //printf("Operacion desde el hilo %d\n", tid);
  //     int temp_cell = 0;
  //     #pragma omp parallel for private(tid)
  //     for(a_col=0;a_col<a_cols;a_col++){
  //       //printf("Operacion desde el hilo %d\n", tid);
  //       temp_cell= temp_cell + (a_matrix[a_row][a_col]*b_matrix[a_col][b_col]);
  //     }
  //     product[a_row][b_col]=temp_cell;
  //   }
  //}
  return 0;
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
//Function para inicializar matriz automatica
void fillAutoMatrix(int rows,int columns,int matrix[rows][columns]){
  #pragma omp for schedule (static, 10)
  for (i=0; i<rows; i++)
    for (j=0; j<columns; j++)
      matrix[i][j]= i+j;
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
  clock_t t_begin, t_end;
  double secs;

  printf("\nNúmero de procesadores: %d\n", omp_get_num_procs());

  printf("Multiplicacion de Matrices\n");

  //Definimos las matrices
  int a_matrix[NRA][NCA];
  int b_matrix[NRB][NCB];
  int product[NRA][NCB];//Se declara la matriz producto

  //Llenamos las matrices
  fillAutoMatrix(NRA,NCA,a_matrix);
  fillAutoMatrix(NRB,NCB,b_matrix);

  //Imprimimos las matrices
  printf("\nMatriz A: Filas: %d, Columnas: %d\n",NRA,NCA);
  printMatrix(NRA,NCA,a_matrix);
  printf("\nMatriz B: Filas: %d, Columnas: %d\n",NRB,NCB);
  printMatrix(NRB,NCB,b_matrix);

  if(NCA == NRB){
    t_begin = clock();
    //Realizamos la multiplicacion
    multiplyMatrix(NRA,NCA,a_matrix,NRB,NCB,b_matrix,product);
    t_end = clock();

    printf("\nMatriz Producto: Filas: %d, Columnas: %d\n",NRA,NCB);
    printMatrix(NRA,NCB,product);

    //Datos de tiempo
    secs = (double)(t_end - t_begin) / CLOCKS_PER_SEC;
    printf("\nLa operacion se realizo en %.16g milisegundos\n", secs * 1000.0);

    //Exportamos la matriz a un fichero de texto
    FILE *export = fopen("matrix_product.txt","w");
    exportMatrix(export,NRA,NCB,product);
    fclose(export);
  }else{
    printf("\nNo es posible realizar la multiplicacion\n");
  }

  return 0;
}
