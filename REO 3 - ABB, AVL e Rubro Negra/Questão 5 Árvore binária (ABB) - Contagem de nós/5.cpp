#include <iostream>

typedef int Dado;

using namespace std;

class Noh{
	friend class ArvoreABB;
	private:
		Noh* mEsquerdo;
		Noh* mDireito;
		Noh* mPai;
		Dado valor;
	
	public:
		Noh(Dado d);
};

Noh::Noh(Dado d){ //Construtor
	mEsquerdo = NULL;
	mDireito = NULL;
	mPai = NULL;
	valor = d;
};

class ArvoreABB{
	private:
		Noh* mRaiz;
		
	public:
		ArvoreABB();
		~ArvoreABB();
		void insere(Dado d);
		void D(){
			destruct(mRaiz);
		}
		void destruct(Noh* atual);
		Dado Nivel(Dado chave);
		int contarNohs(Noh* mRaiz);
		int diferencaNohs(Noh* mRaiz);
		void imprime();
		void imprimeDiferenca();
};

ArvoreABB::ArvoreABB(){ //Contructor
	mRaiz = NULL;
};

ArvoreABB::~ArvoreABB(){ //Destructor
	D();
};

void ArvoreABB::insere(Dado d){ //Insere interativamente
	Noh* novo = new Noh(d);
	
	if(mRaiz == NULL){
		mRaiz = novo;
	} else {
		Noh* atual = mRaiz;
		Noh* ant = NULL;
		
		while(atual != NULL){
			ant = atual;
			if(atual->valor > d){
				atual = atual->mEsquerdo;
			} else {
				atual = atual->mDireito;
			}
		}
		novo->mPai = ant;
		if(ant->valor > novo->valor){
			ant->mEsquerdo = novo;
		} else {
			ant->mDireito = novo;
		}
	}
}

void ArvoreABB::destruct(Noh* atual){
	if(atual != NULL){
		destruct(atual->mEsquerdo);
		destruct(atual->mDireito);
		delete atual;
	}
}

Dado ArvoreABB::Nivel(Dado chave){
	Noh* atual = mRaiz;
	Dado cont = 0;
	
	while(atual != NULL){
		if(atual->valor == chave){
			return cont;
		} else if(atual->valor > chave){
			atual = atual->mEsquerdo;
		} else {
			atual = atual->mDireito;
		}
		cont++;
	}
	return cont;
}

int ArvoreABB :: contarNohs(Noh* mRaiz){
	if(mRaiz == NULL){
		return 0;
	}else{
		return 1 + contarNohs(mRaiz->mEsquerdo) + contarNohs(mRaiz->mDireito);
	}
	imprime();
}

void ArvoreABB :: imprime(){
	cout << contarNohs(mRaiz) - 1 << " ";
}

int ArvoreABB :: diferencaNohs(Noh* mRaiz){
	if(mRaiz == NULL){
		return 0;
	}else{
		return 1 - diferencaNohs(mRaiz->mEsquerdo) - diferencaNohs(mRaiz->mDireito);
	}
	
	imprimeDiferenca();
}

void ArvoreABB :: imprimeDiferenca(){
	int aux;
	if(diferencaNohs(mRaiz) < 0){
		aux = diferencaNohs(mRaiz);
		aux *= -1;
		cout << aux;
	}else{
		cout << diferencaNohs(mRaiz);
	}
}

int main(){
	ArvoreABB minhaArvore;
	int n = 0;
	
	while (n != -1){
		cin >> n;
		minhaArvore.insere(n);
	}
	
	cout << endl;
	
	minhaArvore.imprime();
	minhaArvore.imprimeDiferenca();
	
	return 0;
}
