
# Instalação 

O framework possui duas depêndencias que devem ser instaladas
 - Glut:      para gerenciamento de janelas, teclado, ...
 - FreeImage: para salvamento de capturas da tela
```console
sudo apt-get install freeglut3-dev libfreeimage-dev
``` 

#    Como compilar e rodar o programa

Vá até a pasta warehouse-framework e digite:
```console
make
```
o programa vai ser compilado em uma pasta build/ (dentro da pasta 'warehouse-framework')

Para rodar digite:
```console
./build/program -m NOMEDOMAPA.txt wave.txt
```
onde NOMEDOMAPA.txt é um mapa existente na pasta maps/ 

por exemplo:  
```console
./build/program -m ml_a.txt wave.txt
```

# Controles do framework

- w a s d : move a camera

- '+' '-'     : altera zoom da camera

- v b     : muda modos de visualização

- espaço  : pausa/despausa a simulação

- n       : quando pausada, executa a próxima iteração da simulação (passo-a-passo)

- ESC     : fecha programa

