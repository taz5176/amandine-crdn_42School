#   include "Server.hpp"

void Server::checker(User &user, std::vector<std::string> messages)
{
	switch (user.getFirstConnexion())
	{
		case true : {
			this->firstConnexion(user, messages);
			clientMessage(user, RPL_WELCOME); break ; }
		case false : dispatcher(user, messages); break ;
	}
}
#include <thread>
#include <chrono>

void Server::firstConnexion(User &user, std::vector<std::string> messages) 
{
	std::vector<std::string>::iterator itm;
	std::string password = "";
	std::string nickname = "";
	std::string username = "";

	for (itm = messages.begin() ; itm != messages.end(); itm++)
	{
		std::string msg = *itm;
		std::vector<std::string> data = this->split(msg);
		
		std::cout << "\t🪶  🖥️  IRSSI = >" << *itm << "<" << std::endl;

		if (data[0].compare("PASS") == 0) {
			if (this->passCmd(user, data, 1) == "" ) { 
				this->disconnected(user); return ; }}
		else if (data[0].compare("NICK") == 0) {
			nickname = this->nickCmd(user, data, 1);
			if (nickname == "") {
				this->disconnected(user); return ; }}
		else if (data[0].compare("USER") == 0)
			username = this->userCmd(user, data, 1);

		data.clear();
	}
		
	if (nickname == "" || username == "")
		this->disconnected(user);
	else {
		user.setNickName(nickname); 
		user.setUserName(username);
		user.setFirstConnexion(false); }

	messages.clear();
	std::cout << std::endl;
}

void Server::dispatcher(User &user, std::vector<std::string> messages)
{
	std::vector<std::string>::iterator itm;
	(void)user;

	for (itm = messages.begin() ; itm != messages.end(); itm++)
	{
		std::string msg = *itm;
		std::vector<std::string> data = this->split(msg);
		
		std::cout << "\t🪶  🖥️  IRSSI = >" << *itm << "<" << std::endl;

		if (data[0].compare("MODE") == 0)
			modeCmd(user, data);
		else if (data[0].compare("PASS") == 0)
			passCmd(user, data, 0);
		else if (data[0].compare("NICK") == 0)
			nickCmd(user, data, 0);
		else if (data[0].compare("userhost") == 0)
			userCmd(user, data, 0);
		else if (data[0].compare("QUIT") == 0)
			quitCmd(user, data);
		else if (data[0].compare("PING") == 0)
			pingCmd(user, data);
		
		data.clear();
	}
		
	

	messages.clear();
	std::cout << std::endl;

}


// /connect localhost 6667 coco

void Server::disconnected(User &user)
{
	getpeername(user.getFd(), (struct sockaddr *)&my_addr, (socklen_t *)&peer_addr_size);
	std::cout << "Host disconnected , ip " << inet_ntoa(my_addr.sin_addr) << " , port " << ntohs(my_addr.sin_port) << std::endl;
	close(user.getFd());
	users_list.erase(user.getFd());
}