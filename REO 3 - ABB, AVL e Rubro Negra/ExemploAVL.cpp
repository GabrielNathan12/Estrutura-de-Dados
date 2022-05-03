#include <iostream>
#include <string>
#include <stdexcept>
#include <queue>

class NohAVL; // declaração avançada

typedef std::string TChave; // chave da árvore
typedef float TValor; // valor da árvore

// Declarações das classes ===================================================

class NohAVL {
	friend class AVL;
	friend std::ostream& operator<<(std::ostream& saida, NohAVL* ptNoh);

	private:
		NohAVL (TChave c, const TValor& v);
		~NohAVL();
		NohAVL* ArrumarBalanceamento();
		void AtualizarAltura();
		void DesalocarFilhosRecursivo();
		int FatorBalanceamento();
		NohAVL* InserirRecursivo(NohAVL* ptNoh);
		NohAVL* RotacionarEsquerda();
		NohAVL* RotacionarDireita();
		/*NÃO USEI AINDA, MAS PODERIA AJUDAR
		void TrocarFilho(NohAVL* ptAntigo, NohAVL* ptNovo);
		*/
		TChave mChave;
		TValor mValor;
		NohAVL* mPtEsq;
		NohAVL* mPtDir;
		NohAVL* mPtPai;
		unsigned int mAltura; // folhas têm altura 1
};

class AVL {
	public:
		AVL();
		~AVL();
		void EscreverNivelANivel(std::ostream& saida);
		void Inserir(TChave chave, const TValor& valor);

	private:
		NohAVL* mPtRaiz;
};

// === Classe NohAVL ==============================================================================

using namespace std;

NohAVL::NohAVL(TChave c, const TValor& v) : mChave(c), mValor(v) {
	mPtEsq = nullptr;
	mPtDir = nullptr;
	mPtPai = nullptr;
	mAltura = 1;
	//~ #warning NohAVL::NohAVL não implementado.
}

NohAVL::~NohAVL() {
	//o destrutor da AVL já manda deletar a raiz, então, a partir dela, só precisa deletar os filhos
	DesalocarFilhosRecursivo();
}

// Faz as rotações e ajustes necessários inclusive do nó pai. Atualiza a altura.
// Retorna o nó que ficar na posição dele após os ajustes.
NohAVL* NohAVL::ArrumarBalanceamento() {
	//inicialmente atualiza a altura do noh (para caso seja em remoção(?))
	AtualizarAltura();
	//checa o balanceamento do noh
	int fator_balanceamento = FatorBalanceamento();
	/*ENTÃO, CASO NÃO OCORRA NENHUM DOS 4 CASOS, RETORNA O PRÓPRIO NOH(?)
	//retorna o noh acima na árvore, caso esteja balanceado
	if(fator_balanceamento >= -1 && fator_balanceamento <= 1){
		return this;
	}
	*/
	//Caso o noh esteja desbalanceado, há 4 SITUAÇÕES
	//1. Desbalanceamento Esquerda Esquerda
	if(fator_balanceamento > 1 && mPtEsq->FatorBalanceamento() >= 0){//se fator de balanceamento do noh e de seu filho esquerdo são positivos
		return RotacionarDireita();
	}
	//2. Desbalanceamento Esquerda Direita
	if(fator_balanceamento > 1 && mPtEsq->FatorBalanceamento() < 0){//se o fator de balanceamento do noh é positivo mas de seu filho é negativo
		mPtEsq = mPtEsq->RotacionarEsquerda();
		return RotacionarDireita();
	}
	//3. Desbalanceamento Direita Direita
	if(fator_balanceamento < -1 && mPtDir->FatorBalanceamento() <= 0){//se fator de balanceamento do noh e de seu filho dirito são negativos
		return RotacionarEsquerda();
	}
	//4. Desbalanceamento Direita Esquerda
	if(fator_balanceamento < -1 && mPtDir->FatorBalanceamento() > 0){//se o fator de balanceamento do noh é negativo mas de seu filho é positivo
		mPtDir = mPtDir->RotacionarDireita();
		return RotacionarEsquerda();
	}
	return this;
}

