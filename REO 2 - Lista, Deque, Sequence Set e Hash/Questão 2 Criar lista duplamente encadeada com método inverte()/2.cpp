#include <iostream>
#include <cstdlib>

using namespace std;

typedef string Dado;

class noh{
	friend class Lista;
	private:
		const Dado dado;
		noh* proximo;
		noh* anterior;
	public:
		noh(Dado d = 0);
};

noh :: noh(Dado d) : dado(d){
	proximo = NULL;
}

class Lista{
	private:
		noh* primeiro;
		noh* ultimo;
		int tamanho;
		void removeTodos();
		void imprimeReversoAux(noh* umNoh);
		
	public:
		Lista();
		Lista(const Lista& lista);
		~Lista();
		Lista& operador (const Lista& lista);
		void insere(Dado dado);
		void insereFim(Dado dado);
		void imprime();
		void imprimeReverso();
		void reverso();
		void encerrarPrograma();
		inline bool vazia();
};

Lista :: Lista(){
	tamanho = 0;
	primeiro = NULL;
	ultimo = NULL;
}

Lista :: Lista(const Lista& lista){
	tamanho = 0;
	primeiro = NULL;
	ultimo = NULL;
	
	noh* aux = lista.primeiro;
	
	while(aux != NULL){
		insereFim(aux->dado);
		aux = aux->proximo;
	}
}

Lista :: ~Lista(){
	removeTodos();
}

void Lista :: removeTodos(){
	noh* aux = primeiro;
	noh* temp;
	
	while(aux != NULL){
		temp = aux;
		aux = aux->proximo;
		delete temp;
	}
	tamanho = 0;
	primeiro = NULL;
	ultimo = NULL;
}

Lista& Lista :: operador (const Lista& lista){
	removeTodos();
	noh* aux = lista.primeiro;
	
	while(aux != NULL){
		insereFim(aux->dado);
	}
	return *this;
}

void Lista :: insere(Dado dado){
	insereFim(dado);
}

void Lista :: insereFim(Dado dado){
	noh* novo = new noh(dado);
	
	if(vazia()){
		primeiro = novo;
		ultimo = novo;
	}else{
		ultimo->proximo = novo;
		ultimo = novo;
	}
	tamanho++;
}

void Lista :: encerrarPrograma(){
	imprime();
	cout << endl;
	imprimeReverso();
}

void Lista :: reverso(){
	if(vazia()){
		cout << "Esta vazia" << endl;
	}else{
		noh* atual = primeiro->anterior;
		noh* aux = primeiro;
		noh* temp = primeiro;
		
		while(aux != NULL){
			temp = temp->proximo;
			aux->proximo = atual;
			aux->anterior = temp;
			atual = aux;
			aux = temp;
		}
		
		primeiro->proximo = NULL;
		ultimo = primeiro;
		primeiro = atual;
		atual = NULL;
		aux = NULL;
		temp = NULL;
		delete atual;
		delete aux;
		delete temp;
	}
}

void Lista :: imprime(){
	noh* aux = primeiro;
	
	while(aux != NULL){
		cout << aux->dado << " ";
		aux = aux->proximo;
	}
}

void Lista :: imprimeReverso(){
	imprimeReversoAux(primeiro);
}

void Lista :: imprimeReversoAux(noh * umNoh){
	if(umNoh != NULL){
		imprimeReversoAux(umNoh->proximo);
		cout << umNoh->dado << " ";
	}
}

inline bool Lista :: vazia(){
	return primeiro == NULL;
}

int main(){
	Lista lista;
	Dado valor;
	
	char opcao;
	
	cin >> opcao;
	
	while (opcao != 'Q') {
		switch(opcao) {
			case 'I':
				cin >> valor;
				lista.insere(valor);
				break;
			case 'X':
				lista.reverso();
				break;
			case 'P':
				lista.imprime();
				break;
			case 'R':
				lista.imprimeReverso();
				break;
			default:
				cerr << "Erro: opcao invalida!\n";
		}
		cin >> opcao;
	}
	
	lista.encerrarPrograma();
	
	return 0;
}

