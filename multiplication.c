/*
  Multiplicación de Matrices (Algoritmo Paralelo) en C
  Por: Luis David Arias Manjarrez
  Universidad Tecnologica de Pereira
*/

#include "omp.h"
#include "stdio.h"
#include "stdlib.h"
#include "time.h"

//Funcion para multiplicar matrices
int multiplyMatrix(int a_rows,
                    int a_cols,
                    int a_matrix[a_rows][a_cols],
                    int b_rows,
                    int b_cols,
                    int b_matrix[b_rows][b_cols],
                    int product[a_rows][b_cols]){

  int a_row,a_col,b_col=0;
  int tid,chunk = 10;

  #pragma omp parallel shared(a_matrix,b_matrix,c_matrix,chunk) private(tid,a_row,b_col,a_col)
  {
    tid = omp_get_thread_num();
    printf("\nMultiplicacion\n");
    #pragma omp for schedule (static, chunk){
    for (a_row=0; a_row<a_rows; a_row++)
      printf("Thread=%d did row=%d\n",tid,i);
      for(b_col=0; b_col<b_cols; b_col++)
        for (a_col=0; a_col<a_cols; a_col++)
          product[i][j] += a[i][k] * b[k][j];
        }
  }
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
  //Declaramos los elementos de la matriz a
  int a_rows, a_columns = 0;
  //Declaramos los elementos de la matriz b
  int b_rows, b_columns = 0;

  printf("\nNúmero de procesadores: %d\n", omp_get_num_procs());

  //Declaramos los archivos a leer
  FILE *file1,*file2;

  if(argc > 1){//Para controlar la lectura por linea de comandos
    file1 = fopen(argv[1], "r");
    file2 = fopen(argv[2], "r");
    printf("Archivos:\n");
    printf("%s\n",argv[1]);
    printf("%s\n",argv[2]);
  }else{
    //Declaramos los archivos a leer
    file1 = fopen("matrix_a.txt", "r");
    file2 = fopen("matrix_b.txt", "r");
  }

  if(file1==NULL){//Comprobamos la lectura exitosa
    printf("Error, no se encuentra el archivo\n");
  }else{
    printf("Multiplicacion de Matrices\n");

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

      t_begin = clock();
      //Realizamos la multiplicacion
      product[a_rows][b_columns] = multiplyMatrix(a_rows,a_columns,a_matrix,b_rows,b_columns,b_matrix,product);
      t_end = clock();

      printf("\nMatriz Producto: Filas: %d, Columnas: %d\n",a_rows,b_columns);
      printMatrix(a_rows,b_columns,product);

      //Datos de tiempo
      secs = (double)(t_end - t_begin) / CLOCKS_PER_SEC;
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
