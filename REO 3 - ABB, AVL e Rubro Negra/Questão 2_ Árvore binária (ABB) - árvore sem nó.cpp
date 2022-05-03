#include <iostream>
#include <string>
#include <queue>
#include <stdexcept>

typedef int TChave;
typedef int TValor;

class Arvore {
    friend std::ostream& operator << (std::ostream& saida, const Arvore& arv);
    public:
        Arvore();
        Arvore(const Arvore& arv);
        ~Arvore(); // Destrutor recursivo!
        // Retorna o valor associado a uma chave.
        TValor& Buscar(const TChave& chave);
        // Escreve uma árvore nível a nível.
        void EscreverNivelANivel(std::ostream& saida);
        // Insere um chave/valor numa árvore.
        void Inserir(const TChave& chave, const TValor& valor);
        // Remove chave da árvore. Retorna falso se não foi possível remover.
        bool Remover(const TChave& chave);
        // Verifica se uma árvore é vazia.
        bool Vazia() { return mVazia; }
    protected:
        // Retorna ponteiro para a subárvore do menor valor.
        Arvore* Menor();
        // Substitui a raiz pela raiz de uma subárvore. A raiz da subárvore é removida.
        // Usado na remoção de chaves/valores.
        void SubstituirPor(Arvore* ptArv);

        bool mVazia;
        TChave mChave;
        TValor mValor;
        Arvore* mPtEsq;
        Arvore* mPtDir;
};

using namespace std;

Arvore::Arvore()
    : mVazia(true), mChave(), mValor(), mPtEsq(NULL), mPtDir(NULL) {
}

Arvore::Arvore(const Arvore& arv)
    : mVazia(true), mChave(), mValor(), mPtEsq(NULL), mPtDir(NULL) {
    if (not arv.mVazia) {
        mVazia = false;
        mChave = arv.mChave;
        mValor = arv.mValor;
        mPtEsq = new Arvore(*arv.mPtEsq);
        mPtDir = new Arvore(*arv.mPtDir);
    }
}
//DÚVIDA: deletar filhos esquerdo e direito é o mesmo que chamar o destrutor nas duas sub árvores?
Arvore::~Arvore() {
    delete mPtEsq;
    delete mPtDir;
}

void Arvore::EscreverNivelANivel(ostream& saida) {
    queue<Arvore*> filhos;
    Arvore aux;
    Arvore* fimDeNivel = &aux; // marcador especial para fim de nivel
    filhos.push(this);
    filhos.push(fimDeNivel);
    while (not filhos.empty()) {
        Arvore* ptArv = filhos.front();
        filhos.pop();
        if (ptArv == fimDeNivel) {
            saida << "\n";
            if (not filhos.empty())
                filhos.push(fimDeNivel);
        }
        else {
            saida << '[';
            if (not ptArv->mVazia) {
                saida << ptArv->mChave << '/' << ptArv->mValor;
                filhos.push(ptArv->mPtEsq);
                filhos.push(ptArv->mPtDir);
            }
            saida << ']';
        }
    }
}

Arvore* Arvore::Menor() {//---------------------------certo?
	Arvore* iter = this;
	while (!iter->mPtEsq->mVazia)//enquanto a proxima sub arvore não for vazia
	{
		iter = iter->mPtEsq;
	}
	return iter;
}

void Arvore::SubstituirPor(Arvore* ptArv) {//--------------------------------------certo?-----------------CORRIGIR
    //DEU CERTO ATÉ O TESTE "2 2"            TESTAR DESENHANDO
    if (ptArv->mVazia)//mudei o teste de "ptArv == NULL" para "ptArv->mVazia". Pois devido ao meu Inserir, os filhos nunca são NULL(?)
        throw invalid_argument("Substitutição de nós falhou.");
    // complete aqui
    Arvore* raiz_antiga = this;
    Arvore* nova_raiz = new Arvore(*ptArv);//copia toda a sub_árvore de raiz ptArv (mudar?)
    nova_raiz->mPtEsq = raiz_antiga->mPtEsq;
    nova_raiz->mPtDir = raiz_antiga->mPtDir;
    raiz_antiga->mPtEsq = NULL;
    raiz_antiga->mPtDir = NULL;
    //os ponteiros estão apontando pra NULL e agora os ponteiros da nova raiz estão apontando para ele, então não há nessecidade de dar delete na raiz_antiga
    delete ptArv;
    ptArv = new Arvore;//necessário para manter minha ideia do Inserir
    *this = *nova_raiz;
}

