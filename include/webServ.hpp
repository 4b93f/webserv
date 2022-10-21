/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webServ.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shyrno <shyrno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/17 17:10:21 by shyrno            #+#    #+#             */
/*   Updated: 2022/10/21 02:37:19 by shyrno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP
#include "header.hpp"
class Response;
class Socket;
class Conf;
class Autodex;
class Cgi;

class webServ
{
    public:
        webServ();
        ~webServ();
        webServ(webServ & other);
        webServ(std::string argv, char **envp);
        webServ &operator=(webServ const & other);
        Conf & getConf();
        char** getEnv();
        Request & getReq();
        Response & getRes();
		Cgi&	getCgi();
        Autodex & getAutodex();
        int getMax_body_size();
        int getConnection();
        std::vector<Socket> &getSock();
        std::pair<std::string, std::string> &getbool_redir();
        void del_redir();
        void cleave_info();
        void setMax_body_size(int i);
        void setbool_redir(std::vector<std::string> vec);
		void	setServ_Root(char **env);
		const std::string&	getServ_Root() const;
        void    setEnv(char** envp);
        void setSocket(std::vector<Socket> s);
        void setConnection(int fd);

    private:
		char** env;
		Cgi* _cgi;
        Conf* conf;
        Request* req;
        Response* res;
        int connection;
        Autodex* indexing;
        int max_body_size;
		std::string	serv_root;
        std::vector<Socket> sock;
        std::pair<std::string, std::string> bool_redir;
};

#endif

