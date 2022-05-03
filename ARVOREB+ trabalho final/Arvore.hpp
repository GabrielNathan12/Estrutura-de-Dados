#include <iostream>
#include "sequenceset.hpp"

using namespace std;
 
const unsigned MAXIMODEDADOS = 9; 
const unsigned MINIMODEDADOS = 4; 
const unsigned MAXIMODEFILHOS = 10; 
const unsigned MEIODOSDADOS = 4; 

ostream& operator<<(ostream& saida, const Pokemon& Vet) {
	saida << "(" << Vet.id << "," << Vet.nome << "," << Vet.tipo << "," << Vet.total << "," << Vet.ataque <<","<< Vet.defesa <<
			 "," << Vet.at_esp << "," << Vet.def_esp << "," << Vet.velocidade << "," << ")";
	return saida;
}

typedef Pokemon Dado;

class Noh{
	friend class ArvoreBMais;
	private:
		bool folha;
		unsigned num;
		Pokemon vetorDados[MAXIMODEDADOS]; 
		Noh* filhos[MAXIMODEFILHOS];
		unsigned vetorDeDados[MAXIMODEFILHOS];
		
	public:
		Noh();
		~Noh();
};

Noh :: Noh(){
	num = 0;
	folha = false;
	for (unsigned i = 0; i < MAXIMODEFILHOS; i++){
		filhos[i] = nullptr;
	}
	for (unsigned i = 0; i < MAXIMODEFILHOS; i++){
		vetorDeDados[i] = 0;
	}
}
	
Noh :: ~Noh(){
	for (unsigned i = 0; i < num + 1; i++){
		delete filhos[i];
	}
}

class ArvoreBMais{
	private:
		cabecalhoArqSS meuCabecalho;
		string umArquivo; 
		friend ostream& operator<<(ostream& output, ArvoreBMais& arvore);
		Noh* Raiz;
		void percorreEmOrdemAux(Noh* atual , int nivel);	
		Dado buscaAux(Noh* RaizSub , unsigned chave);
		Noh* insereAux(Noh* umNoh , const Pokemon& umDado , Pokemon& umDadoPromovido);
		Noh* divideNoh(Noh* umNoh , const Pokemon& umDado , Pokemon& umDadoPromovido);
		void insereEmNohFolhaNaoCheio(Noh* umNoh , Pokemon umDado);
		void insereEmNohIntermediarioNaoCheio(Noh* umNoh , Noh* novoNoh , Pokemon& umDadoPromovido);
		void imprimirAux(Noh* RaizSub , int nivel);
		void atualizaEmOrdemAux(Noh* atual , int& nivel);
		void LerPaginaDoArquivo(Noh* RaizSub , unsigned Posicao);
		void EscreverPaginaDoArquivo(Noh* RaizSub , unsigned Posicao);
	public:
		ArvoreBMais(string ArquivoDeEntrada);
		~ArvoreBMais();
		void insere(const Pokemon& umDado);
		void Busca(unsigned chave);
		void percorreEmOrdem();
		void imprimir();
		void atualizaEmOrdem();
		Dado buscaEmArquivo(unsigned Pos , unsigned umaChave);
		bool ArvoreVazia(){
			return Raiz == nullptr;
		}
};

ArvoreBMais :: ArvoreBMais(string ArquivoDeEntrada){
	Raiz = nullptr;
	umArquivo = ArquivoDeEntrada;
	
	ifstream ArquivoE(umArquivo.c_str() , ios::binary);
	
	if(ArquivoE){
		ArquivoE.read((char*) &meuCabecalho , sizeof(cabecalhoArqSS));
		ArquivoE.close();
	}else{
		meuCabecalho.capacidadeMaxPacote = MAXIMODEDADOS;
		meuCabecalho.capacidadeMinPacote = MINIMODEDADOS;
		meuCabecalho.numPacotes = 0;
		meuCabecalho.posPrimeiroPacote = 0;
		meuCabecalho.posicaoMeio = MEIODOSDADOS;
		meuCabecalho.proxPosicaoVazia = -1;
		ofstream ArquivoS(umArquivo.c_str() , ios::binary);
		ArquivoS.write((const char*) &meuCabecalho , sizeof(cabecalhoArqSS));
		ArquivoS.close();
	}
}

