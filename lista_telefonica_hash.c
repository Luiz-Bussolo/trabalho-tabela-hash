#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TABLE_SIZE 100  // Tamanho da tabela hash

// Estrutura para armazenar um número de telefone
typedef struct PhoneNumber {
    char phone[15];
    struct PhoneNumber* next;  // Ponteiro para o próximo número de telefone
} PhoneNumber;

// Estrutura para armazenar o nome e a lista de números de telefone
typedef struct {
    char name[100];
    PhoneNumber* phoneList;  // Lista de números de telefone
} PhoneEntry;

// Tabela hash para armazenar os pares nome/telefone
PhoneEntry* hashTable[TABLE_SIZE];

// Função de hash simples (soma dos valores ASCII dos caracteres)
unsigned int hash(const char* name) {
    unsigned int hashValue = 0;
    while (*name) {
        hashValue += *name++;
    }
    return hashValue % TABLE_SIZE;
}

// Função para inicializar a tabela hash
void initHashTable() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        hashTable[i] = NULL;
    }
}

// Função para adicionar um número de telefone a um nome
void adicionarContato() {
    char name[100], phone[15];
    printf("Digite o nome do contato: ");
    scanf("%s", name);
    printf("Digite o numero de telefone: ");
    scanf("%s", phone);
    
    // Medindo o tempo de inserção
    clock_t start_time = clock();
    
    unsigned int index = hash(name);
    
    // Se não existe nenhum contato com esse nome, cria uma nova entrada
    if (hashTable[index] == NULL) {
        PhoneEntry* newEntry = (PhoneEntry*)malloc(sizeof(PhoneEntry));
        strcpy(newEntry->name, name);
        newEntry->phoneList = NULL;
        
        // Criação de um novo número de telefone
        PhoneNumber* newPhone = (PhoneNumber*)malloc(sizeof(PhoneNumber));
        strcpy(newPhone->phone, phone);
        newPhone->next = NULL;
        
        // Adiciona o número de telefone à lista
        newEntry->phoneList = newPhone;
        
        hashTable[index] = newEntry;
        printf("Contato adicionado com sucesso!\n");
    } else {
        // Se já existe o nome, apenas adiciona um novo número de telefone à lista
        PhoneNumber* current = hashTable[index]->phoneList;
        while (current != NULL) {
            if (strcmp(current->phone, phone) == 0) {
                printf("Este numero de telefone ja esta associado a esse contato.\n");
                clock_t end_time = clock();
                printf("Tempo de insercao: %.5f ms\n", ((double)(end_time - start_time)) / CLOCKS_PER_SEC * 1000);
                return;
            }
            current = current->next;
        }
        
        // Criação de um novo número de telefone
        PhoneNumber* newPhone = (PhoneNumber*)malloc(sizeof(PhoneNumber));
        strcpy(newPhone->phone, phone);
        newPhone->next = hashTable[index]->phoneList;  // Adiciona no início da lista
        hashTable[index]->phoneList = newPhone;
        
        printf("Numero de telefone adicionado ao contato!\n");
    }
    
    clock_t end_time = clock();
    printf("Tempo de insercao: %.5f ms\n", ((double)(end_time - start_time)) / CLOCKS_PER_SEC * 1000);
}

// Função para buscar um contato por nome
void buscarContato() {
    char name[100];
    printf("Digite o nome do contato que deseja buscar: ");
    scanf("%s", name);
    
    // Medindo o tempo de busca
    clock_t start_time = clock();
    
    unsigned int index = hash(name);
    if (hashTable[index] != NULL && strcmp(hashTable[index]->name, name) == 0) {
        printf("Contatos encontrados para %s:\n", name);
        PhoneNumber* current = hashTable[index]->phoneList;
        while (current != NULL) {
            printf("Telefone: %s\n", current->phone);
            current = current->next;
        }
    } else {
        printf("Contato nao encontrado.\n");
    }
    
    clock_t end_time = clock();
    printf("Tempo de busca: %.5f ms\n", ((double)(end_time - start_time)) / CLOCKS_PER_SEC * 1000);
}

// Função para remover um contato
void removerContato() {
    char name[100];
    printf("Digite o nome do contato que deseja remover: ");
    scanf("%s", name);
    
    unsigned int index = hash(name);
    if (hashTable[index] != NULL && strcmp(hashTable[index]->name, name) == 0) {
        PhoneNumber* current = hashTable[index]->phoneList;
        PhoneNumber* prev = NULL;
        
        // Liberar a memória de todos os números associados ao nome
        while (current != NULL) {
            prev = current;
            current = current->next;
            free(prev);  // Libera a memória do número de telefone
        }
        
        // Libera a memória da entrada do nome
        free(hashTable[index]);
        hashTable[index] = NULL;
        printf("Contato removido com sucesso!\n");
    } else {
        printf("Contato nao encontrado.\n");
    }
}

// Função para exibir todos os contatos
void exibirContatos() {
    printf("Lista de contatos:\n");
    for (int i = 0; i < TABLE_SIZE; i++) {
        if (hashTable[i] != NULL) {
            printf("Nome: %s\n", hashTable[i]->name);
            PhoneNumber* current = hashTable[i]->phoneList;
            while (current != NULL) {
                printf("  Telefone: %s\n", current->phone);
                current = current->next;
            }
        }
    }
}

int main() {
    int opcao;
    initHashTable();  // Inicializa a tabela hash
    
    do {
        printf("\nEscolha uma opcao:\n");
        printf("1 - Adicionar contato\n");
        printf("2 - Buscar contato por nome\n");
        printf("3 - Remover contato\n");
        printf("4 - Exibir todos os contatos\n");
        printf("0 - Sair\n");
        printf("Digite uma opcao: ");
        
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                adicionarContato();
                break;
            case 2:
                buscarContato();
                break;
            case 3:
                removerContato();
                break;
            case 4:
                exibirContatos();
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida! Tente novamente.\n");
        }
    } while (opcao != 0);

    return 0;
}
