#include <iostream>

using namespace std;

class noh {
friend class fila;
private:
	unsigned chave;
	unsigned valor; 
	noh* prox;
public:
	noh(unsigned k, unsigned v) {
		chave = k;
		valor = v;
		prox = NULL;
	}
};

class fila {
private:
	noh* primeiro;
	noh* ultimo;
	int tamFila;
	
public:
	fila();
	~fila();
	void enqueue(unsigned k, unsigned v) { enfileira(k,v); }
	void insere(unsigned k, unsigned v) { enfileira(k,v); }
	void enfileira(unsigned k, unsigned v);
	unsigned dequeue() { return desenfileira(); } 
	unsigned retira() { return desenfileira(); } 
	unsigned desenfileira();
	void limpaFila();
	void consulta(unsigned& k, unsigned& v);
	bool vazia();
	unsigned tamanho() { return tamFila; }
	void depura();
};

fila::fila() {
	tamFila = 0;
	primeiro = NULL;
	ultimo = NULL;
}

fila::~fila() {
	while (primeiro != NULL) {
		desenfileira();
	}
}

void fila::enfileira(unsigned k, unsigned v) {
	noh* temp = new noh(k,v);
	if (ultimo != NULL) {
		ultimo->prox = temp;
	}
	ultimo = temp;
	if (vazia()) {
		primeiro = temp;
	}
	tamFila++; 
}

unsigned fila::desenfileira() {
	unsigned removido;
	noh* temp;
	removido = primeiro->valor;
	temp = primeiro;
	primeiro = primeiro->prox;
	if (vazia()){
		ultimo = NULL;
	}
	delete temp;
	tamFila--;
	return removido;    
}

void fila::consulta(unsigned& k, unsigned& v) {
	k = primeiro->chave;
	v = primeiro->valor;
}

void fila::limpaFila(){
	while (primeiro != NULL)
		desenfileira();
}
	
bool fila::vazia() {
	return (primeiro == NULL);
}

void fila::depura() {
	noh* aux = primeiro;
	while (aux != NULL) {
		cout << "<" << aux->chave << "," << aux->valor << ">";
		aux = aux->prox;
	}
}

class sistema {
private:
	int capacidade;
	fila* vetorFilas;
public:
	sistema(int c);
	~sistema();
	void enfileira(unsigned posFila, unsigned k, unsigned v);
	unsigned desenfileira(unsigned posFila);
	void consulta(unsigned posFila, unsigned& k, unsigned& v); 
	void move(unsigned posFila1, unsigned posFila2, unsigned k);
	void depura();   
	 
};

sistema::sistema(int c) {
	vetorFilas = new fila[c];
	capacidade = c;
}

sistema::~sistema() {
	delete[] vetorFilas;
}

void sistema::enfileira(unsigned posFila, unsigned k, unsigned v) {
	vetorFilas[posFila].enfileira(k,v);
}

unsigned sistema::desenfileira(unsigned posFila) {
	return vetorFilas[posFila].desenfileira();
}

void sistema::consulta(unsigned posFila, unsigned& k, unsigned& v) {
	vetorFilas[posFila].consulta(k,v);
}

void sistema::depura() {
	for (int i = 0; i < capacidade; i++) {
		cout << "[" << i << "]: ";
		vetorFilas[i].depura();
		cout << endl;
	}
}

void sistema::move(unsigned posFila1, unsigned posFila2, unsigned k) {
	unsigned chaveAux;
	unsigned resultado;
	
	int cont = vetorFilas[posFila1].tamanho();
	
	while(cont > 0) {
		vetorFilas[posFila1].consulta(chaveAux,resultado);
		if(chaveAux == k){
			vetorFilas[posFila2].enfileira(chaveAux,resultado);
			vetorFilas[posFila1].desenfileira();
		}else{
			vetorFilas[posFila1].desenfileira();
			vetorFilas[posFila1].enfileira (chaveAux,resultado);
		}
		cont--;
	}
}

int main() {
	int capacidade;
	cin >> capacidade;
	
	sistema superPowerExtremZystem(capacidade);
	
	unsigned pos1, pos2, k, v;
	
	char opcao;

	
	do {
		cin >> opcao;
		
		switch (opcao) {    
			case 'E':
				cin >> pos1 >> k >> v;
				superPowerExtremZystem.enfileira(pos1,k,v);
				break;
			case 'D':
				cin >> pos1;
				cout << superPowerExtremZystem.desenfileira(pos1) << endl;
				break;
			case 'C':
				cin >> pos1;
				superPowerExtremZystem.consulta(pos1,k,v);
				cout << k << " " << v << endl;
				break;
			case 'P':
				superPowerExtremZystem.depura();
				break;
			case 'M':
				cin >> pos1 >> pos2 >> k;
				superPowerExtremZystem.move(pos1,pos2,k);
				break;
			case 'F':
				break;
			default:
				cout << "OpÃ§Ã£o invÃ¡lida" << endl;
		}
	} while (opcao != 'F');

	return 0;
}
