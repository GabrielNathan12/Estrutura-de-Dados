#include <iostream>
#include <string>
#include <exception>

typedef int TValor;

class InfoHash {
	friend class Hash;
	InfoHash();
	InfoHash(const std::string& chave, const TValor& valor);
	std::string mChave;
	TValor mValor;
};

class Hash {
	public:
		Hash(unsigned capacidade = 50);
		~Hash();
		void EscreverEstrutura(std::ostream& saida) const;
		void Inserir(const std::string& chave, const TValor& valor);
		void Remover(const std::string& chave);
		TValor Valor(const std::string& chave) const;
	
	protected:
		unsigned Buscar(const std::string& chave) const;
		void Inserir(InfoHash* ptPar);
		unsigned Posicao(const std::string& chave) const;
		void Redimensionar(unsigned novaCapacidade);
		InfoHash** mVetPtDados;
		InfoHash* REMOVIDO;
		unsigned mCapacidade;
		unsigned mTamanho;
};

using namespace std;

InfoHash::InfoHash()
	: mChave(), mValor() {
}

InfoHash::InfoHash(const std::string& chave, const TValor& valor)
	: mChave(chave), mValor(valor) {
}

Hash::Hash(unsigned capacidade)
	: mVetPtDados(new InfoHash*[capacidade]), REMOVIDO(new InfoHash()), mCapacidade(capacidade), mTamanho(0) {
	for (unsigned i = 0; i < capacidade; i++){
		mVetPtDados[i] = NULL;
	}
}

Hash::~Hash() {
	for (unsigned i = 0; i < mTamanho; i++){
		mVetPtDados[i] = NULL;
	}
	delete REMOVIDO;
	delete[] mVetPtDados;
}

unsigned Hash::Buscar(const std::string& chave) const {
	unsigned posicao = Posicao(chave);
	unsigned cont = 0;
	bool achou = false;
	
	while((cont < mCapacidade) and (!achou)){
		if((mVetPtDados[posicao] != NULL) and (mVetPtDados[posicao] != REMOVIDO)){
			if (mVetPtDados[posicao]->mChave == chave){
				achou = true;
			} else {
				if (posicao + 1 == mCapacidade){
					posicao = 0;
				} else {
					posicao++;
				}
				cont++;
			}
		} else{
			if (posicao + 1 == mCapacidade){
				posicao = 0;
			} else{
				posicao++;
			}
			cont++;
		}
	}
	if ((mVetPtDados[posicao] != NULL) and (mVetPtDados[posicao]->mChave == chave)){
		return posicao;
	} else{
		throw invalid_argument("Nao existe essa chave");
	}
}

void Hash::EscreverEstrutura(std::ostream& saida) const {
	for (unsigned i = 0; i < mCapacidade; ++i) {
		saida << '[' << i;
		if (mVetPtDados[i] != NULL) {
			if (mVetPtDados[i] == REMOVIDO){
				saida << "/removido";
			} else {
				saida << '/' << mVetPtDados[i]->mChave << '/' << mVetPtDados[i]->mValor;
			}
		}
		saida << "] ";
	}
}

void Hash::Inserir(const string& chave, const TValor& valor) {
	if (mTamanho < mCapacidade){
		InfoHash* novoElemento = new InfoHash(chave,valor);
		int posicao = Posicao(chave);
		while (mVetPtDados[posicao] != NULL and mVetPtDados[posicao]->mChave != chave and mVetPtDados[posicao] != REMOVIDO){
			posicao++;
			posicao %= mCapacidade;
		}
		if(mVetPtDados[posicao] == NULL or mVetPtDados[posicao] == REMOVIDO) {
			mTamanho++; 
			mVetPtDados[posicao] = novoElemento; 
		} else {
			throw invalid_argument("chave repetida");
		}
	} else {
		Redimensionar(mCapacidade+5);
		Inserir(chave,valor);
	}
}

void Hash::Inserir(InfoHash* ptPar) {
	unsigned posicao = Posicao(ptPar->mChave);
	unsigned cont = 0;
	
	while (mVetPtDados[(posicao + cont)% mCapacidade] != NULL){
		cont++;
	}
	mVetPtDados[(posicao + cont) % mCapacidade] = ptPar;
}

unsigned Hash::Posicao(const string& chave) const {
	unsigned posicao = 1;
	unsigned tam = unsigned(chave.length());
	
	for (unsigned i = 0; i < tam; i++){
		posicao = (7 * posicao) + chave[i];
	}
	return posicao % mCapacidade;
}

void Hash::Redimensionar(unsigned novaCapacidade) {
	if (novaCapacidade < mTamanho){
		throw invalid_argument("Invalida");
	} else {
		InfoHash** auxVet  = mVetPtDados;
		unsigned old = mCapacidade;
		mCapacidade = novaCapacidade;
		mVetPtDados = new InfoHash*[novaCapacidade];
		
		for (unsigned i = 0; i < novaCapacidade; i++){
			mVetPtDados[i] = NULL;
		}
		InfoHash* aux;
		
		for (unsigned i = 0; i < old; i++){
			aux = auxVet[i];
			if (aux != NULL){
				Inserir(aux);
			}
			auxVet[i] = NULL;
		}
		delete[] auxVet;
	}
}

void Hash::Remover(const std::string& chave) {
	unsigned posicao = Buscar(chave);
	delete mVetPtDados[posicao];
	
	mVetPtDados[posicao] = REMOVIDO;
	mTamanho--;
}

TValor Hash::Valor(const std::string& chave) const {
	unsigned pos = Buscar(chave);
	return mVetPtDados[pos]->mValor;
}

int main() {
	unsigned capacidade;
	cin >> capacidade;
	Hash tabela(capacidade);
	char operacao;
	string chave;
	TValor valor;
	
	do {
		try {
			cin >> operacao;
			switch (operacao) {
				case 'i':{
					cin >> chave >> valor;
					tabela.Inserir(chave, valor);
					break;
				}
				case 'r':{
					cin >> chave;
					tabela.Remover(chave);
					break;
				}
				case 'c':{
					cin >> chave;
					valor = tabela.Valor(chave);
					cout << valor << endl;
					break;
				}
				case 'd':{
					tabela.EscreverEstrutura(cout);
					cout << endl;
					break;
				}
				case 'f':{
					break;
				}
				default:{
					cerr << "operação inválida\n";
				}
			}
		}
		catch (exception& e) {
			cout << "ERRO" << endl;
		}
	} while (operacao != 'f');
	
	return 0;
}



//ldssh
