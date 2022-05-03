/* avl - Árvore Binária de Busca com Balanceamento AVL
 *
 * by Joukim, Outubro de 2020 - Estrutura de Dados (GCC216)
 * Caracteristicas de implementação outubro de 2020:
 * -> tratamento de exceção
 * -> usa dado na forma chave/valor
 * -> uso de métodos recursivos
 * -> sem duplo encadeamento
 * -> invés de transplanta, usa removeMenor
 */

#include <iostream>
#include <stdexcept>
#include <cstring>

using namespace std;

struct dado {
	unsigned chave;
	string nomeAnimal;
	string especie;
	string raca;
};

ostream& operator<<(ostream& saida, const dado& e) {
	saida << "(" << e.chave << "," << e.nomeAnimal << "," << e.especie << "," << e.raca << ")";
	return saida;
}

istream& operator>>(istream& entrada, dado& e) {
	entrada >> e.chave >> e.nomeAnimal >> e.especie >> e.raca;
	return entrada;
}

typedef unsigned tipoChave; // tipo da chave usada na comparação

class noh {
	friend class avl;
	private:
		dado elemento;
		noh* esq;
		noh* dir;
		unsigned altura;
		int informarAltura(noh* umNoh);// criei essas funcaoes para facilitar no codigo pois sem elas nao tinha a minima ideia de como faz a questao , espero que nao se importem
		void atualizarAltura();
	public:
		noh(const dado& umDado):
			elemento(umDado),  esq(NULL), dir(NULL), altura(1) { }
		~noh() { }
		int fatorBanceamento();
};

int noh::fatorBanceamento() {
	return informarAltura(esq) - informarAltura(dir);
}

void noh :: atualizarAltura(){
	int alturaEsq = 0, alturaDir = 0;
	alturaEsq = informarAltura(esq);
	alturaDir = informarAltura(dir);
	altura = 1 + max(alturaEsq , alturaDir);
}

int noh :: informarAltura(noh* umNoh){
	if(umNoh == nullptr){
		return 0;
	}else{
		return umNoh->altura;
	}
}

class avl {
	friend ostream& operator<<(ostream& output, avl& arvore);
	private:
		noh* raiz;
		// percorrimento em ordem da árvore
		void percorreEmOrdemAux(noh* atual, int nivel);
		// funções auxiliares para remoção
		noh* encontraMenor(noh* raizSub);
		noh* removeMenor(noh* raizSub);
		// funções auxiliares para inserção e remoção usando método recursivo
		// retorna o nó para ajustar o pai ou o raiz
		noh* insereAux(noh* umNoh, const dado& umDado , int& cont);
		noh* removeAux(noh* umNoh, tipoChave chave);
		// métodos para manutenção do balanceamento
		noh* rotacaoEsquerda(noh* umNoh);
		noh* rotacaoDireita(noh* umNoh);
		// achei mais facil de trabalhar com as rotacoes direita e esquerda e a esquerda direita , espero que nao se importem
		noh* rotacaoDireitaEsquerda(noh* umNoh);
		noh* rotacaoEsquerdaDireita(noh* umNoh);
		noh* arrumaBalanceamento(noh* umNoh);
		// busca, método iterativo
		noh* buscaAux(tipoChave chave , int& cont);
		// função auxiliar do destrutor, usa percorrimento pós-ordem
		void destruirRecursivamente(noh* umNoh);
		void imprimirDir(const std::string& prefixo, const noh* meuNoh);
		void imprimirEsq(const std::string& prefixo, const noh* meuNoh, bool temIrmao);
		int levantamentoAux(noh* umNoh, string especie, string  raca);
		noh* minimo(noh* atual);
	public:
		avl() { raiz = NULL; }
		~avl();
		void imprimir();
		// inserção e remoção são recursivos
		void insere(const dado& umDado);
		void remove(tipoChave chave);
		// inserção e remoção, métodos recursivos
		// busca retorna uma cópia do objeto armazenado
		dado busca(tipoChave chave);
		// efetua levantamento de quantos animais de uma espécie e raça
		int levantamento(string especie, string raca);
		// criei esta funcao para facilitar achar o nivel da arvore 
		int Nivel(tipoChave chave);
};

