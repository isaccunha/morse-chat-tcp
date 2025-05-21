#include <iostream>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstdlib>   // Para system()
#include <unistd.h>  // Para usleep()

void beep(int freq, int duration_ms) {
    std::string cmd = "play -nq -t alsa synth " + std::to_string(duration_ms/1000.0) + " sine " + std::to_string(freq) + " >/dev/null 2>&1";
    system(cmd.c_str());
}

void bipMorse(const std::string& morse) {
    for(char c : morse){
        if(c == '.'){
            beep(1000, 200); // ponto
        }else if(c == '-'){
            beep(1000, 600); // traço
        }else if(c == ' '){
            usleep(200 * 1000); // Espaço 
            continue;
        }
        usleep(100 * 1000); // Pausa entre letras
    }
}

int main(){
    // Criando socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket < 0) {
        std::cout << "Erro ao criar socket." << std::endl;
        return 0;
    }

    // Vinculando socket
    sockaddr_in service;
    service.sin_family = AF_INET;
    service.sin_addr.s_addr = inet_addr("192.168.1.7");
    service.sin_port = htons(65432);
    
    // Usando funçao bind
    if(bind(serverSocket, (struct sockaddr*)&service, sizeof(service)) < 0){
        std::cout<<"bind() falhou."<<std::endl;
        close(serverSocket);
        return 0;
    }else{
        std::cout<<"bind() funcionou!"<<std::endl;
    }

    // Ouvir conexões
    if(listen(serverSocket, 1) < 0){
        std::cout<<"listen(): Erro ouvindo socket." <<std::endl;
    }else{
        std::cout << "listen() funcionou! Aguardando conexoes..." << std::endl;
    }

    // Aceitar conexões
    int acceptSocket;
    acceptSocket = accept(serverSocket,nullptr,nullptr);

    // Checando conexão
    if(acceptSocket < 0){
        std::cout<<"accept() falhou."<<std::endl;
        close(serverSocket);
        return 0;
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
        if(bytesSent <0){
            std::cout << "Erro ao enviar mensagem "<<std::endl;
            break;
        }
    }

    close(acceptSocket);
    close(serverSocket);

}
