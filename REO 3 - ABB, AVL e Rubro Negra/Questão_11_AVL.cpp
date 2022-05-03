/* AVL - Arvore Binaria de Busca com Balanceamento AVL
 *
 * by Joukim, Outubro de 2019 - Estrutura de Dados (GCC216)
 * Caracteristicas de implementação outubro de 2019:
 * -> tratamento de excessão
 * -> usa Dado na forma chave/valor
 * -> uso de metodos recursivos
 * -> sem duplo encadeamento
 * -> invez de transplanta, usa RemoveMenor
 */

#include <iostream>
#include <stdexcept>

using namespace std;

struct Dado {
    unsigned chave;
    string nome;
    unsigned long long int cpf;
};


ostream& operator<<(ostream& saida, const Dado& e) {
    saida << "Nome: " << e.nome << " | CPF: "  << e.cpf <<  " | Cod: " << e.chave ;
    return saida;
}

istream& operator>>(istream& entrada, Dado& e) {
    entrada >> e.chave >> e.nome >> e.cpf;
    return entrada;
}

typedef unsigned tipoChave; // tipo da chave usada na comparação

class NohAVL {
    friend class AVL;
    private:
        Dado mElemento;
        NohAVL* mPtEsq;
        NohAVL* mPtDir;
        unsigned mAltura;
    public:
        NohAVL(const Dado& umDado):
            mElemento(umDado),  mPtEsq(nullptr), mPtDir(nullptr), mAltura(1) { }
        ~NohAVL() { }
        int FatorBalanceamento();
};

int NohAVL::FatorBalanceamento() {
    int alturaEsq = 0;
    int alturaDir = 0;

    if(mPtEsq != nullptr) {
        alturaEsq = mPtEsq->mAltura;
    }
    if(mPtDir != nullptr) {
        alturaDir = mPtDir->mAltura;
    }

    int fatorBal = (alturaEsq - alturaDir);
    return fatorBal;
}

class AVL {
    friend ostream& operator<<(ostream& output, AVL& arvore);
    private:
        NohAVL* mPtRaiz;
        // percorrimento em ordem da Arvore
        void PercorreEmOrdemAux(NohAVL* atual, int nivel);
        void ImprimeAutorizadosAux(NohAVL* atual, bool impar);
        void AtualizarAltura(NohAVL* umNoh);
        // funções auxiliares para remoção
        NohAVL* EncontraMenor(NohAVL* raizSub);
        NohAVL* RemoveMenor(NohAVL* raizSub);
        // funções auxiliares para inserção e remoção usando metodo recursivo
        // retorna o noh para ajustar o pai ou o mPtRaiz
        NohAVL* InsereAux(NohAVL* umNoh, const Dado& umDado);
        NohAVL* RemoveAux(NohAVL* umNoh, tipoChave chave);
        // metodos para manutenção do balanceamento
        NohAVL* RotacaoEsquerda(NohAVL* umNoh);
        NohAVL* RotacaoDireita(NohAVL* umNoh);
        NohAVL* ArrumaBalanceamento(NohAVL* umNoh);
        // Busca, metodo iterativo
        NohAVL* BuscaAux(tipoChave chave);
        // função auxiliar do destrutor, usa percorrimento pos-ordem
        void DestruirRecursivamente(NohAVL* umNoh);
        void ImprimirDir(const std::string& prefixo, const NohAVL* node);
        void ImprimirEsq(const std::string& prefixo, const NohAVL* node, bool temIrmao);
    public:
        AVL() { mPtRaiz = nullptr; }
        ~AVL();
        void Imprimir();
        // inserção e remoção são recursivos
        void Insere(const Dado& umDado);
        void Remove(tipoChave chave);
        // inserção e remoção, metodos recursivos
        // Busca retorna uma copia do objeto armazenado
        Dado Busca(tipoChave chave);
        void ImprimeAutorizados(int dia);
        bool VerificaAutorizado(tipoChave chave, int dia);
};

// destrutor
AVL::~AVL() {
    if(mPtRaiz != nullptr) {
        DestruirRecursivamente(mPtRaiz);
        delete mPtRaiz;
    }
}

