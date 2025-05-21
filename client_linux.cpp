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
    int clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket < 0) {
        std::cout << "Erro ao criar socket." << std::endl;
        return 0;
    }

    // Conectando ao servidor
    sockaddr_in clientService;
    clientService.sin_family = AF_INET;
    clientService.sin_addr.s_addr = inet_addr("192.168.1.7");
    clientService.sin_port = htons(65432);
    
    // Usando a função connect
    if (connect(clientSocket, (struct sockaddr*)&clientService, sizeof(clientService)) < 0) {
        std::cout << "Erro ao conectar ao servidor." << std::endl;
        close(clientSocket);
        return 0;
    } else {
        std::cout << "Conectado ao servidor!" << std::endl;
    }

    // Chat
    char buffer[1024];
    while(true){
        // Enviar mensagem
        std::string mensagem;
        std::cout << "Voce (digite Morse ou 'sair'): ";
        std::getline(std::cin, mensagem);

        int bytesSent = send(clientSocket, mensagem.c_str(), mensagem.size(),0);
        if(bytesSent < 0){
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

    close(clientSocket);
    return 0;
}