#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
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
   int length;
   MPI_Get_processor_name(hostname, &length);

   if (nb_proc > 3) {
      printf("Nombre de machines incorrect !\n");
      MPI_Finalize();
      exit(2);
   }
    
   timer = time(NULL);

   if (rank == 0) {
      
      int size = 3;
      char **commandLine = (char**)malloc(sizeof(char *) * size);
      for(int i = 0; i < (size-1); i++){
        commandLine[i] = (char *)malloc(sizeof(char)*20);
      }

      strcpy(commandLine[0], "start");
      strcpy(commandLine[1], "progs/loop");
      commandLine[2] = NULL;

   

        workers[1] = 2;
        workers[2] = 15;

        int global_pid = 0;
        for (int i = 0; i < 10; i++){
            start(commandLine, 1, global_pid);
            global_pid++;
        }
        
        gps(1);
        sleep(60);
        printf("2eme .....................gps\n");
        gps(1);

   } 
   
   if (rank == 1) {
    	
        workers[2] = 1;
        int flag;
        int res;

        while(1){
            workers[2] = 1;
            
            balancer_test_surcharge();
            MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &status); 
            if (flag){
                //printf("reception msg\n");
                switch(status.MPI_TAG){
                    
                    case START:       
                        //printf("j'ai recu un start\n");
                        rcv_start();
                        break;
                    
                    case GPS:
                        int opt;
                        //printf("j'ai recu un gps\n");
                        MPI_Recv(&opt, 1, MPI_INT, MPI_ANY_SOURCE, GPS, MPI_COMM_WORLD, &status); 
                        rcv_gps(opt);
                        break;
                }
            
        }
     }
   }

   if (rank == 2) {
        int flag;
        int res;
        
        while(1){

            workers[rank] = 1;
            int cpt = 0;

            // Vérification de la présence de messages GPS
            MPI_Iprobe(MPI_ANY_SOURCE, GPS, MPI_COMM_WORLD, &flag, &status); 
            if (flag){
                int opt;
                // Réception et traitement des messages GPS
                MPI_Recv(&opt, 1, MPI_INT, MPI_ANY_SOURCE, GPS, MPI_COMM_WORLD, &status); 
                rcv_gps(opt);
            }

            MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &status); 
            if (flag){
                //printf("tag : ......... %d\n", status.MPI_TAG);
                switch(status.MPI_TAG){
                    
                    case TRANSFER:
                        cpt++;
                        //printf("j'ai recu un transfer.....................%d\n", cpt);
                        receive_transfer();
                        break;

                    case GPS:
                        int opt;
                        //printf("j'ai recu un gps\n");
                        MPI_Recv(&opt, 1, MPI_INT, MPI_ANY_SOURCE, GPS, MPI_COMM_WORLD, &status); 
                        rcv_gps(opt);
                        break;
                }

        }
    }
        
   }


  
   MPI_Finalize();
   return 0;
}
