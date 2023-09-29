- Adicionar retorno para a palavra reservada: Será retornada a palavra e sua posição na tabela de símbolos
- Reorganizar a numeração dos cases
	- Reorganizar da forma que a professora quer, utilizando os estados "Falhar" para trocar de máquinas.
	- Case '0' serve para checar apena soperadores, case '1' serve praa id's e etc. Quando falhamos em um jogamos para o outro
- Adicionar o reconhecimento de strings
	- Basta ler até encontrar as aspas duplas novamente, depois salvar a string na tabela de símbolos e retornar a string e sua posição na tabela
- Adicionar reconhecimento de comentários de uma linha e de múltiplas linhas
	- Comentário longo começa com '--[[' e termina com ']]'
	- Lembrar que aninhamento deve ser possível. Pode ser resolvido com uma pilha ou um contador
	- 
	
- Não será preciso reconhecer pontos flutuantes, apenas inteiros 
- Caso encontre '~' e depois não encontre um '=', deve ser apontado um erro, esse caso deve ser estudo para ser generalizado
- Ajeitar os 'TODO's' e 'FIXME's' do código
- Bug:

(Copiar a partir de um editor de texto, md colapsa tudo errado)

<=
absac
while
for
a
a == abs"

