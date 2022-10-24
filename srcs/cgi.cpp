#include "cgi.hpp"

#define BUFF_SIZE 1000
std::string	Cgi::start_script(webServ& web)
{
	std::string rep;
	int id;
	int outpip[2];
	char** argtmp = getArgv();
	char** envtmp = getEnvp();
	
	pipe(outpip);
	char buffer[10000];
	int ret;
	//std::cout << "This :" << web.getCgi().getPath() << " received this brutbody::::::::::::::::::::::::::::::::::::::::::::::::::"<< std::endl;
	if (!ReadWriteProtection(web.getReq().getBrutbody_fileno(), 1))
		web.cleave_info("Error with select brutbody ...", STOP);
	while ((ret = read(web.getReq().getBrutbody_fileno(), buffer, 10000)) > 0)
		if ((write(1, buffer, ret)) < 0)
			web.cleave_info("Error with write ...", STOP);
	if (ret == -1)
		web.cleave_info("Error with read ...", STOP);
	//std::cout << "finish ---------------------------------------" << std::endl;
	lseek(web.getReq().getBrutbody_fileno(), 0, SEEK_SET);
	id = fork();
	if (id == -1)
	{
		//std::cout << "could'nt fork" << std::endl;
		return rep;
	}
	if (id == 0)
	{
		close (outpip[0]);
		dup2(outpip[1], STDOUT_FILENO);
		dup2(web.getReq().getBrutbody_fileno(), STDIN_FILENO);
		
		if (execve(getPath().data(), argtmp, envtmp) < 0)
		{
			perror("EXECVE ERROR :");
			exit(10);
		}
	}
	else
	{
		close(outpip[1]);
		char buff[BUFF_SIZE + 1];
		buff[BUFF_SIZE] = '\0';
		int size;
		if (!ReadWriteProtection(web.getReq().getBrutbody_fileno(), 0))
			printerr("Error with select on read execve outpout ...");	
		while((size = read(outpip[0], buff, BUFF_SIZE)) > 0)
		{
			for (int i = size; i <= BUFF_SIZE; i++)
				buff[i] = '\0';
			rep += buff;
		}
		if (size == -1)
			printerr("Error with read ...");
	}
	// std::cout << "php script answered with ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::" << std::endl << rep << std::endl << "-------------------------------------------------------------------------------------------" << std::endl;
	close(outpip[0]);
	delete[] argtmp;
	delete[] envtmp;
	return rep;
}

void Cgi::run_api(webServ& web, confData& conf)
{
	setCGIBool(1);
	if (conf_php_ini(web, conf) >= 0)
	{
		web.getCgi().setFullpath(web, conf);
		web.getCgi().setEnv(web, conf);
		body = web.getReq().getBody();
		web.getRes().setBody(web.getCgi().start_script(web));
	}
	else
	{
		web.getRes().setStatus(500);
		web.getRes().setBody("");
	}
}

Cgi::Cgi()
{
	cgi_on = 0;
}

Cgi::~Cgi()
{
	pathmap.clear();
	env.clear();
	std::cout << "Cgi destructor called" << std::endl;
}

std::string& Cgi::getPath()
{
	std::string ext = this->scripath.substr(this->scripath.rfind('.') + 1, this->scripath.size());
	return pathmap[ext];
}

void	Cgi::set_transla_path(char** envp)
{
	std::vector<std::string> paths;
	std::string searched = "PATH=";
	for (int i = 0; envp[i]; i++)
	{
		std::string tmp(envp[i]);
		if (!tmp.substr(0, searched.size()).compare(searched))
		{
			splitstring(tmp.substr(5, tmp.size()), paths, ':');
			break;
		}
	}
	find_transla_path("php-cgi", "php", paths);
	find_transla_path("python3", "py", paths);
}

void	Cgi::find_transla_path(std::string scri, std::string ext, std::vector<std::string> paths)
{
	for (unsigned long i = 0; i < paths.size(); i++)
	{
		std::string tmp = paths[i] + "/" + scri;
		if (!access(tmp.data(), X_OK))
		{
			pathmap[ext] = tmp;
			//std::cout << "i initialized script :" << ext << " with :" << tmp << std::endl;
			break ;
		}
	}
}

char**	Cgi::getArgv()
{
	char **res = new char*[3];
	res[0] = new char[this->getPath().size() + 1];
	res[1] = new char[this->scripath.size() + 1];
	res[2] = NULL;
	std::string name = this->getPath();
	for (unsigned long i = 0; i < name.size(); i++)
	{
		res[0][i] = name[i];
	}
	res[0][name.size()] = '\0';
	for (unsigned long i = 0; i < this->scripath.size(); i++)
	{
		res[1][i] = this->scripath[i];
	}
	res[1][this->scripath.size()] = '\0';
	return res;
}

const std::vector<std::string>& Cgi::getEnv() const
{
	return env;
}

const std::string& Cgi::getFullpath() const
{
	return this->scripath;
}

void	Cgi::setFullpath(webServ& web,confData& conf)
{
	std::string temp (web.getReq().getUrl());
	if (temp.rfind('?') != std::string::npos)
	{
		temp = temp.substr(0, temp.rfind('?'));
	}
	std::string locname = location_exe(conf, temp);
	std::string locroot;
	if (!locname.empty())
	{
		locroot = conf.getGoodLocation(locname).getPath();
	}
	else
	{
		locroot = conf.getPath();
	}
	if (locroot.size() > 0 && locroot[0] == '.')
		locroot = locroot.substr(1, locroot.size());
	this->scripath = web.getServ_Root() + locroot + temp;
}

