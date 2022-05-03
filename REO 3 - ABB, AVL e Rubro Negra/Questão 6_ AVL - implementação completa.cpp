#include <iostream>
#include <algorithm>
#include <string>
#include <stdexcept>
#include <queue>

typedef std::string TChave; // chave da Ã¡rvore
typedef float TValor; // valor da Ã¡rvore

// === DeclaraÃ§Ãµes das classes ===================================================
class NohAVL; // declaraÃ§Ã£o avanÃ§ada

class AVL {
    public:
        AVL();//ok
        ~AVL();//ok
        TValor& Valor(TChave chave);//ok
        void EscreverEmOrdem();//ok
        void EscreverNivelANivel(std::ostream& saida);//ok
        void Inserir(TChave chave, const TValor& valor);//ok
        bool ConferirLigacoes();//ok
        void Remover(TChave chave);//ok
    private:
        NohAVL* mPtRaiz;
};

class NohAVL {
    friend std::ostream& operator<<(std::ostream& saida, NohAVL* ptNoh);//ok
    friend void AVL::EscreverNivelANivel(std::ostream& saida);//ok
    public:
        NohAVL (TChave c, const TValor& v);//ok
        ~NohAVL();//ok
        void DesalocarFilhosRecursivo();//ok
        NohAVL* InserirRecursivo(NohAVL* ptNoh);//ok
        NohAVL* MenorRecursivo();//ok
        NohAVL* RemoverRecursivo(TChave chave);
        TValor& Valor(TChave chave);//ok
    private:
        NohAVL* ArrumarBalanceamento();//ok
        void AtualizarAltura();//ok
        int FatorBalanceamento();//ok
        NohAVL* Remover();
        NohAVL* RotacionarEsquerda();//ok
        NohAVL* RotacionarDireita();//ok
        void SubstitutirPor(NohAVL* ptNoh);//ok
        void Transplantar(NohAVL* ptFilho);//ok
        void TrocarFilho(NohAVL* ptAntigo, NohAVL* ptNovo);//ok
        TChave mChave;
        TValor mValor;
        NohAVL* mPtEsq;
        NohAVL* mPtDir;
        NohAVL* mPtPai;
        unsigned int mAltura; // folhas tÃªm altura 1
};

// === ImplementaÃ§Ãµes das classes ================================================

using namespace std;

// === Classe NohAVL =============================================================

NohAVL::NohAVL(TChave c, const TValor& v)
    : mChave(c), mValor(v), mPtEsq(NULL), mPtDir(NULL), mPtPai(NULL), mAltura(1) {
}

NohAVL::~NohAVL() {
    DesalocarFilhosRecursivo();
}

// Faz as rotaÃ§Ãµes e ajustes necessÃ¡rios inclusive do nÃ³ pai. Atualiza a altura.
// Retorna o ponteiro para o nÃ³ que ficar na posiÃ§Ã£o dele apÃ³s os ajustes.
NohAVL* NohAVL::ArrumarBalanceamento() {
    AtualizarAltura();
    int fator_balanceamento = FatorBalanceamento();
    if(fator_balanceamento > 1 and mPtEsq->FatorBalanceamento() >= 0){
        return RotacionarDireita();
    }
    if(fator_balanceamento > 1 and mPtEsq->FatorBalanceamento() < 0){
        mPtEsq = mPtEsq->RotacionarEsquerda();
        return RotacionarDireita();
    }
    if(fator_balanceamento < -1 and mPtDir->FatorBalanceamento() <= 0){
        return RotacionarEsquerda();
    }
    if(fator_balanceamento < -1 and mPtDir->FatorBalanceamento() > 0){
        mPtDir = mPtDir->RotacionarDireita();
        return RotacionarEsquerda();
    }
    return this;
}

// Calcula e atualiza a altura de um nÃ³.
void NohAVL::AtualizarAltura() {
    int altArvEsq;
    int altArvDir;
    if(mPtEsq != NULL){
        altArvEsq = mPtEsq->mAltura;
    }else{
        altArvEsq = 0;
    }
    if(mPtDir != NULL){
        altArvDir = mPtDir->mAltura;
    }else{
        altArvDir = 0;
    }
    if(altArvEsq >= altArvDir){
        mAltura = altArvEsq + 1;
    }else{
        mAltura = altArvDir + 1;
    }
}

// Desaloca todos os descendentes.
void NohAVL::DesalocarFilhosRecursivo() {
    if(mPtEsq != NULL){
        delete mPtEsq;
    }
    if(mPtDir != NULL){
        delete mPtDir;
    }
}

// Calcula e retorna o fator de balanceamento do nÃ³.
int NohAVL::FatorBalanceamento() {
    int AltEsq;
    int AltDir;
    if(mPtEsq != NULL){
        AltEsq = mPtEsq->mAltura;
    } else {
        AltEsq = 0;
    }
    if(mPtDir != NULL){
        AltDir = mPtDir->mAltura;
    } else {
        AltDir = 0;
    }
    int fatorBal = AltEsq - AltDir;
    return fatorBal;
}