ArvoreBMais :: ~ArvoreBMais(){
	fstream ArquivoS(umArquivo.c_str() , ios::in | ios::out | ios::binary);
	ArquivoS.write((const char*) &meuCabecalho , sizeof(cabecalhoArqSS));
	ArquivoS.close();
	delete Raiz;
}

Dado ArvoreBMais :: buscaEmArquivo(unsigned Pos , unsigned umaChave){
	unsigned posicaoDoArquivo;
	pacote Pacote;
	unsigned i = 0;
	
	fstream arquivoDeEntrada("teste.dat" , ios::in | ios::out | ios::binary);
	posicaoDoArquivo = sizeof(cabecalhoArqSS) + Pos * sizeof(pacote);
	arquivoDeEntrada.seekg(posicaoDoArquivo);
	arquivoDeEntrada.read((char*) &Pacote , sizeof(pacote));
	arquivoDeEntrada.close();
	
	while(i < Pacote.numElementos and Pacote.elementos[i].id <= umaChave){
		i++;
	}
	
	i--;
	
	if(Pacote.elementos[i].id == umaChave){
		return Pacote.elementos[i];
	}else{
		Dado naoAchou;
		naoAchou.id = 0;
		return naoAchou;
	}
} 

void ArvoreBMais :: insere(const Pokemon& umDado){

	if(Raiz == nullptr){
		Raiz = new Noh;
		Raiz->folha = true;
		Raiz->vetorDados[0] = umDado;
		Raiz->num = 1;
		fstream ArquivoE(umArquivo.c_str() , ios::in | ios::out | ios::binary);
		meuCabecalho.proxPosicaoVazia = -1;
		meuCabecalho.numPacotes = 1;
		ArquivoE.write((const char*) &meuCabecalho,sizeof(cabecalhoArqSS));
		ArquivoE.close();
		EscreverPaginaDoArquivo(Raiz , 0);
	}else{
		Dado umDadoPromovido;
		Noh* novoNoh; 
		novoNoh = insereAux(Raiz , umDado , umDadoPromovido);
		if(novoNoh != nullptr){

			Noh* antigaRaiz = Raiz;
			Raiz = new Noh;
			Raiz->vetorDados[0] = umDadoPromovido;
			Raiz->num = 1;
			Raiz->filhos[0] = antigaRaiz;
			Raiz->filhos[1] = novoNoh;

		}
	}
}

Noh* ArvoreBMais :: insereAux(Noh* umNoh ,const Pokemon& umDado , Dado& umDadoPromovido){

	if(umNoh->folha){
		if(umNoh->num < MAXIMODEDADOS){
			insereEmNohFolhaNaoCheio(umNoh , umDado);
			return nullptr;
		}else{
			Noh* novoNoh;
			novoNoh = divideNoh(umNoh , umDado , umDadoPromovido);
			if(umDado.id <= novoNoh->vetorDados[0].id){
				insereEmNohFolhaNaoCheio(umNoh , umDado);
			}else{
				insereEmNohFolhaNaoCheio(novoNoh , umDado);
			}
			return novoNoh;
		}
	}else{
		int i = umNoh->num-1;
		
		while(i >= 0 and umNoh->vetorDados[i].id > umDado.id){
			--i;
		}
		Noh* nohAux;
		nohAux = insereAux(umNoh->filhos[i+1] , umDado , umDadoPromovido);
		if(nohAux != nullptr){
			Dado itemAux = umDadoPromovido;
			if(umNoh->num < MAXIMODEDADOS){
				insereEmNohIntermediarioNaoCheio(umNoh , nohAux , itemAux); 
				return nullptr;
			}else{
				Noh* novoNoh; 
				novoNoh = divideNoh(umNoh , umDado , umDadoPromovido);
				
				if(itemAux.id <= umNoh->vetorDados[MEIODOSDADOS].id){ 
					insereEmNohIntermediarioNaoCheio(umNoh , nohAux , itemAux); 
				}else{
					insereEmNohIntermediarioNaoCheio(novoNoh , nohAux , itemAux); 
				}
				return novoNoh;
			}
		} 
	}
	return nullptr;
}

