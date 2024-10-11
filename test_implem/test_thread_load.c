#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h> 
#include "../implem/load.h"
#include "../implem/tag.h"
#include "../implem/inout.h"
#include "../implem/work.h"
#include "../implem/struct.h"
#include <pthread.h>

int fini = 1;
pthread_mutex_t	mutex;
void *updatetest(){ /*machine passive a un load de 0*/

    int charge;
    int flag = 0;
    int nb_worker = 0;
    int net = 0;
    
    while(1){
        pthread_mutex_lock(&mutex);
        printf("lock thread\n");
        MPI_Iprobe(MPI_ANY_SOURCE, LOAD, MPI_COMM_WORLD, &flag, &status); 

        if(flag){
            switch(status.MPI_TAG){
                case LOAD:
                    MPI_Recv(&workers[status.MPI_SOURCE], 1, MPI_INT, MPI_ANY_SOURCE, LOAD, MPI_COMM_WORLD, &status); 
                    printf("Thread %d\n", workers[status.MPI_SOURCE]);
                    break;
            }
        }

        nb_worker = nb_workers(workers);
        net = 0;

        for(int i = 1; i < nb_proc; i++){
            net += workers[i]; 
        }
        
        if (nb_worker != 0){
            netload = (int) net / nb_worker;
        }
        pthread_mutex_unlock(&mutex);
        printf("unlock thread\n");
    }
    
    //pthread_exit(NULL);

    

}

int main(int argc, char* argv[]) {
   
   pthread_mutex_init(&mutex, NULL);

   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD, &nb_proc);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);

   if (nb_proc > 4) {
      printf("Nombre de machines incorrect !\n");
      MPI_Finalize();
      exit(2);
   }
  
   if (rank == 0) {
      /*thread*/
      // Création de la variable qui va contenir le thread

	    pthread_t thread;
        pthread_create(&thread, NULL, updatetest, NULL);
        //int i = 0;
        int c;
        while(1){
            pthread_mutex_lock(&mutex);
            printf("lock proc \n");
            MPI_Recv(&c, 1, MPI_INT, MPI_ANY_SOURCE, TRANSFER, MPI_COMM_WORLD, &status);
            printf("PROC %d\n", c);
            pthread_mutex_unlock(&mutex);
            printf("unlock proc \n");
            //i++;
        }

        printf("avt join\n");
    
        fini = 0;

        pthread_join(thread, NULL);

        printf("apres join\n");
      



   } else {
    
        int i = 0;
        while (1){
            MPI_Send(&i, 1, MPI_INT, 0, LOAD, MPI_COMM_WORLD);
            int j = 555;
            MPI_Send(&j, 1, MPI_INT, 0, TRANSFER, MPI_COMM_WORLD);
            i++;

        }
    	


   }

   printf("termine : %d\n", rank);
  
   MPI_Finalize();
   return 0;
}