// Insere um nÃ³ numa subÃ¡rvore. Retorna o ponteiro para o nÃ³ que ficou no lugar do que recebeu
// a mensagem.
NohAVL* NohAVL::InserirRecursivo(NohAVL* ptNoh) {
    if(ptNoh->mChave < mChave){
        if(mPtEsq == NULL){
            mPtEsq = ptNoh;
            ptNoh->mPtPai = this;
        } else {
            mPtEsq = mPtEsq->InserirRecursivo(ptNoh);
        }
    } else {
        if(mPtDir == NULL){
            mPtDir = ptNoh;
            ptNoh->mPtPai = this;
        } else { 
            mPtDir = mPtDir->InserirRecursivo(ptNoh);
        }
    }
    return ArrumarBalanceamento();
}

// Busca o nÃ³ que tem a menor chave. Retorna o ponteiro para ele.
NohAVL* NohAVL::MenorRecursivo() {
    if(mPtEsq != NULL){
        mPtEsq->MenorRecursivo();
    }
    cout << "achei o menor" << endl;
    return mPtEsq;
}

// Remove o nÃ³. Retorna o ponteiro para o nÃ³ que ficou no lugar dele.
// Confira com RemoverRecursivo(TChave).
NohAVL* NohAVL::Remover() {
    if(this != NULL){
        if(mPtPai != NULL){
            if(mPtEsq == NULL){
                cout << "condicao com esq == NULL" << endl;
                this->Transplantar(mPtDir);
                cout << "transplantei" << endl;
            } else if(mPtDir == NULL){
                cout << "condicao com esq == NULL" << endl;
                this->Transplantar(mPtEsq);
                cout << "tranplantei" << endl;
            } else {
                NohAVL* sucessor = mPtDir->MenorRecursivo();
                cout << "achei o sucessor" << endl;
                if(sucessor->mPtPai != this){
                    TrocarFilho(this, sucessor);
                    sucessor->mPtDir = mPtDir;
                    sucessor->mPtDir->mPtPai = sucessor;
                }
                TrocarFilho(this, sucessor);
                sucessor->mPtEsq = this->mPtEsq;
                sucessor->mPtEsq->mPtPai = sucessor;
            }
        } else {
            NohAVL* aux = mPtDir->MenorRecursivo();
            TrocarFilho(this, aux);
        }
        cout << "deletando..." << endl;
        delete this;
    } 
    throw runtime_error("ERRO");
}

// Busca recursivamente um nÃ³ com dada chave e o remove. Retorna o ponteiro para o nÃ³ que ficou
// no lugar do nÃ³ que recebeu a mensagem.
// Confira com Remover().
NohAVL* NohAVL::RemoverRecursivo(TChave chave) {
    if(mChave != chave){
        if(mChave < chave){
            mPtDir->RemoverRecursivo(chave);
        } else {
            mPtEsq->RemoverRecursivo(chave);
        }
    }
    return this->Remover();
}

// Rotaciona Ã  direita a subÃ¡rvore. Retorna a nova raiz da subÃ¡rvore.
NohAVL* NohAVL::RotacionarDireita() {
    NohAVL* aux = mPtEsq;
    this->mPtEsq = aux->mPtDir;
    if(aux->mPtDir != NULL){
        aux->mPtDir->mPtPai = this;
    }
    aux->mPtDir = this;
    this->mPtPai = aux;
    
    this->AtualizarAltura();
    aux->AtualizarAltura();

    return aux;
}

// Rotaciona Ã  esquerda a subÃ¡rvore. Retorna a nova raiz da subÃ¡rvore.
NohAVL* NohAVL::RotacionarEsquerda() {
    NohAVL* aux = mPtDir;
    this->mPtDir =  aux->mPtEsq;
    if(aux->mPtEsq != NULL){
        aux->mPtEsq->mPtPai = this;
    }
    aux->mPtPai = this->mPtPai;
    aux->mPtEsq = this;
    this->mPtPai = aux;
    
    this->AtualizarAltura();
    aux->AtualizarAltura();
    
    return aux;
}

// Atualiza todas as ligaÃ§Ãµes, para que ptNoh, fique no lugar deste nÃ³.
// Usado na remoÃ§Ã£o com dois filhos.
void NohAVL::SubstitutirPor(NohAVL* ptNoh) {//Será mesmo só substituir desse jeito???
//Sera que tenho que dar delete aqui?
    if(mPtEsq != NULL and mPtDir != NULL){
        ptNoh->mPtPai = mPtPai;
        ptNoh->mPtEsq = mPtEsq;
        ptNoh->mPtDir = mPtDir;
    }
}

// Muda as ligaÃ§Ãµes do pai e do filho para desligar o nÃ³ atual.
// Usado para remoÃ§Ã£o na situaÃ§Ã£o em que nÃ£o existem os dois filhos.
void NohAVL::Transplantar(NohAVL* ptFilho) {//Preciso checar se sou filho dir ou esq do pai ???
    cout << "transplantar" << endl;
    ptFilho->mPtPai = mPtPai;
}

