/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webServ.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shyrno <shyrno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/18 18:43:26 by chly-huc          #+#    #+#             */
/*   Updated: 2022/10/21 02:38:06 by shyrno           ###   ########.fr       */
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
	max_body_size = 0;
	connection = -1;
}

webServ::webServ(std::string argv, char **envp)
{
	conf = new Conf;
	req = new Request;
	res = new Response;
	indexing = new Autodex;
	_cgi = new Cgi;
	max_body_size = 0;
	connection = -1;
	this->env = envp;

    conf->parsing(argv);
    sock.reserve(conf->getNbrServer());
	
	std::cout << "Nombre de server = " << conf->getNbrServer() << std::endl;
	for (unsigned long i = 0; i < conf->getNbrServer(); i++)
	{
		Socket next;
		sock.push_back(next);
	}
}

webServ::~webServ()
{
	delete conf;
	delete req;
	delete res;
	delete indexing;
	delete _cgi;
	std::cout << "webServ destructor called" << std::endl;
}

webServ::webServ(webServ & other):conf(new Conf(other.getConf())), req(new Request(other.getReq())), res(new Response(other.getRes())), indexing(new Autodex(other.getAutodex())), serv_root(other.getServ_Root()), sock(other.getSock())
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

std::pair<std::string, std::string> & webServ::getbool_redir()
{
    return bool_redir;
}

int webServ::getMax_body_size()
{
	return max_body_size;
}

int webServ::getConnection()
{
	return connection;
}

void webServ::setbool_redir(std::vector<std::string> vec)
{
    std::string str;
    std::string loc;
    
    loc = vec.at(0);
	if (loc.compare("301"))
		loc = "301";
    str = vec.at(1);
    bool_redir = std::make_pair(loc, str);
}

void webServ::setMax_body_size(int i)
{
	max_body_size = i;
}

void webServ::setConnection(int fd)
{
	connection = fd;
}


void webServ::del_redir()
{
    bool_redir.first = "";
    bool_redir.second = "";
}

void webServ::cleave_info()
{
	std::cout << "Cleave info ..." << std::endl;
    max_body_size = 0;
    req->clear_info();
	for (std::vector<Socket>::iterator it = sock.begin(); it != sock.end(); it++)
	{
		std::cout << "Closing " << it->getFd() << std::endl;
		it->close_fd();
	}
	fclose(req->getBrutBody());
	std::cout << "\nBye bye" << std::endl;
}

char ** webServ::getEnv()
{
	return env;
}