TValor& Arvore::Buscar(const TChave& chave) {
    if (mVazia)
        throw runtime_error("Chave não existe na árvore.");
    if (chave == mChave)
        return mValor;
    if (chave < mChave)
        return mPtEsq->Buscar(chave);
    else
        return mPtDir->Buscar(chave);
}

void Arvore::Inserir(const TChave& chave, const TValor& valor) {//------------------------certo?    provavelmente precisarei mudar?
	if(mVazia){//se estiver vazia, simplesmente insere a chave e o valor
		mChave = chave;
		mValor = valor;
		mVazia = false;
		//já prepara as sub_árvores esquerda e direita para futura inserção
		mPtEsq = new Arvore;
		mPtDir = new Arvore;
	}else{//caso contrário
		if(chave < mChave){//se a chave é menor, tenta inserir no filho esquerdo
			mPtEsq->Inserir(chave, valor);
		}else if(chave > mChave){//se a chave é maior, tenta inserir no filho direito
			mPtDir->Inserir(chave, valor);
		}	
	}
}

bool Arvore::Remover(const TChave& chave) {//----------------------------certo?----------------ERRADO. PRECISA PROCURAR A CHAVE(mudei)
	//até aqui deu para seguir com minha ideia do inserir
	if(mVazia){//se a chave for fazia, retorna falso
		return false;
	}
	if(mPtEsq->mVazia && mPtDir->mVazia){//caso seja uma folha
		this->mVazia = true;//é excluída logicamente
		return true;
	}
	bool removeu = true;//é true até que prove o contrário
	try{
		
        if(chave == mChave){//se achou a sub árvore com a chave
            if(!mPtDir->mVazia){//se tiver direito, substitui pelo sucessor
                SubstituirPor(mPtDir->Menor());
            }else{//caso contrário, tenta substituir pelo filho esquerdo
                SubstituirPor(mPtEsq);//caso seja vazio, vai cair em um throw e retornar false
            }
        }else{
            if(chave < mChave){//se a chave está na sub árvore esquerda
                removeu = mPtEsq->Remover(chave);
            }else if(chave > mChave){//se a chave está na sub árvore direita
                removeu = mPtDir->Remover(chave);
            }
        }
	}catch(runtime_error& e){
        return false;//ignora a mensagem de erro e retorna false
    }
    return removeu;
}

ostream& operator << (ostream& saida, const Arvore& arv) {
    if (arv.mVazia)
        saida << "()";
    else
        saida << '(' << arv.mChave << '/' << arv.mValor << ' '
              << *arv.mPtEsq << ' ' << *arv.mPtDir << ')';
    return saida;
}

int main() {
    Arvore arvore;
    TChave chave;
    TValor valor;
    int operacao;
    do {
        cin >> operacao;
        switch (operacao) {
            case 1: // Inserir
                cin >> chave >> valor;
                arvore.Inserir(chave, valor);
                break;
            case 2: // Remover
                cin >> chave;
                if (not arvore.Remover(chave))
                    cerr << "Impossível remover. A chave não existe." << endl;
                break;
            case 3: // Buscar
                try {
                    cin >> chave;
                    valor = arvore.Buscar(chave);
                    cout << valor << endl;
                }
                catch (runtime_error& e) {
                    cerr << e.what() << endl;
                }
                break;
            case 4: // Escrever tudo
                cout << arvore << endl;
                break;
            case 5: // Escrever nivel a nivel
                arvore.EscreverNivelANivel(cout);
                break;
        }
    } while (operacao > 0);
    return 0;
}
