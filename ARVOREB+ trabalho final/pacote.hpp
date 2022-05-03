/* Vetor Expansível - Pacote de dados
 *
 * by Joukim, Fevereiro de 2021 - Estrutura de Dados (GCC216)
 * Caracteristicas da primeira implementação - fevereiro de 2021:
 * -> tamanho do pacote e tipo de dados definidos 
 * -> em arquivo de configuração
 */

#include <iostream>
#include "configuracao.hpp"

using namespace std;
// essa classe também poderia se chamar bloco, pagina, packet ou pedaco
class pacote {
	friend class ArvoreBMais;
    friend class sequenceset;
    // classe não possui métodos públicos
    // objetos da classe são criados e manuseados
    // apenas pela classe sequenceset
    private:
        dado elementos[CAP_PACOTE];
        unsigned numElementos;
        int posProximoPacote;
        pacote(): numElementos(0), posProximoPacote(POS_INVALIDA) { }
        bool cheio() { return (numElementos == CAP_PACOTE); }
        void inserir(dado umDado);
        inline bool chaveEhMaiorQueTodos(tipoChave chave);
        inline bool chaveEhMenorQueTodos(tipoChave chave);
        void imprimir();
};

void pacote::inserir(dado umDado) {
    int posicao = numElementos - 1;
    // Faz a procura pela posição de inserção do elemento de forma decrescente
    while ( (posicao >= 0) and umDado.id < elementos[posicao].id) {
        elementos[posicao+1] = elementos[posicao];
        posicao--;
    }
    elementos[posicao+1] = umDado;
    numElementos++;
}

void pacote::imprimir() {
    cout << "[";
    for (unsigned i = 0; i < numElementos; i++)
        cout << "(" << elementos[i].id << "/" << elementos[i].total << ")";
    cout << "]";
}

bool pacote::chaveEhMaiorQueTodos(tipoChave chave) {
    return ( elementos[numElementos-1].id < chave );
}

bool pacote::chaveEhMenorQueTodos(tipoChave chave) {
    return ( elementos[0].id > chave );
}