// destrutor
avl::~avl() {
	destruirRecursivamente(raiz);
}

// destrutor é recursivo, fazendo percorrimento pós-ordem
void avl::destruirRecursivamente(noh* umNoh) {
	if(umNoh != nullptr){
		destruirRecursivamente(umNoh->esq);
		destruirRecursivamente(umNoh->dir);
		delete umNoh;
	}
}

void avl::insere(const dado& umDado) {
	int cont = 0;
	raiz = insereAux(raiz, umDado , cont);
	cout << "Nohs acessados: " << cont << endl;
}

// inserção recursiva, devolve nó para atribuição de pai ou raiz
noh* avl::insereAux(noh* umNoh, const dado& umDado , int &cont) {
	if(umNoh == nullptr){
		noh* novoNoh = new noh(umDado);
		return novoNoh;
	}else{
		if(umDado.chave < umNoh->elemento.chave){
			cont++;
			umNoh->esq = insereAux(umNoh->esq , umDado , cont);
		}else{
			cont++;
			umNoh->dir = insereAux(umNoh->dir , umDado , cont);
		}
	}
	return arrumaBalanceamento(umNoh);
}

// checa e arruma, se necessário, o balanceamento em umNoh,
// fazendo as rotações e ajustes necessários
noh* avl::arrumaBalanceamento(noh* umNoh) {
	int cont = 0;
	
	if(umNoh == nullptr){
		return umNoh;
	}
	umNoh->atualizarAltura();
	
	int fatorB = umNoh->fatorBanceamento();
	
	if((fatorB >= -1) and (fatorB <= 1)){
		return umNoh;
	}
	if((fatorB > 1) and (umNoh->esq->fatorBanceamento() >= 0)){
		cont++;
		cout << "Rotacao Dir: " << cont << endl;
		return rotacaoDireita(umNoh);
	}
	if((fatorB < -1) and (umNoh->dir->fatorBanceamento() <= 0)){
		cont++;
		cout << "Rotacao Esq: " << cont << endl;
		return rotacaoEsquerda(umNoh);
	}
	if((fatorB < -1 ) and (umNoh->dir->fatorBanceamento() > 0)){
		cont += 2;
		cout << "Rotacao EsqDir: " << cont << endl;
		return rotacaoDireitaEsquerda(umNoh);
	}
	if((fatorB > 1) and (umNoh->esq->fatorBanceamento() < 0)){
		//cout << fatorB << " " << umNoh->esq->fatorBanceamento();
		cont += 2;
		cout << "Rotacao DirEsq: " << cont << endl;
		return rotacaoEsquerdaDireita(umNoh);
	}
	
	return umNoh;
}


// rotação à esquerda na subárvore com raiz em umNoh
// retorna o novo pai da subárvore
noh* avl::rotacaoEsquerda(noh* umNoh) {
	noh* aux = umNoh->dir;
	umNoh->dir = aux->esq;
	aux->esq = umNoh;
	umNoh->atualizarAltura();
	aux->atualizarAltura();
	return aux;
	// 2 nohs acessados
}


// rotação à direita na subárvore com raiz em umNoh
// retorna o novo pai da subárvore
noh* avl::rotacaoDireita(noh* umNoh) {
	noh* aux = umNoh->esq;
	umNoh->esq = aux->dir;
	aux->dir = umNoh;
	umNoh->atualizarAltura();
	aux->atualizarAltura();
	return aux;
	// 2 nohs acessados
}

noh* avl :: rotacaoEsquerdaDireita(noh* umNoh){
	umNoh->esq = rotacaoEsquerda(umNoh->esq);
	return rotacaoDireita(umNoh);
	//4 nohs acessados
}

