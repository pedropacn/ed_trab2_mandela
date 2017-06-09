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

void insereNode(node * raiz, int * tabuleiro, int pontosPlayer, int pontosAI){
	if(raiz == NULL)
		raiz = criaNode();
	raiz->pontosPlayer = pontosPlayer;
	raiz->pontosAI = pontosAI;
	for (int i = 0; i < 12; ++i)
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

int jogada(int * tabuleiro){// interface para o player fazer sua jogada
	int d;
	printf("escolha uma casa para jogar: ");
	scanf("%d", &d);
	while((d < 1 || d > 6) || (tabuleiro[d+5] == 0)){
		printf("casa invalida\n");
		printf("escolha uma casa para jogar: ");
		scanf("%d", &d);
	}
	return d+5;
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


int copiaDados(node * r1, node * r2){// r1 eh o original e r2 eh o destino da copia
	if(r1 == r2)
		printf("Mesmo node\n");
	else if(r1 == NULL)
		r2 = NULL;
	else{
		r2->pontosAI = r1->pontosAI;
		r2->pontosPlayer = r1->pontosPlayer;
		for(int i = 0; i < 12; ++i)
			r2->tabuleiro[i] = r1->tabuleiro[i];
	}
}

int dif = 0;
int escolheDificuldade(){// o valor retornado eh a altura da arvore - 1
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
			printf("\n%d\t\t\t\t\t\t\t%d\n",raiz->pontosAI, raiz->pontosPlayer);
		}	
		printf("\t%d", raiz->tabuleiro[i]);
	}
	printf("\n");
}

int jogador = 0;
int vez(){
	if(!jogador){
	printf("Deseja jogar primeiro? s/n\n");
		char ch;
		scanf("%c", &ch);
		if(ch == 's'){
			jogador = 2;// apenas para jogador se tornar true e nao entrar na condicao anterior
			return 2;
		}
		else{
			jogador = 1;
			return 1;
		}
	}else{
		if(jogador == 2){
			jogador = 1;
			return 1;
		}else{
			jogador = 2;
			return 2;
		}
	}
}
int bonus = 0;
node * calculaJogada(int vez, int opcao, node * raiz){// modifica um tabuleiro ja criado
	int i = opcao+1;
	int d = 0;
	node * temp = raiz->opcao[0];
	if(temp == NULL){
		temp = criaNode();
	}
	copiaDados(raiz, temp);
	if(opcao == 5 && vez == 1)// antes do pontosAI
		d = 1;
	else if(opcao == 11 && vez == 2)// antes do pontosPlayer
		d = 2;
	while(temp->tabuleiro[opcao] != 0){
		if(i == 12)//reseta a contagem das casas
			i = 0;
		temp->tabuleiro[opcao]--;
		if(d == 1){// caso pontosAI
			temp->pontosAI++;
			bonus = 1;
			d = 0;
		}else if(d == 2){//  caso pontosPlayer
			temp->pontosPlayer++;
			bonus = 2;
			d = 0;
		}else{// caso tabuleiro
			bonus = 0;
			temp->tabuleiro[i]++;
			if(opcao == 5 && vez == 1)// antes do pontosAI
				d = 1;
			else if(opcao == 11 && vez == 2)// antes do pontosPlayer
				d = 2;
			i++;// esse incremento fica aqui dentro pois, do contario, as casas 0 e 7 nunca seiam alcancadas
		}
	}
	return temp;
}

int valorJogada(node * raiz){// retorna uma quantificacao do quao valiosa eh uma jogada
	int soma = 0;
	int valorAI = 0, ValorPlayer = 0;// estas armazenam quantas casas vazias existem
	for(int i = 0; i < 12; i++){// varre o tabuleiro e somando o total de cada tabuleiro
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

int somaFolhas(node * r){
	if(r == NULL){
		return 0;
	int soma = 0;
	for (int i = 0; i < 6; ++i)
		soma += valorJogada(r->opcao[i]);
	return soma + valorJogada(r);
	}
}

void criaFolhas(node * r, int d){//ta gastando muito
	if(d < dif){
		for(int i = 0; i < 6; ++i){
			if(r->opcao[i] == NULL)
				r->opcao[i] = criaNode();
			copiaDados(r, r->opcao[i]);
			calculaJogada(vez(), i, r->opcao[i]);
			criaFolhas(r->opcao[i], d++);
		}
	}
}

int vezDoAI(node * raizAtual, node * r){
	int maior = -1000, temp, d;
	copiaDados(raizAtual, r);
	criaFolhas(r, 0);
	for (int i = 0; i < 6; ++i){//analise das opcoes e selecao da jogada
		if(maior < somaFolhas(r->opcao[i])){
			maior = somaFolhas(r->opcao[i]);
			d = i;
		}
	}
	raizAtual = calculaJogada(jogador, d, raizAtual);
	return resultado(vitoria(raizAtual), raizAtual->pontosAI, raizAtual->pontosPlayer);
}

int vezDoPlayer(node * raizAtual){
	raizAtual = calculaJogada(jogador, jogada(raizAtual->tabuleiro), raizAtual);
	return resultado(vitoria(raizAtual), raizAtual->pontosAI, raizAtual->pontosPlayer);
}

int rafik(node * raiz){
	raiz = criaNode();
	node * raizAtual = raiz;//raizAtual serve para mexer na arvore principal (lista) sem comprometer o accesso ao inicio
	raizAtual->pontosAI = 0;
	raizAtual->pontosPlayer = 0;
	for(int i = 0; i < 12; ++i){
		raizAtual->tabuleiro[i] = 6;
	}
	int dif = escolheDificuldade();
	vez();// player escolhe se joga primeiro ou nao
	imprimeTabuleiro(raizAtual);
	node * r = criaNode();

	int a = 1;
	while(a){//loop infinito
		if (jogador == 1){
			if(vezDoAI(raizAtual, r))
				break;
		}else if(jogador == 2){
			if(vezDoPlayer(raizAtual))
				break;
		}
		if(!bonus)// condicao da jogada bonus
			vez();
	}
}

int main()
{
	node * raiz = criaNode();
	rafik(raiz);
	return 0;
}