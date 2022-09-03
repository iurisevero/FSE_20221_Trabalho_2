# FSE 2022.1 - Trabalho 2
Repositório para desenvolvimento do trabalho 2 da disciplina de Fundamentos de Sistemas Embarcados, 2022.1

## Compilação e Execução

Para compilação do código basta executar o comando ```make```, ele irá compilar utilizando C++14 e criará um executável na pasta ```bin/```.

Caso a compilação seja bem sucedida, o programa poderá ser executado utilizando o comando ```make run```. Nenhum parâmetro é necessário.

Após a execução, é possível limpar os arquivos utilizando ```make clean```.

## Dentro do programa

Após a execução, o programa ficará executando em loop até que seja enviado um sinal de interrupção (Ctrl + C = sinal SIGINT).

O comando de usuário seguem as explicações definidas no [enunciado do trabalho](https://gitlab.com/fse_fga/trabalhos-2022_1/trabalho-2-2022-1).

### Comando Menu (0x07)

O comando de menu coloca o programa em modo de alimentos pré-programados. Não é possível alterar a temperatura pré-definida e nem a escolha de alimento após inicialização do aquecimento. A alteração de tempo continua livre.

Alimentos pré-programados
| Nome | Temperatura | Tempo |
| ---- | ----------- | ----- |

### Dados

Os dados do programa são salvos a cada segundo em um arquivo CSV, ```dados.csv``` que ficará salvo na pasta raiz do projeto após a primeira execução.

## Testes

![Graficos](dataExamples/Screenshot%20from%202022-09-03%2017-43-15.png)

A foto acima representa o gráfico da execução salva no arquivo CSV ```dataExample.csv```

Apenas um teste foi realizado.