// Substitui um dos filhos por um novo nÃ³.
void NohAVL::TrocarFilho(NohAVL* ptAntigo, NohAVL* ptNovo) {
    cout << "troca filho" << endl;
    if(ptNovo->mPtPai == ptAntigo){
        ptNovo->mPtPai = ptNovo;
    } else if (ptAntigo == ptAntigo->mPtPai->mPtEsq){ 
        ptAntigo->mPtPai->mPtEsq = ptNovo;
    } else {
        ptAntigo->mPtPai->mPtDir = ptNovo;
    }
    if(ptNovo != NULL){ 
        ptNovo->mPtPai = ptAntigo->mPtPai;
    }
    cout << "fim troca filho" << endl;
}

// Busca recursivamente uma dada chave e retorna o valor associado a ela.
// Levanta exceÃ§Ã£o se nÃ£o encontrar a chave.
TValor& NohAVL::Valor(TChave chave) {
    if(mChave != chave){
        if(mPtEsq != NULL and mChave > chave){
            return mPtEsq->Valor(chave);
        } else if(mPtDir != NULL and mChave < chave) {
            return mPtDir->Valor(chave);
        }
    } else {
        return mValor;
    }
    throw runtime_error("ERRO BUSCA REC");
}

// Escreve o conteÃºdo de um nÃ³ no formato [altura:chave/valor].
// Escreve "[]" se o ponteiro recebido for NULL.
ostream& operator<<(ostream& saida, NohAVL* ptNoh) {
    if (ptNoh == NULL)
        saida << "[]";
    else
        saida << '[' << ptNoh->mAltura << ':' << ptNoh->mChave << '/' << ptNoh->mValor << ']';
    return saida;
}

// === Classe AVL =================================================================================
AVL::AVL() : mPtRaiz(NULL) {
}

AVL::~AVL() {
    delete mPtRaiz;
}

// Retorna o valor associado a uma dada chave.
TValor& AVL::Valor(TChave chave) {
    if (mPtRaiz == NULL)
        throw runtime_error("Tentativa de buscar valor numa arvore vazia.");
     return mPtRaiz->Valor(chave);
}

// Escreve o conteÃºdo da Ã¡rvore nÃ­vel a nÃ­vel, na saÃ­da de dados informada.
// Usado para conferir se a estrutra da Ã¡rvore estÃ¡ correta.
void AVL::EscreverNivelANivel(ostream& saida) {
    queue<NohAVL*> filhos;
    NohAVL* fimDeNivel = new NohAVL(TChave(), TValor()); // nÃ³ especial para marcar fim de um nÃ­vel
    filhos.push(mPtRaiz);
    filhos.push(fimDeNivel);
    while (not filhos.empty()) {
        NohAVL* ptNoh = filhos.front();
        filhos.pop();
        if (ptNoh == fimDeNivel) {
            saida << "\n";
            if (not filhos.empty())
                filhos.push(fimDeNivel);
        }
        else {
            saida << ptNoh << ' ';
            if (ptNoh != NULL) {
                filhos.push(ptNoh->mPtEsq);
                filhos.push(ptNoh->mPtDir);
            }
        }
    }
    delete fimDeNivel;
}

// Insere um par chave/valor na Ã¡rvore.
void AVL::Inserir(TChave chave, const TValor& valor) {
    NohAVL* novo = new NohAVL(chave, valor);
    if(mPtRaiz == NULL){
        mPtRaiz = novo;
    }else{
        mPtRaiz = mPtRaiz->InserirRecursivo(novo);
    }
}

// Remove da Ã¡rvore o nÃ³ que tiver uma dada chave.
void AVL::Remover(TChave chave) {
    if (mPtRaiz == NULL)
        throw runtime_error("Tentativa de remover elemento de uma Ã¡rvore vazia.");
    else { 
        mPtRaiz = mPtRaiz->RemoverRecursivo(chave);
    }
}

// === Programa ===================================================================================
int main() {
    AVL minhaArvore;
    char opcao;
    TChave chave;
    TValor valor;
    do {
        cin >> opcao;
        switch (opcao) {
            case 'i': // Inserir
                cin >> chave >> valor;
                minhaArvore.Inserir(chave, valor);
                break;
            case 'r': // Remover
                try {
                    cin >> chave;
                    minhaArvore.Remover(chave);
                }
                catch (runtime_error& erro) {
                    cerr << erro.what() << endl;
                }
                break;
            case 'b': // Buscar
                try {
                    cin >> chave;
                    valor = minhaArvore.Valor(chave);
                    cout << valor << endl;
                }
                catch (runtime_error& erro) {
                    cerr << erro.what() << endl;
                }
                break;
            case 'e': // Escrever nÃ³s nÃ­vel a nÃ­vel
                minhaArvore.EscreverNivelANivel(cout);
                break;
            case 'f': // Finalizar o programa
                // vai verificar depois
                break;
            default:
                cerr << "OpÃ§Ã£o invÃ¡lida\n";
        }
    } while (opcao != 'f');
    return 0;
}