void ArvoreBMais :: LerPaginaDoArquivo(Noh* RaizSub , unsigned Posicao){
	fstream ArquivoE(umArquivo.c_str() , ios::in | ios::out | ios::binary);
	unsigned i = meuCabecalho.posPrimeiroPacote;
	unsigned TamanhoDasPaginas = sizeof(char) + sizeof(int) +
								 (sizeof(int) * ((2 * i) + 1))+
								 (sizeof(Dado) * (2* i));
	int PosicaoNoArquivo = sizeof(meuCabecalho) + Posicao * TamanhoDasPaginas;
	ArquivoE.seekg(PosicaoNoArquivo);
	ArquivoE.read((char*) & (RaizSub->folha) , sizeof(char));
	ArquivoE.read((char*)& (RaizSub->num), sizeof(int));
	ArquivoE.read((char*) RaizSub->filhos , sizeof(int)*((2*i) + 1));
	ArquivoE.read((char*) RaizSub->vetorDados , sizeof(Dado)*(RaizSub->num));
	ArquivoE.close();
}
	
void ArvoreBMais :: EscreverPaginaDoArquivo(Noh* RaizSub , unsigned Posicao){
	fstream ArquivoS(umArquivo.c_str() , ios::in | ios::out | ios::binary);
	int i = meuCabecalho.posPrimeiroPacote;
	
	unsigned TamanhoDasPaginas = sizeof(char) + sizeof(int) +
								 (sizeof(int) * ((2 * i) + 1))+
								 (sizeof(Dado) * (2* i));
								 
	int PosicaoNoArquivo = sizeof(meuCabecalho) + Posicao * TamanhoDasPaginas;
	
	ArquivoS.seekp(PosicaoNoArquivo);
	ArquivoS.write((char*) &(RaizSub->folha),sizeof(char));
	ArquivoS.write((char*) &(RaizSub->num),sizeof(int));
	ArquivoS.write((char*) RaizSub->filhos , sizeof(int)*((2*i) + 1));
	ArquivoS.write((char*) RaizSub->vetorDados,sizeof(Dado)*(RaizSub->num));
	ArquivoS.close();
}

Noh* ArvoreBMais :: divideNoh(Noh* umNoh ,const Pokemon& umDado , Pokemon& umDadoPromovido){

	Noh* novoNoh = new Noh;
	novoNoh->folha = umNoh->folha;
	
	if(not umNoh->folha){ 
		for (unsigned i = 0; i < MEIODOSDADOS + 1; i++){
			novoNoh->filhos[i] = umNoh->filhos[MEIODOSDADOS + 1 + i];
		}
	}
	
	for (unsigned i = 0; i < MEIODOSDADOS + 1; i++){
		novoNoh->vetorDados[i] = umNoh->vetorDados[MEIODOSDADOS + 1 + i];
		novoNoh->num++;
		umNoh->num--;
	}
	
	umDadoPromovido = novoNoh->vetorDados[0];
	
	return novoNoh;
}


void ArvoreBMais :: insereEmNohFolhaNaoCheio(Noh* umNoh , Pokemon umDado){
	int i = umNoh->num-1;
	
	while(i >= 0 and umNoh->vetorDados[i].id > umDado.id){
		umNoh->vetorDados[i + 1] = umNoh->vetorDados[i];
		i--;
	}
	
	umNoh->vetorDados[i + 1] = umDado; 
	umNoh->num++;
}

void ArvoreBMais :: insereEmNohIntermediarioNaoCheio(Noh* umNoh , Noh* novoNoh , Pokemon& umDadoPromovido){
	int i = umNoh->num-1;
	
	while (i >= 0 and umNoh->vetorDados[i].id > umDadoPromovido.id){ 
		umNoh->vetorDados[i+1] = umNoh->vetorDados[i];  
		umNoh->filhos[i+2] = umNoh->filhos[i+1];
		i--;
	}
	
	umNoh->vetorDados[i+1] = umDadoPromovido; 
	umNoh->filhos[i+2] = novoNoh; 	
	umNoh->num++;
}

void ArvoreBMais :: atualizaEmOrdem(){
	int cont = 0;
	if(Raiz == nullptr){
		cerr << "Arvore Inicializada " << endl;
	}else{
		atualizaEmOrdemAux(Raiz , cont);
		cout << endl;
	}
}

