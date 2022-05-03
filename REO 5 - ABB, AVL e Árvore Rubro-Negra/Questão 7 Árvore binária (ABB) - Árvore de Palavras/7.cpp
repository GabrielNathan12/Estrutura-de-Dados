#include <iostream>
#include <string>
#include <fstream>

using namespace std;

class Lista1{
	friend class Lista;
	private:
		Lista1 *proximo;
		int cont;
	public:
		Lista1(int p){
			cont = p; 
			proximo = nullptr;
		}
};

class Lista{
	friend class Lista1;
	private:
		Lista1 *fim;
		Lista1 *inicio;
	public:
		Lista(){
			fim = nullptr;
		}
		void insere(int cont);
		void imprimir();
};

void Lista::insere(int cont){
	Lista1* novo = new Lista1(cont);
	
	if(fim != nullptr){
		fim->proximo = novo;
	} else {
		inicio = novo;
	}
	fim = novo;
}

void Lista::imprimir(){
	Lista1* aux = inicio;
	
	while (aux != nullptr){
		cout << aux->cont << " ";
		aux = aux->proximo;
	}
	cout << endl;
}

class ABB{
	friend class AvoreABB;
	private:
		Lista cont;
		string palavra;
		ABB *direita;
		ABB *esquerda;
	public:
		ABB(string pa, int po){
			direita = nullptr;
			esquerda = nullptr;
			palavra = pa;
			cont.insere(po);
		}
};

class AvoreABB{
	friend class ABB;
	private:
		ABB *raiz;
	public:
		AvoreABB(){
			raiz = nullptr;
		}
		~AvoreABB();
		void insere(string palavra, int cont);
		void buscar(string palavra);
		void destruirrecursivo(ABB *chave);
};

void AvoreABB::destruirrecursivo(ABB *chave){
	if(chave != nullptr){
		destruirrecursivo(chave->esquerda);
		destruirrecursivo(chave->direita);
		delete(chave);
	}
}

AvoreABB::~AvoreABB(){
	destruirrecursivo(raiz);
}

void AvoreABB::insere(string palavra, int cont){
	ABB *aux = new ABB(palavra, cont);
	
	if(raiz == nullptr){
		raiz = aux;
	} else {
		ABB *auxRaiz = raiz;
		ABB *antes = nullptr;
		
		while(auxRaiz != nullptr and auxRaiz->palavra != palavra){
			antes = auxRaiz;
			
			if(palavra > auxRaiz->palavra){
				auxRaiz = auxRaiz->direita;
			} else {
				auxRaiz = auxRaiz->esquerda;
			}
		}
		if(auxRaiz != nullptr and palavra == auxRaiz->palavra){
			auxRaiz->cont.insere(cont);
			delete(aux);
		} else {
			if(palavra > antes->palavra){
				antes->direita = aux;
			} else {
				antes->esquerda = aux;
			}
		}
	}
}

void AvoreABB::buscar(string palavra){
	ABB *auxRaiz = raiz;
	
	while(auxRaiz != nullptr and auxRaiz->palavra != palavra){
		if(palavra > auxRaiz->palavra){
			auxRaiz = auxRaiz->direita;
		} else {
			auxRaiz = auxRaiz->esquerda;
		}
	}
	if(auxRaiz == nullptr){
		cout << "-1" << endl;
	} else {
		auxRaiz->cont.imprimir();
	}
}

int main(){
	ifstream arquivoE ("entrada.txt");
	string palavra;
	int cont = 1;
	AvoreABB ABB;
	
	if(arquivoE){
		while (arquivoE >> palavra){
			ABB.insere(palavra, cont);
			cont++;
		}
		arquivoE.close();
	}
	
	cin >> palavra;
	
	ABB.buscar(palavra);
	
	return 0;
}
