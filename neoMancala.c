#include <stdio.h>
#include <stdlib.h>

typedef struct no{
	int tabuleiro[12];// de 0 a 5 -> computador
	int pontosAI;
	int pontosPlayer;
	int bonus;
	int vez;
	struct no * opcao[6];
}node;


node * criaNode(){
    int i;
	node * raiz = (node *)malloc(sizeof(node));
	for (i = 0; i < 6; i++)
		raiz->opcao[i] = NULL;
	return raiz;
}

int folha(node * r){//retorna 1 se o node analisado eh uma folha
    if(r == NULL)
		return 0;
	int i;
	for (i = 0; i < 6; ++i)
	{
		if(r->opcao[i] != NULL){
			return 0;
		}
	}
	return 1;
}

void insereNode(node * raiz, int * tabuleiro, int pontosPlayer, int pontosAI){
	if(raiz == NULL)
		raiz = criaNode();
	raiz->pontosPlayer = pontosPlayer;
	raiz->pontosAI = pontosAI;
    int i;
	for (i = 0; i < 12; ++i)
	{
		raiz->tabuleiro[i] = tabuleiro[i];
	}
}

int resultado(int d, int pontosAI, int pontosPlayer){
	if(d){
		printf("Jogador: %d\tComputador: %d\n", pontosPlayer, pontosAI);
		if(d == 3)
			printf("Empate!\n");
		else if(d == 1)
			printf("Vitoria do jogador!\n");
		else
			printf("Vitoria do computador!\n");
		return 1;
	}
	return 0;
}

int vitoria(node * raiz){
    int i, dAI = 0,dPlayer = 0;
	for (i = 0; i < 6; ++i)// verifica se algum tabuleiro esta vazio
		dAI += raiz->tabuleiro[i];
	for (i = 0; i < 6; ++i)
		dPlayer += raiz->tabuleiro[6+i];
	if(dAI && dPlayer)
		return 0;
	//se chegou aqui, ja ocorreu game over
	for (i = 0; i < 6; ++i){
		raiz->pontosAI += raiz->tabuleiro[i];
		raiz->pontosPlayer += raiz->tabuleiro[6+i];
	}
	if(raiz->pontosPlayer == raiz->pontosAI)// empate
		return 3;
	else if(raiz->pontosPlayer > raiz->pontosAI)// vitoria do jogador
		return 1;
	return 2;// vitoria computador
}

int jogada(int * tabuleiro){// interface para o player fazer sua jogada
	int d;
	printf("escolha uma casa para jogar: ");
	scanf("%d", &d);
	while((d < 1 || d > 6) || (tabuleiro[d+5] == 0)){
		printf("casa invalida\n");
		printf("escolha uma casa para jogar: ");
		scanf("%d", &d);
	}
	return d;
}

void copiaDados(node * r1, node * r2){// r1 eh o original e r2 eh o destino da copia
    int i;
	if(r2 == NULL){
		printf("destino NULL \n");
	}
	if(r1 == r2){
		printf("Mesmo node\n");
	}
	else if(r1 == NULL){
		r2 = NULL;
	}
	else{
		//printf("CHEGUEI AQUI %d\n", r1->pontosAI);
		r2->pontosAI = r1->pontosAI;
		r2->pontosPlayer = r1->pontosPlayer;
		r2->bonus = r1->bonus;
		r2->vez = r1->vez;
		for(i = 0; i < 12; ++i)
			r2->tabuleiro[i] = r1->tabuleiro[i];
	}
}

int dif = 0;
void leArvore(node * r, int d){
	if(d < dif){
		for (int i = 0; i < 6; ++i)
		{
			//printf("pontosAI = %d\n", r->pontosAI);
			leArvore(r, d+1);
		}
	}
}

int escolheDificuldade(){// o valor retornado eh a altura da arvore - 1
	printf("Qual a dificuldade?\n");
	printf("1 - Facil\n");
	printf("2 - Medio\n");
	printf("3 - Dificil\n");
	int d;
	scanf("%d", &d);
	switch(d){
		case 1:
			return 2;
		case 2:
			return 4;
		case 3:
			return 6;
	}
}

