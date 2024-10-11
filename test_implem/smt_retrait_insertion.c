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
      strcpy(commandLine[1], "./../progs/loop");
      commandLine[2] = NULL;

   

        workers[1] = 3;
        workers[2] = 8;

        int global_pid = 0;
        for (int i = 0; i < 5; i++){
            start(commandLine, 1, global_pid);
            global_pid++;
        }

        remove_worker(1);
        

        for (int i = 5; i < 10; i++){
            start(commandLine, 1, global_pid);
            global_pid++;
        }

        
        sleep(65);
        printf("GPS.................................................\n");
        gps(1);

        printf("INSERTION WORKER ............................................................. 1\n");
        workers[1] = 0;
        insert_worker(1);
        sleep(100);
        printf("2eme gps....\n");
        gps(1);


   } 

   if (rank == 1) {
        int flag;
        workers[2] = 80;
        workers[rank] = 101;
        while(1){
            
            if (!idle){
                balancer_test_souscharge();
            }

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
                //printf(" /........................................................MPI TAG 1 FNINNNNNNNNNNNN ..........................%d\n", status.MPI_TAG);

                switch(status.MPI_TAG){
                    case START:
                        //printf("start reception 1 ....................starttttt........................................\n");
                        rcv_start();
                        break;
                            
                    case TRANSFER:
                        receive_transfer();
                        break;

                    case GPS:
                        int opt;
                        MPI_Recv(&opt, 1, MPI_INT, MPI_ANY_SOURCE, GPS, MPI_COMM_WORLD, &status); 
                        rcv_gps(opt);
                        break;

                    case REMOVE:
                        //printf("REMOVE reception 1 ....................REMOVEEEE %d........................................\n", getpid());
                        int rm;
                        MPI_Recv(&rm, 1, MPI_INT, MPI_ANY_SOURCE, REMOVE, MPI_COMM_WORLD, &status); 
                        receive_remove();
                        break;

                    case INSERT:
                        //printf("INSERT reception 1 ..iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii........\n");
                       
                        receive_insert();
                        workers[rank] = 0;
                        //printf("IDLE...........................INSERTION.......................................... %d\n", idle);
                        workers[2] = 100;
                        break; 
                }  
            }
        }
        
   }

   
   if (rank == 2) {
    	
        int flag;

        while(1){
            update();
            
            MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &status); 
            if (flag){
                
                switch(status.MPI_TAG){
                
                    case START:
                        //printf("start reception 2 .................STARTTTTTTTTT...........................................\n");                   
                        rcv_start();
                        break;

                    case CLAIM:
                        //printf("reception ..................................................................................................................CLAIM ............\n");
                        receive_claim();
                        break;

                    case TRANSFER:
                        //printf("TRANSFER reception 2 .................TRANSFERRR...........................................\n");                   
                        receive_transfer();
                        break;
                    
                    case GPS:
                        int opt;
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
