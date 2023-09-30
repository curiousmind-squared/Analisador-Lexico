#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>


// TOKENS
#define RELOP 259
#define ID 260
#define NUM 261
#define STRING 270

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
#define MAXSTRSIZE 200
#define MAXARRSIZE 100

#define RESERVEDKEYWORDS 21

// FIXME: Essa númeração é temporária, quando ajeitarmos os cases e a númeração dos tokens ajeitamos isso 
#define BASECASEFORKEYWORDS 300

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
// Apenas um aviso, o espaço 21 fica vazio, não sei se devemos deixar assim ou não


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

bool keyword_check(char* word, int *pos) {
	for (size_t i=0; i<RESERVEDKEYWORDS;i++) {
		if (strcmp(word, tabela[i]) == 0){
			*pos = i;
			return true;
		}
	}
	*pos = -1;
	return false;
}

Token proximo_token() {

	Token token;
	char c;

	char id_str[IDSIZE];
	int p_id;

	char str[MAXSTRSIZE];
	int p_str;

	char num_str[MAXSTRSIZE];
	int p_num;

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

				if (isdigit(c)){
					p_num = 0;
					num_str[p_num] = c;
					estado = 23;
					
				}

				else if (c == '-') {
					estado = 25;
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
				
				else if (c == '"'){ 
					p_str = 0;
					estado = 22;
				}

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
				int pos;
				bool isKeyword = keyword_check(id_str, &pos);
				if (isKeyword){
					printf("<%s, %d>\n", id_str, pos);
					token.nome_atributo = BASECASEFORKEYWORDS + pos;
					token.nome_atributo = pos;

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

			case 22: // TODO: Adicionar else para caso a string não feche(tem que checar por fim de arquivo), subir um erro
				cont_sim_lido++;
				c = code[cont_sim_lido];
				if (c == '"') {
					str[p_str] = '\0';
					strcpy(tabela[tabela_pointer], str);
					printf("<%s, %zu>\n", str, tabela_pointer);

					token.atributo = STRING;
					token.nome_atributo = tabela_pointer;
					
					cont_sim_lido++;
					tabela_pointer++;
					estado=0;
					return (token);
					break;

				} else {
					str[p_str] = c;
					estado = 22;
					p_str++;
				}
				break;

			case 23:
				cont_sim_lido++;
				c = code[cont_sim_lido];
				if (c == '\n' || c == ' ')
					estado=24;
				else if (isdigit(c)) {
					p_str++;
					num_str[p_str] = c;
					estado=23;
				} 
				// TODO: Tem que ter um estado para caso não seja número, se for letra por exemplo, devemos apresentar um erro
				break;
			case 24: // TODO: Adicionar um case para caso o número seja muito longo retornar problema
				p_str++;
				num_str[p_str] = '\0';

				int num = atoi(num_str);
				
				printf("<NUM, %d>\n", num);

				token.nome_atributo = NUM;
				token.atributo = num;
				estado=0;
				return (token);
				break;

			case 25:
				cont_sim_lido++;
				c = code[cont_sim_lido];
				
				char e = code[(cont_sim_lido+1)];

				if (c == '-' && e != '[') // Comentário curto
					estado=26;
				// TODO: Aqui teremos que ter um erro
				break;
			case 26:
				cont_sim_lido++;
				c = code[cont_sim_lido];

				if (c == '\n'){
					cont_sim_lido++;
					estado=0;
				}
					
				else
					estado=26;
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
	printf("\n"); // FIXME: Isso resolve um bug muito esquisito que aparece
	              // Se quiser reproduzir basta pegar o trecho em TODO.md e remover esse print

	code = readFile("src.txt"); //TODO: Esse arquivo deve ser passado como parâmetro 

	while (code[cont_sim_lido] != '\0')
		token = proximo_token();	

	printf("\n"); // Adicionei isso apenas para ficar organizado com o print de cima, se o bug for 
	 	      // resolvido pode remover
	
	/*
	// Vamos printar a tabela de simbolos
	printf("Tabela de simbolos");
	for (size_t i = 0; i<tabela_pointer; i++){
		printf("%zu - %s\n",i,  tabela[i]);
	}
	*/

	free(code);

	return 0;
}