noh* avl :: rotacaoDireitaEsquerda(noh* umNoh){
	umNoh->dir = rotacaoDireita(umNoh->dir);
	return rotacaoEsquerda(umNoh);
	// 4 nohs acessos
}

// método de busca auxiliar (retorna o nó), iterativo
noh* avl::buscaAux(tipoChave chave , int& cont) {
	noh* atual = raiz;
	
	while(atual != nullptr){
		if(atual->elemento.chave == chave){
			return atual;
		}else if(atual->elemento.chave < chave){
			cont++;
			atual = atual->dir;
		}else{
			cont++;
			atual = atual->esq;
		}
	}
	
	return atual;
}

noh* avl :: minimo(noh* atual){
	while(atual->esq != nullptr){
		atual = atual->esq;
	}
	return atual;
}

// busca elemento com uma dada chave na árvore e retorna o registro completo
dado avl::busca(tipoChave chave) {
	int cont = 0;
	noh* resultado = buscaAux(chave , cont);
	cout << "Nos acessados do elemento buscado : " << cont << endl;
	if (resultado != NULL){
		return resultado->elemento;
	}else{
		throw runtime_error("Erro na busca: elemento não encontrado!");
	}
}

// nó mínimo (sucessor) de subárvore com raiz em raizSub (folha mais à esquerda)
noh* avl::encontraMenor(noh* raizSub) {
	if(raizSub->esq != nullptr){
		raizSub->esq = encontraMenor(raizSub->esq);
	}
	return raizSub->esq;
}

// procedimento auxiliar para remover o sucessor substituíndo-o pelo
// seu filho à direita
noh* avl::removeMenor(noh* raizSub) {
	if(raizSub->esq == nullptr){
		return raizSub->dir;
	}else{
		raizSub->esq = removeMenor(raizSub->esq);
		return arrumaBalanceamento(raizSub);
	}
}

// remoção recursiva
void avl::remove(tipoChave chave) {
	raiz = removeAux(raiz, chave);
}

noh* avl::removeAux(noh* umNoh, tipoChave chave) {
	
	if(umNoh == nullptr){
		cout << "INVALIDO" << endl;
	}
	
	noh* novaRaiz = umNoh;
	
	if(chave < umNoh->elemento.chave){
		umNoh->esq = removeAux(umNoh->esq , chave);
	}else if(chave > umNoh->elemento.chave){
		umNoh->dir = removeAux(umNoh->dir , chave);
	}else{
		if(umNoh->esq == nullptr){
			novaRaiz = umNoh->dir;
		}else if(umNoh->dir == nullptr){
			novaRaiz = umNoh->esq;
		}else{
			novaRaiz = minimo(umNoh->dir);
			novaRaiz->dir = removeMenor(umNoh->dir);
			novaRaiz->esq = umNoh->esq;
		}
		delete umNoh;
	}
	return arrumaBalanceamento(novaRaiz);
}


int avl :: Nivel(tipoChave chave){
	noh* atual = raiz;
	int cont = 0;
	
	while(atual->elemento.chave != chave){  // nao consegui arrumar o warning nessa parte 
		if(atual->elemento.chave < chave){
			atual = atual->dir;
		}else{
			atual = atual->esq;
		}
		cont++;
	}
	return cont;
}

int avl :: levantamentoAux(noh* umNoh , string especie , string raca){
	if(umNoh == nullptr){
		return 0;
	}
	if(umNoh->elemento.especie == especie and umNoh->elemento.raca == raca){
		return 1 + levantamentoAux(umNoh->esq , especie , raca) + levantamentoAux(umNoh->dir , especie , raca);
	}else{
		return levantamentoAux(umNoh->esq , especie , raca) + levantamentoAux(umNoh->dir , especie , raca);
	}
}

