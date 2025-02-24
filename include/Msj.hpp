#ifndef Msj_HPP
#define Msj_HPP

#include <iostream>

class Msj
{   
	private:
		// std::string Msj;
		std::string command;
		std::string argument;
		std::string channel_name;
		std::string message;
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
		// void print_Msj(std::string Msj);
		

};

#endif
