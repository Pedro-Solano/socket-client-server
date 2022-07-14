# Programas cliente-servidor que utilizam a interface POSIX de sockets de redes e protocolo TCP.

## Parâmetros para execução:

O programa servidor recebe dois parâmetros: tipo de endereço (<v4> ou <v6>) e o número da porta.

    Ex.: $ ./server v4 51511
    

O programa cliente recebe dois parâmetros: o endereço IP e a porta do servidor para estabelecimento da conexão.

    Ex.: $ ./client 127.0.0.1 51511

## Funções implementadas:

O programa servidor implementa as funções "factorial" e "fib", que calculam o fatorial e o n-ésimo número da sequência de Fibonacci, respectivamente.
O valor calculado é impresso na tela da máquina servidor e da máquina cliente.
As funções são invocadas com os comandos "fat" e "fib", respectivamente, seguidos de um número inteiro.

Exemplo:

  Na máquina cliente: 
    
    $ fat 5
    Imprimirá na tela: 120
    
    $ fib 10
    Imprimirá na tela: 55
  
## Encerramento da conexão:

No terminal cliente:
O comando "exit" encerra a conexão com o servidor, o qual permanece disponível para nova conexão.
O comando "close" encerra a conexão e também encerra a execução do programa servidor.
