#include <stdio.h> //Biblioteca para entrada e saída de dados.
#include <string.h> //Biblioteca para manipulação de strings.
#include <stdlib.h> //Biblioteca para gerenciamento de memória e controle de execução.
#include <windows.h> //Biblioteca para utilizar a função Sleep e outras funcionalidades do sistema operacional Windows.
#include <locale.h> //Biblioteca para definir a localidade do programa.


typedef struct { //Cria a estrutura com os dados do produto.
    int codigo;
    char nome[30];
    float preco;
} Produto;

typedef struct { //Estrutua que contém as infomações dos produtos que estão no carrinho.
    Produto produto;
    int quantidade;
} Carrinho;

void infoProduto(Produto prod); //Função que apresenta as informações do produto.
void menu(); //Função que exibe o menu.
void cadastrarProduto(); //Função que cadastra um produto.
void listarProdutos(); //Função que lista os produtos diponíveis cadastrados.
void comprarProduto(); //Função que realiza a compra do produto, adicionando-o ao carrinho.
void visualizarCarrinho(); //Função que exibe os produtos que estão no carrinho.
Produto pegarProdutoPorCodigo(int codigo); //Função que escolhe um produto cadastrado, pelo código.
int* temNoCarrinho(int codigo); //Função que verifica se existe o produto no carrinho.
void fecharPedido(); //Função que fecha o pedido, finalizando a compra

static int contador_produto = 0; // static preserva o valor da variável após a execução das funções
static int contador_carrinho = 0;
static Carrinho carrinho[50];
static Produto produtos[50];

// Função principal que contém o menu e inicia o programa.
int main() {
    setlocale(0, "Portuguese");
	menu();
    return 0;
}

// Exibe as informações do produto
void infoProduto(Produto prod) { //"prod" é um objeto do tipo "Produto".
    
    printf("Código: %d \nNome: %s \nPreço: %.2f\n", prod.codigo, prod.nome, prod.preco);
}

// Exibe o menu principal
void menu() {
    printf("=======================================\n");
    printf("============== Bem-vindo(a) ===========\n");
    printf("==============  Super Shop ============\n");
    printf("=======================================\n");

    printf("Selecione uma opção abaixo: \n");
    printf("1 - Cadastrar produto\n");
    printf("2 - Listar produtos\n");
    printf("3 - Comprar produto\n");
    printf("4 - Visualizar carrinho\n");
    printf("5 - Fechar pedido\n");
    printf("6 - Sair do sistema\n");

    int opcao; 
    scanf("%d", &opcao); //Recebe a opção digitada pelo usuário.
    getchar(); // Limpar o buffer

    switch (opcao) { //Faz a escolha da função de acordo com a opção digitada pelo usuário.
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
            printf("Opção inválida.\n");
            Sleep(2);
            menu();
            break;
    }
}

//Função que cadastra um produto
void cadastrarProduto() {
    printf("Cadastro de Produto\n");
    printf("====================\n");

    printf("Informe o nome do produto: \n"); 
    fgets(produtos[contador_produto].nome, 30, stdin); //O fgets pode capturar o "\n" quando o usuário digita o nome do produto e tecla enter
    //produtos é um array de estrutura do tipo Produto.
    
    produtos[contador_produto].nome[strcspn(produtos[contador_produto].nome, "\n")] = '\0'; //Para retirar o "\n" é usada a função strcspn
	//a função substitui o "\n" por um caractere nulo "\0"

    printf("Informe o preço do produto: \n"); //Recebe o preço do produto 
    scanf("%f", &produtos[contador_produto].preco);
    getchar(); // Limpar o buffer

    produtos[contador_produto].codigo = (contador_produto + 1); //É utilizado o contador_produto + 1, para representar o código do produto 
    //dessa forma o código do produto inicia com "1"
    printf("O produto %s foi cadastrado com sucesso.\n", produtos[contador_produto].nome);

    contador_produto++;
    sleep(2);
    menu();
}


void listarProdutos() { //Função que lista os produtos diponíveis cadastrados.
    if (contador_produto > 0) { // Verifica se existem produtos antes de listá-los
        printf("Listagem de produtos.\n");
        printf("---------------------\n");
        
        //A variável i foi decalara fora do laço por questões de compatibilidade
        int i;
        for (i = 0; i < contador_produto; i++) { //Mostra os produtos e suas informações
            infoProduto(produtos[i]);
            printf("------------------\n");
            sleep(1);
        }
        
        sleep(2);
        menu();
    } else {
        printf("Não temos ainda produtos cadastrados.\n");
        sleep(2);
        menu();
    }
}

