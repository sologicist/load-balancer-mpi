#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024

void afficherLigne(char** ligne) {
  printf("affichage de la commande : \n");
  int i = 0;
  while (ligne[i] != NULL) {
    printf("%s ", ligne[i]);
    i++;
  }
  printf("\n");
}

void choixCommande(char** ligne) {
    if (!strcmp(ligne[0], "start")) {
    
    int i = 0;
    while (ligne[i] != NULL){
      ligne[i] = ligne[i+1];
      i++;
    }
    free(ligne[i]);

    afficherLigne(ligne);

  }
  else if (!strcmp(ligne[0], "gps")) {
    
    if (ligne[1] == NULL){
      printf("Gps sans -l\n");
    }
    else{
      printf("Gps avec -l\n");
    }

  }
  else if (!strcmp(ligne[0], "gkill")) {

    //Retirer le tiret
    char tmp[strlen(ligne[1])];
    strcpy(tmp, ligne[1]);
    int i = 0;
    while(tmp[i] != '\0'){
      tmp[i] = tmp[i+1];
      i++;
    }

    printf("Gkill %s %s\n", tmp, ligne[2]);
  }
  else{
    printf("Error command %s not found \n", ligne[0]);
  }
}


int main(int argc, char** argv){
    char** ligne = NULL;
    char ligneLue[MAX_LINE_LENGTH];
    int tailleLigne = 0;

    printf("Taper une commande: ");
    fgets(ligneLue,  MAX_LINE_LENGTH, stdin);

    char* mot = strtok(ligneLue, " \t\n"); // Delimiters are space, tab, and newline

    while (mot != NULL) {
        if (tailleLigne == 0) {
            ligne = realloc(ligne, sizeof(char*) * (tailleLigne + 1));
        } else {
            ligne = realloc(ligne, sizeof(char*) * (tailleLigne + 2));
        }

        ligne[tailleLigne++] = malloc(sizeof(char) * (strlen(mot) + 1));

        strcpy(ligne[tailleLigne - 1], mot);

        mot = strtok(NULL, " \t\n");
    }

    ligne[tailleLigne++] = NULL;

    afficherLigne(ligne);
    choixCommande(ligne);

    return 0;
}