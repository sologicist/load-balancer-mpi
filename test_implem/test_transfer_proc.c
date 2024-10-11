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
      
		procs[5].pid = 123;
    	procs[5].global_pid = 8;
      int size = 3;
      procs[5].commandLine = (char**)malloc(sizeof(char *) * size+1);
      for(int i = 0; i < 4; i++){
        procs[5].commandLine[i] = (char *)malloc(sizeof(char)*10);
      }
strcpy(procs[5].commandLine[0], "../progs/test");
      strcpy(procs[5].commandLine[1], "555");
      strcpy(procs[5].commandLine[2], "../progs/dico.txt");
      procs[5].commandLine[3] = NULL;


      //printf("%s\n", procs[5].commandLine[2]);
		MPI_Send(&procs[5], sizeof(Process), MPI_BYTE, 1, TRANSFER, MPI_COMM_WORLD);
      MPI_Send(&size, 1, MPI_INT, 1, CMD, MPI_COMM_WORLD);
      for(int i = 0; i < size; i++){  
         MPI_Send(procs[5].commandLine[i], strlen(procs[5].commandLine[i])+1, MPI_CHAR, 1, CMD, MPI_COMM_WORLD);
         printf("%s\n", procs[5].commandLine[i]);
    }


   } else {
    	int size;
      int len;
		Process bin;
      MPI_Recv(&procs[10], sizeof(Process), MPI_BYTE, MPI_ANY_SOURCE, TRANSFER, MPI_COMM_WORLD, &status); 
      //printf("%d\t%d\n", bin.pid, bin.global_pid);
      int sender = status.MPI_SOURCE;
      MPI_Recv(&size, 1, MPI_INT, sender, CMD, MPI_COMM_WORLD, &status);
      printf("%d\n", size);
      //free(bin.commandLine);
      procs[10].commandLine = (char **)malloc(sizeof(char *) * size+1);
      for(int i = 0; i < size; i++){  
         MPI_Probe(sender, CMD, MPI_COMM_WORLD, &status);
         MPI_Get_count(&status, MPI_CHAR, &len);
         printf("%d\n", len);
         procs[10].commandLine[i] = (char*) malloc(sizeof(char)*len);
         MPI_Recv(procs[10].commandLine[i], len, MPI_CHAR, sender, CMD, MPI_COMM_WORLD, &status);
         printf("%s\n", procs[10].commandLine[i]);
      }
      procs[10].commandLine[size] = NULL;
		printf("%d\t%d\t%s\n", procs[10].pid, procs[10].global_pid, procs[10].commandLine[0]);
      start_task(10);
   }
  
   MPI_Finalize();
   return 0;
}
