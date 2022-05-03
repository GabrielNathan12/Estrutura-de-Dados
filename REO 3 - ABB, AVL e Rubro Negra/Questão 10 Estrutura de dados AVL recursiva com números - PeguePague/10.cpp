/* avl - Ãrvore BinÃ¡ria de Busca com Balanceamento AVL
 *
 * by Joukim, Outubro de 2019 - Estrutura de Dados (GCC216)
 * Caracteristicas de implementaÃ§Ã£o outubro de 2019:
 * -> tratamento de exceÃ§Ã£o
 * -> usa dado na forma chave/valor
 * -> uso de mÃ©todos recursivos
 * -> sem duplo encadeamento
 * -> invÃ©s de transplanta, usa removeMenor
 */

#include <iostream>
#include <stdexcept>
#include <queue>

typedef unsigned tipoChave;

using namespace std;

struct dado {
	unsigned chave;
	string nomeProduto;
	string marca;
	int quantidadeDisponivel;
};

ostream& operator<<(ostream& saida, const dado& e) {
	saida << "(" << e.chave << "," << e.nomeProduto << "," << e.marca << "," << e.quantidadeDisponivel << ")";
	return saida;
}

istream& operator>>(istream& entrada, dado& e) {
	entrada >> e.chave >> e.nomeProduto >> e.marca >> e.quantidadeDisponivel;
	return entrada;
}

class noh {
	friend class avl;
	private:
		dado elemento;
		noh* esq;
		noh* dir;
		noh* pai;
		unsigned int altura;
		void atualizaAltura();
		void trasplanta(noh* umNoh);
		void trocarFilho(noh* antes , noh* novo);
		int inforAltura(noh* umNoh);
	public:
		noh(const dado& umDado):
			elemento(umDado), esq(NULL), dir(NULL), altura(1) { }
		~noh() { }
		int fatorBalanceamento();

};

int noh::fatorBalanceamento() { // feito
	int altEsquerda;
	int altDireita;
	
	if(esq != NULL){
		altEsquerda = esq->altura;
	}else{
		altEsquerda = 0;
	}
	if(dir != NULL){
		altDireita = dir->altura;
	}else{
		altDireita = 0;
	}
	int fatorBalanceamento = altEsquerda - altDireita;
	
	return fatorBalanceamento;
}

int  noh :: inforAltura(noh* umNoh){ // ok
	if(umNoh == NULL){
		return 0;
	}else{
		return umNoh->altura;
	}
}

void noh :: atualizaAltura(){
	int altEsq = 0 , altDir = 0;
	altEsq = inforAltura(esq);
	altDir = inforAltura(dir);
	altura = 1 + max(altEsq , altDir);
}



class avl {
	friend ostream& operator<<(ostream& output, avl& arvore);
	private:
		noh* raiz;
		// percorrimento em ordem da Ã¡rvore
		void percorreEmOrdemAux(noh* atual, int nivel);
		// funÃ§Ãµes auxiliares para remoÃ§Ã£o
		noh* encontraMenor(noh* umNoh);
		noh* removeMenor(noh* raiz);
		// funÃ§Ãµes auxiliares para inserÃ§Ã£o e remoÃ§Ã£o usando mÃ©todo recursivo
		// retorna o nÃ³ para ajustar o pai ou o raiz
		noh* insereAux(noh* umNoh, const dado& umDado);
		noh* removeAux(noh* umNoh, tipoChave chave);
		// mÃ©todos para manutenÃ§Ã£o do balanceamento
		noh* rotacaoEsquerda(noh* umNoh);
		noh* rotacaoDireita(noh* umNoh);
		noh* rotacaoEsqeDir(noh* umNoh);
		noh* rotacaoDireEsq(noh* umNoh);
		noh* arrumaBalanceamento(noh* umNoh);
		// busca, mÃ©todo iterativo
		noh* buscaAux(tipoChave chave);
		// funÃ§Ã£o auxiliar do destrutor, usa percorrimento pÃ³s-ordem
		void destruirRecursivamente(noh* umNoh);
		void imprimirDir(const std::string& prefixo, const noh* meuNoh);
		void imprimirEsq(const std::string& prefixo, const noh* meuNoh, bool temIrmao);
		int levantamentoAux(string marca ,int nivel, int& cont , noh* atual);
		unsigned altura;
	public:
		avl() { raiz = NULL; }
		~avl();
		void imprimir();
		// inserÃ§Ã£o e remoÃ§Ã£o sÃ£o recursivos
		void insere(const dado& umDado);
		void remove(tipoChave chave);
		// inserÃ§Ã£o e remoÃ§Ã£o, mÃ©todos recursivos
		// busca retorna uma cÃ³pia do objeto armazenado
		dado busca(tipoChave chave);
		// efetua levantamento de quantos produtos existem de uma dada marca
		int levantamento(string marca , int nivel);
		unsigned Nivel(unsigned chave);
};

