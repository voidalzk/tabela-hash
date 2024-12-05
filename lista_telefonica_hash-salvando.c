#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define TABLE_SIZE 100
#define JSON_FILE "contatos.json"

typedef struct Contact {
    char name[50];
    char phone[20];
    struct Contact* next;
} Contact;

Contact* hashTable[TABLE_SIZE] = {NULL};

unsigned int hashFunction(const char* name) {
    unsigned int hash = 0;
    for(int i = 0; name[i]; i++) {
        hash += tolower(name[i]);
    }
    return hash % TABLE_SIZE;
}

void adicionarContato() {
    char name[50], phone[20];
    printf("Nome: ");
    scanf("%s", name);
    printf("Telefone: ");
    scanf("%s", phone);

    unsigned int index = hashFunction(name);
    Contact* newContact = malloc(sizeof(Contact));
    if(!newContact) {
        printf("Erro ao alocar memória.\n");
        return;
    }
    strcpy(newContact->name, name);
    strcpy(newContact->phone, phone);
    newContact->next = hashTable[index];
    hashTable[index] = newContact;

    printf("Contato adicionado com sucesso.\n");
}

void buscarContato() {
    char name[50];
    printf("Nome: ");
    scanf("%s", name);

    unsigned int index = hashFunction(name);
    Contact* current = hashTable[index];
    clock_t start = clock();
    while(current) {
        if(strcmp(current->name, name) == 0) {
            clock_t end = clock();
            double time_spent = (double)(end - start) * 1000.0 / CLOCKS_PER_SEC;
            printf("Telefone de %s: %s (tempo de busca: %.2f ms)\n", name, current->phone, time_spent);
            return;
        }
        current = current->next;
    }
    clock_t end = clock();
    double time_spent = (double)(end - start) * 1000.0 / CLOCKS_PER_SEC;
    printf("Contato não encontrado. (tempo de busca: %.2f ms)\n", time_spent);
}

void removerContato() {
    char name[50];
    printf("Nome: ");
    scanf("%s", name);

    unsigned int index = hashFunction(name);
    Contact* current = hashTable[index];
    Contact* prev = NULL;

    while(current) {
        if(strcmp(current->name, name) == 0) {
            if(prev) {
                prev->next = current->next;
            } else {
                hashTable[index] = current->next;
            }
            free(current);
            printf("Contato removido com sucesso.\n");
            return;
        }
        prev = current;
        current = current->next;
    }
    printf("Contato não encontrado.\n");
}

void exibirContatos() {
    for(int i = 0; i < TABLE_SIZE; i++) {
        Contact* current = hashTable[i];
        while(current) {
            printf("Nome: %s, Telefone: %s\n", current->name, current->phone);
            current = current->next;
        }
    }
}

void salvarContatos() {
    FILE* file = fopen(JSON_FILE, "w");
    if(!file) {
        printf("Erro ao abrir o arquivo para salvar contatos.\n");
        return;
    }

    fprintf(file, "{\n  \"contatos\": [\n");
    int first = 1;
    for(int i = 0; i < TABLE_SIZE; i++) {
        Contact* current = hashTable[i];
        while(current) {
            if(!first) {
                fprintf(file, ",\n");
            }
            fprintf(file, "    {\n      \"nome\": \"%s\",\n      \"telefone\": \"%s\"\n    }", current->name, current->phone);
            first = 0;
            current = current->next;
        }
    }
    fprintf(file, "\n  ]\n}\n");
    fclose(file);
    printf("Contatos salvos em %s.\n", JSON_FILE);
}

void carregarContatos() {
    FILE* file = fopen(JSON_FILE, "r");
    if(!file) {
        return;
    }

    char line[256];
    char name[50] = "";
    char phone[20] = "";
    int nomeEncontrado = 0;

    while(fgets(line, sizeof(line), file)) {
        if(strstr(line, "\"nome\"")) {
            // Extrai o nome
            sscanf(line, "    \"nome\": \"%49[^\"]\"", name);
            nomeEncontrado = 1;
        }
        if(strstr(line, "\"telefone\"") && nomeEncontrado) {
            sscanf(line, "      \"telefone\": \"%19[^\"]\"", phone);
            unsigned int index = hashFunction(name);
            Contact* newContact = malloc(sizeof(Contact));
            if(!newContact) {
                printf("Erro ao alocar memória.\n");
                fclose(file);
                return;
            }
            strcpy(newContact->name, name);
            strcpy(newContact->phone, phone);
            newContact->next = hashTable[index];
            hashTable[index] = newContact;
            nomeEncontrado = 0;
        }
    }
    fclose(file);
}

int main() {
    carregarContatos();
    int opcao;
    do {
        printf("\nEscolha uma opcao:\n");
        printf("1 - Adicionar contato\n");
        printf("2 - Buscar contato por nome\n");
        printf("3 - Remover contato\n");
        printf("4 - Exibir todos os contatos\n");
        printf("5 - Salvar contatos\n");
        printf("0 - Sair\n");
        printf("Digite uma opcao: ");

        if(scanf("%d", &opcao) != 1) {
            printf("Entrada inválida. Por favor, digite um número.\n");
            while(getchar() != '\n');
            opcao = -1;
            continue;
        }

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
            case 5:
                salvarContatos();
                break;
            case 0:
                salvarContatos();
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida! Tente novamente.\n");
        }
    } while (opcao != 0);

    for(int i = 0; i < TABLE_SIZE; i++) {
        Contact* current = hashTable[i];
        while(current) {
            Contact* temp = current;
            current = current->next;
            free(temp);
        }
    }

    return 0;
}