void imprimeTabuleiro(node * raiz){
 //    int i;
	// for (i = 0; i < 6; ++i)
	// 	printf("\t%d", raiz->tabuleiro[5-i]);
	// printf("\n%d\t\t\t\t\t\t\t%d\n",raiz->pontosAI, raiz->pontosPlayer);
	// for (i = 0; i < 6; ++i)
	// 	printf("\t%d", raiz->tabuleiro[6+i]);
	// printf("\n");
	printf("  %d %d %d %d %d %d\n", raiz->tabuleiro[5], raiz->tabuleiro[4], raiz->tabuleiro[3], raiz->tabuleiro[2], raiz->tabuleiro[1], raiz->tabuleiro[0]);
	printf("%d             %d\n",raiz->pontosAI, raiz->pontosPlayer);
	printf("  %d %d %d %d %d %d\n\n", raiz->tabuleiro[6], raiz->tabuleiro[7], raiz->tabuleiro[8], raiz->tabuleiro[9], raiz->tabuleiro[10], raiz->tabuleiro[11]);
}

int jogador = 0;
int vez(){
	if(!jogador){// selecao  de primeiro jogador
		getchar();
		printf("Deseja jogar primeiro? s/n\n");
		char ch = 97;
		scanf("%c", &ch);
		if(ch == 's')
			jogador = 2;//vez do player
		else
			jogador = 3;// vez do AI
	}else{// alterna entre os estados de jogador
		if(jogador == 2)
			jogador = 3;
		else
			jogador = 2;
	}
}
//int bonus = 1;
node * calculaJogada(int opcao, node * raiz){// modifica um tabuleiro ja criado
	int d = 0;
	node * temp = raiz;
	if(temp->vez == 2)
		opcao += 5;
	int i = opcao+1;
	if(opcao == 5 && temp->vez == 3)// antes do pontosAI
		d = 1;
	else if(opcao == 11 && temp->vez == 2)// antes do pontosPlayer
		d = 2;
	while(temp->tabuleiro[opcao] > 0){
		// printf("i = %d opcao = %d [opcao] = %d\n", i, opcao, temp->tabuleiro[opcao]);
		if(i == 12)//reseta a contagem das casas
			i = 0;
		temp->tabuleiro[opcao]--;
		if(d == 1){// caso pontosAI
			temp->pontosPlayer++;
			temp->bonus = 1;
			d = 0;
		}else if(d == 2){//  caso pontosPlayer
			temp->pontosAI++;
			temp->bonus = 1;
			d = 0;
		}else{// caso tabuleiro
			temp->bonus = 0;
			//printf("i = %d\n", i);
			temp->tabuleiro[i]++;
			if(i == 11 && temp->vez == 2){// antes do pontosPlayer
				d = 1;
			}
			else if(i == 5 && temp->vez == 3){// antes do pontosAI
				d = 2;
			}
			i++;// esse incremento fica aqui dentro pois, do contario, as casas 0 e 7 nunca seiam alcancadas
		}
	}
	if(temp->bonus == 0){//se bonus eh true, nao ocorre inversao de jogador
		if(temp->vez == 3)
			temp->vez = 2;
		else
			temp->vez = 3;
	}
	//printf("\n");
	// return raiz->opcao[0];
}

int valorJogada(node * raiz){// retorna uma quantificacao do quao valiosa eh uma jogada
	int soma = 0, i;
	int valorAI = 0, ValorPlayer = 0;// estas armazenam quantas casas vazias existem
	for(i = 0; i < 12; i++){// varre o tabuleiro e somando o total de cada tabuleiro
		if(i <= 5){
			soma += raiz->tabuleiro[i];
			valorAI++;
		}
		if(i > 5){
			soma -= raiz->tabuleiro[i];
			ValorPlayer++;
		}
	}
	soma -= 2*raiz->pontosPlayer;// valor dos pontos ja adquiridos
	soma += 2*raiz->pontosAI;
	soma += (valorAI - ValorPlayer)*5;// valor da quantidade da diferença das opcoes de jogadas entre jogadores
	if (valorAI == 0)// verifica se algum dos tabuleiros esta vazio
		soma -= 100;
	if (ValorPlayer == 0)
		soma += 100;
	// printf("inicio debug\n");
	// printf("valorAI = %d\t valorPlayer = %d\n", valorAI, ValorPlayer);
	// printf("pontosAI = %d\t pontosPlayer = %d\n", raiz->pontosAI, raiz->pontosPlayer);
	// printf("soma = %d\n", soma);
	// printf("fim debug\n");
	if(raiz->vez == 2)// essas condições checam de que é a vez.
		return -soma;
	else// se a vez é do AI, retorna normal. Do caontrário, retorna o oposto.
		return soma;
}