// destrutor noh recursivo, fazendo percorrimento pos-ordem
void AVL::DestruirRecursivamente(NohAVL* umNoh) {
    if(umNoh->mPtEsq != nullptr) {
        DestruirRecursivamente(umNoh->mPtEsq);
        delete umNoh->mPtEsq;
    }
    if(umNoh->mPtDir != nullptr) {
        DestruirRecursivamente(umNoh->mPtDir);
        delete umNoh->mPtDir;
    }
}

void AVL::Insere(const Dado& umDado) {
    mPtRaiz = InsereAux(mPtRaiz, umDado);
}

// inserção recursiva, devolve noh para atribuição de pai ou mPtRaiz
NohAVL* AVL::InsereAux(NohAVL* umNoh, const Dado& umDado) {
   if(umNoh == nullptr) {
       NohAVL* novoNoh = new NohAVL(umDado);
        return novoNoh;
    }
    else {
        if(umDado.chave < umNoh->mElemento.chave) {
            umNoh->mPtEsq = InsereAux(umNoh->mPtEsq, umDado);
        }
        else {
            umNoh->mPtDir = InsereAux(umNoh->mPtDir, umDado);
        }
    }
    return ArrumaBalanceamento(umNoh);
    
}

// checa e arruma, se necessario, o balanceamento em umNoh,
// fazendo as rotações e ajustes necessarios
NohAVL* AVL::ArrumaBalanceamento(NohAVL* umNoh) {
    if(umNoh == nullptr) {
        return umNoh;
    }

    AtualizarAltura(umNoh);
    int fatorBal = umNoh->FatorBalanceamento();

    if((fatorBal >= -1) and (fatorBal <= 1)){ // Está balanceado
        return umNoh;
    }

    // Está desbalanceada, vamos encontrar o desbalanceamento. 
    // Existem 4 casos para ser verificados.
    if((fatorBal > 1) and (umNoh->mPtEsq->FatorBalanceamento() >= 0)) { // Desbalanceamento esquerda esquerda
        return RotacaoDireita(umNoh);
    }
    if((fatorBal > 1) and (umNoh->mPtEsq->FatorBalanceamento() < 0 )) { // Desbalanceamento esquerda direita
        umNoh->mPtEsq = RotacaoEsquerda(umNoh->mPtEsq);
        return RotacaoDireita(umNoh);
    }
    if((fatorBal < -1) and (umNoh->mPtDir->FatorBalanceamento() <= 0)) { // Desbalanceamento direita direita
        return RotacaoEsquerda(umNoh);
    }
    // Desbalanceamento direita esquerda
    umNoh->mPtDir = RotacaoDireita(umNoh->mPtDir);
    return RotacaoEsquerda(umNoh);
}

void AVL::AtualizarAltura(NohAVL* umNoh) {
    int alturaEsq = 0;
    int alturaDir = 0;

    if(umNoh != nullptr) {
        if(umNoh->mPtEsq != nullptr) {
            alturaEsq = umNoh->mPtEsq->mAltura;
        }
        if(umNoh->mPtDir != nullptr) {
            alturaDir = umNoh->mPtDir->mAltura;
        }
    }

    umNoh->mAltura = 1 + max(alturaEsq, alturaDir);
}

// rotação a esquerda na subArvore com mPtRaiz em umNoh
// retorna o novo pai da subArvore
NohAVL* AVL::RotacaoEsquerda(NohAVL* umNoh) {
    NohAVL* nohAux = umNoh->mPtDir;
    umNoh->mPtDir = nohAux->mPtEsq;
    nohAux->mPtEsq = umNoh;

    AtualizarAltura(umNoh);
    AtualizarAltura(nohAux);
    return nohAux;
}


// Rotação a direita na subArvore com mPtRaiz em umNoh
// retorna o novo pai da subArvore
NohAVL* AVL::RotacaoDireita(NohAVL* umNoh) {
    NohAVL* nohAux = umNoh->mPtEsq;
    umNoh->mPtEsq = nohAux->mPtDir;
    nohAux->mPtDir = umNoh;

    AtualizarAltura(umNoh);
    AtualizarAltura(nohAux);
    return nohAux;
}


// metodo de Busca auxiliar (retorna o noh), iterativo
NohAVL* AVL::BuscaAux(tipoChave chave) {
    NohAVL* percorre = mPtRaiz;

    while((percorre != nullptr) and (percorre->mElemento.chave != chave)) {
        if(percorre->mElemento.chave > chave) {
            percorre = percorre->mPtEsq;
        }
        else {
            percorre = percorre->mPtDir;
        }
    }
    return percorre;
}

