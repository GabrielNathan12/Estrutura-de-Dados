/* avl - Ãrvore BinÃ¡ria de Busca com Balanceamento AVL
 *
 * by Joukim, Outubro de 2019 - Estrutura de Dados (GCC216)
 * Caracteristicas de implementaÃ§Ã£o outubro de 2019:
 * -> tratamento de exceÃ§Ã£o
 * -> usa pokemon na forma id/valor
 * -> uso de mÃ©todos recursivos
 * -> sem duplo encadeamento
 * -> invÃ©s de transplanta, usa removeMenor
 * Changed by Perninha, Julho de 2020 - Aluno de Estrutura de Dados (GCC216)
 *  Todas caracteristicas de implementaÃ§Ã£o Julho de 2020 estao marcadas como "//Implementado"
 */

#include <iostream>
#include <stdexcept>
#include <cstring>

using namespace std;

typedef unsigned tipoChave;

struct pokemon {
	unsigned id;
	string nome;
	string tipo;
	int nivel;
};

typedef pokemon dado;

ostream& operator<<(ostream& saida, const pokemon& e) {
	saida << "(" << e.id << "," << e.nome << "," << e.tipo << "," << e.nivel << ")";
	return saida;
}

istream& operator>>(istream& entrada, pokemon& e) {
	entrada >> e.id >> e.nome >> e.tipo >> e.nivel;
	return entrada;
}

class Noh {
	friend class avl;
	private:
		pokemon elemento;
		Noh* PtEsq;
		Noh* PtDir;
		unsigned altura;
		int InformarAltura(Noh* UmNoh);
		void AtualizarAltura();
	public:
		Noh(const pokemon& umPokemon):
			elemento(umPokemon),  PtEsq(NULL), PtDir(NULL), altura(1) { }
		~Noh() { }
		int fatorBalanceamento();
};

int Noh::fatorBalanceamento() {
	//Implementado
	return InformarAltura(PtEsq) - InformarAltura(PtDir);
}

void Noh::AtualizarAltura(){
	//Implementado
	int AltArvEsq = 0, AltArvDir = 0;
	AltArvEsq = InformarAltura(PtEsq);
	AltArvDir = InformarAltura(PtDir);
	altura = 1 + max(AltArvEsq, AltArvDir);
}

int Noh::InformarAltura(Noh* UmNoh){
	//Implementado
	if(UmNoh == NULL){
		return 0;
	}
	else{
		return UmNoh->altura;
	}
}

class avl {
	friend ostream& operator<<(ostream& output, avl& arvore);
	private:
		Noh* raiz;
		// percorrimento em ordem da Ã¡rvore
		void percorreEmOrdemAux(Noh* atual, int nivel);
		// funÃ§Ãµes auxiliares para remoÃ§Ã£o
		Noh* removeMenor(Noh* raizSub);
		// funÃ§Ãµes auxiliares para inserÃ§Ã£o e remoÃ§Ã£o usando mÃ©todo recursivo
		// retorna o nÃ³ para ajustar o pai ou o raiz
		Noh* insereAux(Noh* umNoh, const pokemon& umPokemon);
		Noh* removeAux(Noh* umNoh, tipoChave id);
		// mÃ©todos para manutenÃ§Ã£o do balanceamento
		Noh* rotacaoEsquerda(Noh* umNoh); //ok
		Noh* rotacaoDireita(Noh* umNoh);  //ok
		Noh* RotacaoEsqDir(Noh* UmNoh); //ok
		Noh* RotacaoDirEsq(Noh* UmNoh); //ok
		Noh* arrumaBalanceamento(Noh* umNoh); //ok
		// busca, mÃ©todo iterativo
		Noh* buscaAux(tipoChave id);
		// funÃ§Ã£o auxiliar do destrutor, usa percorrimento pÃ³s-ordem
		void destruirRecursivamente(Noh* umNoh);
		void imprimirDir(const std::string& prefixo, const Noh* meuNoh);
		void imprimirEsq(const std::string& prefixo, const Noh* meuNoh, bool temIrmao);
		int levantamentoAux(string tipo, int nivel, int& cont, Noh* Atual);
		Noh* Minimo(Noh* Atual);
	public:
		avl() { raiz = NULL; }
		~avl();
		void imprimir();
		// inserÃ§Ã£o e remoÃ§Ã£o sÃ£o recursivos
		void insere(const pokemon& umPokemon);
		void remove(tipoChave id);
		// inserÃ§Ã£o e remoÃ§Ã£o, mÃ©todos recursivos
		// busca retorna uma cÃ³pia do objeto armazenado
		pokemon busca(tipoChave id);
		// efetua levantamento de quantos pokemons existem de um dado tipo e nÃ­vel
		int levantamento(string tipo, int nivel);
		int Nivel(tipoChave chave);
};

