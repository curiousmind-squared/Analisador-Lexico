#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>

// TODO: Palavras reservadas não precisam ser macros, podemos calcular utilizando alguma fórmula 
//       que limite o range(280-301 por exemplo)

// TOKENS
#define IF 256
#define THEN 257
#define ELSE 258
#define RELOP 259
#define ID 260
#define NUM 261

// ATRIBUTOS
#define LT 262
#define LE 263
#define EQ 264
#define NE 265
#define GT 266
#define GE 267
#define TWODOTS 268

// SIZES
#define IDSIZE 20 
#define MAXSTRSIZE 20
#define MAXARRSIZE 100

#define RESERVEDKEYWORDS 21

typedef struct {
	int nome_atributo;
	int atributo;
} Token;

// Tabela de símbolos
char tabela[MAXARRSIZE][MAXSTRSIZE] = {
		"and",
		"break",
		"do",
		"else",
		"elseif",
		"end",
		"false",
		"for",
		"function",
		"if",
		"in",
		"local",
		"nil",
		"not",
		"or",
		"repeat",
		"return",
		"then",
		"true",
		"until",
		"while",
};


int estado = 0;
int partida = 0;
int cont_sim_lido = 0;
char *code; 

size_t tabela_pointer = 22; // 21 palavras reservadas, a var começa apontando para o próximo espaço livre na tabela


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

bool keyword_check(char* word) {
	for (size_t i=0; i<RESERVEDKEYWORDS;i++) {
		if (strcmp(word, tabela[i]) == 0)
			return true;
	}
	return false;
}

Token proximo_token() {

	Token token;
	char c;

	char id_str[IDSIZE];
	int p_id;

	while (code[cont_sim_lido] != '\0'){
		switch (estado)
		{
			case 0:
				c = code[cont_sim_lido];
				
				if ((c == ' ')||(c == '\n'))
				{
					estado=0;
					cont_sim_lido++;
				}

				if (isalpha(c)) { 
					p_id = 0; // Primeira posição do char
					id_str[p_id] = c;
					estado = 9;
				}
				else if (c == '<') estado = 1;
				else if (c == '=') estado = 5;
				else if (c == '>') estado = 6;

				else if (c == '(') estado = 11;
				else if (c == ')') estado = 12;
				else if (c == '[') estado = 13;
				else if (c == ']') estado = 14;
				else if (c == '{') estado = 15;
				else if (c == '}') estado = 16;
				
				else if (c == ';') estado = 17;
				else if (c == ':') estado = 18;
				else if (c == ',') estado = 19;
				else if (c == '.') estado = 20; // Aqui tem que ficar esperto por é um pouco diferente

				else if (c == '~') estado = 3; // NOTE: Adição minha

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
				c = code[cont_sim_lido];
				if (c == '=') {
					printf("<relop, NE>\n");
					token.nome_atributo = RELOP;
					token.atributo = NE;
					estado = 0;
					return (token);
				}
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
				c = code[cont_sim_lido];
				// TODO: Isso está certo, porém esse 'if' deve virar um estado novo
				if (c == '=') {
					printf("<relop, EQ>\n");
					token.nome_atributo = RELOP;
					token.atributo = EQ;
					//estado=0; // Isso estava causando um bug
					return (token);
				}
				estado=0; // Por isso isso veio para cá
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
			case 9:
				cont_sim_lido++;
				c = code[cont_sim_lido];
				if (c == '\n' || c == ' ')
					estado = 10;
				else {
					p_id++;
					id_str[p_id] = c;
					estado = 9;
				}
				break;

			case 10:
				p_id++;
				id_str[p_id] = '\0';

				// Checamos se a string é uma palavra reservada
				bool isKeyword = keyword_check(id_str);
				if (isKeyword){
					printf("<%s>\n", id_str);
					// TODO: Implementar uma forma de entender qual código 
					//       deverá ser retornado para token.nome_atributo
					//token.nome_atributo = numero_do_atributo();
					// IF == x; ELSE == x+1 e etc.
				}	
				else {
					strcpy(tabela[tabela_pointer], id_str);
					printf("<ID, %zu>\n", tabela_pointer);
					token.nome_atributo = ID;
					token.atributo = tabela_pointer; 
					tabela_pointer++;
				}
				estado=0;
				return (token);
				break;							
			case 11:
				cont_sim_lido++;
				printf("<(>\n");
				token.nome_atributo = c;
				estado=0;
				return (token);
				break;
			case 12:
				cont_sim_lido++;
				printf("<)>\n");
				token.nome_atributo = c;
				estado=0;
				return (token);
				break;
			case 13:
				cont_sim_lido++;
				printf("<[>\n");
				token.nome_atributo = c;
				estado = 0;
				return (token);
				break;
				
			case 14:
				cont_sim_lido++;
				printf("<]>\n");
				token.nome_atributo = c;
				estado = 0;
				return (token);
				break;
			case 15: 
				cont_sim_lido++;
				printf("<{>\n");
				token.nome_atributo = c;
				estado = 0;
				return (token);
				break;
			case 16:
				cont_sim_lido++;
				printf("<}>\n");
				token.nome_atributo = c;
				estado = 0;
				return (token);
				break;
			case 17:
				cont_sim_lido++;
				printf("<;>\n");
				token.nome_atributo = c;
				estado = 0;
				return (token);
				break;
			case 18:
				cont_sim_lido++;
				printf("<:>\n");
				token.nome_atributo = c;
				estado = 0;
				return (token);
				break;
			case 19:
				cont_sim_lido++;
				printf("<,>\n");
				token.nome_atributo = c;
				estado = 0;
				return (token);
				break;
			case 20:
				cont_sim_lido++;
				char d;
				d = code[cont_sim_lido];
				if (d == '.') estado = 21;
				else {
					printf("<.>\n");
					token.nome_atributo = c;
			        	estado = 0;
					return (token);
				}
				break;
			case 21:
				cont_sim_lido++;
				printf("<..>\n");
				token.nome_atributo = TWODOTS;
				estado=0;
				return (token);
				break;
		}
	}

 	// Imagino que essa inicialização seja para possíveis erros
	token.nome_atributo = EOF;
	token.atributo = -1;
	return token;
}


int main() {
	Token token;
	
	code = readFile("src.txt"); //TODO: Esse arquivo deve ser passado como parâmetro 
	
	while (code[cont_sim_lido] != '\0')
		token = proximo_token();	

	free(code);

	return 0;
}