int avl :: levantamento(string especie , string raca){
	noh* aux = raiz;
	return levantamentoAux(aux , especie , raca);
}
// utiliza o nó atual e seu nível na árvore (para facilitar visualização)
void avl::percorreEmOrdemAux(noh* atual, int nivel) {
	if(atual != nullptr){
		percorreEmOrdemAux(atual->esq , nivel);
		cout << atual->elemento.chave << " / " << Nivel(atual->elemento.chave) << " ";
		percorreEmOrdemAux(atual->dir , nivel);
	}
}

ostream& operator<<(ostream& output, avl& arvore) {
	// arvore.percorreEmOrdemAux(arvore.raiz,0);
	arvore.imprimir();
	return output;
}

// imprime formatado seguindo o padrao tree as subarvores direitas de uma avl
void avl::imprimirDir(const std::string& prefixo, const noh* meuNoh)
{
	
	if( meuNoh != NULL )
	{
	cout << prefixo << "|_d__" << "(" << meuNoh->elemento.chave << "," << meuNoh->elemento.nomeAnimal << ")"<<endl;

		// Repassa o prefixo para manter o historico de como deve ser a formatacao e chama no filho direito e esquerdo
		imprimirEsq( prefixo + "      " , meuNoh->esq , meuNoh->dir!=NULL );
		imprimirDir( prefixo + "      " , meuNoh->dir );
	}
}

// imprime formatado seguindo o padrao tree as subarvores direitas de uma avl
void avl::imprimirEsq(const std::string& prefixo, const noh* meuNoh, bool temIrmao)
{
	if( meuNoh != NULL )
	{
		cout << prefixo ;
		// A impressao da arvore esquerda depende da indicacao se existe o irmao a direita
		if (temIrmao)
			cout << "|_e__";
		else
			cout << "|_e__" ;

		cout << "(" << meuNoh->elemento.chave << "," << meuNoh->elemento.nomeAnimal << ")" <<endl;

		// Repassa o prefixo para manter o historico de como deve ser a formatacao e chama no filho direito e esquerdo
		imprimirEsq( prefixo + "|     " , meuNoh->esq, meuNoh->dir==NULL );
		imprimirDir( prefixo + "|     " , meuNoh->dir );
	}
}

// imprime formatado seguindo o padrao tree uma avl
void avl::imprimir()
{
	if(raiz != NULL){
		cout << "(" << raiz->elemento.chave << "," << raiz->elemento.nomeAnimal << ")" << endl;
		// apos imprimir a raiz, chama os respectivos metodos de impressao nas subarvore esquerda e direita
		// a chamada para a impressao da subarvore esquerda depende da existencia da subarvore direita
		imprimirEsq(" ", raiz->esq, raiz->dir == NULL);
		imprimirDir(" ", raiz->dir);
	}
	else
		cout << "*arvore vazia*" << endl;
}

int main() {
	avl arvore;
	tipoChave chave;
	dado umDado;
	string especie;
	string raca;
	int quantidade;
	char operacao;

	do {
		try {
			cin >> operacao;
			switch (operacao) {
				case 'i': // Inserir recursivamente
					// objeto recebe chave, nome do animal, espécie e raça
					cin >> umDado;
					arvore.insere(umDado);
					break;
				case 'r': // Remover recursivamente
					cin >> chave;
					arvore.remove(chave);
					break;
				case 'b': // Buscar
					cin >> chave; // ler a chave
					umDado = arvore.busca(chave); // escrever os dados do animal
					cout << "Elemento buscado: " << umDado << endl;
					break;
				case 'l': // Levantamento por espécie e raça
					cin >> especie >> raca; // ler os dados para levantamento
					quantidade = arvore.levantamento(especie, raca);
					cout << "Levantamento de animais: " << quantidade << endl;
					break;
				case 'e': // Escrever tudo (em ordem)
					cout << arvore;
					break;
				case 'f': // Finalizar execução
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