int somaFolhas(node * r){
	if(r == NULL){// verifica se eh uma folha
		return 0;
	}
	int soma = 0;
	for(int i = 0; i < 6; ++i){
		soma += somaFolhas(r->opcao[i]);
	}
	return soma + valorJogada(r);
}
void imprimeTudo(node * r){
	printf("##################\n");
	imprimeTabuleiro(r);
	printf("bonus = %d\n", r->bonus);
	printf("vez = %d\n", r->vez);
	printf("##################\n");
	getchar();
}

int count = 0;
void criaFolhas(node * r, int d){//ta gastando muito
	int i; 
    //printf("criaFolhas d=%d dif=%d\n", d, dif);
	if(d < dif){
		for(i = 0; i < 6; i++){
			// if(d == 0)
			// 	printf("\n");
			// printf("d=%d\t", d);
			// if(r->opcao[i] == NULL){
			// }
			/*int a;
			if(r->bonus == 0)
				a = ((d+1)%2)+2;
			else
				a = ((d)%2)+2;*/
			if(r->vez == 3){
				if(r->tabuleiro[i] != 0){
					r->opcao[i] = criaNode();
					copiaDados(r, r->opcao[i]);
					calculaJogada(i, r->opcao[i]);
					criaFolhas(r->opcao[i], d+1);
				}
			}else{
				if(r->tabuleiro[i+5] != 0){
					r->opcao[i] = criaNode();
					copiaDados(r, r->opcao[i]);
					calculaJogada(i, r->opcao[i]);
					criaFolhas(r->opcao[i], d+1);
				}
			}
			//printf("i = %d\n", i);
			//imprimeTudo(r->opcao[i]);
			// a = vez();
			// printf("vez = %d	opcao = %d\t", ((d+1)%2)+2, i);
			// if(r->opcao[i] == NULL)
			// 	printf("opcao[%d] NULL\n", i);
			// else
			// 	printf("opcao[%d] nao NULL\n", i);
		}
	}
}

int vezDoAI(node * raizAtual){
	node * r = criaNode();
	int maior = -1000, temp, d, i;
	copiaDados(raizAtual, r);
	//imprimeTudo(raizAtual);
	criaFolhas(r, 0);
	printf("\n");
	// leArvore(r, 0);
	for (i = 0; i < 6; ++i){//analise das opcoes e selecao da jogada
		// printf("valor do maior atual= %d\n", maior);
		// printf("valor de opcao [%d] = %d\n", i, somaFolhas(r->opcao[i]));
		// imprimeTabuleiro(r->opcao[i]);
		if(r->tabuleiro[i] != 0){
			if(maior < somaFolhas(r->opcao[i])){
				maior = somaFolhas(r->opcao[i]);
				d = i;
			}
		}
	}
	//printf("opcao %d\n", i);
	printf("d = %d maior = %d\n", d, maior);
	calculaJogada(d, raizAtual);
	printf("Minha vez\n");
}


int vezDoPlayer(node * raizAtual){
	int d = jogada(raizAtual->tabuleiro);
	calculaJogada(d, raizAtual);
}

int rafik(node * raiz){
    int i;
	raiz = criaNode();
	node * raizAtual = raiz;//raizAtual serve para mexer na arvore principal (lista) sem comprometer o accesso ao inicio
	raizAtual->pontosAI = 0;
	raizAtual->pontosPlayer = 0;
	raizAtual->bonus = 0;
	for(i = 0; i < 12; ++i){
		raizAtual->tabuleiro[i] = 4;
	}
	dif = escolheDificuldade();
	vez();// player escolhe se joga primeiro ou nao
	raizAtual->vez = jogador; 
	imprimeTabuleiro(raizAtual);
	// node * r = criaNode();
	int a = 1;
	while(a){//loop infinito
		if(raizAtual->vez == 3){
			vezDoAI(raizAtual);
		}else if(raizAtual->vez == 2){
			vezDoPlayer(raizAtual);
		}

		/*if(raizAtual->bonus)
			printf("JOGADA BONUS\n");
		else{
			if(!raizAtual->bonus){// condicao da jogada bonus
				if(raizAtual->vez == 2)
					raizAtual->vez = 3;
				else
					raizAtual->vez = 2;	
			}
		}*/
		imprimeTabuleiro(raizAtual);

		//printf("vitoria? %d\n", vitoria(raizAtual));
		if(vitoria(raizAtual)){
			resultado(vitoria(raizAtual), raizAtual->pontosAI, raizAtual->pontosPlayer);
			break;
		}
	}
}

int main()
{
	node * raiz = criaNode();
	rafik(raiz);
	return 0;
}
