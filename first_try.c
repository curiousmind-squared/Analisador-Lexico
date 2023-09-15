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
				
				// TODO: Add teste para c == ' ' e c == '\n'

				if (c == '<') estado = 1;
				else if (c == '=') estado = 5;
				else if (c == '>') estado = 6;

				else if (c == '~') estado = 30; // NOTE: Adição minha

				// TODO: Add estado para falhar
				
				break;

			case 1:
				cont_sim_lido++;
				
				c = code[cont_sim_lido];

				if (c == '=') estado = 2;
				else estado = 4;
				break;
				
			case 2:
				cont_sim_lido++;
				printf("<relop, LE>\n");
				token.nome_atributo = RELOP;
				token.atributo = LE;
				estado = 0;
				return (token);
				break;

			case 3:
				cont_sim_lido++;
				printf("<relop, NE>\n");
				token.nome_atributo = RELOP;
				token.atributo = NE;
				estado = 0;
				return (token);
				break;

			case 4:
				cont_sim_lido++;
				printf("<relop, LT>\n");
				token.nome_atributo = RELOP;
				token.atributo = LT;
				estado = 0;
				return (token);
				break;

			case 5:
				cont_sim_lido++;
				printf("<relop, EQ>\n");
				token.nome_atributo = RELOP;
				token.atributo = EQ;
				estado = 0;
				return (token);
				break;
			case 6:
				cont_sim_lido++;
				
				c = code[cont_sim_lido];

				if (c == '=') estado = 7;
				else estado = 8;
				break;
			case 7:
				cont_sim_lido++;
				printf("<relop, GE>\n");
				token.nome_atributo = RELOP;
				token.atributo = GE;
				estado = 0;
				return (token);
				break;
			case 8:
				cont_sim_lido++;
				printf("<relop, GT>\n");
				token.nome_atributo = RELOP;
				token.atributo = GT;
				estado = 0;
				return (token);
				break;

							
			// Está mais separado do resto do código porque é uma adição minha	
			case 30:
				cont_sim_lido++;
				 
				c = code[cont_sim_lido];

				if (c == '=') estado = 3;
				break;
		}
	}

	return token;
}


int main() {
	Token token;
	
	code = readFile("src.txt"); //TODO: Esse arquivo deve ser passado como parâmetro 
	
	token = proximo_token();	

	free(code);

	return 0;
}