// Calcula e atualiza a altura de um nó.
void NohAVL::AtualizarAltura() {
	int altArvEsq;
	int altArvDir;

	if(mPtEsq != nullptr){
		altArvEsq = mPtEsq->mAltura;
	}else{
		altArvEsq = 0;
	}
	if(mPtDir != nullptr){
		altArvDir = mPtDir->mAltura;
	}else{
		altArvDir = 0;
	}
	if(altArvEsq >= altArvDir){
		mAltura = altArvEsq + 1;
	}else{
		mAltura = altArvDir + 1;
	}
	//~ #warning NohAVL::AtualizarAltura não implementado.
}

// Desaloca todos os descendentes.
void NohAVL::DesalocarFilhosRecursivo() {//-----------------------------certo?
	//o delete chama o destrutor e o destrutor chama o DesalocarFilhosRecursivo
	if(mPtEsq != nullptr){
		delete mPtEsq;
	}
	if(mPtDir != nullptr){
		delete mPtDir;
	}
	//~ delete this; não precisa (?)
	//~ #warning NohAVL::DesalocarFilhosRecursivo não implementado.
}

// Calcula e retorna o fator de balanceamento do nó.
int NohAVL::FatorBalanceamento() {
	int altArvEsq;
	int altArvDir;

	if(mPtEsq != nullptr){
		altArvEsq = mPtEsq->mAltura;
	} else{
		altArvEsq = 0;
	}
	if(mPtDir != nullptr){
		altArvDir = mPtDir->mAltura;
	} else {
		altArvDir = 0;
	}
	int fator_balanceamento = altArvEsq - altArvDir;

	return fator_balanceamento;
}

// Insere um nó numa subárvore.
NohAVL* NohAVL::InserirRecursivo(NohAVL* ptNoh) {
	if(ptNoh->mChave < mChave){
		if(mPtEsq == nullptr){
			mPtEsq = ptNoh;
			ptNoh->mPtPai = this;
		} else {
			mPtEsq = mPtEsq->InserirRecursivo(ptNoh);
		}
	} else {//se ptNoh->mChave >= mChave
		if(mPtDir == nullptr){
			mPtDir = ptNoh;
			ptNoh->mPtPai = this;
		} else {
			mPtDir = mPtDir->InserirRecursivo(ptNoh);
		}
	}
	return ArrumarBalanceamento();
}

// Rotaciona a subárvore à direita. Retorna a nova raiz da subárvore.
NohAVL* NohAVL::RotacionarDireita() {
	NohAVL* aux = mPtEsq;
	//armazena subárvore à direita de aux na direita da raiz atual(se aux virar a nova raiz, o filho direito dele precis ser
	//a antiga raiz, certo? então seu antigo filho direito precisa ir para algum lugar, caso ele exista. Esse lugar é na posição
	//de filho esquerdo da antiga raiz
	this->mPtEsq = aux->mPtDir;
	//atualiza o pai do noh que agora é filho esquerdo da antiga raiz, se ele existir
	if(aux->mPtDir != nullptr){
		aux->mPtDir->mPtPai = this;
	}
	//faz a antiga raiz da subárvore como filho direito de aux;
	aux->mPtDir = this;
	this->mPtPai = aux;

	//atualiza alturas
	this->AtualizarAltura();

	aux->AtualizarAltura();

	//retorna a nova raiz da subárvore
	return aux;
}