// Função para comprar um produto
void comprarProduto() {
    if (contador_produto > 0) {
        printf("Informe o código do produto que deseja adicionar ao carrinho.\n");

        printf("========== Produtos Disponíveis ===========\n");
        int i;
		for (i = 0; i < contador_produto; i++) { //Exibe os produtos disponíveis
            infoProduto(produtos[i]);
            printf("---------------------\n");
            Sleep(1);
        }
        int codigo;
        scanf("%d", &codigo); //recebe o código do produto a ser escolhido
        getchar(); // Limpar o buffer

        int tem_mercado = 0;
        
		for (i = 0; i < contador_produto; i++) {
            if (produtos[i].codigo == codigo) { //Verifica se o código digitado corresponde ao código de algum produto 
                tem_mercado = 1; //Se o código for válido, o valor de "tem_mercado" passa a ser "1"
								//e executa a função temNoCarrinho, que verifica se o produto está no carrinho
								
                int* retorno = temNoCarrinho(codigo);
                if (retorno[0] == 1) { //Se retorno for igual a 1, significa que o produto já existe no carrinho
                    carrinho[retorno[1]].quantidade++; //Será adicionada mais uma quantidade do produto 
                    printf("Aumentei a quantidade do produto %s já existente no carrinho.\n",
                        carrinho[retorno[1]].produto.nome);
                    Sleep(2);
                    menu();
                } else { //Se não houver, o produto será adicionado ao carrinho
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
        if (tem_mercado < 1) { //Será executada caso o produto não seja encontrado.
            printf("Não foi encontrado o produto com código %d.\n", codigo);
            Sleep(2);
            menu();
        }
    } else { //Será mostrado se não ouver produtos cadastrados.
        printf("Ainda não existem produtos para vender.\n");
        Sleep(2);
        menu();
    }
}

//Função que exibe os produtos que estão no carrinho.
void visualizarCarrinho() {
    if (contador_carrinho > 0) {
        printf("Produtos do Carrinho\n");
        printf("--------------------\n");
        int i;
		for (i = 0; i < contador_carrinho; i++) { //Percorre a estrutura "carrinho" e informa os produtos que estão nele
            infoProduto(carrinho[i].produto);
            printf("Quantidade: %d\n", carrinho[i].quantidade);
            printf("-----------------\n");
            Sleep(1);
        }
        sleep(2);
        menu();
    } else { 
        printf("Não temos ainda produtos no carrinho.\n");
        Sleep(2);
        menu();
    }
}

//Função que escolhe um produto cadastrado, pelo código.
Produto pegarProdutoPorCodigo(int codigo) {
    Produto p; //Cria a variável "p" do tipo "Produto"
    int i;
	for (i = 0; i < contador_produto; i++) {
        if (produtos[i].codigo == codigo) {
            p = produtos[i];
        }
    }
    return p;
}

// Função para verificar se um produto está no carrinho
int* temNoCarrinho(int codigo) {
    static int retorno[] = {0, 0};
    int i;
	for (i = 0; i < contador_carrinho; i++) {
        if (carrinho[i].produto.codigo == codigo) {
            retorno[0] = 1; // Tem o produto com este código no carrinho
            retorno[1] = i; // O índice do produto no carrinho
        }
    }
    return retorno;
}

// Função para fechar o pedido e limpar o carrinho
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
            infoProduto(p); //Exibe as informações do produto 
            printf("Quantidade: %d\n", quantidade); //Exibe a quantidade de produtos
            printf("---------------\n");
            Sleep(1);
        }
        printf("Sua fatura é R$ %.2f\n", valorTotal); //Exibe o valor total da compra

        // Limpar carrinho
        contador_carrinho = 0; //Zera o carrinho.
        printf("Obrigado pela preferência.\n");
        Sleep(5);
        menu();
    } else { //Executa se não houve produtos no carrinho e retorna o menu.
        printf("Você não tem nenhum produto no carrinho ainda.\n");
        Sleep(3);
        menu();
    }
}

