#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <set>


using namespace std;

typedef struct AutomatoDeterministico
{
    vector<string> estadosDet;                     // Colecao de estados do AFD
    vector<char> alfabetoDet;                      // Conjunto de simbolos do AFD
    vector<string> finaisDet;                      // Estados de aceitacao do AFD
    string inicialDet;                             // Estado inicial do AFD
    map<pair<string, char>, string> transicoesDet; // Mapeamento de transicoes no AFD
} AutomatoDeterministico;

typedef struct AutomatoNaoDeterministico
{
    vector<string> estadosNaoDet;                          // Colecao de estados do AFN
    vector<char> alfabetoNaoDet;                           // Conjunto de simbolos do AFN
    vector<string> finaisNaoDet;                           // Estados de aceitacao do AFN
    string inicialNaoDet;                                  // Estado inicial do AFN
    map<pair<string, char>, set<string>> transicoesNaoDet; // Mapeamento de transicoes no AFN
} AutomatoNaoDeterministico;

/* Declaracao das funcoes */

// Exibe as informacoes de um AFD na tela
void exibirAFD(AutomatoDeterministico afd);
// Calcula o fecho epsilon de um dado estado no AFN
set<string> calcularFechoEpsilon(AutomatoNaoDeterministico afn, string estado);
// Transforma um AFN em um AFD equivalente
AutomatoDeterministico converterAFNparaAFD(AutomatoNaoDeterministico afn);
// Coleta uma lista de strings da entrada do usuario com base na mensagem de input
vector<string> capturarEntrada(string input);
// Combina multiplos estados em uma unica string representativa
string unirConjuntosDeEstados(set<string> estados);
// Funcao para capturar e processar a entrada do usuario
vector<string> capturarEntrada(string input)
{
    input = "0"; // Ignorado, nao utilizado
    // remove o waring
    vector<string> resultados;
    string linha;
    getline(cin, linha);
    stringstream ss(linha);
    string item;
    while (getline(ss, item, ','))
    {
        // Remocao de espacos extras no inicio e fim de cada item
        item.erase(0, item.find_first_not_of(" \t"));
        item.erase(item.find_last_not_of(" \t") + 1);
        if (item != "." && !item.empty())
        {
            resultados.push_back(item);
        }
    }
    return resultados;
}

// Calcula o fecho epsilon de um estado especifico no AFN
set<string> calcularFechoEpsilon(AutomatoNaoDeterministico afn, string estado)
{
    set<string> fecho;
    vector<string> pilha;
    pilha.push_back(estado);

    while (!pilha.empty())
    {
        string atual = pilha.back();
        pilha.pop_back();

        if (fecho.find(atual) == fecho.end())
        {
            fecho.insert(atual);
            if (afn.transicoesNaoDet.find({atual, '\0'}) != afn.transicoesNaoDet.end())
            {
                for (string proximo : afn.transicoesNaoDet[{atual, '\0'}])
                {
                    pilha.push_back(proximo);
                }
            }
        }
    }
    return fecho;
}

// Transforma o AFN fornecido em um AFD correspondente
AutomatoDeterministico converterAFNparaAFD(AutomatoNaoDeterministico afn)
{
    AutomatoDeterministico afd;
    afd.alfabetoDet = afn.alfabetoNaoDet;                                                  // O alfabeto do AFD e identico ao do AFN
    afd.inicialDet = unirConjuntosDeEstados(calcularFechoEpsilon(afn, afn.inicialNaoDet)); // Define o estado inicial do AFD

    set<set<string>> novosEstados;          // Colecao de novos estados criados no AFD
    map<set<string>, string> mapaDeEstados; // Mapeamento de conjuntos de estados para novos estados do AFD

    set<string> estadoInicial = calcularFechoEpsilon(afn, afn.inicialNaoDet); // Fecho epsilon do estado inicial
    novosEstados.insert(estadoInicial);
    mapaDeEstados[estadoInicial] = afd.inicialDet;
    afd.estadosDet.push_back(afd.inicialDet);

    // Processo de conversao de estados e transicoes do AFN para o AFD
    while (!novosEstados.empty())
    {
        set<string> estadoAtual = *novosEstados.begin();
        novosEstados.erase(novosEstados.begin());

        for (char simbolo : afd.alfabetoDet)
        {
            set<string> novoEstado;
            for (string estado : estadoAtual)
            {
                if (afn.transicoesNaoDet.find({estado, simbolo}) != afn.transicoesNaoDet.end())
                {
                    for (string destino : afn.transicoesNaoDet[{estado, simbolo}])
                    {
                        set<string> fecho = calcularFechoEpsilon(afn, destino);
                        novoEstado.insert(fecho.begin(), fecho.end());
                    }
                }
            }

            string novoEstadoUnido = unirConjuntosDeEstados(novoEstado);
            if (mapaDeEstados.find(novoEstado) == mapaDeEstados.end())
            {
                mapaDeEstados[novoEstado] = novoEstadoUnido;
                afd.estadosDet.push_back(novoEstadoUnido);
                novosEstados.insert(novoEstado);
            }
            afd.transicoesDet[{mapaDeEstados[estadoAtual], simbolo}] = novoEstadoUnido;
        }
    }

    // Identifica e marca os estados de aceitacao no AFD
    for (map<set<string>, string>::iterator it = mapaDeEstados.begin(); it != mapaDeEstados.end(); ++it)
    {
        set<string> estadoConjunto = it->first;
        string nomeEstado = it->second;

        for (set<string>::iterator sit = estadoConjunto.begin(); sit != estadoConjunto.end(); ++sit)
        {
            if (find(afn.finaisNaoDet.begin(), afn.finaisNaoDet.end(), *sit) != afn.finaisNaoDet.end())
            {
                afd.finaisDet.push_back(nomeEstado);
                break;
            }
        }
    }

    return afd;
}

