Estou desenvolvendo um projeto final para a disciplina de Computação para
Engenharia (curso de Engenharia de Redes de Comunicação, UnB). Esta é a
primeira mensagem do nosso chat — venho de outro contexto onde já tomei
todas as decisões abaixo.

## Sobre o projeto

- **Nome:** jogo Pong multiplayer com foco em netcode (camada de rede)
- **Linguagem:** C++ sem POO (sem class, herança, métodos) — estilo procedural
  com struct e funções livres
- **Justificativa acadêmica:** projeto de Engenharia de Redes. O foco
  pedagógico não é a complexidade do jogo, mas a implementação do netcode.
- **Arquitetura:** cliente-servidor autoritativa sobre UDP, servidor é
  fonte única da verdade do estado do jogo.
- **Gráficos:** Raylib (apenas no cliente).
- **Rede:** sockets BSD puros (`<sys/socket.h>`), sem biblioteca de rede.
- **Sistema:** desenvolvimento em WSL Ubuntu 26.04, deploy do servidor
  numa VPS Linux que eu tenho.

## Restrições importantes

- Sem POO. C++ no subconjunto procedural (structs + funções livres).
- Sem biblioteca de rede (sem ENet, sem ASIO). Só sockets BSD puros.
- Não usar threads — preferir socket UDP em modo não-bloqueante com
  `fcntl(sock, F_SETFL, O_NONBLOCK)`.
- Sem dependências externas além do Raylib.

## Funcionalidades planejadas

- Menu inicial: "Criar Sala" (host) e "Entrar Sala" (com senha)
- Matchmaking via senha — servidor mantém lista de salas em memória
- Multiplayer de 2 jogadores
- Netcode: client-side prediction, server reconciliation, entity
  interpolation
- Simulador de condições de rede (latência, jitter, perda de pacotes
  configuráveis) — feature para demonstração na defesa

## Estrutura de pastas decidida

pong-net/

├── src/

│   ├── comum/

│   │   ├── protocolo.hpp     # structs de pacote

│   │   ├── rede.hpp          # funções de socket

│   │   ├── logica.hpp     # declarações da lógica do jogo

│   │   └── logica.cpp     # implementação da lógica do jogo

│   ├── servidor/

│   │   └── servidor.cpp      # sem Raylib

│   └── cliente/

│       └── cliente.cpp       # com Raylib

├── Makefile

└── README.md

A lógica do jogo (`logica.cpp/hpp`) é compartilhada entre cliente e
servidor — o servidor roda como autoridade, o cliente usa a MESMA função
para client-side prediction. Isso garante simulação determinística.

## Status atual

- Já tenho protótipo offline básico do Pong em Raylib funcionando
  (bola, dois bastões, colisões básicas com paredes e bastões, contagem
  regressiva)
- Já fiz um exercício separado de eco UDP cliente-servidor para entender
  sockets
- Vou agora começar o projeto final do zero usando a estrutura acima
- Estou na Fase 1: implementar o jogo completo offline com a nova
  estrutura modular antes de adicionar rede

## Roadmap por fases

1. Jogo offline completo, dois jogadores no mesmo teclado, com estrutura
   modular pronta (simulacao.cpp separado de cliente.cpp)
2. Menu local com botões "Criar Sala" / "Entrar Sala" e campo de senha
   (visual, ainda sem rede)
3. Rede básica: servidor recebe inputs, envia estado. Sem matchmaking
   ainda — servidor aceita os 2 primeiros que conectarem.
4. Matchmaking funcional: senhas, salas, pareamento.
5. Netcode avançado: prediction, reconciliation, interpolation
6. Simulador de condições adversas + deploy na VPS

## Como quero trabalhar com você

Não quero "vibecoding". Quero entender cada decisão para defender o
projeto na avaliação. Prefiro que você:

- Aponte problemas em código que eu escrevi, em vez de gerar tudo
- Explique o porquê de sugestões antes de aplicá-las
- Pergunte antes de criar arquivos novos ou tomar decisões de arquitetura
- Me ensine, não substitua meu raciocínio

Pode começar perguntando em que ponto da Fase 1 estou agora.