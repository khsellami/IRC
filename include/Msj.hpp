#ifndef Msj_HPP
#define Msj_HPP

#include <iostream>
#include <vector> // Add this line

class Msj
{   
	private:
		// std::string Msj;
		std::string command;
		std::string argument;
		std::string channel_name;
		std::string message;
		std::vector<std::string> args;
	public:
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
		void SetArgs(std::string args) {
			this->args.clear(); // Clear previous args
			std::string arg;
			size_t pos = 0;
			bool messageFound = false;

			while (pos < args.length()) {
				if (args[pos] == ':') {
					// The remaining part is the message
					this->message = args.substr(pos + 1);
					messageFound = true;
					break;
				} 
				else if (args[pos] == ' ') {
					if (!arg.empty()) {
						this->args.push_back(arg);
						arg.clear();
					}
				} 
				else {
					arg += args[pos];
				}
				pos++;
			}

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