// Busca mElemento com uma dada chave na Arvore e retorna o registro completo
Dado AVL::Busca(tipoChave chave) {
    if (mPtRaiz != nullptr) {
        NohAVL* resultado = BuscaAux(chave);

        if (resultado != nullptr)
            return resultado->mElemento;
        else {
            throw runtime_error("Erro na busca: elemento não encontrado!");
        }
    }
    else {
        throw runtime_error("*arvore vazia*");
    }
}

// noh minimo (sucessor) de subArvore com mPtRaiz em raizSub (folha mais a esquerda)
NohAVL* AVL::EncontraMenor(NohAVL* raizSub) {
    NohAVL* menor = nullptr;

    while(raizSub != nullptr) {
        menor = raizSub;
        raizSub = raizSub->mPtEsq;
    }
    return menor;
}

// procedimento auxiliar para remover o sucessor substituindo-o pelo
// seu filho a direita
NohAVL* AVL::RemoveMenor(NohAVL* raizSub) {
    NohAVL* remover = nullptr;

    if(raizSub->mPtEsq == nullptr) {
        return raizSub->mPtDir;
    }
    else {
        raizSub->mPtEsq = RemoveMenor(raizSub->mPtEsq);
        return ArrumaBalanceamento(raizSub);
    }
}

// Remoção recursiva
void AVL::Remove(tipoChave chave) {
    mPtRaiz = RemoveAux(mPtRaiz, chave);
}

NohAVL* AVL::RemoveAux(NohAVL* umNoh, tipoChave chave) {
    if(umNoh == nullptr) {
        return umNoh;
    }

    NohAVL* novaRaizSubArvore = umNoh;

    if(umNoh->mElemento.chave > chave) {
        umNoh->mPtEsq = RemoveAux(umNoh->mPtEsq, chave);
    }
    else if (umNoh->mElemento.chave < chave) {
        umNoh->mPtDir = RemoveAux(umNoh->mPtDir, chave);
    }
    else {
        if(umNoh->mPtEsq == nullptr) {
            novaRaizSubArvore = umNoh->mPtDir;
        }
        else if (umNoh->mPtDir == nullptr) {
            novaRaizSubArvore = umNoh->mPtEsq;
        }
        else {
            novaRaizSubArvore = EncontraMenor(umNoh->mPtDir);
            novaRaizSubArvore->mPtDir = RemoveMenor(umNoh->mPtDir);
            novaRaizSubArvore->mPtEsq = umNoh->mPtEsq;
        }
        delete umNoh;
    }
    return ArrumaBalanceamento(novaRaizSubArvore);
}

// utiliza o noh atual e seu nivel na Arvore (para facilitar visualização)
void AVL::ImprimeAutorizadosAux(NohAVL* atual, bool impar) {
    if(atual != nullptr) {
        ImprimeAutorizadosAux(atual->mPtEsq, impar);
        if(atual->mElemento.cpf % 2 == impar) {
            cout << atual->mElemento << endl;
        }
        ImprimeAutorizadosAux(atual->mPtDir, impar);
    }
}

// utiliza o noh atual e seu nivel na Arvore (para facilitar visualização)
void AVL::ImprimeAutorizados(int dia) {
    if(mPtRaiz != nullptr) {
        ImprimeAutorizadosAux(this->mPtRaiz, dia % 2);
    }
    else {
        throw runtime_error("*arvore vazia*");
    }
}

// utiliza o noh atual e seu nivel na Arvore (para facilitar visualização)
bool AVL::VerificaAutorizado(tipoChave chave, int dia) {
    bool controle = false;
    if(mPtRaiz != nullptr) {
        NohAVL* verificar = BuscaAux(chave);
        if(verificar != nullptr) {
            if(dia % 2 == 0) {
                if(verificar->mElemento.cpf % 2 == 0) {
                    controle = true;
                }
                else {
                    controle = false;
                }
            }
            else if(dia % 2 != 0) {
                if(verificar->mElemento.cpf % 2 != 0) {
                    controle = true;
                }
                else {
                    controle = false;
                }
            }
        }
        else {
            throw runtime_error("Erro na verificacao: chave inexistente!");
        }
    }
    else {
        throw runtime_error("*arvore vazia*");
    } 
    return controle;
}

