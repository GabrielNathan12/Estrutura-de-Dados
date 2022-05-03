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
		
	public:
		Lista();
		Lista(const Lista& lista);
		~Lista();
		Lista& operador (const Lista& lista);
		void insere(Dado dado);
		void insereFim(Dado dado);
		void imprime();
		inline bool vazia();
		noh* ListaPos(int& pos);
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


void Lista :: imprime(){
	noh* aux = primeiro;
	
	while(aux != NULL){
		cout << aux->dado << " ";
		aux = aux->proximo;
	}
}


inline bool Lista :: vazia(){
	return primeiro == NULL;
}

noh* Lista::ListaPos(int& pos){
	noh* aux = primeiro;
	int cont = 0;
	
	while(cont<pos){
		cont++;
		aux = aux->proximo;
	}
	
	return aux;
	
}

int main(){
	int n , m;
	Dado valor1 , valor2;
	Lista lista1 , lista2;
	int cont1 = 0 , cont2 = 0;
	
	cin >> n;
	
	while(cont1 < n){
		cin >> valor1;
		lista1.insereFim(valor1);
		cont1++;
	}
	
	cin >> m;
	
	while(cont2 < m){
		cin >> valor2;
		lista2.insereFim(valor2);
		cont2++;
	}
	
	bool diferente;
	for (int i = 0; i < n; i++){
		diferente = true;
		for (int j = 0; j < m; j++){
			if(lista1.ListaPos(i) == lista2.ListaPos(j) and diferente){
				diferente = false;
			}
		}
		if(diferente){
			lista1.imprime();
			lista2.imprime();
		}
	}
	
	return 0;
}
