/* Questao 4, Dredd, Implementar um Deque - controle de Undo,
programa le primeiramente a quantidade maxima de instrucoes,
toda ves que ler undo deve desfazer a ultima instrucao.
Se a capacidade máxima for atingida, o programa deve "esquecer" 
a instrução mais antiga antes de inserir a nova instrução.
By: Perninha, 26/06/2020 */

#include <iostream>

using namespace std;

class Noh{
	friend class Lista;
	private:
		Noh* PtProximo;
		Noh* PtAnterior;
		string Comando;
	public:
		Noh(string Comando);
};

class Lista{
	private:
		int Tam;
		Noh* PtPrimeiro;
		Noh* PtUltimo;
		bool Vazia();
		void InserirNoFim(string &Dado);
	public:
		Lista();
		~Lista();
		void InserirEmListaVazia(string &Dado);
		string RemoverNoInicio();
		string RemoverNoFim();
};

Noh::Noh(string c){
	Comando = c;
	PtProximo = NULL;
	PtAnterior = NULL;
}

Lista::Lista(){
	Tam = 0;
	PtPrimeiro = NULL;
	PtUltimo = NULL;
}

Lista::~Lista(){
	Noh* Del = PtPrimeiro;
	Noh* ProximoAux = NULL;
	while(Del != NULL){
		ProximoAux = Del->PtProximo;
		delete Del;
		Del = ProximoAux;
	}
	Del = NULL;
	ProximoAux = NULL;
	delete Del;
	delete ProximoAux;
}

bool Lista::Vazia(){
	return(Tam==0);
}

void Lista::InserirEmListaVazia(string &Dado){
	if(Vazia()){
		Noh* NovoNoh = new Noh(Dado);
		PtPrimeiro = NovoNoh;
		PtUltimo = NovoNoh;
		Tam++;
	}
	else{
		InserirNoFim(Dado);
	}
}

void Lista::InserirNoFim(string &Dado){
	Noh* NovoNoh = new Noh(Dado);
	NovoNoh->PtAnterior = PtUltimo;
	PtUltimo->PtProximo = NovoNoh;
	PtUltimo = NovoNoh;
	Tam++;
}

string Lista::RemoverNoInicio(){
	string Valor;
	if(Vazia()){
		cerr << "ERRO" << endl;
	}
	else{
		if(Tam == 1){
			Valor = PtPrimeiro->Comando;
			PtPrimeiro = NULL;
			PtUltimo = NULL;
			Tam--;
			return Valor;
		}
		else if(Tam > 1){
			Noh* Aux = PtPrimeiro;
			Valor = Aux->Comando;
			PtPrimeiro = Aux->PtProximo;
			PtPrimeiro->PtAnterior = NULL;
			Aux = NULL;
			delete Aux;
			Tam--;	
		}
	}
	return Valor;
}

string Lista::RemoverNoFim(){
	string Valor;
	if(Vazia()){
		cerr << "ERRO" << endl;
	}
	else{
		if(Tam == 1){
			Valor =PtPrimeiro->Comando;
			PtPrimeiro = NULL;
			PtUltimo = NULL;
			Tam--;
		}
		else if(Tam > 1){
			Noh*  Aux = PtUltimo;
			PtUltimo = Aux->PtAnterior;
			PtUltimo->PtProximo = NULL;
			Valor = Aux->Comando;
			Aux = NULL;
			delete Aux;
			Tam--;
		}
	}
	return Valor;
}


int main(){
	Lista Perninha;
	int Cap = 0, i = 0;
	cin >> Cap;
	string Comandos;
	cin.ignore();
	getline(cin, Comandos);
	while(Comandos != "sair"){
		if(Comandos == "undo"){
			if(i > 0){
				cout << "desfazer: " << Perninha.RemoverNoFim() << endl;
				i--;
			}
			else if(i == 0){
				cout << "ERRO" << endl;
			}
		}
		else{
			if(i == Cap){
				cout << "esqueci: " << Perninha.RemoverNoInicio() << endl;
				Perninha.InserirEmListaVazia(Comandos);
			}
			else if(i < Cap){
				Perninha.InserirEmListaVazia(Comandos);
				i++;
			}
		}
	getline(cin, Comandos);
	}
	return 0;
}
