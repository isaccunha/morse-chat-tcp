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
    SOCKET serverSocket;
    serverSocket = INVALID_SOCKET;
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // Checando criação do socket
    if(serverSocket == INVALID_SOCKET){
        std::cout<<"Erro em socket(): "<< WSAGetLastError() <<std::endl;
        WSACleanup();
        return 0;
    }else{
        std::cout<<"Socket criado corretamente!"<<std::endl;
    }

    // Vinculando socket
    sockaddr_in service;
    service.sin_family = AF_INET;
    service.sin_addr.s_addr = inet_addr("192.168.1.7"); // IP DESEJADO
    service.sin_port = htons(65432); // PORTA ESCOLHIDA

    // Usando funçao bind
    if(bind(serverSocket, reinterpret_cast<SOCKADDR*>(&service), sizeof(service)) == SOCKET_ERROR){
        std::cout<<"bind() falhou: "<< WSAGetLastError() <<std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 0;
    }else{
        std::cout<<"bind() funcionou!"<<std::endl;
    }

    // Ouvir conexões
    if(listen(serverSocket, 1) == SOCKET_ERROR){
        std::cout<<"listen(): Erro ouvindo socket: "<< WSAGetLastError() <<std::endl;
    }else{
        std::cout << "listen() funcionou! Aguardando conexoes..." << std::endl;
    }

    // Aceitar conexões
    SOCKET acceptSocket;
    acceptSocket = accept(serverSocket,nullptr,nullptr);

    // Checando conexão
    if(acceptSocket == INVALID_SOCKET){
        std::cout<<"accept() falhou: "<< WSAGetLastError() <<std::endl;
        WSACleanup();
        return -1;
    }else{
        std::cout<<"accept() funcionou! Conexao estabelecida!"<<std::endl;
    }

    // Chat
    char buffer[1024];
    while(true){
        // Receber mensagem
        int bytesReceived = recv(acceptSocket, buffer, sizeof(buffer)-1, 0);
        if(bytesReceived<=0){
            std::cout << "Erro. Cliente desconectado." << std::endl;
            break;
        }
        buffer[bytesReceived] = '\0'; // adiciona terminador
        if(strcmp(buffer, "sair") == 0) break;
        std::cout << "Cliente: " << buffer << std::endl;
        bipMorse(buffer);
        
        // Enviar mensagem
        std::string resposta;
        std::cout << "Voce (digite Morse ou 'sair'): ";
        std::getline(std::cin, resposta);
        if(resposta == "sair") break;

        int bytesSent = send(acceptSocket, resposta.c_str(), resposta.size(), 0);
        if(bytesSent == SOCKET_ERROR){
            std::cout << "Erro ao enviar mensagem "<<std::endl;
            break;
        }
    }

    closesocket(acceptSocket);
    closesocket(serverSocket);
    WSACleanup();

    return 0;
}


