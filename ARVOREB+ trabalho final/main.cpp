#include <iostream>
#include "Arvore.hpp"

using namespace std;

//~ ostream& operator<<(ostream& saida, const Pokemon& Vet) {
	//~ saida << "(" << Vet.id << "," << Vet.nome << "," << Vet.tipo << "," << Vet.total << "," << Vet.ataque <<","<< Vet.defesa <<
			 //~ "," << Vet.at_esp << "," << Vet.def_esp << "," << Vet.velocidade << "," << ")";
	//~ return saida;
//~ }

istream& operator>>(istream& entrada, Pokemon& Vet) {
	entrada >> Vet.id >> Vet.nome >> Vet.tipo >> Vet.total >> Vet.ataque >> Vet.defesa >> 
			   Vet.at_esp >> Vet.def_esp >> Vet.velocidade;
	return entrada;
}

int main() {
	sequenceset meuSeqSet("teste.dat");
	dado umDado;
	char operacao;
	tipoChave umaChave;
		
	do {
		try {
			cin >> operacao;
			switch (operacao) {
				case 'i': // inserir
					cin >> umDado;
					meuSeqSet.inserirDado(umDado);
					break;
				case 'b': // buscar
					cin >> umaChave;
					umDado = meuSeqSet.buscar(umaChave);
					cout << "Busca: "<< umDado << endl;
					break;
				case 'p': // mostrar estrutura
					meuSeqSet.imprimir();
					break;
				case 'd': // mostrar estrutura
					meuSeqSet.depurar();
					break;
				case 's': // sair
					// será tratado no while
					break;
				default:
					cout << "Opção inválida!" << endl;
			}
		} 
		catch (runtime_error& e) {
			cerr << e.what() << endl;
		}
	} while (operacao != 's');
	
	ArvoreBMais minhaArvore("teste.dat");
	minhaArvore.atualizaEmOrdem();
	cabecalhoArqSS Cabecalho;
	unsigned posArq;
	fstream arqEntrada("teste.dat", ios::in | ios::out | ios::binary);
	arqEntrada.read((char*) &Cabecalho, sizeof(cabecalhoArqSS));
	
	for (unsigned i = 1; i < Cabecalho.numPacotes; i++){
		posArq = sizeof(cabecalhoArqSS) + i*sizeof(pacote);
		arqEntrada.seekg(posArq);
		arqEntrada.read((char*) &umDado, sizeof(dado));
		minhaArvore.insere(umDado);
	}
	
	unsigned id;
	
	if(not minhaArvore.ArvoreVazia()){
		do{
			try{
				cin >> operacao;
				
				switch (operacao){
					case 'p':{
						minhaArvore.percorreEmOrdem();
						
						break;
					}
					case 'b':{
						cin >> id;
						minhaArvore.Busca(id);
						break;
					}
					case 'd':{
						cout << minhaArvore;
						
						break;
					}
					case 's':{
						break;
					}
					default:{
						cout << "Invalido\n";
					}
				}
			}
			catch (exception& e){
				cout << "Erro " << endl;
			}
		} while(operacao != 's');
	}
	
	arqEntrada.close();
	
	return 0;
}
