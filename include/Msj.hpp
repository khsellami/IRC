#ifndef Msj_HPP
#define Msj_HPP

#include <iostream>
#include <vector> // Add this line
#include <sstream>

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
		void SetArgs(const std::string& args) {
			this->args.clear();
			this->message.clear();

			std::string arg;
			size_t pos = 0;
			bool messageFound = false;

			while (pos < args.length()) {
				if (args[pos] == ':') {
					this->message = args.substr(pos + 1);
					this->message.erase(std::remove(this->message.begin(), this->message.end(), '\n'), this->message.end());
					this->message.erase(std::remove(this->message.begin(), this->message.end(), '\r'), this->message.end());

					this->args.push_back(this->message);
					messageFound = true;
					break;
				} 
				else if (args[pos] == ' ') {
					if (!arg.empty()) {
						arg.erase(std::remove(arg.begin(), arg.end(), '\n'), arg.end());
						arg.erase(std::remove(arg.begin(), arg.end(), '\r'), arg.end());

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
				arg.erase(std::remove(arg.begin(), arg.end(), '\n'), arg.end());
				arg.erase(std::remove(arg.begin(), arg.end(), '\r'), arg.end());
				this->args.push_back(arg);
			}
		}
		std::string get_message(){
			return this->message;
		}
};

#endif
