#include <stdio.h> //Biblioteca para entrada e sa�da de dados.
#include <string.h> //Biblioteca para manipula��o de strings.
#include <stdlib.h> //Biblioteca para gerenciamento de mem�ria e controle de execu��o.
#include <windows.h> //Biblioteca para utilizar a fun��o Sleep e outras funcionalidades do sistema operacional Windows.
#include <locale.h> //Biblioteca para definir a localidade do programa.


typedef struct { //Cria a estrutura com os dados do produto.
    int codigo;
    char nome[30];
    float preco;
} Produto;

typedef struct { //Estrutua que cont�m as infoma��es dos produtos que est�o no carrinho.
    Produto produto;
    int quantidade;
} Carrinho;

void infoProduto(Produto prod); //Fun��o que apresenta as informa��es do produto.
void menu(); //Fun��o que exibe o menu.
void cadastrarProduto(); //Fun��o que cadastra um produto.
void listarProdutos(); //Fun��o que lista os produtos dipon�veis cadastrados.
void comprarProduto(); //Fun��o que realiza a compra do produto, adicionando-o ao carrinho.
void visualizarCarrinho(); //Fun��o que exibe os produtos que est�o no carrinho.
Produto pegarProdutoPorCodigo(int codigo); //Fun��o que escolhe um produto cadastrado, pelo c�digo.
int* temNoCarrinho(int codigo); //Fun��o que verifica se existe o produto no carrinho.
void fecharPedido(); //Fun��o que fecha o pedido, finalizando a compra

static int contador_produto = 0; // static preserva o valor da vari�vel ap�s a execu��o das fun��es
static int contador_carrinho = 0;
static Carrinho carrinho[50];
static Produto produtos[50];

// Fun��o principal que cont�m o menu e inicia o programa.
int main() {
    setlocale(0, "Portuguese");
	menu();
    return 0;
}

// Exibe as informa��es do produto
void infoProduto(Produto prod) { //"prod" � um objeto do tipo "Produto".
    
    printf("C�digo: %d \nNome: %s \nPre�o: %.2f\n", prod.codigo, prod.nome, prod.preco);
}

// Exibe o menu principal
void menu() {
    printf("=======================================\n");
    printf("============== Bem-vindo(a) ===========\n");
    printf("==============  Super Shop ============\n");
    printf("=======================================\n");

    printf("Selecione uma op��o abaixo: \n");
    printf("1 - Cadastrar produto\n");
    printf("2 - Listar produtos\n");
    printf("3 - Comprar produto\n");
    printf("4 - Visualizar carrinho\n");
    printf("5 - Fechar pedido\n");
    printf("6 - Sair do sistema\n");

    int opcao; 
    scanf("%d", &opcao); //Recebe a op��o digitada pelo usu�rio.
    getchar(); // Limpar o buffer

    switch (opcao) { //Faz a escolha da fun��o de acordo com a op��o digitada pelo usu�rio.
        case 1:
            cadastrarProduto();
            break;
        case 2:
            listarProdutos();
            break;
        case 3:
            comprarProduto();
            break;
        case 4:
            visualizarCarrinho();
            break;
        case 5:
            fecharPedido();
            break;
        case 6:
            printf("Volte sempre!\n"); //Encerra o programa
            Sleep(2);
            exit(0);
        default:
            printf("Op��o inv�lida.\n");
            Sleep(2);
            menu();
            break;
    }
}

//Fun��o que cadastra um produto
void cadastrarProduto() {
    printf("Cadastro de Produto\n");
    printf("====================\n");

    printf("Informe o nome do produto: \n"); 
    fgets(produtos[contador_produto].nome, 30, stdin); //O fgets pode capturar o "\n" quando o usu�rio digita o nome do produto e tecla enter
    //produtos � um array de estrutura do tipo Produto.
    
    produtos[contador_produto].nome[strcspn(produtos[contador_produto].nome, "\n")] = '\0'; //Para retirar o "\n" � usada a fun��o strcspn
	//a fun��o substitui o "\n" por um caractere nulo "\0"

    printf("Informe o pre�o do produto: \n"); //Recebe o pre�o do produto 
    scanf("%f", &produtos[contador_produto].preco);
    getchar(); // Limpar o buffer

    produtos[contador_produto].codigo = (contador_produto + 1); //� utilizado o contador_produto + 1, para representar o c�digo do produto 
    //dessa forma o c�digo do produto inicia com "1"
    printf("O produto %s foi cadastrado com sucesso.\n", produtos[contador_produto].nome);

    contador_produto++;
    sleep(2);
    menu();
}


void listarProdutos() { //Fun��o que lista os produtos dipon�veis cadastrados.
    if (contador_produto > 0) { // Verifica se existem produtos antes de list�-los
        printf("Listagem de produtos.\n");
        printf("---------------------\n");
        
        //A vari�vel i foi decalara fora do la�o por quest�es de compatibilidade
        int i;
        for (i = 0; i < contador_produto; i++) { //Mostra os produtos e suas informa��es
            infoProduto(produtos[i]);
            printf("------------------\n");
            sleep(1);
        }
        
        sleep(2);
        menu();
    } else {
        printf("N�o temos ainda produtos cadastrados.\n");
        sleep(2);
        menu();
    }
}

