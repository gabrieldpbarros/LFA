#include <stdio.h>
#include <stdlib.h>

int** recebe_DFA(char* buffer){
    // receber a entrada no formato [estado inicial] [estados finais] [-1] [transições];
    // uma ideia para montar a matriz é percorrer toda a string do buffer e encontrar o
    // maior número, então usá-lo para definir o tamanho da matriz por alocação dinâmica.
}

char* recebe_string(char* buffer){

}

int main(){
    char buffer[1024];
    int **DFA = recebe_DFA(buffer);
    char *string = recebe_string(buffer);
    
    return 0;
}