// Gera uma unica string que representa um conjunto de estados
string unirConjuntosDeEstados(set<string> estados)
{
    if (estados.empty())
        return "<>"; // Retorna "<>" se o conjunto estiver vazio
    string resultado = "<";
    for (auto it = estados.begin(); it != estados.end(); ++it)
    {
        resultado += *it;
        if (next(it) != estados.end())
        {
            resultado += ", ";
        }
    }
    resultado += ">";
    return resultado;
}

// Exibe as informacoes do AFD no console
void exibirAFD(AutomatoDeterministico afd)
{
    cout << "\n================ AUTOMATO DETERMINISTICO ===============\n"
         << endl;

    // Exibicao dos estados do AFD
    cout << "Estados: { ";
    for (size_t i = 0; i < afd.estadosDet.size(); i++)
    {
        cout << afd.estadosDet[i];
        if (i < afd.estadosDet.size() - 1)
        {
            cout << ", ";
        }
    }
    cout << " }" << endl;

    // Exibicao do alfabeto do AFD
    cout << "Alfabeto: { ";
    for (size_t i = 0; i < afd.alfabetoDet.size(); i++)
    {
        cout << afd.alfabetoDet[i];
        if (i < afd.alfabetoDet.size() - 1)
        {
            cout << ", ";
        }
    }
    cout << " }" << endl;

    // Exibicao da tabela de transicoes do AFD
    cout << "\nTransicoes: " << endl;
    for (map<pair<string, char>, string>::iterator it = afd.transicoesDet.begin(); it != afd.transicoesDet.end(); ++it)
    {
        pair<string, char> transicao = it->first;
        string estadoDestino = it->second;
        cout << "Transicao: { " << transicao.first << ", " << transicao.second << " } = " << estadoDestino << endl;
    }

    // Exibicao do estado inicial do AFD
    cout << "\nEstado inicial: " << afd.inicialDet << endl;

    // Exibicao dos estados finais do AFD
    cout << "Estados finais: { ";
    for (size_t i = 0; i < afd.finaisDet.size(); i++)
    {
        cout << afd.finaisDet[i];
        if (i < afd.finaisDet.size() - 1)
        {
            cout << ", ";
        }
    }
    cout << " }" << endl;
}

int main()
{
    int loop = 1;
    while (loop == 1)
    {
        system("clear");
        AutomatoNaoDeterministico afn; // Criacao de uma instancia do AFN
        string estado, simbolo, estadoFinal;

        // Solicita a entrada dos estados do AFN
        cout << "(Separe os estados com virgulas)\n> Estados: ";
        vector<string> listaDeEstados = capturarEntrada("Estados");
        afn.estadosNaoDet.insert(afn.estadosNaoDet.end(), listaDeEstados.begin(), listaDeEstados.end());

        // Solicita a entrada do alfabeto do AFN
        cout << "(Separe os simbolos com virgulas)\n> Alfabeto: ";
        vector<string> listaDeSimbolos = capturarEntrada("Alfabeto");
        for (const string &simbolo : listaDeSimbolos)
        {
            if (!simbolo.empty())
            {
                afn.alfabetoNaoDet.push_back(simbolo[0]);
            }
        }

        // Solicita a entrada do estado inicial do AFN
        cout << "\n> Estado inicial: ";
        cin >> afn.inicialNaoDet;
        cin.ignore(); // Limpeza do buffer de entrada

        // Solicita a entrada dos estados finais do AFN
        cout << "(Separe os estados finais com virgulas)\n> Estados finais: ";
        vector<string> estadosFinais = capturarEntrada("Estados finais");
        afn.finaisNaoDet.insert(afn.finaisNaoDet.end(), estadosFinais.begin(), estadosFinais.end());

        // Solicita a entrada das transicoes do AFN
        cout << "\nInserir transicoes (separe os estados com ','):" << endl;
        for (string estadoAtual : afn.estadosNaoDet)
        {
            for (char simboloAtual : afn.alfabetoNaoDet)
            {
                cout << "Transicao(" << estadoAtual << ", " << simboloAtual << "): ";
                string transicao;
                getline(cin, transicao);
                if (transicao != "-")
                {
                    stringstream ss(transicao);
                    string destino;
                    while (getline(ss, destino, ','))
                    {
                        afn.transicoesNaoDet[{estadoAtual, simboloAtual}].insert(destino);
                    }
                }
            }
            cout << "Transicao(" << estadoAtual << ", epsilon): ";
            string transicao;
            getline(cin, transicao);
            if (transicao != "-")
            {
                stringstream ss(transicao);
                string destino;
                while (getline(ss, destino, ','))
                {
                    afn.transicoesNaoDet[{estadoAtual, '\0'}].insert(destino);
                }
            }
        }

        // Converte o AFN em AFD e exibe o resultado
        AutomatoDeterministico afd = converterAFNparaAFD(afn);
        exibirAFD(afd);

        cout << "========================" << endl;
        cout << "Testar outro automato:\n ";
        cin >> loop;
        getchar();
        if (loop == 0)
            break;
    }

    return 0;
}
