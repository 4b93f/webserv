/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chly-huc <chly-huc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/28 02:20:16 by shyrno            #+#    #+#             */
/*   Updated: 2022/08/09 18:34:42 by chly-huc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/header.hpp"

Conf conf;
Request req;
Response resp;
fd_set fdset, copyset;
std::vector<Socket> *sock;

void setup(char **argv, int backlog)
{
    int i = -1;

    conf.parsing(argv[1]);
    sock = new std::vector<Socket>(conf.getNbrServer());
    FD_ZERO(&fdset);
    while (++i < conf.getNbrServer())
    {
        (*sock)[i].setup(backlog, conf.getConflist(i));
        FD_SET((*sock)[i].getFd(), &fdset);
    }
}

void error_handling()
{
    int i = -1;
    int j;
    if (!conf.getNbrServer())
        printerr("Error : No server configured ...");
    while(++i < conf.getNbrServer())
    {
        j = -1;
        if (conf.getAddress(i).empty() || conf.getPort(i).empty())
            printerr("Error : No address or port configured ...");
        else
        {
            std::string check_address = conf.getAddress(i);
            std::string check_port = conf.getPort(i);
            while (check_address[++j])
                if (!isdigit(check_address[j]) && check_address[j] != '.')
                    printerr("Error : Address must be numeric ...");
            j = -1;
            while(check_port[++j])
                if (!isdigit(check_port[j]))
                    printerr("Error : Port must be numeric ...");
        }
        if (conf.getServName(i).empty())
            conf.getConflist(i).setServName("My Default Server");
    }
}

void engine(int connection, int addrlen)
{
    for(int i = 0; i <= conf.getNbrServer(); i++)
    {
        if (FD_ISSET((*sock)[i].getFd(), &fdset))
        {
            struct sockaddr client_address;
            addrlen = sizeof((socklen_t *)&client_address);
            std::cout << "Accept ... " << std::endl; 
            if ((connection = accept((*sock)[i].getFd(), (struct sockaddr*)&client_address, (socklen_t*)&addrlen)) < 0)
                printerr("cannot connect ...");
            std::cout << "Accept done ..." << std::endl;
            req.getInfo(connection);
            //Autodex index(req.getUrl(), conf.getConflist(0));
            resp.find_method(req, conf.getConflist(i));
            resp.concat_response();      
            write(connection, resp.getResponse().c_str(), resp.getResponse().size());
            close(connection);
            // exit(0);
        }
    }
}

int main(int argc, char **argv)
{
    struct timeval tv;
    int i = -1;
    int connection = 0;
    int addrlen = 0;
    int backlog = 10;
    int retval = 0;
    tv.tv_sec = 60;
    tv.tv_usec = 0;
    if (argc != 2)
        printerr("Usage : ./Webserv [conf file]");
    setup(argv, backlog);
    error_handling();
    std::cout << "\n+++++++ Waiting for new connection ++++++++\n\n" << std::endl;
    memcpy(&copyset, &fdset, sizeof(fdset));
    while(1)
    {
        if ((retval = select(FD_SETSIZE, &copyset, NULL, NULL, &tv)) == -1)
            printerr("Error with select ...");
        else if (retval)
            engine(connection, addrlen);
    }
}
