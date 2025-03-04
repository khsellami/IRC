#include "../include/Server.hpp"
#include "../include/Client.hpp"
#include "../include/Msj.hpp"
#include "../include/Channel.hpp"
#include <iostream>
#include <vector>
#include <sstream>

#include <fstream> 
void handleDCCSend(Client &client, std::vector<std::string> args) {
	if (args.size() < 6) {
		client.sendMessage("Erreur : Format incorrect. Utilisation : PRIVMSG <destinataire> :DCC SEND <fichier> <ip> <port> <taille>");
		return;
	}

	std::string filename = args[3];       // Nom du fichier
	std::string ip_str = args[4];         // IP en uint32_t (ex: 2130706433 pour 127.0.0.1)
	int port = std::stoi(args[5]);        // Port TCP
	// int filesize = std::stoi(args[6]);    // Taille du fichier

	// Convertir l'IP en string
	struct in_addr addr;
	addr.s_addr = htonl(std::stoul(ip_str));
	std::string ip = inet_ntoa(addr);

	// Vérifie si le fichier existe
	std::ifstream file(filename, std::ios::binary);
	if (!file.is_open()) {
		client.sendMessage("Erreur : Impossible d'ouvrir le fichier " + filename);
		return;
	}

	// Création d'un socket TCP
	int server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket == -1) {
		client.sendMessage("Erreur : Impossible de créer un socket DCC.");
		return;
	}

	// Liaison au port
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(port);

	if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
		client.sendMessage("Erreur : Bind échoué sur le port " + std::to_string(port));
		close(server_socket);
		return;
	}

	// Écoute sur le port
	listen(server_socket, 1);
	client.sendMessage("DCC : Serveur prêt à envoyer " + filename + " sur " + ip + ":" + std::to_string(port));

	// Attente de connexion du client
	struct sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);
	int client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
	if (client_socket < 0) {
		client.sendMessage("Erreur : Échec de la connexion DCC.");
		close(server_socket);
		return;
	}

	// Envoi du fichier
	char buffer[1024];
	while (!file.eof()) {
		file.read(buffer, sizeof(buffer));
		int bytesRead = file.gcount();
		send(client_socket, buffer, bytesRead, 0);
	}

	client.sendMessage("DCC : Envoi terminé !");
	file.close();
	close(client_socket);
	close(server_socket);
}

void sendFile(const std::string &filename, int port) {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        std::cerr << "Erreur lors de la création du socket !" << std::endl;
        return;
    }

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Erreur lors du bind !" << std::endl;
        return;
    }

    listen(server_fd, 1);
    std::cout << "En attente d'une connexion pour envoyer " << filename << "..." << std::endl;

    int client_fd = accept(server_fd, nullptr, nullptr);
    if (client_fd < 0) {
        std::cerr << "Erreur lors de l'acceptation !" << std::endl;
        return;
    }

    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Erreur : fichier introuvable !" << std::endl;
        return;
    }

    char buffer[1024];
    while (file.read(buffer, sizeof(buffer)) || file.gcount() > 0) {
        send(client_fd, buffer, file.gcount(), 0);
    }

    std::cout << "Fichier envoyé !" << std::endl;

    close(client_fd);
    close(server_fd);
}
void receiveFile(const std::string &filename, const std::string &ip, int port) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        std::cerr << "Erreur de création du socket !" << std::endl;
        return;
    }

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &server_addr.sin_addr);

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Erreur de connexion au serveur !" << std::endl;
        return;
    }

    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Erreur de création du fichier !" << std::endl;
        return;
    }

    char buffer[1024];
    int bytesReceived;
    while ((bytesReceived = recv(sock, buffer, sizeof(buffer), 0)) > 0) {
        file.write(buffer, bytesReceived);
    }

    std::cout << "Fichier reçu !" << std::endl;

    close(sock);
}
