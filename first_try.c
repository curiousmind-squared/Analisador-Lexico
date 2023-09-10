#include <stdio.h>
#include <stdlib.h>

// TOKENS
#define IF 256;
#define THEN 257;
#define ELSE 258;
#define RELOP 259;
#define ID 260;
#define NUM 261;

// ATRIBUTOS
#define LT 262;
#define LE 263;
#define EQ 264;
#define NE 265;
#define GT 266;
#define GE 267;

typedef struct {
	int nome_atributo;
	int atributo;
} Token;

int estado = 0;
int partida = 0;
int cont_sim_lido = 0;
char *code; 

char *readFile(char *fileName) {
	FILE *file = fopen(fileName, "r");
	char *code;
	int n = 0;
	int c;

	if (file == NULL) return NULL;

	fseek(file, 0, SEEK_END);
	long f_size = ftell(file);
	fseek(file, 0, SEEK_SET);
	
	code = malloc(sizeof(f_size));

	while ((c = fgetc(file)) != EOF) {
		code[n++] = (char) c;
	}
	code[n] = '\0';
	return code;

}

Token proximo_token() {

	Token token;
	char c;

	while (code[cont_sim_lido] != '\0'){
		switch (estado)
		{
			case 0:
				c = code[cont_sim_lido];
				if (c == '>') estado = 1;

			case 1:
				cont_sim_lido++;
				c = code[cont_sim_lido];

				if (c == '=') estado = 2;
			case 2:
				cont_sim_lido++;
				printf("<relop, LE>\n");
				token.nome_atributo = RELOP;
				token.atributo = LE;
				estado = 0;
				return (token);
				break;

		}
	}

	return token;
}


int main() {
	Token token;
	
	code = readFile("teste.txt");
	
	token = proximo_token();	
	printf("%d, %d\n", token.nome_atributo, token.atributo);
	printf("Alteração para teste com git");
	free(code);
	return 0;
}
