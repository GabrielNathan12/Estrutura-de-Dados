#include <iostream>   // Pode usar varias coisas prontas
#include <vector>     // mas e' preciso completar a classe
#include <limits>     // classe Heap. E' permitido fazer
#include <string>     // alteracoes na classe Heap.
#include <stdexcept>
#include <cmath>
//Peco desculpas pelo codigo nao esta funcionando 100% e que eu tava fazendo ele esse 
//final de semana so que fiquei sem internet praticamente o dia inteiro de sabado volto so na madrugada
//e hoje foi a mesma coisa so que volto mais cedo , nao vai dar tempo deu arrumar o codigo agora mais 
typedef unsigned TCodProcesso;

struct Processos {
	TCodProcesso mVetCodigos;
	int mVetPrioridades;
};

class Heap {
	// classe Heap (max-heap, o maior fica na raiz/inicio)
	public:
		Heap(int cap); // Criar heap sem dados e com capacidade informada
		~Heap(); // Destrutor
		bool Inserir(TCodProcesso codigo, int prioridade); // Inserir um dado na heap
		bool Retirar(TCodProcesso* ptCodigo, int* ptPrioridade); // Retira e retorna a raiz
		bool Vazia(); // Verifica se a heap está vazia
	protected:
		void ArrumarDescendo(int i);
		void ArrumarSubindo(int i);
		inline int Direito(int i){return 2*i + 1;}
		inline int Esquerdo(int i){return 2*i;}
		inline int Pai(int i){return i/2;}
		void Troca(TCodProcesso *vet1, TCodProcesso *vet2);
		TCodProcesso mTamanho;
		TCodProcesso mCapacidade;
		Processos* mVetDados;
};

using namespace std;

Heap::Heap(int cap){
	mCapacidade = cap;
	mVetDados = new Processos[mCapacidade];
	mTamanho = 0;
}

Heap::~Heap() {
	while (!Vazia()){
		delete[] mVetDados;
	}
}

void Heap::ArrumarDescendo(int i){
	// Arruma uma única posição que está menor do que deveria.
	// Usado na remoção.
	TCodProcesso esq = Esquerdo(i);
	TCodProcesso dir = Direito(i);
	
	int maior = i;
	
	if((esq <= mTamanho-1) and (mVetDados[esq].mVetPrioridades > mVetDados[i].mVetPrioridades)){
		maior = esq;
	}
	if((dir <= mTamanho-1) and (mVetDados[dir].mVetPrioridades > mVetDados[maior].mVetPrioridades)){
		maior = dir;
	}
	if(maior != i){
		swap(mVetDados[i], mVetDados[maior]);
		ArrumarDescendo(maior);
	}
}

void Heap::ArrumarSubindo(int i){
	// Arruma uma única posição que está maior do que deveria.
	// Usado na inserção.
	int p = Pai(i);
	
	if(mVetDados[i].mVetPrioridades > mVetDados[p].mVetPrioridades){
		swap(mVetDados[i], mVetDados[p]);
		ArrumarSubindo(p);
	}
}

bool Heap::Retirar(TCodProcesso* ptCodigo, int* ptPrioridade) {
	// Retira um codigo/prioridade, retornando os dois valores.
	// Retorna falso se o heap estiver vazio.
	
	if (mTamanho < 0){
		return false;
	} else {
		swap(mVetDados[0], mVetDados[mTamanho-1]);
		mTamanho--;
		ArrumarDescendo(0);
		return true;
	}
}

bool Heap::Vazia() {
	//Verifica se o heap está vazio.
	if(mTamanho == 0){
		return true;
	} else {
		return false;
	}
}

bool Heap::Inserir(TCodProcesso codigo, int prioridade) {
	if (mTamanho == mCapacidade){
		return false;
	} else {
		mVetDados[mTamanho].mVetCodigos = codigo;
		mVetDados[mTamanho].mVetPrioridades = prioridade;
		ArrumarSubindo(mTamanho);
		mTamanho++;
		return codigo;
	}
}

int main() {
	unsigned capacidade;
	char comando;
	cin >> capacidade;
	
	Heap heap(capacidade);
	TCodProcesso codigo;
	
	int prioridade = capacidade;

	do {
		cin >> comando;
		switch (comando) {
			case 'i':{ // inserir um processo no heap
				cin >> codigo >> prioridade;
				heap.Inserir(codigo, prioridade);
				break;
			}
			case 'o':{ // obter um processo do heap
				if (heap.Retirar(&codigo, &prioridade)){
					cout << codigo << ' ' << prioridade << endl;
				} else {
					cout << "Nao encontrado" << endl;
				}
				break;
			}
			case 's':{ //sair
				break;
			}
		}
	} while (comando != 's');

	return 0;
}
