#include <stdio.h>
#include <mpi.h>

// mpirun --oversubscribe -np xx --map-by node --hostfile ./hostfile.txt ./test_hostfile

int main(int argc, char** argv){

    int rank;
    int nb_proc;
    int nom_len;
    char nom_proc[MPI_MAX_PROCESSOR_NAME];

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nb_proc);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Get_processor_name(nom_proc, &nom_len);

    printf("Processeur numéro %d sur la machine %s parmi %d processeurs\n", rank, nom_proc, nb_proc);

    MPI_Finalize();

    return 0;
}