// Rotaciona a subárvore à esquerda. Retorna a nova raiz da subárvore.
NohAVL* NohAVL::RotacionarEsquerda() {//--------------------------------certo?
	NohAVL* aux = this->mPtDir;
	//armazena subárvore à esquerda de aux na direita da raiz atual (se aux virar a nova raiz, o filho esquerdo dele precisa ser
	//a antiga raiz, certo? então seu antigo filho esquerdo precisa ir para algum lugar, caso ele exista. Esse lugar é na posição
	//de filho direito da antiga raiz
	this->mPtDir =  aux->mPtEsq;
	//atualiza o pai do noh que agora é filho direito da antiga raiz, se ele existir
	if(aux->mPtEsq != nullptr){
		aux->mPtEsq->mPtPai = this;
	}
	//atualiza o pai de aux. Como ele é a nova raiz, seu pai passa a ser o pai da antiga raiz
	aux->mPtPai = this->mPtPai;
	/*A FUNÇÃO RETORNA UM NOH, ENTÃO ESSE RETORNO TOMA CONTA DESSA PARTE. (e não tem como chamar mPtRaiz dentro da classe NohAVL)
	//se raiz atual era raiz da árvore, a raiz da árvore é agora aux
	if(this == mPtRaiz){
		mPtRaiz = aux;
	}else if(this == this->mPtPai->mPtEsq){//caso contrário, coloque aux no lugar da raiz atual
		this->mPtPai->mPtEsq = aux;
	}else{
		this->mPtPai->mPtDir = aux;
	}  
	*/
	//faz a antiga raiz da subárvore como filho esquerdo de aux
	aux->mPtEsq = this;
	this->mPtPai = aux;
	
	//atualiza alturas
	this->AtualizarAltura();
	aux->AtualizarAltura();
	
	//retorna a nova raiz da subárvore
	return aux;
}
/*NÃO USEI AINDA
// Substitui um dos filhos por um novo nó.
void NohAVL::TrocarFilho(NohAVL* ptAntigo, NohAVL* ptNovo) {
	
	#warning NohAVL::TrocarFilho não implementado.
}
*/

// Escreve o conteúdo de um nó no formato [altura:chave/valor].
// Escreve "[]" se o ponteiro recebido for nullptr.
std::ostream& operator<<(std::ostream& saida, NohAVL* ptNoh) {
	if (ptNoh == nullptr)
		saida << "[]";
	else
		saida << '[' << ptNoh->mAltura << ':' << ptNoh->mChave << '/' << ptNoh->mValor << ']';
	return saida;
}

// === Classe AVL =================================================================================
AVL::AVL() {
	mPtRaiz = nullptr;
	//~ #warning AVL::AVL não implementado.
}

AVL::~AVL() {
	delete mPtRaiz;
	//~ #warning AVL::~AVL não implementado.
}

// Escreve o conteúdo da árvore nível a nível, na saída de dados informada.
// Usado para conferir se a estrutra da árvore está correta.
void AVL::EscreverNivelANivel(ostream& saida) {
	queue<NohAVL*> filhos;
	NohAVL* fimDeNivel = new NohAVL(TChave(), TValor()); // nó especial para marcar fim de um nível

	filhos.push(mPtRaiz);
	filhos.push(fimDeNivel);

	while (not filhos.empty()) {
		NohAVL* ptNoh = filhos.front();

		filhos.pop();

		if (ptNoh == fimDeNivel) {
			saida << "\n";
			if (not filhos.empty())
				filhos.push(fimDeNivel);
		} else {
			saida << ptNoh << ' ';

			if (ptNoh != nullptr) {
				filhos.push(ptNoh->mPtEsq);
				filhos.push(ptNoh->mPtDir);
			}
		}
	}
	delete fimDeNivel;
}

// Insere um par chave/valor na árvore.
void AVL::Inserir(TChave chave, const TValor& valor) {
	NohAVL* novo = new NohAVL(chave, valor);

	if(mPtRaiz == nullptr){
		mPtRaiz = novo;
	} else {
		mPtRaiz = mPtRaiz->InserirRecursivo(novo);
	}
	//~ #warning AVL::Inserir não implementado.
}

// === Programa ===================================================================================
int main() {
	AVL minhaArvore;
	char opcao;
	TChave chave;
	TValor valor;

	do {
		cin >> opcao;

		switch (opcao) {
			case 'i':{ // Inserir
				cin >> chave >> valor;

				minhaArvore.Inserir(chave, valor);

				break;
			}
			case 'e':{ // Escrever nós nível a nível
				minhaArvore.EscreverNivelANivel(cout);

				break;
			}
			case 'f':{ // Finalizar o programa
				// vai verificar depois
				break;
			}
			default:{
				cerr << "Opção inválida\n";
			}
		}
	} while (opcao != 'f');

	return 0;
}
