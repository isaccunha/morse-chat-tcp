# 🛰️ Telégrafo TCP-IP em C++

Este projeto implementa um sistema de chat via terminal utilizando **código Morse** entre duas máquinas conectadas por **TCP/IP**. As mensagens são convertidas em sons audíveis, simulando um antigo telégrafo.

## 🎯 Objetivo

Permitir a comunicação em **código Morse** entre cliente e servidor, em plataformas **Windows** ou **Linux**, com som gerado localmente para cada caractere enviado/recebido.

## 📂 Arquivos

- `server_win.cpp`: Código-fonte do servidor para Windows.
- `server_linux.cpp`: Código-fonte do servidor para Linux (com som via `sox`).
- `client_win.cpp`: Código-fonte do cliente para Windows.
- `client_linux.cpp`: Código-fonte do cliente para Linux (com som via `sox`).

---

## ⚙️ Pré-requisitos

### Windows

- [MinGW](https://www.mingw-w64.org) ou outro compilador `g++` com suporte a `winsock2`.
- Nenhuma biblioteca extra é necessária.

### Linux

- `g++`
- `sox` e `libsox-fmt-all` (para reprodução sonora em modo terminal)

Instale com:
```bash
sudo apt update
sudo apt install sox libsox-fmt-all
```

---

## 🚀 Como compilar e executar

### 🪟 No Windows

#### Compilar

Servidor:

```bash
g++ server_win.cpp -o server_win -lws2_32
```

Cliente:
```bash
g++ client_win.cpp -o client_win -lws2_32
```

#### Executar

Servidor:

1. Inicie o servidor:

    ```bash
    server_win.exe
    ```

2. Aguarde a conexão de um cliente.

Cliente:

1. Garanta que há um servidor iniciado.

2. Inicie o cliente:

    ```bash
    client_win.exe
    ```

---

### 🐧 No Linux

#### Compilar cliente

Servidor:

```bash
g++ server_linux.cpp -o server_linux
```

Cliente:
```bash
g++ client_linux.cpp -o client_linux
```

#### Executar 

Servidor:

1. Inicie o servidor:

    ```bash
    ./server_linux
    ```

2. Aguarde a conexão de um cliente.

Cliente:

1. Garanta que há um servidor iniciado.

2. Inicie o cliente:

    ```bash
    ./client_linux
    ```

---

## 📝 Funcionamento

- O **cliente sempre envia a primeira mensagem**.
- As mensagens devem ser digitadas em **código Morse**, utilizando:
  - `.` para ponto
  - `-` para traço
  - ` ` (espaço) para separação de letras
- A palavra `"sair"` encerra a comunicação.

### Exemplo:
```
Voce (digite Morse ou 'sair'): .... . .-.. .-.. ---   (HELLO)
```

---

## 🔊 Sobre os sons

- **Windows**: Usa a função `Beep(frequência, duração)` da API `windows.h`.
- **Linux**: Usa o utilitário `play` da biblioteca `sox` para gerar o som.

### Durações:
- `.` (ponto) → 200ms
- `-` (traço) → 600ms
- Espaço entre letras → 100ms
- Espaço entre palavras → 200ms

---

## 📡 Conexão

- O IP usado nos exemplos é `192.168.1.7` e a porta `65432`.
- Ambos devem estar na **mesma rede local**.
- Para testar em duas máquinas, altere `inet_addr("192.168.1.7")` para o ip local da máquina que será o servidor.
- Para testar em uma máquina só (localhost), altere `inet_addr("192.168.1.7")` para `inet_addr("127.0.0.1")` e execute o servidor em um terminal e o cliente em outro.

---

## 📌 Possíveis melhorias futuras

- Tradução automática de texto comum para código Morse.
- Interface gráfica.
- Uso de threads para comunicação simultânea.
- Uso do mouse como um telégrafo

---

## 📚 Créditos

Desenvolvido como exercício educacional em C++ com foco em:
- Sockets TCP/IP
- Comunicação entre sistemas operacionais
- Manipulação de áudio via terminal

Desenvolvido por [Isac Cunha](https://github.com/isaccunha).
