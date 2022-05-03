#include <iostream>

using namespace std;

typedef int Dado; // para facilitar a troca de int para outro tipo

enum posicao {dir , esq};

class Noh{
	friend class ABB;
	private:
		Dado valor;
		Noh* esq;
		Noh* dir;
		Noh* pai;
	public:
		Noh(Dado d = 0);
		~Noh();
};

Noh :: Noh(Dado d){
	valor = d;
	esq = nullptr;
	dir = nullptr;
	pai = nullptr;
}

Noh :: ~Noh(){ // nao precisa checar se e nulo
	delete esq;
	delete dir;
}

class ABB{
	private:
		Noh* raiz;
		void percorreEmOrdemAux(Noh* atual , int nivel);
		void percorrePreOrdemAux(Noh* atual);
		void percorrePosOrdemAux(Noh* atual);
		Noh* minimoAux(Noh* raizSub);// menor da arvore
		Noh* maximoAux(Noh* raizSub);// maior da arvore
		void trasplanta(Noh* antigo , Noh* novoNoh);//trasplanta o no novo para o local onde estava o antigo no
	public:
		ABB(){raiz = nullptr;}
		~ABB();
		void insere(Dado d);
		void remove(Dado d);
		Noh* busca(Dado d);
		Dado minimo();
		Dado maximo();
		void percorreEmOrdem();
		void percorrePreOrdem();
		void percorrePosOrdem();
};

ABB :: ~ABB(){
	delete raiz;
}

void ABB :: insere(Dado d){
	Noh* novoNoh = new Noh(d);
	posicao posInsercao;
	
	//primeiro caso: arvore vazia
	if(raiz == nullptr){
		raiz = novoNoh;
	}else{
		Noh* atual = raiz;
		Noh* anterior;
		
		// encontrando ponto de insercao
		
		while(atual != nullptr){
			anterior = atual;
			//troca por >= , se quiser elementos iguais
			if(atual->valor > d){
				atual = atual->esq;
				posInsercao = dir;
			}else{
				atual = atual->dir;
				posInsercao = dir;
			}
		}
		
		//inserindo o novo no na arvore
		novoNoh->pai = anterior;
		
		if(posInsercao == dir){
			anterior->dir = novoNoh;
		}else{
			anterior->esq = novoNoh;
		}
	}
}

Noh* ABB :: busca(Dado d){
	Noh* atual = raiz;
	
	while(atual != nullptr){
		if(atual->valor == d){
			return atual;
		}else if(atual->valor > d){
			atual = atual->esq;
		}else{
			atual = atual->dir;
		}
	}
	return atual;
}

Dado ABB :: minimo(){
	if(raiz == nullptr){
		cerr << "ARVORE VAZIA!!" << endl;
		exit(EXIT_FAILURE);
	}else{
		Noh* nohMin = minimoAux(raiz);
		return nohMin->valor;
	}
}

Noh* ABB :: minimoAux(Noh* raizSub){
	while(raizSub->esq != nullptr){
		raizSub = raizSub->esq;
	}
	return raizSub;
}

Dado ABB :: maximo(){
	if(raiz == nullptr){
		cerr << "ARVORE VAZIA!!" << endl;
		exit(EXIT_FAILURE);
	}else{
		Noh* nohMax = maximoAux(raiz);
		return nohMax->valor;
	}
}

Noh* ABB :: maximoAux(Noh* raizSub){
	while(raizSub->dir != nullptr){
		raizSub = raizSub->dir;
	}
	return raizSub;
}

//trasplanta muda uma arvore com raiz em novo para o local onde
// antes estava o no antigo
void ABB :: trasplanta(Noh* antigo , Noh* novoNoh){
	if(raiz == antigo){
		raiz = novoNoh;
	}else if(antigo == antigo->pai->esq){
		antigo->pai->dir = novoNoh;
	}else{
		antigo->pai->dir = novoNoh;
	}
	if(novoNoh != nullptr){
		novoNoh->pai = antigo->pai;
	}
}

void ABB :: remove(Dado dado){
	// achamos o no na arvore
	Noh* nohRemover = busca(dado);
	
	if(nohRemover == nullptr){
		cerr << "VALOR NAO ENCONTRADO!!" << endl;
	}else{
		//no nao tem filhos a esquerda
		if(nohRemover->esq == nullptr){
			trasplanta(nohRemover, nohRemover->dir);
		}
		// no nao tem filhos a direita
		else if(nohRemover->dir == nullptr){
			trasplanta(nohRemover , nohRemover->dir);
		}
	// no tem 2 filhos .. substituimos pelo sucessor
	else{
		Noh* sucessor = minimoAux(nohRemover->dir);
		
		if(sucessor->pai != nohRemover){
			trasplanta(sucessor , sucessor->dir);
			sucessor->dir = nohRemover->dir;
			sucessor->dir->pai = sucessor;
		}
		trasplanta(nohRemover , sucessor);
		sucessor->esq = nohRemover->esq;
		sucessor->dir->pai = sucessor;
	}
		nohRemover->esq = nullptr;
		nohRemover->dir = nullptr;
		delete nohRemover;
	}
}

void ABB :: percorreEmOrdem(){
	percorreEmOrdemAux(raiz , 0);
	cout << endl;
}

void ABB :: percorreEmOrdemAux(Noh* atual , int nivel){
	if(atual != nullptr){
		percorreEmOrdemAux(atual->esq , nivel + 1);
		cout << atual->valor << "/" << nivel << " ";
		percorreEmOrdemAux(atual->dir , nivel + 1);
	}
}

void ABB :: percorrePreOrdem(){
	percorrePreOrdemAux(raiz);
	cout << endl;
}

void ABB :: percorrePreOrdemAux(Noh* atual){
	if(atual != nullptr){
		cout << atual->valor << "/" << " ";
		percorrePreOrdemAux(atual->esq);
		percorrePreOrdemAux(atual->dir);
	}
}

void ABB :: percorrePosOrdem(){
	percorrePosOrdemAux(raiz);
	cout << endl;
}

void ABB :: percorrePosOrdemAux(Noh* atual){
	if(atual != nullptr){
		percorrePosOrdemAux(atual->esq);
		percorrePosOrdemAux(atual->dir);
		cout << atual->valor << "/" << " ";
	}
}

int main(){
	ABB arvore;
	int valor;
	
	for (int i = 0; i < 10; i++){
		cin >> valor;
		arvore.insere(valor);
	}
	
	arvore.percorreEmOrdem();
	
	for (int i = 0; i < 5; i++){
		cin >> valor;
		arvore.remove(valor);
	}
	
	arvore.percorreEmOrdem();
	
	for (int i = 0; i < 10; i++){
		cin >> valor;
		arvore.insere(valor);
	}
	
	arvore.percorreEmOrdem();
	
	for (int i = 0; i < 5; i++){
		cin >> valor;
		arvore.remove(valor);
	}
	
	arvore.percorreEmOrdem();
	
	return 0;
}

