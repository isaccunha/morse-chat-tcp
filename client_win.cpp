#include <iostream>
#include <winsock2.h>
#include <windows.h>

// Função que lê uma string morse (..-- -.--) e faz o computador beepar 
void bipMorse(const std::string& morse){
    for(char c : morse){
        if(c == '.'){
            Beep(1000, 200); // .
        }else if(c == '-'){
            Beep(1000, 600); // -
        }else if(c == ' '){
            Sleep(200); // Espaço
        }
        Sleep(100); // Pausa entre letras
    }
}

int main(){
    // Inicializando variaveis WSA
    WSADATA wsaData;
    int wsaerr;
    WORD wVersionRequested = MAKEWORD(2,2);
    wsaerr = WSAStartup(wVersionRequested, &wsaData);

    // Checando inicialização
    if (wsaerr != 0 ){
        std::cout<<"Dll Winsock nao encontrada!"<<std::endl;
    }else{
        std::cout<<"Dll Winsock encontrada!"<<std::endl;
        std::cout<<"Status: "<<wsaData.szSystemStatus<<std::endl;
    }

    // Criando socket
    SOCKET clientSocket;
    clientSocket = INVALID_SOCKET;
    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // Checando criação do socket
    if(clientSocket == INVALID_SOCKET){
        std::cout<<"Erro em socket(): "<< WSAGetLastError() <<std::endl;
        WSACleanup();
        return 0;
    }else{
        std::cout<<"Socket criado corretamente!"<<std::endl;
    }

    // Conectando ao servidor
    sockaddr_in clientService;
    clientService.sin_family = AF_INET;
    clientService.sin_addr.s_addr = inet_addr("192.168.1.7"); // IP DESEJADO
    clientService.sin_port = htons(65432); // PORTA ESCOLHIDA

    // Usando a função connect
    if(connect(clientSocket, reinterpret_cast<SOCKADDR*>(&clientService), sizeof(clientService)) == SOCKET_ERROR){
        std::cout<<"Client: connect() - Falha ao conectar: "<< WSAGetLastError() <<std::endl;
        WSACleanup();
        return 0;
    }else{
        std::cout<<"Client: Conexao realizada!"<<std::endl;
    }

    // Chat
    char buffer[1024];
    while(true){
        // Enviar mensagem
        std::string mensagem;
        std::cout << "Voce (digite Morse ou 'sair'): ";
        std::getline(std::cin, mensagem);

        int bytesSent = send(clientSocket, mensagem.c_str(), mensagem.size(),0);
        if(bytesSent == SOCKET_ERROR){
            std::cout << "Erro ao enviar mensagem "<<std::endl;
            break;
        }

        if(mensagem == "sair") break;

        // Receber mensagem
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer)-1, 0);
        if(bytesReceived<=0){
            std::cout << "Erro. Servidor desconectado." << std::endl;
            break;
        }
        buffer[bytesReceived] = '\0'; // adiciona terminador
        if(strcmp(buffer, "sair") == 0) break;
        std::cout << "Servidor: " << buffer << std::endl;
        bipMorse(buffer);
    }

    closesocket(clientSocket);
    WSACleanup();
    return 0;
}