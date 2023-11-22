*Projeto da cadeira de Programação Imperativa Funcional (PIF) do segundo período de Ciência da Computação na Cesar School.*

**COMO RODAR SNAKE GAME**

-Primeiramente é preciso que você verifique se os arquivo que estão no Github estão diretório da máquina. Depois disso utilize a seguinte linha de código:
```
gcc – Snake-PIF main.c keyboard.c screen.c timer.c
```

A seguir, digite essa segunda linha:

```
./Snake-PIF
```

**COMO JOGAR:**

-Assim que o código executar você vai precisar digitar seu nick (uma sigla que possui 3 caracteres), o seu nick irá aparecer na scoreboard caso você esteja entre os jogadores com as maiores pontuações.

-Logo após o jogo iniciará automaticamente, com 3 segundos de delay, você irá conseguir mexer a cobra usando o W (para cima), A (para a esquerda), S (para baixo) e D (para a direita)

-A cobra é um "X" ou uma sequência de "X"s verdes e as maçãs são "0"s vermelhos.

**-OBJETIVO:**

-controlar a cobra e comer o maior número de maçãs possivel sem morrer.

-A cada maçã que a cobra come você ganha 1 ponto e aumenta de tamanho.

**-COMO A COBRA PODE MORRER:**

-Se a cobra colidir com a parede ou com o próprio corpo ela morrerá.

-Quando a cobra morrer irá aparecer "Game over" e apareçerá a tabela de pontuações, mostrando quem obteve as maiores pontuações.
