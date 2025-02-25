#ifndef Msj_HPP
#define Msj_HPP

#include <iostream>
#include <vector> // Add this line
#include <sstream>

class Msj
{   
	private:
		std::string command;
		std::string argument;
		std::string channel_name;
		std::string message;
	public:
		std::vector<std::string> args;
		Msj(){
			this->command = "";
			this->argument = "";
			this->channel_name = "";
			this->message = "";
		};
		void setCommand(std::string command){
			this->command = command;
		}
		void setArgument(std::string argument){
			this->argument = argument;
		}
		std::string getCommand(){
			return this->command;
		}
		std::string getArgument(){
			return this->argument;
		}
		std::vector<std::string> getArgs(){
			return this->args;
		}
void SetArgs(const std::string& args) {
    this->args.clear();
    this->message.clear();

    std::string arg;
    size_t pos = 0;
    bool messageFound = false;
    bool hasValidArg = false;  // Vérifie si au moins un argument valide existe

    while (pos < args.length()) {
        if (args[pos] == ':') {  // Détection du message
            this->message = args.substr(pos);

            // Supprimer les `\n` et `\r`
            this->message.erase(std::remove(this->message.begin(), this->message.end(), '\n'), this->message.end());
            this->message.erase(std::remove(this->message.begin(), this->message.end(), '\r'), this->message.end());

            if (!this->message.empty()) {  
                this->args.push_back(this->message);
                hasValidArg = true;
            }
            messageFound = true;
            break;
        } 
        else if (args[pos] == ' ') {  
            if (!arg.empty()) {  // Ajouter seulement si l'argument n'est pas vide ou espace
                arg.erase(0, arg.find_first_not_of(" \n\r")); // Supprime les espaces au début
                arg.erase(arg.find_last_not_of(" \n\r") + 1); // Supprime les espaces à la fin

                if (!arg.empty()) {  // Vérifie si l'argument n'est pas vide après nettoyage
                    this->args.push_back(arg);
                    hasValidArg = true;
                }
                arg.clear();
            }
        } 
        else {
            arg += args[pos];
        }
        pos++;
    }

    // Ajouter le dernier argument s'il est valide
    if (!arg.empty() && !messageFound) {
        arg.erase(0, arg.find_first_not_of(" \n\r"));
        arg.erase(arg.find_last_not_of(" \n\r") + 1);

        if (!arg.empty()) {
            this->args.push_back(arg);
            hasValidArg = true;
        }
    }

    // Si aucun argument valide n'a été ajouté, stocker "NULL"
    if (!hasValidArg) {
        this->args.push_back("NULL");
    }
}

};

#endif
