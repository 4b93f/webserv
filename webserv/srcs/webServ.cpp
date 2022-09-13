/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webServ.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shyrno <shyrno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/18 18:43:26 by chly-huc          #+#    #+#             */
/*   Updated: 2022/09/13 21:36:02 by shyrno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webServ.hpp"

webServ::webServ()
{
    conf = new Conf;
	req = new Request;
	res = new Response;
	indexing = new Autodex;
	_cgi = new Cgi;
}


webServ::webServ(std::string argv)
{
	conf = new Conf;
	req = new Request;
	res = new Response;
	indexing = new Autodex;
	_cgi = new Cgi;

    conf->parsing(argv);
    sock.reserve(conf->getNbrServer());
	std::cout << "nombre de server = " << conf->getNbrServer() << std::endl;
	for (unsigned long i = 0; i < conf->getNbrServer(); i++)
	{
		Socket* next = new Socket;
		sock.push_back(*next);
	}
}

webServ::~webServ()
{
	for (std::vector<Socket>::iterator iter = sock.begin(); iter != sock.end() - 1; iter++)
	{
		delete &(*iter);
	}
	delete conf;
	delete req;
	delete res;
	delete indexing;
}

webServ::webServ(webServ & other):conf(new Conf(other.getConf())), req(new Request(other.getReq())), res(new Response(other.getRes())), sock(other.getSock()), indexing(new Autodex(other.getAutodex())), serv_root(other.getServ_Root())
{
    
}

Conf & webServ::getConf() 
{
    return *conf;
}

std::vector<Socket> & webServ::getSock() 
{
    return sock;
}

Request & webServ::getReq() 
{
    return *req;
}

Response & webServ::getRes() 
{
    return *res;
}

Autodex & webServ::getAutodex() 
{
    return *indexing;
}
void	webServ::setServ_Root(char **env)
{
	std::string searched("PWD=");
	for (int i = 0; env[i]; i++)
	{
		std::string tmp(env[i]);
		if (!tmp.substr(0, searched.size()).compare(searched))
		{
			serv_root = tmp.substr(searched.size(), tmp.size());
			std::cout << "serv_root = " << serv_root << std::endl;
			return;
		}
	}
}

const std::string&	webServ::getServ_Root() const 
{
	return serv_root;
}

Cgi& webServ::getCgi()
{
	return *_cgi;
}
