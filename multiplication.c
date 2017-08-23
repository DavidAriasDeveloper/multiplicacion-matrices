/*
  Multiplicación de Matrices en C
  Por: Luis David Arias Manjarrez
  Universidad Tecnologica de Pereira
*/

#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "omp.h"

#define NRA 2000                 /* number of rows in matrix A */
#define NCA 2000                 /* number of columns in matrix A */
#define NRB 2000                 /* number of rows in matrix A */
#define NCB 2000                  /* number of columns in matrix B */

//Funcion para multiplicar matrices
void sequential_multiplyMatrix(int a_rows,
                    int a_cols,
                    double **a_matrix,
                    int b_rows,
                    int b_cols,
                    double **b_matrix,
                    double **product){
  int a_row,a_col,b_col=0;

  for(a_row=0;a_row<a_rows;a_row++){//Ciclo para moverse entre las filas de la matriz a
    for(b_col=0;b_col<b_cols;b_col++){//Ciclo para moverse entre las columnas de b
      double temp_cell = 0;//Variable para almacenar los datos de la celda en la matriz producto
      for(a_col=0;a_col<a_cols;a_col++){//Ciclo para moverse entre las columnas de a
        temp_cell= temp_cell + (a_matrix[a_row][a_col]*b_matrix[a_col][b_col]);
        //printf("a[%d][%d]xb[%d][%d]=%d\n", a_row,a_col,a_col,b_col,a_matrix[a_row][a_col]*b_matrix[a_col][b_col]);
      }
      //printf("product[%d][%d]= %d\n", a_row,b_col,temp_cell);
      product[a_row][b_col]=temp_cell;
    }
  }
}

//Funcion para multiplicar matrices
void parallel_multiplyMatrix(int a_rows,
                    int a_cols,
                    double **a_matrix,
                    int b_rows,
                    int b_cols,
                    double **b_matrix,
                    double **product){


  int	tid, nthreads, i, j, k;
  int temp_cell;
  int chunk = 10;

  #pragma omp parallel shared(a_matrix,b_matrix,product) private(i,j,k,tid,temp_cell)
  {
    tid = omp_get_thread_num();
    nthreads = omp_get_num_threads();

    printf("Numero de hilos: %d\n",nthreads);

    #pragma omp for schedule (dynamic, chunk)
    for (i=0; i<NRA; i++){
      //printf("Thread=%d -> Fila=%d\n",tid,i);
      for(j=0; j<NCB; j++){
        temp_cell=0;
        for (k=0; k<NCA; k++){
          temp_cell += a_matrix[i][k] * b_matrix[k][j];
        }
        product[i][j] = temp_cell;
      }
    }
  }
}

void allocMatrix(int rows,int columns,double **matrix){
  for(int i = 0; i < rows; i++)
    matrix[i] = (double *)malloc(columns*sizeof(double));
}

//Funcion para llenar matrices
void fillMatrix(FILE *file,int rows,int columns,double **matrix){
  for(int i=0;i<rows;i++){
    for(int j=0;j<columns;j++){
      fscanf(file,"%lf,", &matrix[i][j]);
    }
  }
}

//Funcion para llenar matrices automaticamente
void fillAutoMatrix(int rows,int columns,double **matrix){
  srand((unsigned)time(NULL));
  int i,j = 0;
  for (i=0; i<rows; i++)
    for (j=0; j<columns; j++)
      matrix[i][j]= rand()%100;
}

//Function para inicializar matriz automatica
void parallel_fillAutoMatrix(int rows,int columns,double **matrix){
  srand((unsigned)time(NULL));
  int i,j = 0;
  #pragma omp for schedule (static, 10)
  for (i=0; i<rows; i++){
    for (j=0; j<columns; j++){
      matrix[i][j]= rand()%100;
    }
  }
}

//Funcion para escribir matrices en un fichero
void exportMatrix(FILE *file,int rows,int columns,double **matrix){
  fprintf(file, "%d\n%d\n", rows,columns);
  for(int i=0;i<rows;i++){
    for(int j=0;j<columns;j++){
      fprintf(file,"%lf", matrix[i][j]);
      if(j+1 < columns){//Para conservar el formato especificado
        fprintf(file, ",");
      }
    }
    fprintf(file,"\n");
  }
  return;
}

//Funcion de comparacion de matrices
int compareMatrix(int a_row,int a_col,int b_row,int b_col,double **a,double **b){
  for(int row = 0;row<a_row;row++){
    for(int col = 0;col<a_col;col++){
      if(a[row][col]!=b[row][col]){
        return 0;
      }
    }
  }
  return 1;
}