// destrutor
avl::~avl() {
	destruirRecursivamente(raiz);
}

// destrutor Ã© recursivo, fazendo percorrimento pÃ³s-ordem
void avl::destruirRecursivamente(Noh* UmNoh) {
	//Implementado
	if(UmNoh != NULL){
		destruirRecursivamente(UmNoh->PtEsq);
		destruirRecursivamente(UmNoh->PtDir);
		delete UmNoh;
	}
}

void avl::insere(const dado& UmDado) {
	raiz = insereAux(raiz, UmDado);
	
}

// inserÃ§Ã£o recursiva, devolve nÃ³ para atribuiÃ§Ã£o de pai ou raiz
Noh* avl::insereAux(Noh* UmNoh, const dado& UmDado) {
	//Implementado
	if(UmNoh == NULL){
		Noh* NovoNoh = new Noh(UmDado);			// cria um no com valor
		//NovoNoh->AtualizarAltura();
		return NovoNoh;
	}
	else{
		// nao e folha nula, checa a insercao a esquerda ou direita
		if(UmDado.id < UmNoh->elemento.id){
			UmNoh->PtEsq = insereAux(UmNoh->PtEsq, UmDado);
		}
		else{
			UmNoh->PtDir = insereAux(UmNoh->PtDir, UmDado);
		}
	}
	
	return arrumaBalanceamento(UmNoh);
}

// checa e arruma, se necessÃ¡rio, o balanceamento em umNoh,
// fazendo as rotaÃ§Ãµes e ajustes necessÃ¡rios
Noh* avl::arrumaBalanceamento(Noh* UmNoh) {
	//Implementado
	if(UmNoh == NULL){
		return UmNoh;
	}
	// Inicialmente atualiza a altura de UmNoh
	UmNoh->AtualizarAltura();
	// Checa o balanceamento no Noh
	int FatorBal = UmNoh->fatorBalanceamento();
	// Retorna o noh acima da arvore, caso esteja balanceado
	if((FatorBal >= -1) and (FatorBal <= 1)){
		return UmNoh;
	}
	if((FatorBal > 1) and (UmNoh->PtEsq->fatorBalanceamento() >= 0)){
		return rotacaoDireita(UmNoh);
	}
	if((FatorBal > 1) and (UmNoh->PtEsq->fatorBalanceamento() < 0)){
		cout << FatorBal << " " << UmNoh->PtEsq->fatorBalanceamento();
		return RotacaoEsqDir(UmNoh);
	}
	if((FatorBal < -1) and (UmNoh->PtDir->fatorBalanceamento() <= 0)){
		return rotacaoEsquerda(UmNoh);
	}
	if((FatorBal < -1) and (UmNoh->PtDir->fatorBalanceamento() > 0)){
		return RotacaoDirEsq(UmNoh);
	}
	return UmNoh;
}


// rotaÃ§Ã£o Ã  esquerda na subÃ¡rvore com raiz em umNoh
// retorna o novo pai da subÃ¡rvore
Noh* avl::rotacaoEsquerda(Noh* UmNoh) {
	///Implementado
	// Acha o filho a direita
	Noh* Aux = UmNoh->PtDir;
	// Armazena a subarvore esquerda do noh Aux
	// a direita do UmNoh
	UmNoh->PtDir = Aux->PtEsq;
	// Faz UmNoh como filho a esquerda de Aux
	Aux->PtEsq = UmNoh;
	// Atualiza alturas
	UmNoh->AtualizarAltura();
	Aux->AtualizarAltura();
	return Aux;
}


// rotaÃ§Ã£o Ã  direita na subÃ¡rvore com raiz em umNoh
// retorna o novo pai da subÃ¡rvore
Noh* avl::rotacaoDireita(Noh* UmNoh) {
	//Implementado
	// Acha o filho a esquerda
	Noh* Aux = UmNoh->PtEsq;
	// Armazena a subarvore direita do noh Aux
	// a esquerda do UmNoh
	UmNoh->PtEsq = Aux->PtDir;
	Aux->PtDir = UmNoh;
	// Atualiza alturas
	UmNoh->AtualizarAltura();
	Aux->AtualizarAltura();
	return Aux;
}

