// File: vadd_gpu_dynamic.c
#include <stdio.h>
#include <stdlib.h> // Necessário para malloc e free
#include <omp.h>
#define N 10000000
#define TOL  0.0000001
//
//  Versão com alocação dinâmica de memória (heap).
//
int main()
{
    // --- ALTERAÇÃO 1: Declaração de Ponteiros ---
    float *a, *b, *c, *res;
    int err=0;

    // --- ALTERAÇÃO 2: Alocação com malloc ---
    a = (float*) malloc(sizeof(float) * N);
    b = (float*) malloc(sizeof(float) * N);
    c = (float*) malloc(sizeof(float) * N);
    res = (float*) malloc(sizeof(float) * N);

    // Verificação simples para garantir que a memória foi alocada
    if (a == NULL || b == NULL || c == NULL || res == NULL) {
        printf("Erro na alocação de memória.\n");
        return 1;
    }

    double init_time, compute_time, test_time;
    init_time    = -omp_get_wtime();

   // Etapa de inicialização na CPU
   #pragma omp parallel for
   for (int i=0; i<N; i++){
      a[i] = (float)i;
      b[i] = 2.0*(float)i;
      c[i] = 0.0;
      res[i] = i + 2*i;
   }
   init_time    +=  omp_get_wtime();
   
   compute_time  = -omp_get_wtime();
   
   // --- ALTERAÇÃO 3: Cláusula map atualizada para ponteiros ---
   #pragma omp target teams distribute parallel for map(to:a[0:N], b[0:N]) map(from:c[0:N])
   for (int i=0; i<N; i++){
      c[i] = a[i] + b[i];
   }
   compute_time +=  omp_get_wtime();
   
   test_time     = -omp_get_wtime();

   // Etapa de teste na CPU
   #pragma omp parallel for reduction(+:err)
   for(int i=0;i<N;i++){
      float val = c[i] - res[i];
      val = val*val;
      if(val>TOL) err++;
   }
   test_time    +=  omp_get_wtime();
   
   printf("GPU RESULTS (Dynamic Allocation):\n");
   printf(" vectors added with %d errors\n",err);
   printf("Init time:    %.3fs\n", init_time);
   printf("Compute time: %.3fs\n", compute_time);
   printf("Test time:    %.3fs\n", test_time);
   printf("Total time:   %.3fs\n", init_time + compute_time + test_time);

   // --- ALTERAÇÃO 4: Liberação de Memória ---
   free(a);
   free(b);
   free(c);
   free(res);

   return 0;
}