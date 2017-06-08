#include <stdio.h>
#include <stdlib.h>

typedef struct no{
	int ptabuleiro[6];
	int pkahala;
	int mtabuleiro[6];
	int mkahala;
	struct no * opcao[6];
}node;

int path[10], dif;

node * criaNode();
void insereNode(node * raiz, int * ptab, int * mtab, int pka, int mka);
int jogada();
int altura(node * r);
int folha(node * r);
int escolheDificuldade();
void imprimeTabuleiro(int * ptab, int * mtab, int pka, int mka);
int vitoria(int * ptab, int * mtab, int pka, int mka);
void vencedor(int d, int pka, int mka);
int valorJogada(node * raiz);
int calculaJogada(int player, int opcao, int * ptab, int * mtab, int * pka, int * mka);
int melhor(node * raiz, int vez, int op);

int main()
{
	node * jogo = criaNode();
	int ptabuleiro[6], pkahala = 0, mtabuleiro[6], mkahala = 0;
	int i, d = 0, jbonus;
	char ch;

	for (i = 0; i < 6; ++i){
		mtabuleiro[i] = 6;
		ptabuleiro[i] = 6;
	}

	insereNode(jogo, ptabuleiro, mtabuleiro, pkahala, mkahala);// inicializa o tabuleiro
	imprimeTabuleiro(ptabuleiro, mtabuleiro, pkahala, mkahala);
	dif = escolheDificuldade();
	printf("Voce que jogar primeiro (s/n): \n");
	scanf("%c", &ch);

	while (ch){
		if(ch == 's' && jbonus){//vez do jogador
			jbonus = 0;
			d = jogada();
			jbonus = calculaJogada(1, d, ptabuleiro, mtabuleiro, &pkahala, &mkahala);
			imprimeTabuleiro(ptabuleiro, mtabuleiro, pkahala, mkahala);
		}


		ch = 's';
	}





	return 0;
}

node * criaNode(){
	node * raiz = (node *)malloc(sizeof(node));
	for (int i = 0; i < 6; ++i)
		raiz->opcao[i] = NULL;
	return raiz;
}
void insereNode(node * raiz, int * ptab, int * mtab, int pka, int mka){
	if(raiz == NULL)
		raiz = criaNode();
	raiz->pkahala = pka;
	raiz->mkahala = mka;
	for (int i = 0; i < 6; ++i)
	{
		raiz->ptabuleiro[i] = ptab[i];
		raiz->mtabuleiro[i] = mtab[i];
	}
}
int folha(node * r){
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
int vitoria(int * ptab, int * mtab, int pka, int mka){
	for (int i = 0; i < 6; ++i){// verifica se algum tabuleiro esta vazio
		if (ptab[i] != 0)
			return 0;
		if (mtab[i] != 0)
			return 0;
	}
	for (int i = 0; i < 6; ++i)
	{
		pka += ptab[i];
		mka += mtab[i];
	}
	if(pka == mka)// empate
		return 3;
	else if(pka > mka)// vitoria do jogador
		return 1;
	return 2;// vitoria computador
}
void vencedor(int d, int pka, int mka){
	printf("Jogador: %d\tComputador: %d\n", pka, mka);
	if(d == 3)
		printf("Empate!\n");
	else if(d == 1)
		printf("Vitoria do jogador!\n");
	else
		printf("Vitoria do computador!\n");
}
int jogada(){
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
int escolheDificuldade(){
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
void imprimeTabuleiro(int * ptab, int * mtab, int pka, int mka){
	for(int i = 0; i < 6; i++){
		printf("%d\t", mtab[i]);
	}
	printf("\n%d", mka);
	for(int i = 5; i >= 0; i--){
		printf(" \t");
	}
	printf("%d\n", pka);
	for(int i = 5; i >= 0; i--){
		printf("%d\t", ptab[i]);
	}
	printf("\n");
}
int valorJogada(node * raiz){
	int soma = 0;
	int mq = 0, pq = 0, i;
	for(i = 0; i < 6; i++){
		if(raiz->ptabuleiro[i]){
			soma -= raiz->ptabuleiro[i];
			pq++;
		}
		if(raiz->mtabuleiro[i]){
			soma += raiz->mtabuleiro[i];
			mq++;
		}
	}
	soma -= 2*raiz->pkahala;
	soma += 2*raiz->mkahala;
	soma += (mq - pq)*5;
	if (mq == 0)
		soma -= 100;
	if (pq == 0)
		soma += 100;
	return soma;
}
int calculaJogada(int player, int opcao, int * ptab, int * mtab, int * pka, int * mka){
	int i, d = 0;
	if(player == 1){//player jogador
		if(((opcao + ptab[opcao]) % 7 == 0) && ((opcao + ptab[opcao]) % 2 == 1)){//jogada extra
			d = 1;
		}
		for(i = opcao+1; i <= 6; i++){//apenas no tabuleiro de player
			ptab[opcao]--;
			if(i == 6)
				*pka++;
			else
				ptab[i]++;
		}
		while(ptab[opcao] != 0){
			for(i = opcao+1; i <= 6; i++){//apenas no tabuleiro de oponente de player
				ptab[opcao]--;
				if(i == 6)
					*mka++;
				else
					mtab[i]++;
			}
			for(i = opcao+1; i <= 6; i++){//apenas no tabuleiro de player
				ptab[opcao]--;
				if(i == 6)
					*pka++;
				else
					ptab[i]++;
			}
		}
	}
	else{//player computador
		if(((opcao + ptab[opcao]) % 7 == 0) && ((opcao + ptab[opcao]) % 2 == 1)){//jogada extra
			d = 1;
		}
		for(i = opcao+1; i <= 6; i++){//apenas no tabuleiro de computador
			mtab[opcao]--;
			if(i == 6)
				*mka++;
			else
				mtab[i]++;
		}
		while(mtab[opcao] != 0){
			for(i = opcao+1; i <= 6; i++){//apenas no tabuleiro de player
				mtab[opcao]--;
				if(i == 6)
					*pka++;
				else
					ptab[i]++;
			}
			for(i = opcao+1; i <= 6; i++){//apenas no tabuleiro de computador
				mtab[opcao]--;
				if(i == 6)
					*mka++;
				else
					mtab[i]++;
			}
		}
	}
	return d;//retorna true se ha jogada extra.
}
int melhor(node * raiz, int vez, int op){
	int b, a = -1000;
	if(raiz == NULL)
		return 0;
	else if(!folha)
		return valorJogada(raiz);
	else{
		for (int i = 0; i < 6; ++i){
			b = valorJogada(raiz) + melhor(raiz->opcao[i], !vez, op);
			if(b > a){
				path[dif-altura(raiz)] = op;
				a = b;
			}
		}
	}
	if(vez)
		return a;
	return -a;
}