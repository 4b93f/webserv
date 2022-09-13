/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shyrno <shyrno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/13 21:29:25 by shyrno            #+#    #+#             */
/*   Updated: 2022/09/13 21:36:45 by shyrno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cgi.hpp"

void	start_script(Cgi& cgi)
{
	std::cout << "starting the execute " << std::endl;
	int id;
	int status;
	char** argtmp = cgi.getArgv();
	std::cout << "getArg done " << std::endl;
	char** envtmp = cgi.getEnvp();
	std::cout << "getEnv done " << std::endl;
	id = fork();
	std::cout << "forked with :" << id << std::endl;
	if (id == -1)
	{
		std::cout << "could'nt fork" << std::endl;
		return ;
	}
	if (id == 0)
	{
		std::cout << "trying execve with" << cgi.getPath(cgi.getFullpath()) << " and " << cgi.getFullpath() << std::endl;
		if (execve(cgi.getPath(cgi.getFullpath()).c_str(), argtmp, envtmp) < 0)
		{
			std::cout << "Script couldn't be loaded with path : |" << cgi.getFullpath() << "|" << std::endl;
			std::cout << "ex path :" << cgi.getPath(cgi.getFullpath()) << std::endl;
			std::cout << "errno : " << errno << std::endl;
			perror("execve error ");
			exit(10);
		}
	}
	else
	{
		wait(&status);
	}
	free_table(argtmp);
	free_table(envtmp);
}

// void run_api(webServ& web, confData& conf)
// {
// 	web.getCgi().setFullpath(web, conf);
// 	std::cout << "test" << std::endl;
// 	web.getCgi().setEnv(web, conf);
// 	std::cout << "second test "<< std::endl;
// 	for (unsigned long i = 0; i < web.getCgi().getEnv().size(); i++)
// 	{
// 		std::cout << web.getCgi().getEnv()[i] << std::endl;
// 	}
// 	web.getCgi().start_script();
// }

Cgi::Cgi()
{
	std::cout <<"----------------------------------------------------------------------------------------------------------------------" <<env.size() << std::endl;
	pathmap["php"] = "/opt/homebrew/bin/php";
//	this->env = new std::vector<std::string>;
}

Cgi::~Cgi()
{
	
}

std::string& Cgi::getPath(const std::string& req_file)
{
	std::string ext(req_file.substr(req_file.rfind('.') + 1, req_file.size()));
	return pathmap[ext];
}

char**	Cgi::getArgv()
{
	char **res = new char*[2];
	res[0] = new char(path.size() + 1);
	res[1] = NULL;
	for (unsigned long i = 0; i < path.size(); i++)
	{
		res[0][i] = path[i];
	}
	res[0][path.size()] = '\0';
	return res;
}

/*const std::vector<std::string>& Cgi::getEnv() const
{
	return *env;
}*/

const std::string& Cgi::getFullpath() const
{
	return path;
}

void	Cgi::setFullpath(webServ& web,confData& conf)
{
	std::string path (web.getReq().getUrl());
	location loc = conf.LocationFinder(path);
	std::string locroot (loc.getPath());
	if (locroot.size() >= 1 && locroot[0] == '.')
		locroot = locroot.substr(1, locroot.size());
	path = path.substr(loc.getLocation_name().size(), path.size());
	path = web.getServ_Root() + locroot + "/" + path;
	std::cout << "path :" << path << std::endl;
}

void	Cgi::setEnv(webServ& web, confData& conf)
{
	std::vector<std::string> header;
	splitstring(web.getReq().getHeader(), header, '\n');

	std::cout << "starting env init" << std::endl;
	env.clear();
	std::string tmp = "REDIRECT_STATUS=200";
	env.push_back(tmp);
	tmp = "GATEWAY_INTERFACE=CGI/1.1";
	env.push_back(tmp);
	tmp = "SCRIPT_NAME=" + this->path.substr(path.rfind('/') + 1, path.size());
	env.push_back(tmp);
	tmp = "SCRIPT_FILENAME=" + this->path;
	env.push_back(tmp);
	tmp = "REQUEST_METHOD=" + web.getReq().getMethod();
	env.push_back(tmp);
	tmp = "CONTENT_LENGTH=" + itoa(web.getReq().getBody().size());
	env.push_back(tmp);
	tmp = "CONTENT_TYPE=" + search_value_vect(header, "Accept: ");
	env.push_back(tmp);
	tmp = "PATH_INFO=" + web.getReq().getUrl();
	env.push_back(tmp);
	tmp = "PATH_TRANSLATED=" + web.getReq().getUrl();
	env.push_back(tmp);
	tmp = "QUERY_STRING=" + web.getReq().getUrl().substr(web.getReq().getUrl().rfind('?') + 1, web.getReq().getUrl().size());
	env.push_back(tmp);
	tmp = "REMOTEaddr=" + conf.getAdress();
	env.push_back(tmp);
	tmp = "REMOTE_IDENT=" + search_value_vect(header, "Authorization: ");
	env.push_back(tmp);
	tmp = "REMOTE_USER=" + search_value_vect(header, "Authorization: ");
	env.push_back(tmp);
	tmp = "REQUEST_URI=" + web.getReq().getUrl();
	env.push_back(tmp);
	tmp = "SERVER_NAME=";
	if(search_value_vect(header, "Host: ").size())
	{
		tmp += search_value_vect(header, "Host: ");
	}
	else
	{
		tmp += conf.getServName();
	}
	env.push_back(tmp);
	tmp = "SERVER_PORT=" + conf.getPort();
	env.push_back(tmp);
	tmp = "SERVER_PROTOCOL=HTTP/1.1";
	env.push_back(tmp);
	tmp = "SERVER_SOFTWARE=Webserv/1.0";
	env.push_back(tmp);
	// delete header;
}

char**	Cgi::getEnvp()
{
	char** res = new char*[19];
	res[18] = NULL;
	for (unsigned long i = 0; i < env.size(); i++)
	{
		res[i] = new char(env[i].size() + 1);
		res[i][env[i].size()] = '\0';
		for (unsigned long j = 0; j < env[i].size(); j++)
		{
			res[i][j] = env[i][j];
		}
	}
	return res;
}