//Funcion para imprimir matrices
void printMatrix(int rows,int columns,double matrix[rows][columns]){
  printf("\nMatriz %dx%d: \n",rows,columns);
  for(int i=0;i<rows;i++){
    for(int j=0;j<columns;j++){
      printf("[%d]",(int)matrix[i][j]);
    }
    printf("\n");
  }
}

int main(int argc, char **argv) {
  //Variables de tiempo
  clock_t seq_t_begin, seq_t_end;
  clock_t par_t_begin, par_t_end;

  double seq_secs;
  double par_secs;

  //Declaramos los elementos de la matriz a
  int a_rows, a_columns = 0;
  //Declaramos los elementos de la matriz b
  int b_rows, b_columns = 0;

  //Declaramos los archivos a leer
  FILE *file1,*file2;

  if(argc > 1){//Para controlar la lectura por linea de comandos
    file1 = fopen(argv[1], "r");
    file2 = fopen(argv[2], "r");
    printf("Archivos:\n");
    printf("%s\n",argv[1]);
    printf("%s\n",argv[2]);

    if(file1 != NULL){
      //Obtenemos el numero de filas y columnas de ambas matrices
      fscanf(file1, "%d\n%d\n",&a_rows,&a_columns);
      fscanf(file2, "%d\n%d\n",&b_rows,&b_columns);
    }else{
      printf("Error, no se encuentra el archivo\n");
      return 0;
    }
  }else{
    a_rows = NRA;
    a_columns = NCA;
    b_rows = NRB;
    b_columns = NCB;
  }
  printf("Multiplicacion de Matrices\n");

  printf("%d,%d,%d,%d\n",a_rows,a_columns,b_rows,b_columns );
  //Definimos las matrices
  double **a_matrix=(double **)malloc(a_rows*sizeof(double *));
  double **b_matrix=(double **)malloc(b_rows*sizeof(double *));

  allocMatrix(a_rows,a_columns,a_matrix);
  allocMatrix(b_rows,b_columns,b_matrix);

  if(argc > 1){
    //Llenamos las matrices
    fillMatrix(file1,a_rows,a_columns,a_matrix);
    fillMatrix(file2,b_rows,b_columns,b_matrix);
  }else{
    //Llenamos las matrices automaticamente
    parallel_fillAutoMatrix(NRA,NCA,a_matrix);
    parallel_fillAutoMatrix(NRB,NCB,b_matrix);
  }

  //Imprimimos las matrices
  // printf("\nMatriz A: Filas: %d, Columnas: %d\n",a_rows, a_columns);
  // printMatrix(a_rows,a_columns,a_matrix);
  // printf("\nMatriz B: Filas: %d, Columnas: %d\n",b_rows, b_columns);
  // printMatrix(b_rows,b_columns,b_matrix);

  if(a_columns == b_rows){
    double **sequential_product = (double **)malloc(a_rows*sizeof(double *));//Se declara la matriz producto
    double **parallel_product = (double **)malloc(a_rows*sizeof(double *));

    allocMatrix(a_rows,b_columns,sequential_product);
    allocMatrix(a_rows,b_columns,parallel_product);

    seq_t_begin = clock();
    //Realizamos la multiplicacion
    sequential_multiplyMatrix(a_rows,a_columns,a_matrix,b_rows,b_columns,b_matrix,sequential_product);
    seq_t_end = clock();

    par_t_begin = clock();
    //Realizamos la multiplicacion
    parallel_multiplyMatrix(a_rows,a_columns,a_matrix,b_rows,b_columns,b_matrix,parallel_product);
    par_t_end = clock();

    printf("\nMatriz Producto: Filas: %d, Columnas: %d\n",a_rows,b_columns);
    //printMatrix(a_rows,b_columns,sequential_product);
    //printMatrix(a_rows,b_columns,parallel_product);

    if(!compareMatrix(a_rows,b_columns,a_rows,b_columns,sequential_product,parallel_product)){
      printf("\nLas matrices coinciden\n");
    }

    //Datos de tiempo
    seq_secs = (double)(seq_t_end - seq_t_begin) / CLOCKS_PER_SEC;
    par_secs = (double)(par_t_end - par_t_begin) / CLOCKS_PER_SEC;

    printf("\nSecuencial: La operacion se realizo en %.16g milisegundos\n", seq_secs * 1000.0);
    printf("\nParalelo: La operacion se realizo en %.16g milisegundos\n", par_secs * 1000.0);

    //Exportamos la matriz a un fichero de texto
    FILE *export = fopen("matrix_product.txt","w");
    exportMatrix(export,a_rows,b_columns,sequential_product);
    fclose(export);
  }else{
    printf("\nNo es posible realizar la multiplicacion\n");
  }
  if(argc > 1){
    fclose(file1);
    fclose(file2);
  }

  return 0;
}
