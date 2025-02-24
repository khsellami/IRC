#ifndef Msj_HPP
#define Msj_HPP

#include <iostream>
#include <vector>

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
		void SetArgs(std::string args){
			std::string arg;
			for (size_t i = 0; i < args.length(); i++)
			{
				if(args[i] == ' ')
				{
					this->args.push_back(arg);
					arg = "";
				}
				else
				{
					arg += args[i];
				}
			}
			this->args.push_back(arg);
		}
		// void print_Msj(std::string Msj);
		

};

#endif
