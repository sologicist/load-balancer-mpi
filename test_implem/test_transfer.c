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
      
		procs[0].pid = 123;
    	procs[0].global_pid = 8;
        procs[0].size = 3;
      procs[0].commandLine = (char**)malloc(sizeof(char *) * procs[0].size+1);
      for(int i = 0; i < procs[0].size; i++){
        procs[0].commandLine[i] = (char *)malloc(sizeof(char)*10);
      }
	  strcpy(procs[0].commandLine[0], "../progs/test");
      strcpy(procs[0].commandLine[1], "555");
      strcpy(procs[0].commandLine[2], "../progs/dico.txt");
      procs[0].commandLine[3] = NULL;

    start_task(0);

    workers[1] = 8;
    work_transfer(1, 0);


   } else {
    	
        receive_transfer();
		//printf("%d\t%d\t%s\n", procs[0].pid, procs[0].global_pid, procs[0].commandLine[0]);
      //start_task(10);
   }
  
   MPI_Finalize();
   return 0;
}