// Fun��o para comprar um produto
void comprarProduto() {
    if (contador_produto > 0) {
        printf("Informe o c�digo do produto que deseja adicionar ao carrinho.\n");

        printf("========== Produtos Dispon�veis ===========\n");
        int i;
		for (i = 0; i < contador_produto; i++) { //Exibe os produtos dispon�veis
            infoProduto(produtos[i]);
            printf("---------------------\n");
            Sleep(1);
        }
        int codigo;
        scanf("%d", &codigo); //recebe o c�digo do produto a ser escolhido
        getchar(); // Limpar o buffer

        int tem_mercado = 0;
        
		for (i = 0; i < contador_produto; i++) {
            if (produtos[i].codigo == codigo) { //Verifica se o c�digo digitado corresponde ao c�digo de algum produto 
                tem_mercado = 1; //Se o c�digo for v�lido, o valor de "tem_mercado" passa a ser "1"
								//e executa a fun��o temNoCarrinho, que verifica se o produto est� no carrinho
								
                int* retorno = temNoCarrinho(codigo);
                if (retorno[0] == 1) { //Se retorno for igual a 1, significa que o produto j� existe no carrinho
                    carrinho[retorno[1]].quantidade++; //Ser� adicionada mais uma quantidade do produto 
                    printf("Aumentei a quantidade do produto %s j� existente no carrinho.\n",
                        carrinho[retorno[1]].produto.nome);
                    Sleep(2);
                    menu();
                } else { //Se n�o houver, o produto ser� adicionado ao carrinho
                    Produto p = pegarProdutoPorCodigo(codigo);
                    carrinho[contador_carrinho].produto = p;
                    carrinho[contador_carrinho].quantidade = 1;
                    contador_carrinho++;
                    printf("O produto %s foi adicionado ao carrinho.\n", p.nome);
                    Sleep(2);
                    menu();
                }
            }
        }
        if (tem_mercado < 1) { //Ser� executada caso o produto n�o seja encontrado.
            printf("N�o foi encontrado o produto com c�digo %d.\n", codigo);
            Sleep(2);
            menu();
        }
    } else { //Ser� mostrado se n�o ouver produtos cadastrados.
        printf("Ainda n�o existem produtos para vender.\n");
        Sleep(2);
        menu();
    }
}

//Fun��o que exibe os produtos que est�o no carrinho.
void visualizarCarrinho() {
    if (contador_carrinho > 0) {
        printf("Produtos do Carrinho\n");
        printf("--------------------\n");
        int i;
		for (i = 0; i < contador_carrinho; i++) { //Percorre a estrutura "carrinho" e informa os produtos que est�o nele
            infoProduto(carrinho[i].produto);
            printf("Quantidade: %d\n", carrinho[i].quantidade);
            printf("-----------------\n");
            Sleep(1);
        }
        sleep(2);
        menu();
    } else { 
        printf("N�o temos ainda produtos no carrinho.\n");
        Sleep(2);
        menu();
    }
}

//Fun��o que escolhe um produto cadastrado, pelo c�digo.
Produto pegarProdutoPorCodigo(int codigo) {
    Produto p; //Cria a vari�vel "p" do tipo "Produto"
    int i;
	for (i = 0; i < contador_produto; i++) {
        if (produtos[i].codigo == codigo) {
            p = produtos[i];
        }
    }
    return p;
}

// Fun��o para verificar se um produto est� no carrinho
int* temNoCarrinho(int codigo) {
    static int retorno[] = {0, 0};
    int i;
	for (i = 0; i < contador_carrinho; i++) {
        if (carrinho[i].produto.codigo == codigo) {
            retorno[0] = 1; // Tem o produto com este c�digo no carrinho
            retorno[1] = i; // O �ndice do produto no carrinho
        }
    }
    return retorno;
}

// Fun��o para fechar o pedido e limpar o carrinho
void fecharPedido() {
    if (contador_carrinho > 0) { //Executa se houver produtos no carrinho
        float valorTotal = 0.0; //Inicializa o valor total da compra
        printf("Produtos do Carrinho\n");
        printf("--------------------\n");
        int i;
		for (i = 0; i < contador_carrinho; i++) { //Percorre todos os itens no carrinho
            Produto p = carrinho[i].produto; //Acessa o produto no carrinho
            int quantidade = carrinho[i].quantidade; //Acessa a quantidade do produto no carrinho
            valorTotal += p.preco * quantidade; //Soma o valor total para cada produto
            infoProduto(p); //Exibe as informa��es do produto 
            printf("Quantidade: %d\n", quantidade); //Exibe a quantidade de produtos
            printf("---------------\n");
            Sleep(1);
        }
        printf("Sua fatura � R$ %.2f\n", valorTotal); //Exibe o valor total da compra

        // Limpar carrinho
        contador_carrinho = 0; //Zera o carrinho.
        printf("Obrigado pela prefer�ncia.\n");
        Sleep(5);
        menu();
    } else { //Executa se n�o houve produtos no carrinho e retorna o menu.
        printf("Voc� n�o tem nenhum produto no carrinho ainda.\n");
        Sleep(3);
        menu();
    }
}