// destrutor
avl::~avl() { // ok
	destruirRecursivamente(raiz);
}

// destrutor Ã© recursivo, fazendo percorrimento pÃ³s-ordem
void avl::destruirRecursivamente(noh* umNoh) {
	if(umNoh->esq != NULL){
		delete umNoh->esq;
	}
	if(umNoh->dir != NULL){
		delete umNoh->dir;
	}
}

void avl::insere(const dado& umDado) {
	raiz = insereAux(raiz, umDado);
}


// inserÃ§Ã£o recursiva, devolve nÃ³ para atribuiÃ§Ã£o de pai ou raiz
noh* avl::insereAux(noh* umNoh, const dado& umDado) {
	
	if(umNoh == NULL){
		noh* novoNoh = new noh(umDado);
			return novoNoh;
	}else{
		if(umDado.chave < umNoh->elemento.chave){
			umNoh->esq = insereAux(umNoh->esq,umDado);
		}else{
			umNoh->dir = insereAux(umNoh->dir,umDado);
		}
	}
	return arrumaBalanceamento(umNoh);
}

// checa e arruma, se necessÃ¡rio, o balanceamento em umNoh,
// fazendo as rotaÃ§Ãµes e ajustes necessÃ¡rios
noh* avl::arrumaBalanceamento(noh* umNoh) { // ok
	if (umNoh == NULL){
		return umNoh;
	}
	umNoh->atualizaAltura();
	int fatorB = umNoh->fatorBalanceamento();
	
	if((fatorB >= -1) and (fatorB <= 1)){
		return umNoh;
	}
	if((fatorB > 1) and (umNoh->esq->fatorBalanceamento() >= 0)){
		return rotacaoDireita(umNoh);
	}
	if((fatorB > 1) and (umNoh->esq->fatorBalanceamento() < 0)){
		umNoh->esq = rotacaoEsquerda(umNoh->esq);
		return rotacaoEsqeDir(umNoh);
	}
	if((fatorB < -1) and (umNoh->dir->fatorBalanceamento() <= 0)){
		return rotacaoEsquerda(umNoh);
	}
	if((fatorB < -1) and (umNoh->dir->fatorBalanceamento() > 0)){
		umNoh->dir = rotacaoDireita(umNoh->dir);
		return rotacaoDireEsq(umNoh);
	}
	return umNoh;
}

// rotaÃ§Ã£o Ã  esquerda na subÃ¡rvore com raiz em umNoh
// retorna o novo pai da subÃ¡rvore

unsigned avl :: Nivel(unsigned chave){
	noh* atual = raiz;
	int cont = 0;
	
	while(atual->elemento.chave != chave){
		if(atual->elemento.chave < chave){
			atual = atual->dir;
		}else{
			atual = atual->esq;
		}
		cont++;
	}
	return cont;
}

noh* avl::rotacaoEsquerda(noh* umNoh){ //ok
	noh* aux = umNoh->dir;
	umNoh->dir = aux->esq;
	
	aux->esq = umNoh;
	umNoh->atualizaAltura();
	aux->atualizaAltura();
	
	return aux;
}


