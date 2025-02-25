#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <vector>
#include <string>
#include <map>
#include "Client.hpp"

class Channel {
private:
    std::string name_channel;
    std::string topic;
    std::vector<Client> members;
    std::vector<Client> operators;
    std::vector<Client> invited;
    // bool inviteOnly;
    // size_t limit;
    std::string password;
    
public:
    Channel() {}
    Channel(std::string name) : name_channel(name) {}
    std::string  getName() { return name_channel; }
    void setName(std::string name) { name_channel = name; }
    std::vector<Client> getMembers() { return members; }
    void addMember(Client &client) { members.push_back(client); }
    void addOperator(Client &client) { operators.push_back(client); }
    void invite(Client &client) { invited.push_back(client); }
    
    // bool isMember(Client &client) {
    //     return std::find(members.begin(), members.end(), client) != members.end();
    // }
    // bool isOperator(Client &client) {
    //     return std::find(operators.begin(), operators.end(), client) != operators.end();
    // }
    // bool isInvited(Client &client) {
    //     return std::find(invited.begin(), invited.end(), client) != invited.end();
    // }
    
    // bool isInviteOnly() { return inviteOnly; }
    // bool isFull() { return limit > 0 && members.size() >= limit; }
    
    // bool hasPassword() { return !password.empty(); }
    // bool checkPassword(std::string pass) { return password == pass; }

    // std::string getTopic() { return topic; }
    // std::string getMemberList() {
    //     std::string list;
    //     for (size_t i = 0; i < members.size(); i++) {
    //         list += members[i].getName() + " ";
    //     }
    //     return list;
    // }
};

#endif

