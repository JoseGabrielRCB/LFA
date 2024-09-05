#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRAVOU 'x'
void clear_screen()
{
#ifdef _linux_
    system("clear");
#elif _WIN32
    system("cls");
#endif
}

void get_input(char *, int, int *);
void split(char *, char *, char, int *);
int pertence(char, char *);
char delta(char, char, char[], char[], char[][10], int, int);

void ERRO()
{
    getchar();
    clear_screen();
    printf("ERRO/VALOR INVALIDO(PRECIONE ENTER PARA CONTINUAR)\n");
    getchar();
    clear_screen();
}

void Pessoa_gets(char *str, int t, int *tamW)
{
    fgets(str, t, stdin); // coloca \n e \0 no fim stdin Ã© entrada padrao (teclado)
    *tamW = strlen(str) - 1;
    str[*tamW] = '\0';
}

void split(char *Q, char *str, char sep, int *tam)
{
    int j = 0;
    for (int i = 0; i < *tam; i++)
        if (str[i] != sep) // se nÃ£o encontrou o separador
            Q[j++] = str[i];
    *tam = j; // guarda o tamanho dos elementos
}

int pertence(char c, char *Str)
{
    for (int i = 0; Str[i] != '\0'; i++)
        if (c == Str[i])
            return i;
    return -1; // retorna uma falha
}

char delta(char estadoAtual, char simboloEntrada, char ConjuntoEstados[], char alfabeto[], char matrixDelta[][10], int tamConjuEstados, int tamAlafbeto)
{
    for (int i = 0; i < tamConjuEstados; i++)      // Percorre o conjunto de estados para encontrar o índice do estado atual
        if (ConjuntoEstados[i] == estadoAtual)     // Verifica se o estado atual corresponde ao estado na posição i
            for (int a = 0; a < tamAlafbeto; a++)  // Percorre o alfabeto para encontrar o indice do símbolo de entrada
                if (alfabeto[a] == simboloEntrada) // Verifica se o símbolo de entrada corresponde ao símbolo na posição a
                    return matrixDelta[i][a];      // Retorna o próximo estado a partir da matriz de transição
    return TRAVOU;
}

void menu()
{
    clear_screen();
    int escolha = 0;
    printf("MENU DE INICIO\n");
    printf("Escolha: 1-Automato 2-VOLTAR\n");
    scanf("%d", &escolha);
    getchar(); // Limpar o buffer
    if (escolha == 1)
    {

        // variveis principais
        char Q[10], A[10], qo, F[10], mDelta[10][10];
        char str[30];
        int tamQ, tamF, tamA, i, j, loop = 1;
        int Falha = 0;
        // Receber valores a ser trabalhados

        // Entra com os conjuntos de estados
        printf("Entre com o conjunto de Estados separados por virgulas: ");
        Pessoa_gets(str, 30, &tamQ);
        split(Q, str, ',', &tamQ); // Separa cada estado

        // Introduz alfabeto
        printf("\nEntre os simbolos do alfabeto separados por virgulas: ");
        Pessoa_gets(str, 30, &tamA);
        split(A, str, ',', &tamA);

        // Intoduz estados inicias
        printf("Entre com o estado inicial: ");
        qo = getchar();
        setbuf(stdin, NULL);

        // Entra com os finais
        printf("\nEntre os estado(s) de final(s) separados por virgulas: "); // implementar para que nÃ£o se entre com sÃ­mbolos que nÃ£o estejam em Q
        Pessoa_gets(str, 30, &tamF);
        split(F, str, ',', &tamF);

        // Leitura da matriz Delta
        for (i = 0; i < tamQ; i++)
            for (j = 0; j < tamA; j++)
            {
                printf("\ndelta(%c, %c) = ", Q[i], A[j]);
                scanf(" %c%*[^\n]", &mDelta[i][j]); // espaco em " %c%*[^n]" faz a leitura do buffer com '\n' e ignora
            }

        getchar();
        while (loop == 1)
        {
            char W[20], q, c;
            int tamW, aux = 0, p = 0;

            // Introduz a palavra a ser verificada
            printf("\nEntre com a palavra a ser verificada: ");
            Pessoa_gets(W, 30, &tamW);
            printf("\nPalavra : %s\n", W);
            q = qo;
            c = W[p++]; // nextchar()
            printf("\nSequencia de estados: ");
            printf("%c", q);
            while (c != '\0')
            {
                q = delta(q, c, Q, A, mDelta, tamQ, tamA);
                if (q == TRAVOU)
                {
                    Falha = 1;
                    break;
                }
                c = W[p++];
                printf(",%c", q);
            } // c = ProximoChar

            if (pertence(q, F) || Falha)
                printf("\nPalavra nao reconhecida\n ");
            else
                printf("\nPalavra reconhecida\n ");
            while (aux != 1)
            {
                printf("Deseja testar outra palavra: 1-sim 2-nao\n");
                scanf("%d", &aux);
                getchar();
                if (aux != 1 && aux != 2)
                    ERRO();
                if (aux == 2)
                    break;
            }
            if (aux == 2)
            {
                break;
            }
        }
    }
}
int main()
{
    int loop = 1;
    while (loop != 2)
    {
        printf("Deseja um automato: 1-sim 2-nao\n");
        scanf("%d", &loop);
        if (loop != 1 && loop != 2)
            ERRO();
        else if (loop == 2)
            break;
        else
            menu();
    }
}