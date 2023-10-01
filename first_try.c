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
	
	//code = malloc(sizeof(f_size));
	code = (char *)malloc(f_size + 1);
	
	if (code == NULL) {
        	fclose(file); // Close the file before returning
        	return NULL;
	}


	while ((c = fgetc(file)) != EOF) {
		code[n++] = (char) c;
	}
	
	code[n] = '\0';
	fclose(file);

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

bool table_check(char* word, int *se_existe_pos) {
	for (size_t i=(RESERVEDKEYWORDS+1); i<tabela_pointer; i++){
		if (strcmp(word, tabela[i]) == 0){
			*se_existe_pos = i;
			return true;
		}
	}
	*se_existe_pos = -1;
	return false;
}

bool checar_formacao_num(char c) {
	char possíveis_fechamentos[] = {' ','\n','=','<','>','(',')','*','/','+','-','^',';',':','.',',',']'};
	for (size_t i=0; i<17;i++)
	{
		if (c == possíveis_fechamentos[i])
			return true;
	}
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
				
				if ((c == ' ')||(c == '\n') || iscntrl(c))
				{
					estado=0;
					cont_sim_lido++;
					break;
				}

				if (isalpha(c)) { 
					p_id = 0; 
					id_str[p_id] = c;
					estado = 9;
					break;
				}

				if (isdigit(c)){
					p_num = 0;
					num_str[p_num] = c;
					estado = 23;
					break;
					
				}

				else if (c == '-') {
					estado = 25;
					break;
				}
				else if (c == '+'){ estado = 27; break;}
				else if (c == '*'){ estado = 29; break;}
				else if (c == '/'){ estado = 30; break;}
				else if (c == '^'){ estado = 31;break;}

				else if (c == '<'){ estado = 1;break;}
				else if (c == '='){ estado = 5;break;}
				else if (c == '>'){ estado = 6;break;}

				else if (c == '(') {estado = 11;break;}
				else if (c == ')') {estado = 12;break;}
				else if (c == '[') {estado = 13;break;}
				else if (c == ']') {estado = 14;break;}
				else if (c == '{') {estado = 15;break;}
				else if (c == '}') {estado = 16;break;}
				
				else if (c == ';') {estado = 17;break;}
				else if (c == ':') {estado = 18;break;}
				else if (c == ',') {estado = 19;break;}
				else if (c == '.') {estado = 20;break;} 
				
				else if (c == '"'){ 
					p_str = 0;
					estado = 22;
					break;
				}

				else if (c == '~') {estado = 3;break;}
				
				else {
					printf("Erro léxico: Um caracter não reconhecido pela linguagem foi inserido\n");
					token.nome_atributo = EOF;
					token.atributo = -1;
					code[cont_sim_lido] = '\0';
					return (token);
				}
				
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
					cont_sim_lido++;
					estado = 0;
					return (token);
				} else {
					printf("Erro léxico: ~ deve ser sucedido de '='\n");
					token.nome_atributo = EOF;
					token.atributo = -1;
					code[cont_sim_lido] = '\0'; 
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
				
				if (c == '=') {
					printf("<relop, EQ>\n");
					token.nome_atributo = RELOP;
					token.atributo = EQ;
					cont_sim_lido++;
					estado=0; 
					return (token);
				} else {  
					printf("<atribuição>\n");
					token.nome_atributo = c;
					estado=0; 
					return (token);

				} 
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
				if (!isalnum(c) && c != '_') 
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

				
				int pos;
				int se_existe_pos;
				bool isKeyword = keyword_check(id_str, &pos);
				bool isOnTable = table_check(id_str, &se_existe_pos); // Caso em que o id já existe na tabela
				if (isKeyword){
					printf("<%s, %d>\n", id_str, pos);
					token.nome_atributo = BASECASEFORKEYWORDS + pos;
					token.nome_atributo = pos;

				} else if (isOnTable) {
					printf("<ID, %d>\n", se_existe_pos);
					token.nome_atributo = ID;
					token.atributo = se_existe_pos;

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

			case 22: 
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

				}else if (c == '\n') {
					printf("Erro Léxico: String não terminada\n");
					token.nome_atributo = EOF;
					token.atributo = -1;
					code[cont_sim_lido] = '\0';
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
				bool checar_numero = checar_formacao_num(c);
				if (checar_numero)
					estado=24;
				else if (isdigit(c)) {
					p_num++;
					num_str[p_num] = c;
					estado=23;
				} else {
					printf("Erro Léxico: má formação do inteiro\n");
					token.nome_atributo = EOF;
					token.atributo = -1;
					code[cont_sim_lido] = '\0';
					return (token);
					break;
				}
				break;
			case 24: // NOTA: Caso o número seja muito longo dá problema
				p_num++;
				num_str[p_num] = '\0';

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
				else if (isdigit(c) || isalpha(c) || c == ' ' || c == '\n') // Pode ser só um sinal de menos
					estado=28;
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
			case 27:
				cont_sim_lido++;
				printf("<+>\n");
				token.nome_atributo = c;
				estado=0;
				return (token);
				break;
			case 28:
				cont_sim_lido++;
				printf("<->\n");
				token.nome_atributo = c;
				estado=0;
				return (token);
				break;
			case 29:
				cont_sim_lido++;
				printf("<*>\n");
				token.nome_atributo = c;
				estado=0;
				return (token);
				break;
			case 30:
				cont_sim_lido++;
				printf("</>\n");
				token.nome_atributo = c;
				estado=0;
				return (token);
				break;
			case 31:
				cont_sim_lido++;
				printf("<^>\n");
				token.nome_atributo = c;
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
	
	if (code == NULL){
		printf("Arquivo não encontrado\n");
	} else {
		while (code[cont_sim_lido] != '\0')
			token = proximo_token();	
	}
	

	
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