Noh* avl::RotacaoEsqDir(Noh* UmNoh){
	//Implementado
	UmNoh->PtEsq = rotacaoEsquerda(UmNoh->PtEsq);
	return rotacaoDireita(UmNoh);
}

Noh* avl::RotacaoDirEsq(Noh* UmNoh){
	//Implementado
	UmNoh->PtDir = rotacaoDireita(UmNoh->PtDir);
	return rotacaoEsquerda(UmNoh);
}

int avl::levantamentoAux(string t, int n, int& cont, Noh* Atual){
	//Implementado
	if(Atual == NULL){
		return 0;
	}
	if(Atual->elemento.tipo == t and Atual->elemento.nivel == n){
		return 1 + levantamentoAux(t, n, cont, Atual->PtEsq) + levantamentoAux(t, n, cont, Atual->PtDir);
	}
	else{
		return levantamentoAux(t, n, cont, Atual->PtEsq) + levantamentoAux(t, n, cont, Atual->PtDir);
	}
}

int avl::levantamento(string t, int n){
	//Implementado
	int cont = 0;
	return levantamentoAux(t, n, cont, raiz);
}

// mÃ©todo de busca auxiliar (retorna o nÃ³), iterativo
Noh* avl::buscaAux(tipoChave chave) {
	//Implementado
	Noh* Atual = raiz;
	while(Atual != NULL){
		if(Atual->elemento.id == chave){
			return Atual;
		}
		else if(Atual->elemento.id < chave){
			Atual = Atual->PtDir;
		}else{
			Atual = Atual->PtEsq;
		}
	}
	return Atual;
}

// busca elemento com uma dada chave na Ã¡rvore e retorna o registro completo
dado avl::busca(tipoChave chave) {
	Noh* resultado = buscaAux(chave);
	if (resultado != NULL)
		return resultado->elemento;
	else
		throw runtime_error("Erro na busca: elemento nao encontrado!");
}

// procedimento auxiliar para remover o sucessor substituÃ­ndo-o pelo
// seu filho Ã  direita
Noh* avl::removeMenor(Noh* raizSub) {
	//Implementado
	// procedimento auxiliar para remover o sucessor substituindo-o
	// pelo seu filho à direita
	if(raizSub->PtEsq == NULL){	// encontrou o sucessor
		return raizSub->PtDir;
	}
	else{
		raizSub->PtEsq = removeMenor(raizSub->PtEsq);
		return arrumaBalanceamento(raizSub);
	}
}

Noh* avl::Minimo(Noh* Atual){
	//Implementado
	while(Atual->PtEsq != NULL){
		Atual = Atual->PtEsq;
	}
	return Atual;
}

// remoÃ§Ã£o recursiva
void avl::remove(tipoChave chave) {
	raiz = removeAux(raiz, chave);
}

Noh* avl::removeAux(Noh* UmNoh, tipoChave chave) {
	//Implementado
	if(UmNoh == NULL){
		throw runtime_error("ERRO");
	}
	Noh* NovaRaiz = UmNoh;
	// valor é menor que nó atual, vai para subárvore esquerda
	if(chave < UmNoh->elemento.id){
		UmNoh->PtEsq = removeAux(UmNoh->PtEsq, chave);
	}
	// valor é maior que nó atual, vai para subárvore direita
	else if(chave > UmNoh->elemento.id){
		UmNoh->PtDir = removeAux(UmNoh->PtDir, chave);
	}
	else{
		// nó não tem filhos à esquerda
		if(UmNoh->PtEsq == NULL){
			NovaRaiz = UmNoh->PtDir;
		}
		 // nó não tem filhos à direita
		else if(UmNoh->PtDir == NULL){
			NovaRaiz = UmNoh->PtEsq;
		}
		// nó tem dois filhos
		else{
			// trocando pelo sucessor
			NovaRaiz = Minimo(UmNoh->PtDir);
			// onde antes estava o sucessor fica agora seu filho a direita
			NovaRaiz->PtDir = removeMenor(UmNoh->PtDir);
			// filho à esquerda de umNoh torna-se filho à esquerda de sucessor
			NovaRaiz->PtEsq = UmNoh->PtEsq;
		}
		// ponteiros ajustados, apagamos o nó
		delete UmNoh;
	}
	return arrumaBalanceamento(NovaRaiz);
}

