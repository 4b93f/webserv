//variable for cgi

//AUTH_TYPE, CONTENT_LENGTH, CONTENT_TYPE, GATEWAY_INTERFACE, PATH_INFO, PATH_TRANSLATED, QUERY_STRING, REMOTE_ADDR, REMOTE_HOST, REMOTE_IDENT, REMOTE_USER, REQUEST_METHOD, SCRIPT_NAME, SERVER_NAME, SERVER_PORT, SERVER_PROTOCOL et SERVER_SOFTWARE

#ifndef CGI_HPP
#define CGI_HPP

#include "header.hpp"

class Cgi{
	public:
		Cgi();
		~Cgi();
		std::string& getPath();
		const std::vector<std::string>& getEnv() const;
		const std::string& getFullpath() const;
		void	run_api(webServ& web, confData& conf);
		char**	getArgv();
		char**	getEnvp();
		int		getCGIBool();
		void	setCGIBool(int i);
		void	setFullpath(webServ& web, confData& conf);
		std::string	start_script(webServ& web);
		int	set_transla_path(char** envp);
		int	find_transla_path(std::string scri, std::string ext, std::vector<std::string> paths);
		void	setEnv(webServ& web, confData& conf);
		int 	conf_php_ini(webServ& web, confData& conf);

	private:
		std::map<std::string, std::string> pathmap;
		std::vector<std::string> env;
		std::string	scripath;
		std::string body;
		int cgi_on;
};

#endif
