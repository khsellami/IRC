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
		this->args.clear();   // Nettoyer les anciens arguments
		this->message.clear(); // Nettoyer le message précédent

		std::string arg;
		size_t pos = 0;
		bool messageFound = false;

		while (pos < args.length()) {
			if (args[pos] == ':') {
				// Dès qu'on trouve ':', on stocke tout le reste comme un seul argument
				this->message = args.substr(pos + 1);
				
				// Nettoyer les sauts de ligne et les retours chariots
				this->message.erase(std::remove(this->message.begin(), this->message.end(), '\n'), this->message.end());
				this->message.erase(std::remove(this->message.begin(), this->message.end(), '\r'), this->message.end());

				this->args.push_back(this->message); // Ajouter le message à args
				messageFound = true;
				break;
			} else if (args[pos] == ' ') {
				if (!arg.empty()) {
					this->args.push_back(arg);
					arg.clear();
				}
			} else {
				arg += args[pos];
			}
			pos++;
		}

    // Ajouter le dernier argument si nécessaire
    if (!arg.empty() && !messageFound) {
        this->args.push_back(arg);
    }
}





		std::string get_message(){
			return this->message;
		}
		// void print_Msj(std::string Msj);
};

#endif
