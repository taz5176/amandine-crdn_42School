#include "../Server.hpp"

void Server::partCmd(User &user, std::vector<std::string> data)
{
	if (data.size() < 2)
	{
		this->clientMessage(user, ERR_NEEDMOREPARAMS, data[0]);
		return;
	} 
	
	// check if channops list, to delete inside

	std::vector<std::string> channels = this->split(data[1], ',');
	std::vector<std::string>::iterator end = channels.end();

	for (std::vector<std::string>::iterator itc = channels.begin(); itc != end; itc++)
	{
		std::string channel = *itc;
		
		if (this->channelExists(channel) == false)
		{
			this->clientMessage(user, ERR_NOSUCHCHANNEL, channel);
			return ;
		}
	
		if (user.isInChannel(channel) == false)
		{
			this->clientMessage(user, ERR_NOTONCHANNEL, channel);
			return ;
		}
		
		//on le retire du sien
		user.eraseChannel(channel);
		//del le user du chan + check si tjs des user sinon supprimer le channel origin
		this->delUserToChan(channel);	
	}
}
