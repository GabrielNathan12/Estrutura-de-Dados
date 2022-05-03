#include <iostream>

typedef int Dado;

class Noh {
	friend class Pilha;
	
	private:
		Dado mDado; // poderia ser outro tipo de variável
		Noh* mPtProx;
	public:
		Noh(Dado valores = 0);
};

Noh::Noh(Dado valores) {
	mDado = valores;
	mPtProx = NULL;
}

class Pilha {
	public:
		Pilha();// Constrói pilha vazia.
		~Pilha();// Destrutor que desaloca memória.
		// Retira e retorna o valor que estiver no mPtTopo da pilha.
		// Escreve uma mensagem de erro se não for possível desempilhar.
		Dado Desempilhar(); // retorna o mPtTopo da Pilha
		void Empilhar(const Dado& d); // Insere um valor na pilha.
		void LimparTudo(); // Apagar todos os dados da pilha.
		unsigned Tamanho(){ // Informa quantos valores existem na pilha.
			return mTamanho;
		}
		Dado Topo() const; // Retorna o valor que está no mPtTopo sem desempilhar.
		bool Vazia() const; // Informa se a pilha está Vazia.
	private:
		Noh* mPtTopo;
		unsigned mTamanho;
};

using namespace std;

Pilha::Pilha() {
	mPtTopo = NULL;
	mTamanho = 0;
}

Pilha::~Pilha() {
	Noh* atual = mPtTopo;
	Noh* ant = NULL;
	while(atual != NULL){
		ant = atual;
		atual = atual->mPtProx;
		delete ant;
	}
}

Dado Pilha::Desempilhar() {
	if(Vazia()){
		cerr << "Vazia" << endl;
		return 0;
	} else {
		Noh* atual = mPtTopo;
		Noh* ant = mPtTopo->mPtProx;
		mPtTopo = ant;
		Dado temp = atual->mDado;
		delete atual;
		mTamanho--;
		return temp;
	}
}

void Pilha::Empilhar(const Dado& valores) {
	Noh* novo = new Noh;
	if(novo){
		novo->mDado = valores;
		novo->mPtProx = NULL;
		if(mPtTopo == NULL){
			mPtTopo = novo;
		} else {
			novo->mPtProx = mPtTopo;
			mPtTopo = novo;
		}
		mTamanho++;
	}
}

void Pilha::LimparTudo() {
	if(Vazia()){
		cerr << "Vazia" << endl;
	} else {
		Noh* atual = mPtTopo;
		Noh* ant = NULL;
		while(atual != NULL){
			ant = atual;
			atual = atual->mPtProx;
			delete ant;
			mTamanho--;
		}
	}
	mPtTopo = NULL;
	mTamanho = 0;
}

Dado Pilha::Topo() const {
	return mPtTopo->mDado;
}

bool Pilha::Vazia() const {
	return(mTamanho == 0);
}

int main() {
	Pilha pilha;
	Dado valor;
	char comando;
	
	do {
		cin >> comando;
		switch (comando) {
			case 'i':{ // inserir
				cin >> valor;
				pilha.Empilhar(valor);
				break;
			}
			case 'r':{ // remover
				cout << pilha.Desempilhar() << endl;
				break;
			}
			case 'l':{ // limpar tudo
				pilha.LimparTudo();
				break;
			}
			case 't':{// consultar tamanho
				cout << pilha.Tamanho() << endl;
				break;
			}
			case 'e':{// espiar
				cout << pilha.Topo() << endl;
				break;
			}
			case 'f':{ // finalizar
				// checado no do-while
				break;
			}
			default:{
				cerr << "comando inválido\n";
			}
		}
	} while (comando != 'f'); // finalizar execução
	
	while (not pilha.Vazia()) {
		cout << pilha.Desempilhar() << ' ';
	}
	cout << endl;

	return 0;
}

