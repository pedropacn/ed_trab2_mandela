#include <stdio.h>
#include <stdlib.h>

typedef struct no{
	int tabuleiro[12];// de 0 a 5 -> computador
	int pontosAI;
	int pontosPlayer;
	struct no * opcao[6];
}node;

node * criaNode(){
	node * raiz = (node *)malloc(sizeof(node));
	for (int i = 0; i < 6; ++i)
		raiz->opcao[i] = NULL;
	return raiz;
}

int folha(node * r){//retorna 1 se o node analisado eh uma folha
	if(r == NULL)
		return 0;
	for (int i = 0; i < 6; ++i)
	{
		if(r->opcao[i] != NULL){
			return 0;
		}
	}
	return 1;
}

void insereNode(node * raiz, int * tabuleiro, int pka, int mka){
	if(raiz == NULL)
		raiz = criaNode();
	raiz->pontosPlayer = pontosPlayer;
	raiz->pontosAI = pontosAI;
	for (int i = 0; i < 12; ++i)
	{
		raiz->tabuleiro[i] = tabuleiro[i];
	}
}

void resultado(int d, int pontosAI, int pontosPlayer){
	printf("Jogador: %d\tComputador: %d\n", pontosPlayer, pontosAI);
	if(d == 3)
		printf("Empate!\n");
	else if(d == 1)
		printf("Vitoria do jogador!\n");
	else
		printf("Vitoria do computador!\n");
}

int vitoria(node * raiz){
	for (int i = 0; i < 6; ++i){// verifica se algum tabuleiro esta vazio
		if (raiz->tabuleiro[i] != 0)
			return 0;
		if (raiz->tabuleiro[6+i] != 0)
			return 0;
	}//se chegou aqui, ja ocorreu game over
	for (int i = 0; i < 6; ++i){
		raiz->pontosAI += raiz->tabuleiro[i];
		raiz->pontosPlayer += raiz->tabuleiro[6+i];
	}
	if(raiz->pontosPlayer == raiz->pontosAI)// empate
		return 3;
	else if(raiz->pontosPlayer > raiz->pontosAI)// vitoria do jogador
		return 1;
	return 2;// vitoria computador
}

int jogada(){// interface para o player fazer sua jogada
	int d;
	printf("escolha uma casa para jogar: ");
	scanf("%d", &d);
	while(d < 1 || d > 6){
		printf("casa invalida\n");
		printf("escolha uma casa para jogar: ");
		scanf("%d", &d);
	}
	return d;
}

int altura(node * r){
	int hopcao[6], i, d = -1000, h;
    if (r == NULL)
        return 1;
    for(i = 0; i < 6; i++){
    	hopcao[i] = altura(r->opcao[i]);
    }
    for (i = 0; i < 6; ++i){
    	if(hopcao[i] > d)
    		d = hopcao[i];
    }
    return d;
}

int escolheDificuldade(){// o valor retornado eh a altura da arvore
	int dif;
	printf("Qual a dificuldade?\n");
	printf("1 - Facil\n");
	printf("2 - Medio\n");
	printf("3 - Dificil\n");
	scanf("%d", &dif);
	switch(dif){
		case 1:
			return 2;
		case 2:
			return 4;
		case 3:
			return 6;
	}
}

void imprimeTabuleiro(node * raiz){
	for (int i = 0; i < 12; ++i){
		if(i == 6){
			printf("\n%d\t\t\t\t\t\t%d\n",raiz->pontosAI, raiz->pontosPlayer);
		}	
		printf("/t%s\n", raiz->tabuleiro[i]);
	}
	printf("\n");
}

int calculaJogada(int jogador, int opcao, node * raiz){
	int i = opcao+1;
	int d = 0;
	if(opcao == 5)// antes do pontosAI
		d = 1;
	else if(opcao == 11)// antes do pontosPlayer
		d = 2;
	while(raiz->tabuleiro[opcao] != 0){
		if(i == 12)//reseta a contagem das casas
			i = 0;
		raiz->tabuleiro[opcao]--;
		if(d == 1){// caso pontosAI
			raiz->pontosAI++;
			d = 0;
		}else if(d == 2){//  caso pontosPlayer
			raiz->pontosPlayer++;
			d = 0;
		}else{// caso tabuleiro
			raiz->tabuleiro[i]++;
			if(i == 5)// antes do pontosAI
				d = 1;
			else if(i == 11)// antes do pontosPlayer
				d = 2;
			i++;// esse incremento fica aqui dentro pois, do contario, as casas 0 e 7 nunca seiam alcancadas
		}
	}
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
			soma += raiz->tabuleiro[i];
			ValorPlayer;
		}
	}
	soma -= 2*raiz->pontosPlayer;// valor dos pontos ja adquiridos
	soma += 2*raiz->pontosAI;
	soma += (valorAI - ValorPlayer)*5;// valor da quantidade da diferença das opcoes de jogadas entre jogadores
	if (valorAI == 0)// verifica se algum dos tabuleiros esta vazio
		soma -= 100;
	if (ValorPlayer == 0)
		soma += 100;
	return soma;
}