#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>  // Para pow(), sqrt(), NAN e isnan()

#define TAM_MAX 100  // Tamanho máximo da pilha

// Definição do tipo de dado Pilha
typedef struct {
    float dados[TAM_MAX];  // Vetor que armazena os números na pilha
    int topo;              // Índice do topo da pilha (último elemento inserido)
} Pilha;

// Inicializa a pilha vazia
void inicializaPilha(Pilha *p) {
    p->topo = -1;
}

// Verifica se a pilha está vazia
int estaVazia(Pilha *p) {
    return p->topo == -1;
}

// Insere (empilha) um valor no topo da pilha
void push(Pilha *p, float valor) {
    if (p->topo < TAM_MAX - 1) {
        p->topo++;
        p->dados[p->topo] = valor;
    } else {
        printf("Erro: Pilha cheia\n");
    }
}

// Remove (desempilha) e retorna o valor do topo da pilha
float pop(Pilha *p) {
    if (!estaVazia(p)) {
        return p->dados[p->topo--];
    } else {
        printf("Erro: Pilha vazia\n");
        exit(1);  // Encerra o programa em caso de erro crítico
    }
}

// Função que avalia uma expressão em Notação Polonesa Reversa (RPN)
float avaliaRPN(char expressao[]) {
    Pilha p;
    inicializaPilha(&p);

    // Tokenização: divide a string em tokens separados por espaço
    char *token = strtok(expressao, " ");
    while (token != NULL) {

        // Caso o token seja um número (positivo ou negativo), converte e empilha
        if (isdigit(token[0]) || (token[0] == '-' && isdigit(token[1]))) {
            push(&p, atof(token));

        // Caso o token seja um operador binário (+ - * / ^ %)
        } else if (
            strcmp(token, "+") == 0 || strcmp(token, "-") == 0 ||
            strcmp(token, "*") == 0 || strcmp(token, "/") == 0 ||
            strcmp(token, "^") == 0 || strcmp(token, "%") == 0
        ) {
            // Verifica se há pelo menos dois operandos na pilha
            if (p.topo < 1) {
                printf("Erro: expressao mal formada (faltam operandos para o operador %s)\n", token);
                return NAN;
            }

            // Desempilha dois operandos
            float b = pop(&p);
            float a = pop(&p);
            float resultado;

            // Executa a operação correspondente
            if (strcmp(token, "+") == 0) {
                resultado = a + b;
            } else if (strcmp(token, "-") == 0) {
                resultado = a - b;
            } else if (strcmp(token, "*") == 0) {
                resultado = a * b;
            } else if (strcmp(token, "/") == 0) {
                if (b == 0) {
                    printf("Erro: divisao por zero\n");
                    return NAN;
                }
                resultado = a / b;
            } else if (strcmp(token, "^") == 0) {
                resultado = pow(a, b);
            } else if (strcmp(token, "%") == 0) {
                resultado = fmod(a, b);
            }

            // Empilha o resultado da operação
            push(&p, resultado);

        // Caso o token seja um operador unário (sqrt, neg)
        } else if (strcmp(token, "sqrt") == 0 || strcmp(token, "neg") == 0) {
            // Verifica se há pelo menos um operando na pilha
            if (p.topo < 0) {
                printf("Erro: expressao mal formada (falta operando para %s)\n", token);
                return NAN;
            }

            float a = pop(&p);
            float resultado;

            // Executa a operação unária correspondente
            if (strcmp(token, "sqrt") == 0) {
                if (a < 0) {
                    printf("Erro: raiz quadrada de numero negativo\n");
                    return NAN;
                }
                resultado = sqrt(a);
            } else if (strcmp(token, "neg") == 0) {
                resultado = -a;  // Inverte o sinal
            }

            // Empilha o resultado da operação unária
            push(&p, resultado);

        // Caso o token não seja reconhecido
        } else {
            printf("Erro: operador desconhecido %s\n", token);
            return NAN;
        }

        // Pega o próximo token
        token = strtok(NULL, " ");
    }

    // No final, a pilha deve conter exatamente um resultado
    if (p.topo != 0) {
        printf("Erro: expressao mal formada (operandos sobrando)\n");
        return NAN;
    }

    // Retorna o resultado final da expressão
    return pop(&p);
}

int main() {
    char expressao[256];

    // Cabeçalho da calculadora
    printf("=================================\n");
    printf("      Calculadora RPN (C)\n");
    printf("=================================\n");
    printf("Operadores suportados: + - * / ^ %% sqrt neg\n");
    printf("Digite a expressao em RPN (ex: 5 1 2 + 4 * + 3 -)\n");
    printf("Digite 'sair' para encerrar o programa.\n\n");

    // Loop principal para aceitar múltiplas expressões
    while (1) {
        printf("> ");
        fgets(expressao, sizeof(expressao), stdin);

        // Remove o \n do final da string (limpeza)
        expressao[strcspn(expressao, "\n")] = 0;

        // Se o usuário digitar 'sair' ou 'exit', encerra o programa
        if (strcmp(expressao, "sair") == 0 || strcmp(expressao, "exit") == 0) {
            printf("Encerrando a calculadora.\n");
            break;
        }

        // Avalia a expressão RPN e obtém o resultado
        float resultado = avaliaRPN(expressao);

        // Só imprime o resultado se não for um NAN (caso de erro)
        if (!isnan(resultado)) {
            printf("Resultado: %.2f\n", resultado);
        }
    }

    return 0;
}