void  ArvoreBMais :: atualizaEmOrdemAux(Noh* umNoh , int& nivel){
	unsigned i; 
	if(umNoh == nullptr){
		throw runtime_error("Erro!") ;
	}
	
	for (i = 0; i < umNoh->num; i++){
		if(not umNoh->folha){
			atualizaEmOrdemAux(umNoh->filhos[i] , nivel);
		}else{
			umNoh->vetorDeDados[i] = nivel;
			nivel++;
		}
	}
	
	if(not umNoh->folha){
		atualizaEmOrdemAux(umNoh->filhos[i] , nivel);
	}else{
		umNoh->vetorDeDados[i] = nivel;
		nivel++;
	}
}

void ArvoreBMais :: percorreEmOrdem(){
	percorreEmOrdemAux(Raiz , 0);
	cout << endl;
}

void  ArvoreBMais :: percorreEmOrdemAux(Noh* umNoh , int nivel){
	unsigned i; 
	if(umNoh == nullptr){
		throw runtime_error("Erro!") ;
	}
	for (i = 0; i < umNoh->num; i++){
		if(not umNoh->folha){
			percorreEmOrdemAux(umNoh->filhos[i] , nivel + 1);
		}
		cout << umNoh->vetorDados[i].id << '/' << nivel << ' ';
	}
	
	if(not umNoh->folha){
		percorreEmOrdemAux(umNoh->filhos[i] , nivel + 1);
	}
}


void ArvoreBMais :: Busca(unsigned chave){
    Dado elementoBuscado;
    
    if(Raiz == nullptr){
       throw runtime_error ("Arvore vazia!");
    }else{
		elementoBuscado = buscaAux(Raiz , chave);
		if(elementoBuscado.id < 0){ 
			throw runtime_error ("Erro!");
		}else{
			cout << "Elemento : " << elementoBuscado << endl;
		}
		
    }
}

Dado ArvoreBMais :: buscaAux(Noh* RaizSub , unsigned chave){
    unsigned i = 0;
    
    while((i < RaizSub->num) and (RaizSub->vetorDados[i-1].id <= chave)){ 
        i++;
	}
        
   i--;
		
    if(RaizSub->vetorDados[i].id == chave){
        return RaizSub->vetorDados[i];
    }else{
        if(RaizSub->folha){
			return buscaEmArquivo(RaizSub->vetorDeDados[i+1], chave);
        }
        else{
            return buscaAux(RaizSub->filhos[i+1], chave);
        }
    }
}

ostream& operator<<(ostream& output, ArvoreBMais& arvore) {
	arvore.imprimir();
	return output;
}

void ArvoreBMais :: imprimir(){
	imprimirAux(Raiz, 0);
	cout << endl;

}

void ArvoreBMais :: imprimirAux(Noh* RaizSub , int nivel){
	unsigned i; 
	
	if(RaizSub == nullptr){ 
		throw runtime_error("Erro!") ;
	}
	
	for (i = 0; i < RaizSub->num; i++){
		if(not RaizSub->folha){
				imprimirAux(RaizSub->filhos[i] , nivel + 1);
			}
			cout << endl;
			cout << "Nivel da Arvore : " <<"[ "<< nivel << " ]" << endl <<"Id : " << RaizSub->vetorDados[i].id << endl
				 << "Nome : " << RaizSub->vetorDados[i].nome << endl
				 << "Tipo : " << RaizSub->vetorDados[i].tipo << endl
				 << "Total : "<< RaizSub->vetorDados[i].total << endl 
				 << "Ataaque : " << RaizSub->vetorDados[i].ataque << endl 
				 << "Defesa : "<< RaizSub->vetorDados[i].defesa << endl
				 << "At esp : " << RaizSub->vetorDados[i].at_esp << endl 
				 << "At def : "<< RaizSub->vetorDados[i].def_esp << endl
				 << "Velocidade : " << RaizSub->vetorDados[i].velocidade 
				 << endl;
	}
	
	if(not RaizSub->folha){
		imprimirAux(RaizSub->filhos[i] , nivel+1);
	}
}


