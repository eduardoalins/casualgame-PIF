Projeto da cadeira de Programação Imperativa Funcional (PIF) do segundo período de Ciência da Computação na Cesar School.

**COMO RODAR SNAKE GAME**

-Primeiramente é preciso que você verifique se oS arquivo que estão no Github estão diretório da máquina. Depois disso utilize a seguinte linha de código:
```
gcc –o snakegame main.c keyboard.c screen.c timer.c
```

A seguir, digitr essa segunda linha:

```
./snakegame
```

**COMO JOGAR:**

-Assim que código executar você vai precisar digitar o seu nick (é uma sigla que possui 3 caracteres), o seu nick irá aparecer na scoreboard se você estiver entre os jogadores com as maiores pontuações.
-Logo após isso o jogo iniciará automaticamente, você irá conseguir mexer a cobra usando o W (para cima), A (para a esquerda), S (para baixo) e D (para a direita)
-A cobra é um "X" verde e as maçãs são "0"s vermelhos.

**-OBJETIVO:**

-controlar a cobra e comer o maior número de maçãs possivel sem morrer.
-A cada maçã que a cobra come você ganha 1 ponto.

**-COMO A COBRA PODE MORRER:**

-Se a cobra colidir com a parede ou com o próprio corpo ela morrerá.
-Quando a cobre morrer irá aparecer "Game over" e apareçerá a tabela de pontuações, mostrando quem obteve as maiores pontuações.
