#include "../include/Server.hpp"
#include "../include/Client.hpp"
#include "../include/Msj.hpp"
#include "../include/Channel.hpp"
#include <vector>
#include <sstream>
#include <fstream>




void startFileTransfer(const std::string &filename, int port)// this function : Crée un serveur qui envoie un fichier via un socket TCP.
{
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror(" Échec de la création du socket");
        return;
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror(" Échec du bind");
        close(server_fd);
        return;
    }

    if (listen(server_fd, 1) < 0) {
        perror(" Échec du listen");
        close(server_fd);
        return;
    }

    std::cout << " Serveur DCC SEND en attente sur le port " << port << "..." << std::endl;

    new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
    if (new_socket < 0) {
        perror(" Échec de l'accept");
        close(server_fd);
        return;
    }

    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        perror(" Impossible d'ouvrir le fichier");
        close(new_socket);
        close(server_fd);
        return;
    }

    char buffer[1024];
    while (!file.eof()) {
        file.read(buffer, sizeof(buffer));
        size_t bytesRead = file.gcount();
        send(new_socket, buffer, bytesRead, 0);
    }

    file.close();
    close(new_socket);
    close(server_fd);
    std::cout << " Transfert de fichier terminé." << std::endl;
}

void receiveFile(const std::string &ip, int port)// this : Se connecte à ce serveur et reçoit le fichier envoyé.
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror(" Échec de la création du socket");
        return;
    }

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    if (inet_pton(AF_INET, ip.c_str(), &server.sin_addr) <= 0) {
        perror(" Adresse IP invalide");
        close(sock);
        return;
    }

    std::cout << " Connexion au serveur DCC sur " << ip << ":" << port << "..." << std::endl;

    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) == -1)
    {
        perror(" Échec de la connexion");
        close(sock);
        return;
    }

    std::ofstream outputFile("received_file.txt", std::ios::binary);
    if (!outputFile)
    {
        perror(" Impossible de créer le fichier");
        close(sock);
        return;
    }

    char buffer[1024];
    int bytesReceived;
    while ((bytesReceived = recv(sock, buffer, sizeof(buffer), 0)) > 0) {
        outputFile.write(buffer, bytesReceived);
    }

    outputFile.close();
    close(sock);
    std::cout << " Fichier reçu avec succès et sauvegardé sous 'received_file.txt'." << std::endl;
}



struct FileTransferArgs {
    std::string filename;
    int port;
};
//startFileTransferWrapper / receiveFileWrapper → Wrappers pour utiliser pthread_create.
void* startFileTransferWrapper(void* arg) {
    FileTransferArgs* args = static_cast<FileTransferArgs*>(arg);
    startFileTransfer(args->filename, args->port);
    delete args;
    return NULL;
}

struct ReceiveFileArgs {
    std::string ip;
    int port;
};

void* receiveFileWrapper(void* arg) {
    ReceiveFileArgs* args = static_cast<ReceiveFileArgs*>(arg);
    receiveFile(args->ip, args->port);
    delete args;
    return NULL;
}
//handleDCCSend → Détecte un message IRC DCC SEND, extrait les infos et lance deux threads pour gérer le transfert.
void handleDCCSend(Client &sender, const std::string &message) {
    (void)sender;

    std::istringstream iss(message);
    std::string command, target, dcc_line;

    iss >> command >> target;
    std::getline(iss, dcc_line);

    if (dcc_line[0] == ' ')
        dcc_line = dcc_line.substr(1);
    if (dcc_line[0] == ':')
        dcc_line = dcc_line.substr(1);

    std::istringstream dcc_stream(dcc_line);
    std::string dcc, send, filename, ip_str, port_str, size_str;

    dcc_stream >> dcc >> send >> filename >> ip_str >> port_str >> size_str;

    if (dcc != "DCC" || send != "SEND") {
        std::cout << " Format DCC SEND invalide !" << std::endl;
        return;
    }

    int port = std::atoi(port_str.c_str());
    std::cout << " DCC SEND détecté !  Fichier: " << filename << ",  Port: " << port << std::endl;
    pthread_t senderThread;
    FileTransferArgs* senderArgs = new FileTransferArgs;
    senderArgs->filename = filename;
    senderArgs->port = port;
    pthread_create(&senderThread, NULL, startFileTransferWrapper, senderArgs);

    sleep(2);
    pthread_t receiverThread;
    ReceiveFileArgs* receiverArgs = new ReceiveFileArgs;
    receiverArgs->ip = ip_str;
    receiverArgs->port = port;
    pthread_create(&receiverThread, NULL, receiveFileWrapper, receiverArgs);
    pthread_detach(senderThread);
    pthread_detach(receiverThread);
}