int avl::Nivel(tipoChave chave){
	//Implementado
	Noh* Atual = raiz;
	int cont = 0;
	while(Atual->elemento.id != chave){
		if(Atual->elemento.id < chave){
			Atual = Atual->PtDir;
		}
		else{
			Atual = Atual->PtEsq;
		}
		cont++;
	}
	return cont;
}

// utiliza o nÃ³ atual e seu nÃ­vel na Ã¡rvore (para facilitar visualizaÃ§Ã£o)
void avl::percorreEmOrdemAux(Noh* Atual, int nivel) {
	//Implementado
	if(Atual != NULL){
		percorreEmOrdemAux(Atual->PtEsq, nivel);
		cout<< Atual->elemento.id << "/" << Nivel(Atual->elemento.id) <<" ";
		percorreEmOrdemAux(Atual->PtDir, nivel);
	}
}

ostream& operator<<(ostream& output, avl& arvore) {
	arvore.imprimir();
	return output;
}

// imprime formatado seguindo o padrao tree as subarvores direitas de uma avl
// Mesmo sendo pedido para não alterar, tive de fazer pois não foi possivel visualizar a impressao
void avl::imprimirDir(const std::string& prefixo, const Noh* meuNoh)
{
	if( meuNoh != NULL )
	{
	cout << prefixo << "|_d__" << "(" << meuNoh->elemento.id << "," << meuNoh->elemento.nome << ")"<<endl;

		// Repassa o prefixo para manter o historico de como deve ser a formatacao e chama no filho direito e esquerdo
		imprimirEsq( prefixo + "      " , meuNoh->PtEsq , meuNoh->PtDir!=NULL );
		imprimirDir( prefixo + "      " , meuNoh->PtDir );
	}
}

// imprime formatado seguindo o padrao tree as subarvores direitas de uma avl
// Mesmo sendo pedido para não alterar, tive de fazer pois não foi possivel visualizar a impressao
void avl::imprimirEsq(const std::string& prefixo, const Noh* meuNoh, bool temIrmao)
{
	if( meuNoh != NULL )
	{
		cout << prefixo ;
		// A impressao da arvore esquerda depende da indicacao se existe o irmao a direita
		if (temIrmao)
			cout << "|_e__";
		else
			cout << "|_e__" ;

		cout << "(" << meuNoh->elemento.id << "," << meuNoh->elemento.nome << ")" <<endl;

		// Repassa o prefixo para manter o historico de como deve ser a formatacao e chama no filho direito e esquerdo
		imprimirEsq( prefixo + "|     " , meuNoh->PtEsq, meuNoh->PtDir==NULL );
		imprimirDir( prefixo + "|     " , meuNoh->PtDir );
	}
}

// imprime formatado seguindo o padrao tree uma avl
// Mesmo sendo pedido para não alterar, tive de fazer pois não foi possivel visualizar a impressao
void avl::imprimir(){
	if(raiz != NULL){
		cout << "(" << raiz->elemento.id << "," << raiz->elemento.nome << ")" << endl;
		// apos imprimir a raiz, chama os respectivos metodos de impressao nas subarvore esquerda e direita
		// a chamada para a impressao da subarvore esquerda depende da existencia da subarvore direita
		imprimirEsq(" ", raiz->PtEsq, raiz->PtDir == NULL);
		imprimirDir(" ", raiz->PtDir);
	}
	else
		cout << "*arvore vazia*" << endl;
}

int main() {
	avl arvore;
	tipoChave id;
	pokemon umPokemon;
	string tipo;
	int nivel;
	int quantidade;

	char operacao;

	do {
		try {
			cin >> operacao;
			switch (operacao) {
				case 'i': // Inserir recursivamente
					// objeto recebe id, nome, tipo e nÃ­vel
					cin >> umPokemon;
					arvore.insere(umPokemon);
					break;
				case 'r': // Remover recursivamente
					cin >> id;
					arvore.remove(id);
					break;
				case 'b': // Buscar
					cin >> id; // ler o id
					umPokemon = arvore.busca(id); // escrever dados do pokemon
					cout << "Elemento buscado: " << umPokemon << endl;
					break;
				case 'l': // Levantamento por tipo e nÃ­vel
					cin >> tipo >> nivel;
					quantidade = arvore.levantamento(tipo, nivel);
					cout << "Levantamento de pokemons tipo " << tipo
						 << " e nÃ­vel " << nivel << ": " << quantidade << endl;
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