// rotaÃ§Ã£o Ã  direita na subÃ¡rvore com raiz em umNoh
// retorna o novo pai da subÃ¡rvore
noh* avl::rotacaoDireita(noh* umNoh) {
	noh* aux = umNoh->esq;
	umNoh->esq = aux->dir;
	
	aux->dir = umNoh;
	umNoh->atualizaAltura();
	aux->atualizaAltura();
	return aux;
}

noh* avl :: rotacaoEsqeDir(noh* umNoh){
	umNoh->dir = rotacaoEsquerda(umNoh->esq);
	return rotacaoDireita(umNoh);
}

noh* avl :: rotacaoDireEsq(noh* umNoh){
	umNoh->dir = rotacaoDireita(umNoh->dir);
	return rotacaoEsquerda(umNoh);
}

// mÃ©todo de busca auxiliar (retorna o nÃ³), iterativo
noh* avl::buscaAux(tipoChave chave1) { // ok
	noh* atual = raiz;
	
	while(atual != NULL){
		if(atual->elemento.chave == chave1){
			return atual;
		}else if(atual->elemento.chave < chave1){
			atual = atual->dir;
		}else{
			atual = atual->esq;
		}
	}
	return atual;
}

// busca elemento com uma dada chave na Ã¡rvore e retorna o registro completo
dado avl::busca(tipoChave chave) { // ok
	noh* resultado = buscaAux(chave);
	if (resultado != NULL)
		return resultado->elemento;
	else
		throw runtime_error("Erro na busca: elemento nÃ£o encontrado!");
}

// nÃ³ mÃ­nimo (sucessor) de subÃ¡rvore com raiz em raizSub (folha mais Ã  esquerda)
noh* avl::encontraMenor(noh* umNoh) { // ok
	while(umNoh->esq != NULL){
		umNoh = umNoh->esq;
	}
	return umNoh;
}

// procedimento auxiliar para remover o sucessor substituÃ­ndo-o pelo
// seu filho Ã  direita
noh* avl :: removeMenor(noh* raiz) {
	if(raiz->esq == NULL){
		return raiz->dir;
	}else{
		raiz->esq = removeMenor(raiz->esq);
		return arrumaBalanceamento(raiz);
	}
}

void avl::remove(tipoChave chave) {
	raiz = removeAux(raiz, chave);
}

noh* avl::removeAux(noh* umNoh, tipoChave chave) {
	if(umNoh == NULL){
		throw runtime_error("Erro");
	}
	
	noh* aux = umNoh;
	
	if(chave < umNoh->elemento.chave){
		umNoh->esq = removeAux(umNoh->esq , chave);
	}else if(chave > umNoh->elemento.chave){
		umNoh->dir = removeAux(umNoh->dir , chave);
	}else{
		if(umNoh->esq == NULL){
			aux = umNoh->dir;
		}else if(umNoh->dir == NULL){
			aux = umNoh->esq;
		}else{
			aux = encontraMenor(umNoh->dir);
			aux->dir = removeMenor(umNoh->dir);
			aux->esq = umNoh->esq;
		}
		delete umNoh;
	}
	return arrumaBalanceamento(aux);
}


// utiliza o nÃ³ atual e seu nÃ­vel na Ã¡rvore (para facilitar visualizaÃ§Ã£o)
void avl::percorreEmOrdemAux(noh* atual, int nivel) { // ok
	if(atual != NULL){
		percorreEmOrdemAux(atual->esq,nivel);
		cout << atual->elemento.chave << "/" << " ";
		percorreEmOrdemAux(atual->dir , nivel);
	}
}

ostream& operator<<(ostream& output, avl& arvore) {
	arvore.imprimir();
	return output;
}


