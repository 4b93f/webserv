#ifndef CGI_HPP
#define CGI_HPP

#include "header.hpp"

class Cgi
{
	public:
		Cgi();
		~Cgi();
		std::string& getPath(const std::string& req_file);
//		const std::vector<std::string>& getEnv() const;
		const std::string& getFullpath() const;
		char**	getArgv();
		char**	getEnvp();
		void	setFullpath(webServ& web, confData& conf);
//		void	start_script();
		void	setEnv(webServ& web, confData& conf);

	private:
		std::map<std::string, std::string> pathmap;
		std::vector<std::string> env;
		std::string empty;
		std::string	path;
};
/*	std::string redirect_status;
	std::string gateway_interface;
	std::string script_name;
	std::string script_filename;
	std::string request_method;
	std::string content_length;
	std::string content_type;
	std::string path_info;
	std::string path_translated;
	std::string query_string;
	std::string remoteaddr;
	std::string remote_ident;
	std::string remote_user;
	std::string request_uri;
	std::string server_name;
	std::string server_port;
	std::string server_protocol;
	std::string server_software;*/

#endif
