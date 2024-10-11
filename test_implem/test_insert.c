#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h> 
#include "../implem/load.h"
#include "../implem/tag.h"
#include "../implem/inout.h"
#include "../implem/work.h"
#include "../implem/struct.h"


int main(int argc, char* argv[]) {
   
   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD, &nb_proc);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);

   if (nb_proc > 2) {
      printf("Nombre de machines incorrect !\n");
      MPI_Finalize();
      exit(2);
   }
  
   if (rank == 0) {
        workers[1] = 12;
		remove_worker(1);
        MPI_Recv(&workers[1], 1, MPI_INT, 1, LOAD, MPI_COMM_WORLD, &status);
        printf("charge de 1 = %d\n", workers[1]);
        insert_worker(1);
        MPI_Recv(&workers[1], 1, MPI_INT, 1, LOAD, MPI_COMM_WORLD, &status);
        printf("charge de 1 = %d\n", workers[1]);


   } else {

        workers[rank] = 12;
    	
        printf("idle = %d\n", idle);
        receive_remove();
        printf("workerload = %d\n", workers[rank]);
        printf("idle = %d\n", idle);
        receive_insert();
        printf("idle final = %d\n", idle);
   }
  
   MPI_Finalize();
   return 0;
}