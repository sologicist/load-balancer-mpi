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

   

        workers[1] = 3;
        workers[2] = 8;

        int global_pid = 0;
        for (int i = 0; i < 10; i++){
            start(commandLine, 1, global_pid);
            global_pid++;
        }
        
        sleep(55);
        gps(1);
        sleep(100);
        printf("2eme gps....\n");
        gps(1);
        sleep(200);
        printf("3eme gps....\n");
        gps(1);


   } 
   
   if (rank == 1) {
    	
        int flag;
    
        while(1){
            update();

            // Vérification de la présence de messages GPS
            MPI_Iprobe(MPI_ANY_SOURCE, GPS, MPI_COMM_WORLD, &flag, &status); 
            if (flag){
                int opt;
                // Réception et traitement des messages GPS
                MPI_Recv(&opt, 1, MPI_INT, MPI_ANY_SOURCE, GPS, MPI_COMM_WORLD, &status); 
                rcv_gps(opt);
                update(); // Mise à jour des informations
            }
            MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &status); 
            if (flag){
                switch(status.MPI_TAG){
                    
                    case START:                   
                        rcv_start();
                        break;

                    case CLAIM:
                        printf("reception ..................................................................................................................CLAIM ............\n");
                        receive_claim();
                        break;
                    
                    case GPS:
                        int opt;
                        MPI_Recv(&opt, 1, MPI_INT, MPI_ANY_SOURCE, GPS, MPI_COMM_WORLD, &status); 
                        rcv_gps(opt);
                        printf(" MACHINE 1 ...............\n\n");
                        break;
                }
            
            }
        }
    }

   if (rank == 2) {
        int flag;
        sleep(30); //le temps que le site 1 recupere des taches
        
        while(1){
            workers[1] = 80;
            balancer_test_souscharge();
            MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &status); 
            if (flag){
                printf("tag : ......... %d\n", status.MPI_TAG);

            switch(status.MPI_TAG){
                case TRANSFER:
                    receive_transfer();
                    break;

                case GPS:
                    int opt;
                    MPI_Recv(&opt, 1, MPI_INT, MPI_ANY_SOURCE, GPS, MPI_COMM_WORLD, &status); 
                    rcv_gps(opt);
                    printf(" MACHINE 2 ...............\n\n");
                    break;
            }


            
        }
    }
        
   }


   MPI_Finalize();
   return 0;
}
