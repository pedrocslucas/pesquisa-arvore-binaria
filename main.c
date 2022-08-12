#include <stdio.h>
#include <stdlib.h>

typedef struct {
  float desconto;
  char nome[40];
  char pai[40];
} cliente;
typedef cliente elemento;

const elemento VL_NULO = {0, "", ""};
const elemento ROOT = {0,"root", " "};
#include "tad_tree.h"

limpa_tela() {
	system("clear");
}

/*
float soma_bonus(tree t) { 
   if (t != NULL) { 
      return (t->info.pagamento + soma_bonus(t->esq) + soma_bonus(t->dir));
   } 
   else 
      return 0; 
}

int contaNos(tree t) {
  if (t != NULL)
    return (1 + contaNos(t->esq) + contaNos(t->dir));
  else
    return 0;
}
*/

void monta_tela() {
  printf("-------------------------\n");
  printf("\t\tACADEMIA\n");
  printf("-------------------------\n");
  printf("\n1 - Cadastrar Novo Aluno");
  printf("\n2 - Lançar Pagamento");
  printf("\n3 - Imprimir Árvore");
  printf("\n4 - Imprimir Subárvore");
  printf("\n5 - Sair e Salvar\n");
  printf("op> ");
}

char escolhe_opcao() {
	return getchar();
}

int comparaelementos(elemento item1, elemento item2) {
  return strcmp(item1.nome, item2.nome);
}

void adicionarFilho(tree t, elemento item_pai, elemento item){
  tree aux = localiza(t, item_pai);
  if(t != NULL){
    aux = aux->esq;
    if(aux != NULL){
      while(aux->dir != NULL){
        aux = aux->dir;
      }
      adicionarDirPai(t, aux->info, item);
    }else{
      adicionarEsqPai(t, item_pai, item);
    }
  }
}

void incluir(tree *arv) {
  char nome_cliente[40];
  int nome_pai[40];
  char opcao;
  elemento e_cliente, e_pai;
  
  printf("Digite um nome de cliente para incluir: "); 
  scanf("%s", &nome_cliente);
  setbuf(stdin, NULL);
  strcpy(e_cliente.nome, nome_cliente);
  e_cliente.desconto = 0;
  setbuf(stdin, NULL);
  printf("Foi indicado por alguem?(S/N): ");
  opcao = escolhe_opcao();
  setbuf(stdin, NULL);
  if(opcao == 'S' || opcao == 's'){
    printf("Digite o nome do cliente pai (cliente que indicou): ");
    scanf("%s", &nome_pai);
    setbuf(stdin, NULL);
    strcpy(e_pai.nome, nome_pai);
    strcpy(e_cliente.pai, e_pai.nome);
    adicionarFilho(*arv, e_pai, e_cliente);
  }else{
    printf("Adicionado sem indicacao.");
    strcpy(e_cliente.pai, ROOT.nome);
    adicionarFilho(*arv, ROOT, e_cliente);
  }
  printf("\n\n Digite <enter> para continuar");
  getchar();
}

void imprimeArvore(tree t, int cont) {
  tree aux = t->esq;
  if(t != NULL){
    for(int i=1; i<=cont;i++, printf(".."));
    if(!comparaelementos(t->info,ROOT)){
      printf("%s \n", t->info.nome);
    }else
      printf("%s - R$%.2f\n", t->info.nome, t->info.desconto);
    if(aux != NULL){
      cont++;
      imprimeArvore(aux, cont);
      while(aux->dir != NULL){
        imprimeArvore(aux->dir, cont);
        aux = aux->dir;
      }
    }
  }
}

void imprimeSub(tree t, elemento nome){
  tree aux = localiza(t, nome);
  imprimeArvore(aux, 0);
}

elemento dadosCliente(tree t){
  elemento cliente;
  float compra;
  char nome[40];
  tree sub;
  printf("Digite o nome do cliente: ");
  scanf("%s", &nome);
  strcpy(cliente.nome, nome);

  sub = localiza(t, cliente);
  
  return sub->info;
}

void aplicarDesconto(tree t, elemento cliente, float compra){
  float taxa = 5;
  float desconto;
  elemento clientePai;
  tree aux;
  if(t != NULL){
    for(int i=5; i>0; i--){
      taxa = taxa/100;
      desconto = compra*taxa;
      strcpy(clientePai.nome, cliente.pai);
      aux = localiza(t, clientePai); // NULL
      if(aux != NULL){
        aux->info.desconto = desconto;
        strcpy(cliente.pai, aux->info.pai);
        //printf("nome: %s desconto: %.2f\n", clientePai.nome, desconto);
      }
      taxa = i-1;
    }
  }
}

void salvaItens(tree t, FILE *arq, int cont){
  tree aux = t->esq;
  if(t != NULL){
    fprintf(arq, "%s %s %f\n", t->info.pai, t->info.nome, t->info.desconto);
    if(aux != NULL){
      cont++;
      salvaItens(aux, arq, cont);
      while(aux->dir != NULL){
        salvaItens(aux->dir, arq, cont);
        aux = aux->dir;
      }
    }
  }
} 

void salvaArquivo(tree t){
    FILE *arq;
    arq = fopen("arquivo.txt","w");
    salvaItens(t, arq, 0);
    fclose(arq);
}

void carregar_arvore(FILE *arq, tree t){
  arq = fopen("arquivo.txt", "r");
  char nome[40];
  float desconto;
  char pai[40];
  elemento item, item_pai;
  tree aux;
  criaRaiz(&t, ROOT);
  while(fscanf(arq, "%s %s %f\n", pai, nome, &desconto) != EOF){
    strcpy(item.nome, nome);
    item.desconto = desconto;
    strcpy(item.pai, pai);
    printf("%s;%s;%f\n", pai, nome, desconto);
    strcpy(item_pai.nome, item.pai);
    aux = localiza(t, item_pai);
    adicionarFilho(t, aux->info, item);
    printf("Adicionado %s, filho de %s\n", item.nome, aux->info.nome);
    getchar();
  }
}


//---------------------------FUNÇÃO PRINCIPAL-------------------------------
void main() {
  char opcao;
  float compra;
  elemento cliente;
  tree arv;
  FILE *arquivo;
  define(&arv);
  criaRaiz(&arv, ROOT);
  carregar_arvore(arquivo, arv);
  do {
    limpa_tela();	
    monta_tela();
    opcao = escolhe_opcao();
   	switch (opcao) {
      case '1':
        incluir(&arv);
        break;
      case '2':
        cliente = dadosCliente(arv);
        printf("Indicado por %s\n", cliente.pai);
        printf("Informe o valor da compra por %s: ", cliente.nome); 
        scanf("%f", &compra);
        setbuf(stdin, NULL);
        aplicarDesconto(arv, cliente, compra);
        printf("\nDigite <enter> para continuar");
        getchar();
        getchar();
        break;
      case '3':
        imprimeArvore(arv, 0);
        printf("\nDigite <enter> para continuar");
        getchar();
        getchar();
        break;
      case '4':
        printf("Digite a sub árvore que deseja ver: ");
        scanf("%s", cliente.nome);
        imprimeSub(arv, cliente);
        printf("\nDigite <enter> para continuar");
        getchar();
        getchar();
        break;
      case '5':
        salvaArquivo(arv);
        exit(1);
        break;
    }
  } while (1!=0);
}