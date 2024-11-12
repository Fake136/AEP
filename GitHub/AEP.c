#include <stdio.h> 
#include <stdlib.h>
#include <string.h>

#define USERNAME_LEN 50 //Define uma Constante como valor 50
#define PASSWORD_LEN 50 //Define uma Constante como valor 50
#define USER_FILE "usuarios.txt" // substitui o local do arquivo para um nome facilitando a troca do local do arquivo
#define ENCRYPTED_FILE "usuarios_criptografados.txt" // substitui o local do arquivo para um nome facilitando a troca do local do arquivo

char encrypted_username[USERNAME_LEN];//declara um array com espa�o definido pelo Define Username_Len 
char encrypted_password[PASSWORD_LEN];
char decrypted_username[USERNAME_LEN];

// Fun��o para criptografar strings (simples deslocamento ASCII)
void encrypt_string(const char *input, char *output) {
    int i;
    for (i = 0; input[i] != '\0'; i++) {
        output[i] = input[i] + 17; // Desloca o caractere ASCII
    }
    output[i] = '\0'; // Adiciona o terminador nulo
}

// Fun��o para descriptografar strings
void decrypt_string(const char *input, char *output) {
    int i;
    for (i = 0; input[i] != '\0'; i++) {
        output[i] = input[i] - 17; // Reverte o deslocamento ASCII
    }
    output[i] = '\0'; // Adiciona o terminador nulo
}