// imprime formatado seguindo o padrao tree as subarvores direitas de uma avl
void avl::imprimirDir(const std::string& prefixo, const noh* meuNoh)
{
	if( meuNoh != nullptr )
	{
		std::cout << prefixo
				  << "â””dâ”€"
				  << "(" << meuNoh->elemento.chave << "," << meuNoh->elemento.nomeProduto << ")"
				  << std::endl;

		// Repassa o prefixo para manter o historico de como deve ser a formatacao e chama no filho direito e esquerdo
		imprimirEsq( prefixo + "    " , meuNoh->esq , meuNoh->dir==nullptr );
		imprimirDir( prefixo + "    " , meuNoh->dir );
	}
}

// imprime formatado seguindo o padrao tree as subarvores direitas de uma avl
void avl::imprimirEsq(const std::string& prefixo, const noh* meuNoh, bool temIrmao)
{
	if( meuNoh != nullptr )
	{
		std::cout << prefixo ;

		// A impressao da arvore esquerda depende da indicacao se existe o irmao a direita
		if (temIrmao)
			std::cout << "â””eâ”€" ;
		else
			std::cout << "â”œeâ”€";

		std::cout << "(" << meuNoh->elemento.chave << "," << meuNoh->elemento.nomeProduto << ")"
				  << std::endl;

		// Repassa o prefixo para manter o historico de como deve ser a formatacao e chama no filho direito e esquerdo
		imprimirEsq( prefixo + "â”‚   " , meuNoh->esq, meuNoh->dir==nullptr );
		imprimirDir( prefixo + "â”‚   " , meuNoh->dir );
	}
}

int avl :: levantamento(string marca ,int n){
	int cont = 0;
	return levantamentoAux(marca , n , cont , raiz);
}

int avl :: levantamentoAux(string marca , int n , int& cont , noh* atual){
	if(atual == NULL){
		return 0;
	}
	if(atual->elemento.marca == marca and atual->elemento.quantidadeDisponivel == n){
		return 1 + levantamentoAux(marca , n , cont , atual->esq) + levantamentoAux(marca , n , cont,atual->dir);
	}else{
		return levantamentoAux(marca , n , cont , atual->esq) + levantamentoAux(marca , n , cont ,  atual->dir);
	}
}

// imprime formatado seguindo o padrao tree uma avl
void avl::imprimir()
{
	if( this->raiz != nullptr )
	{
		std::cout << "(" << this->raiz->elemento.chave << "," << this->raiz->elemento.nomeProduto << ")" << std::endl;
		// apos imprimir a raiz, chama os respectivos metodos de impressao nas subarvore esquerda e direita
		// a chamada para a impressao da subarvore esquerda depende da existencia da subarvore direita
		imprimirEsq( " " , this->raiz->esq, this->raiz->dir==nullptr );
		imprimirDir( " " , this->raiz->dir );
	} else
		std::cout << "*arvore vazia*" << std::endl;
}

int main() {
	avl arvore;
	tipoChave chave;
	dado umDado;
	string marca;
	int quantidade;

	char operacao;

	do {
		try {
			cin >> operacao;
			switch (operacao) {
				case 'i': // Inserir recursivamente
					// objeto recebe chave, nome do produto, marca, quantidade disponÃ­vel
					cin >> umDado;
					arvore.insere(umDado);
					break;
				case 'r': // Remover recursivamente
					cin >> chave;
					arvore.remove(chave);
					break;
				case 'b': // Buscar
					cin >> chave; // ler a chave
					umDado = arvore.busca(chave); // escrever dados do produto
					cout << "Elemento buscado: " << umDado << endl;
					break;
				case 'l': // Levantamento por marca
					cin >> marca; // ler a marca
					quantidade = arvore.levantamento(marca , quantidade);
					cout << "Levantamento da marca " << marca << ": " << quantidade << endl;
					break;
				case 'e': // Escrever tudo (em ordem)
					cout << arvore;
					break;
				case 'f': // Finalizar execuÃ§Ã£o
					break;
				default:
					cout << "Comando invalido!\n";
			}
		} catch (runtime_error& e) {
			cout << e.what() << endl;
		}
	} while (operacao != 'f');
	
	return 0;
}
