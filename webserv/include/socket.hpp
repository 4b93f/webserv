/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chly-huc <chly-huc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/28 02:31:43 by shyrno            #+#    #+#             */
/*   Updated: 2022/09/13 18:32:18 by swautele         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
#define SOCKET_HPP

#include "header.hpp"

class confData;

class Socket
{
    public:
        Socket();
        Socket(std::string port, std::string ip);
        Socket(const Socket & other);
        Socket &operator=(const Socket & other);
        void setup(int backlog, confData & conf);
        void set_ip(const std::string& ip);
        void set_port(std::string port);
        int create_socket(confData & conf);
        int create_bind();
        int listen_socket(int max_queue);
        sockaddr_in getServ_address() const;
        std::string getIp() const;
        std::string getPort() const;
        const int& getFd() const;
        ~Socket();
    private:
        sockaddr_in serv_address;
        int fd;
        std::string ip;
        std::string port;
};

#endif