int main() {
    int option;

    do {
        printf("\nEscolha uma op��o:\n");
        printf("1. Cadastrar\n");
        printf("2. Fazer Login\n");
        printf("3. Sair\n");
        printf("4. Listar Usuarios\n");
        printf("Op��o: ");
        scanf("%d", &option);

        if (option == 1) { // Cadastrar
            FILE *file = fopen(USER_FILE, "w");
            if (file == NULL) {
                printf("Erro ao abrir o arquivo de usu�rios.\n");
                return 1;
            }

            char username[USERNAME_LEN];
            char password[PASSWORD_LEN];

            printf("Digite o nome de usu�rio: ");
            scanf("%s", username);
            printf("Digite a senha: ");
            scanf("%s", password);

            // Salva o usu�rio e a senha em texto simples
            fprintf(file, "%s %s\n", username, password);
            fclose(file);

            // Cria o arquivo com nomes de usu�rios e senhas criptografados
            char encrypted_username[USERNAME_LEN];
            char encrypted_password[PASSWORD_LEN];

            encrypt_string(username, encrypted_username);
            encrypt_string(password, encrypted_password);

            FILE *encrypted_file = fopen(ENCRYPTED_FILE, "w");
            if (encrypted_file == NULL) {
                printf("Erro ao abrir o arquivo de senhas criptografadas.\n");
                return 1;
            }

            // Salva o nome de usu�rio e a senha criptografada
            fprintf(encrypted_file, "%s %s\n", encrypted_username, encrypted_password);
            fclose(encrypted_file);

            printf("Usu�rio cadastrado com sucesso!\n");
        }
		if (option == 4) { // Listar Usuarios
           FILE *arquivo;
    	 char usuarios[USERNAME_LEN][PASSWORD_LEN];
    	 int contador = 0;
    	 int i = 0;

    	// Abre o arquivo em modo de leitura
    	arquivo = fopen(USER_FILE, "r");
    	if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        return EXIT_FAILURE;
    	}

   		 // L� os usu�rios do arquivo
   		 while (fgets(usuarios[contador], USERNAME_LEN, arquivo) != NULL && contador < USERNAME_LEN) {
   	  	   // Remove a nova linha no final da string
      	  usuarios[contador][strcspn(usuarios[contador], "\n")] = '\0';
     	   contador++;
  		  }

    		// Fecha o arquivo
    		fclose(arquivo);

    		// Exibe a lista de usu�rios
    		printf("Lista de usu�rios registrados:\n");
   			 for (i = 0; i < contador; i++) {
    	    printf("%s\n", usuarios[i]);
    		}

   			 return EXIT_SUCCESS;
			}
            
		else if (option == 2) { // Fazer Login
            char username[USERNAME_LEN];
            char password[PASSWORD_LEN];

            printf("Digite o nome de usu�rio: ");
            scanf("%s", username);
            printf("Digite a senha: ");
            scanf("%s", password);

            // Verifica no arquivo de usu�rios
            FILE *file = fopen(USER_FILE, "r");
            if (file == NULL) {
                printf("Erro ao abrir o arquivo de usu�rios.\n");
                return 1;
            }

            char file_username[USERNAME_LEN];
            char file_password[PASSWORD_LEN];
            int found = 0;

            while (fscanf(file, "%s %s", file_username , file_password) != EOF) {
                if (strcmp(file_username, username) == 0 && strcmp(file_password, password) == 0) {
                    found = 1;
                    break;
                }
            }
            fclose(file);

            if (found) {
                printf("Login bem-sucedido!\n");

                // Op��es para alterar a senha ou excluir o usu�rio
                int action;
                printf("Escolha uma a��o:\n");
                printf("1. Alterar senha\n");
                printf("2. Alterar nome de usu�rio\n");
                printf("3. Excluir usu�rio\n");
                printf("4. Sair\n");
                printf("Op��o: ");
                scanf("%d", &action);

                if (action == 1) { // Alterar senha
                    char new_password[PASSWORD_LEN];
                    printf("Digite a nova senha: ");
                    scanf("%s", new_password);

                    // Atualiza o arquivo de usu�rios
                    FILE *temp_file = fopen("temp.txt", "w");
                    file = fopen(USER_FILE, "r");
                    if (temp_file == NULL || file == NULL) {
                        printf("Erro ao abrir arquivos.\n");
                        return 1;
                    }

                    while (fscanf(file, "%s %s", file_username, file_password) != EOF) {
                        if (strcmp(file_username, username) == 0) {
                            fprintf(temp_file, "%s %s\n", file_username, new_password); // Atualiza a senha
                        } else {
                            fprintf(temp_file, "%s %s\n", file_username, file_password);
                        }
                    }
                    fclose(file);
                    fclose(temp_file);
                    remove(USER_FILE);
                    rename("temp.txt", USER_FILE);

                    // Atualiza o arquivo de senhas criptografadas
                    FILE *encrypted_temp_file = fopen("encrypted_temp.txt", "w");
                    FILE *encrypted_file = fopen(ENCRYPTED_FILE, "r");
                    if (encrypted_temp_file == NULL || encrypted_file == NULL) {
                        printf("Erro ao abrir arquivos criptografados.\n");
                        return 1;
                    }

                    char decrypted_username[USERNAME_LEN];
                    while (fscanf(encrypted_file, "%s %s", encrypted_username, encrypted_password) != EOF) {
                        decrypt_string(encrypted_username, decrypted_username); // Chama a fun��o para descriptografar
                        if (strcmp(decrypted_username, username) == 0) {
                            encrypt_string(new_password, encrypted_password);
                            fprintf(encrypted_temp_file, "%s %s\n", encrypted_username, encrypted_password); // Atualiza a senha criptografada
                        } else {
                            fprintf(encrypted_temp_file, "%s %s\n", encrypted_username, encrypted_password);
                        }
                    }
                    
                    fclose(encrypted_file);
                    fclose(encrypted_temp_file);
                    remove(ENCRYPTED_FILE);
                    rename("encrypted_temp.txt", ENCRYPTED_FILE);

                    printf("Senha alterada com sucesso!\n");
                } else if (action == 2) { // Alterar nome de usu�rio
                    char new_username[USERNAME_LEN];
                    printf("Digite o novo nome de usu�rio: ");
                    scanf("%s", new_username);

                    // Atualiza o arquivo de usu�rios
                    FILE *temp_file = fopen("temp.txt", "w");
                    file = fopen(USER_FILE, "r");
                    if (temp_file == NULL || file == NULL) {
                        printf("Erro ao abrir arquivos.\n");
                        return 1;
                    }

                    while (fscanf(file, "%s %s", file_username, file_password) != EOF) {
                        if (strcmp(file_username, username) == 0) {
                            fprintf(temp_file, "%s %s\n", new_username, file_password); // Atualiza o nome de usu�rio
                        } else {
                            fprintf(temp_file, "%s %s\n", file_username, file_password);
                        }
                    }
                    fclose(file);
                    fclose(temp_file);
                    remove(USER_FILE);
                    rename("temp.txt", USER_FILE);

                    // Atualiza o arquivo de senhas criptografadas
                    FILE *encrypted_temp_file = fopen("encrypted_temp.txt", "w");
                    FILE *encrypted_file = fopen(ENCRYPTED_FILE, "r");
                    if (encrypted_temp_file == NULL || encrypted_file == NULL) {
                        printf("Erro ao abrir arquivos criptografados.\n");
                        return 1;
                    }

                    while (fscanf(encrypted_file, "%s %s", encrypted_username, encrypted_password) != EOF) {
                        decrypt_string(encrypted_username, decrypted_username);
                        if (strcmp(decrypted_username, username) == 0) {
                            encrypt_string(new_username, encrypted_username);
                            fprintf(encrypted_temp_file, "%s %s\n", encrypted_username, encrypted_password); // Atualiza o nome de usu�rio criptografado
                        fprintf(encrypted_temp_file, "%s %s\n", encrypted_username, encrypted_password);
                        }
                    }
                    fclose(encrypted_file);
                    fclose(encrypted_temp_file);
                    remove(ENCRYPTED_FILE);
                    rename("encrypted_temp.txt", ENCRYPTED_FILE);

                    printf("Nome de usu�rio alterado com sucesso!\n");
                } else if (action == 3) { // Excluir usu�rio
                    // Atualiza o arquivo de usu�rios
                    FILE *temp_file = fopen("temp.txt", "w");
                    file = fopen(USER_FILE, "r");
                    if (temp_file == NULL || file == NULL) {
                        printf("Erro ao abrir arquivos.\n");
                        return 1;
                    }

                    while (fscanf(file, "%s %s", file_username, file_password) != EOF) {
                        if (strcmp(file_username, username) != 0) {
                            fprintf(temp_file, "%s %s\n", file_username, file_password); // Mant�m os outros usu�rios
                        }
                    }
                    fclose(file);
                    fclose(temp_file);
                    remove(USER_FILE);
                    rename("temp.txt", USER_FILE);

                    // Atualiza o arquivo de senhas criptografadas
                    FILE *encrypted_temp_file = fopen("encrypted_temp.txt", "w");
                    FILE *encrypted_file = fopen(ENCRYPTED_FILE, "r");
                    if (encrypted_temp_file == NULL || encrypted_file == NULL) {
                        printf("Erro ao abrir arquivos criptografados.\n");
                        return 1;
                    }

                    while (fscanf(encrypted_file, "%s %s", encrypted_username, encrypted_password) != EOF) {
                        decrypt_string(encrypted_username, decrypted_username);
                        if (strcmp(decrypted_username, username) != 0) {
                            fprintf(encrypted_temp_file, "%s %s\n", encrypted_username, encrypted_password); // Mant�m as outras senhas
                        }
                    }
                    fclose(encrypted_file);
                    fclose(encrypted_temp_file);
                    remove(ENCRYPTED_FILE);
                    rename("encrypted_temp.txt", ENCRYPTED_FILE);

                    printf("Usu�rio exclu�do com sucesso!\n");
                }
            } else {
                printf("Nome de usu�rio ou senha incorretos.\n");
            }
        }
    } while (option != 3);

    return 0;
}