void	Cgi::setEnv(webServ& web, confData& conf)
{
	(void)conf;
	std::vector<std::string> header;
	splitstring(web.getReq().getHeader(), header, '\n');

	env.clear();
//	std::cout << "body req is :" << std::endl << web.getReq().getBody() << std::endl;
	std::string tmp = "REDIRECT_STATUS=200";
	env.push_back(tmp);
	tmp = "GATEWAY_INTERFACE=CGI/1.1";
	env.push_back(tmp);
	// tmp = "SCRIPT_NAME=" + this->scripath.substr(this->scripath.rfind('/') + 1, this->scripath.size());
	// env.push_back(tmp);
	tmp = "SCRIPT_FILENAME=" + this->scripath;
	env.push_back(tmp);
	tmp = "REQUEST_METHOD=" + web.getReq().getMethod();
	env.push_back(tmp);
	if (web.getReq().getContentLength().compare("0"))
	{
		tmp = "CONTENT_LENGTH="+ web.getReq().getContentLength();
		if (!tmp.empty() && !isalnum(tmp.back())) {
   	     tmp.resize(tmp.size() - 1);
   		}
		env.push_back(tmp);
	}
	//  + itoa(web.getReq().getQuery_string().size());
	tmp = search_value_vect(header, "Content-Type:");
	if (tmp.size())
	{
		// if (tmp.find(';') != std::string::npos)
		// 	tmp = tmp.substr(0, tmp.find(';'));
		tmp = "CONTENT_TYPE=" + tmp;
		env.push_back(tmp);
	}
	tmp = "PATH_INFO=" + web.getReq().getUrl();
	env.push_back(tmp);
	tmp = "PATH_TRANSLATED=" + web.getReq().getUrl();
	env.push_back(tmp);
	if (!web.getReq().getQuery_string().empty())
	{
		//std::cout << "GET" << std::endl;
		tmp = "QUERY_STRING=" + web.getReq().getQuery_string();
		env.push_back(tmp);
	}
	// else
	// {
		// tmp = "QUERY_STRING=" + web.getReq().getBody();
		// env.push_back(tmp);
	// }
	tmp = "REMOTEaddr=" + conf.getAdress();
	env.push_back(tmp);
	// tmp = "REMOTE_IDENT=" + search_value_vect(header, "Authorization:");
	// env.push_back(tmp);
	// tmp = "REMOTE_USER=" + search_value_vect(header, "Authorization:");
	// env.push_back(tmp);
	tmp = "REQUEST_URI=" + web.getReq().getUrl();
	if (!web.getReq().getQuery_string().empty())
		tmp += "?" + web.getReq().getQuery_string();
	env.push_back(tmp);
	tmp = "SERVER_NAME=";
	if(search_value_vect(header, "Host:").size())
	{
		tmp += search_value_vect(header, "Host:");
	}
	else
	{
		tmp += conf.getServName();
	}
	if (!tmp.empty() && !isalnum(tmp.back())) {
        tmp.resize(tmp.size() - 1);
    }
	env.push_back(tmp);
	tmp = "PATH_INFO=" + web.getReq().getUrl();
	env.push_back(tmp);
	tmp = "SERVER_PROTOCOL=HTTP/1.1";
	env.push_back(tmp);
	tmp = "SERVER_SOFTWARE=Webserv/1.0";
	env.push_back(tmp);
	tmp = "PHPRC=" + web.getServ_Root() + "/www/php/include/php.ini";
	env.push_back(tmp);
	for (unsigned long i = 0; i < env.size(); i++)
	{
		for (unsigned long j = 0; j < env[i].size(); j++)
		{
			if (env[i][j] == '\r')
				std::cout << "WARNING FANTOM CHAR STILL SOMEWHERE " << "OIUFHIEUFHWOIFOEWIFJOIEWJOFIEW" << std::endl;
		}
	}
}

char**	Cgi::getEnvp()
{
	char** res = new char*[env.size() + 1];
	res[env.size()] = NULL;
	for (unsigned long i = 0; i < env.size(); i++)
	{
		res[i] = new char[env[i].size() + 1];
		res[i][env[i].size()] = '\0';
		for (unsigned long j = 0; j <env[i].size(); j++)
		{
			res[i][j] = env[i][j];
		}
	}
	return res;
}

int Cgi::getCGIBool()
{
	return cgi_on;
}

void Cgi::setCGIBool(int i)
{
	cgi_on = i;
}

int 	Cgi::conf_php_ini(webServ& web, confData& conf)
{
	std::fstream php_ini;
	std::string loc = location_exe(conf, web.getReq().getUrl());
	php_ini.open(web.getServ_Root() + "/www/php/include/php.ini", std::fstream::out | std::fstream::trunc);
	if (php_ini.is_open())
	{
		std::string content = ("file_uploads = 1\nupload_max_filesize = 1000M\nupload_tmp_dir = ");
		php_ini << content;
		if (loc.empty())
			php_ini << conf.getCGIPath();
		else if (!conf.getGoodLocation(loc).getCGIPath().empty())
			php_ini << conf.getGoodLocation(loc).getCGIPath();
		else
			php_ini << "../../uploads/";
	}
	else
		return -1;
	return 0;
}