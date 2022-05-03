#include <iostream>
#include <string>

using namespace std;

int funcaoHash(string c) {
	int tam = c.size();
	return tam % 23;
}

class noh {
	friend class tabelaHash;
	private:
		string chave;
		string valor;
		noh* proximo = NULL;
		
	public:
		noh(string c, string v) {
			chave = c;
			valor = v;
		}
};

class tabelaHash {
	private:
		noh** elementos;
		int capacidade;
		
	public:
		tabelaHash(int cap = 23);
		~tabelaHash();
		void insere(string palavra, string significado);
		void recupera(string c);
};

tabelaHash::tabelaHash(int cap) {
	elementos = new noh*[cap];
	capacidade = cap;
	for (int i = 0; i < cap; i++){
		elementos[i] = NULL;
	}
}

tabelaHash::~tabelaHash() {
	for (int i = 0; i < capacidade; i++) {
		noh* atual = elementos[i];
		while (atual != NULL){
			noh* aux = atual;
			atual = atual-> proximo;
			delete aux;
		}
	}
	delete[] elementos;
}

void tabelaHash::insere(string c, string v) {
	int posicao = funcaoHash(c);
	
	if(elementos[posicao] == NULL) {
		noh* novo = new noh(c, v);
		elementos[posicao] = novo;
	} else {
		if(elementos[posicao]->proximo == NULL) {
			noh* novo = new noh(c,v);
			noh* atual = elementos[posicao];
			novo->proximo = atual;
			elementos[posicao] = novo;
			atual->proximo = NULL;
		} else {
			noh* novo = new noh(c,v);
			noh* atual = elementos[posicao];
			novo->proximo = atual;
			elementos[posicao] = novo;
		}
	}
}

void tabelaHash::recupera(string c) {
	int posicao = funcaoHash(c);
	string retorna = "NULL";
	bool encontrou = false;
	
	if(elementos[posicao] != NULL) {
		if(elementos[posicao]->chave == c) {
			retorna = elementos[posicao]->valor;
		} else {
			noh* atual = elementos[posicao];
			while(encontrou == false and atual != NULL) {
				if(atual->chave == c) {
					retorna = atual->valor;
					encontrou = true;
				}
				atual = atual->proximo;
			}
		}
	}
	cout << "[" << c << "]" << " => " << retorna << endl;
}

int main() {
	tabelaHash th(23);
	int quant;
	string palavra;
	string significado;
	cin >> quant;
	cin.ignore();
	
	for (int i = 0; i < quant; i++) {
		cin >> palavra;
		getline(cin, significado);
		th.insere(palavra, significado);
	}
	
	string procura;
	
	while(procura != "-1") {
		cin >> procura;
		if(procura != "-1") {
			th.recupera(procura);
		}
	}
	
	return 0;
}
