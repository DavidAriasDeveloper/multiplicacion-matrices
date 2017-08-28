/******************************************************************************
* FILE: omp_mm.c
* DESCRIPTION:
*   OpenMp Example - Matrix Multiply - C Version
*   Demonstrates a matrix multiply using OpenMP. Threads share row iterations
*   according to a predefined chunk size.
* AUTHOR: Blaise Barney
* LAST REVISED: 06/28/05
******************************************************************************/
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include "time.h"

#define NRA 200                 /* number of rows in matrix A */
#define NCA 200                 /* number of columns in matrix A */
#define NCB 200                  /* number of columns in matrix B */

int main (int argc, char *argv[])
{
int	tid, nthreads, i, j, k, chunk;
clock_t par_t_begin, par_t_end;
double par_secs;

double	a[NRA][NCA],           /* matrix A to be multiplied */
	b[NCA][NCB],           /* matrix B to be multiplied */
	c[NRA][NCB];           /* result matrix C */

chunk = 10;                    /* set loop iteration chunk size */

/*** Spawn a parallel region explicitly scoping all variables ***/
#pragma omp parallel shared(a,b,c,nthreads,chunk) private(tid,i,j,k)
  {
  tid = omp_get_thread_num();
  if (tid == 0)
    {
    nthreads = omp_get_num_threads();
    printf("Starting matrix multiple example with %d threads\n",nthreads);
    printf("Initializing matrices...\n");
    }
  /*** Initialize matrices ***/
  #pragma omp for schedule (static, chunk)
  for (i=0; i<NRA; i++)
    for (j=0; j<NCA; j++)
      a[i][j]= i+j;
  #pragma omp for schedule (static, chunk)
  for (i=0; i<NCA; i++)
    for (j=0; j<NCB; j++)
      b[i][j]= i*j;
  #pragma omp for schedule (static, chunk)
  for (i=0; i<NRA; i++)
    for (j=0; j<NCB; j++)
      c[i][j]= 0;

  /*** Do matrix multiply sharing iterations on outer loop ***/
  /*** Display who does which iterations for demonstration purposes ***/
  printf("Thread %d starting matrix multiply...\n",tid);
  par_t_begin = clock();
  #pragma omp for schedule (static, chunk)
  for (i=0; i<NRA; i++)
    {
    printf("Thread=%d did row=%d\n",tid,i);
    for(j=0; j<NCB; j++)
      for (k=0; k<NCA; k++)
        c[i][j] += a[i][k] * b[k][j];
    }
  }   /*** End of parallel region ***/
  par_t_end = clock();

  par_secs = (double)(par_t_end - par_t_begin) / CLOCKS_PER_SEC;

/*** Print results ***/
printf("******************************************************\n");
printf("\nParalelo: La operacion se realizo en %.16g milisegundos\n", par_secs * 1000.0);
printf("******************************************************\n");
printf ("Done.\n");

}