// utiliza o noh atual e seu nivel na Arvore (para facilitar visualização)
void AVL::PercorreEmOrdemAux(NohAVL* atual, int nivel) {
    if(atual != nullptr) {
        unsigned nivel_seguinte = nivel +1;
        PercorreEmOrdemAux(atual->mPtEsq, nivel_seguinte);
        cout << atual->mElemento << endl;
        PercorreEmOrdemAux(atual->mPtDir, nivel_seguinte);
    }
}

ostream& operator<<(ostream& output, AVL& arvore) {
    //arvore.PercorreEmOrdemAux(arvore.mPtRaiz,0);
    arvore.Imprimir();
    return output;
}

// imprime formatado seguindo o padrao tree as subarvores direitas de uma avl
void AVL::ImprimirDir(const std::string& prefixo, const NohAVL* node)
{
    if( node != nullptr )
    {
        std::cout << prefixo
                  << "└d─"
                  << "(" << node->mElemento.chave << "," << node->mElemento.nome << ")"
                  << std::endl;

        // Repassa o prefixo para manter o historico de como deve ser a formatacao e chama no filho direito e esquerdo
        ImprimirEsq( prefixo + "    " , node->mPtEsq , node->mPtDir==nullptr );
        ImprimirDir( prefixo + "    " , node->mPtDir );
    }
}

// imprime formatado seguindo o padrao tree as subarvores direitas de uma AVL
void AVL::ImprimirEsq(const std::string& prefixo, const NohAVL* node, bool temIrmao)
{
    if( node != nullptr )
    {
        std::cout << prefixo ;

        // A impressao da arvore esquerda depende da indicacao se existe o irmao a direita
        if (temIrmao)
            std::cout << "└e─" ;
        else
            std::cout << "├e─";

        std::cout << "(" << node->mElemento.chave << "," << node->mElemento.nome << ")"
                  << std::endl;

        // Repassa o prefixo para manter o historico de como deve ser a formatacao e chama no filho direito e esquerdo
        ImprimirEsq( prefixo + "│”‚   " , node->mPtEsq, node->mPtDir==nullptr );
        ImprimirDir( prefixo + "│”‚   " , node->mPtDir );
    }
}

// imprime formatado seguindo o padrao tree uma AVL
void AVL::Imprimir()
{
    if( this->mPtRaiz != nullptr )
    {
        std::cout << "(" << this->mPtRaiz->mElemento.chave << "," << this->mPtRaiz->mElemento.nome << ")" << std::endl;
        // apos Imprimir a mPtRaiz, chama os respectivos metodos de impressao nas subarvore esquerda e direita
        // a chamada para a impressao da subarvore esquerda depende da existencia da subarvore direita
        ImprimirEsq( " " , this->mPtRaiz->mPtEsq, this->mPtRaiz->mPtDir==nullptr );
        ImprimirDir( " " , this->mPtRaiz->mPtDir );
    } else
        std::cout << "*arvore vazia*" << std::endl;
}

int main() {
    AVL arvore;
    tipoChave chave;
    Dado umDado;
    int dia = 0;

    char operacao;

    do {
        try {
            cin >> operacao;
            switch (operacao) {
                case 'i': // Inserir recursivamente
                    // objeto recebe id, nome, quantidade, valor
                    cin >> umDado;
                    arvore.Insere(umDado);
                    break;
                case 'r': // Remover recursivamente
                    cin >> chave;
                    arvore.Remove(chave);
                    break;
                case 'b': // Buscar
                    cin >> chave; // ler a chave
                    umDado = arvore.Busca(chave); // escrever o valor
                    cout << umDado << endl;
                    break;
                case 'e': // Escrever arvore no formato tree
                    cout << arvore ;
                    break;
                case 'l': // lista os autorizados 
                    cin >> dia;
                    arvore.ImprimeAutorizados(dia);
                    break;     
                case 'v': // Verifica se funcionario esta autorizado
                    cin >> chave >> dia;
                    if ( arvore.VerificaAutorizado(chave, dia) )
                        cout << "Autorizado." << endl;
                    else
                        cout << "Não autorizado." << endl;
                    break;   
                case 'f': // Finalizar execuÃ§Ã£o
                    break;
                default:
                    cout << "Comando invalido!\n";
            }
        } catch (runtime_error& e) {
            cout << e.what() << endl;
        }
    } while (operacao != 'f');

    return 0;
}