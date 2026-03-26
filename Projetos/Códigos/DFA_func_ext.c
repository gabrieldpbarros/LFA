#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define BUFFER_SIZE 1024

int encontra_maior(char* string){
    int maior = 0;
    int max_size = strlen(string);

    for (int i = 0; i < max_size; i++) {
        if (string[i] == '-') i += 1; // pula o '-'
        if (string[i] - '0' < 0) break;
        if ((int) string[i] - '0' > maior) maior = (int) string[i] - '0';
    }

    return maior + 1;
}

int** recebe_DFA(char* buffer, int* inicial, int** finais, int* qtd_estados, int* qtd_finais, FILE *fp){
    // recebe a entrada no formato [estado inicial] [estados finais] [-] [transições];
    if (fgets(buffer, BUFFER_SIZE, fp) == NULL) return NULL;
    // Salva o estado inicial
    *inicial = (int) buffer[0] - '0';
    // Encontra a quantidade de estados finais
    *qtd_finais = 0;
    for (int i = 1; buffer[i] != '-'; i++)
        (*qtd_finais)++;
    // Aloca dinamicamente o vetor e salva os estados finais
    *finais = (int*) malloc((*qtd_finais) * sizeof(int));
    for (int i = 0; i < *qtd_finais; i++)
        (*finais)[i] = buffer[i + 1] - '0';
    // Cria a matriz de adjacência
    *qtd_estados = encontra_maior(buffer);
    int **matriz = (int**) malloc((*qtd_estados) * sizeof(int*));
    for (int i = 0; i < (*qtd_estados); i++)
        matriz[i] = (int*) malloc(2 * sizeof(int)); // alfabeto binario
    // Insere os valores na matriz
    int buffer_pos = (*qtd_finais) + 2; // pula os valores iniciais e o '-'
    for (int i = 0; i < (*qtd_estados); i++)
        for (int j = 0; j < 2; j++)
            matriz[i][j] = buffer[buffer_pos++] - '0';

    return matriz;
}

int* recebe_string(char* buffer, int* tamanho_func, FILE *fp){
    if (fgets(buffer, BUFFER_SIZE, fp) == NULL) return NULL;
    *tamanho_func = strlen(buffer);
    // Elimina caracteres extras
    if (*tamanho_func > 0 && buffer[(*tamanho_func) - 1] == '\n')
        (*tamanho_func)--;
    if (*tamanho_func > 0 && buffer[(*tamanho_func) - 1] == '\r')
        (*tamanho_func)--;
        
    // Aloca a string do tamanho do delta chapeu
    int* func_ext = (int*) malloc((*tamanho_func) * sizeof(int));
    for (int i = 0; i < *tamanho_func; i++) {
        if (buffer[i] - '0' > 1 || buffer[i] - '0' < 0) return NULL; // valores não contidos no alfabeto
        else func_ext[i] = buffer[i] - '0';
    }

    return func_ext;
}

void valida_funcao(int **dfa, int *func_ext, int* finais, int inicial, int qtd_finais, int tamanho_func, int index){
    //printf("TAMANHO EXT: %d", tamanho_func);
    //for (int i = 0; i < tamanho_func; i++)
    //    printf("%d ", func_ext[i]);
    int estado = inicial;
    // Percorre o DFA
    for (int i = 0; i < tamanho_func; i++) {
        //printf("ESTADO ATUAL: %d\n", estado);
        int entrada = func_ext[i];
        estado = dfa[estado][entrada];
    }
    //printf("ESTADO FINAL: %d", estado);
    // Checa se é estado final
    bool final = false;
    //printf("TAMANHO FINAIS: %d", qtd_finais);
    for (int i = 0; i < qtd_finais && !final; i++)
        if (finais[i] == estado) final = true;

    if (final) printf("STRING %d: Pertence a linguagem do DFA.\n", index);
    else printf("STRING %d: Nao pertence a linguagem do DFA.\n", index);
}

void libera_matriz(int** matrix, int qtd_linhas){
    for (int i = 0; i < qtd_linhas; i++) 
        free(matrix[i]);
    free(matrix);
}

//void imprime_matriz(int** matriz, int qtd_linhas){
//    for (int i = 0; i < qtd_linhas; i++) {
//        for (int j = 0; j < 2; j++)
//            printf("%d ", matriz[i][j]);
//        printf("\n");
//    }
//}

int main(int argc, char *argv[]){
    if (argc < 2) {
        // Instruções para o usuário
        printf("Se voce esta vendo esta mensagem, significa que não executou o programa corretamente.\n");
        printf("Ao executar pelo terminal, adicione o nome de um arquivo .txt que contenha o automato na primeira linha e as strings nas linhas subseguintes.\n");
        printf("INSTRUCOES\n");
        printf("Escreva o DFA seguindo o seguinte formato: <ESTADO INICIAL><ESTADOS FINAIS>-<TRANSICOES>\n");
        printf("EXEMPLO: 134-2103412444\n");
        printf("DESCRICAO\nestado inicial: 1\nestados finais: 3,4\nmatriz de transicao:\n");
        printf("   0: 2 | 1\n-> 1: 0 | 3\n   2: 4 | 1\n * 3: 2 | 4\n * 4: 4 | 4\n\n");
        return 0;
    } else {
        char buffer[BUFFER_SIZE];
        int inicial, qtd_finais, qtd_estados, tamanho_func;
        int *finais, *func_ext;
        int **dfa;
        FILE *fp;

        for (int i = 1; i < argc; i++) {
            if ((fp = fopen(argv[i], "r")) == NULL) {
                printf("ERRO: O arquivo %s nao existe\n", argv[i]);
                return 1;
            } else {
                // Leitura
                dfa = recebe_DFA(buffer, &inicial, &finais, &qtd_estados, &qtd_finais, fp);
                //imprime_matriz(dfa, qtd_estados);
                int index = 1;
                while (true) {
                    func_ext = recebe_string(buffer, &tamanho_func, fp);
                    if (!func_ext) break;
                    // Validação
                    else valida_funcao(dfa, func_ext, finais, inicial, qtd_finais, tamanho_func, index++);
                    free(func_ext);
                }
                // Liberação das alocações dinâmicas
                libera_matriz(dfa, qtd_estados);
                free(finais);
                // Fechamento do arquivo
                fclose(fp);
            }
        }

        return 0;
    }
}