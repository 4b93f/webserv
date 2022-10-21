/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shyrno <shyrno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/28 02:20:16 by shyrno            #+#    #+#             */
/*   Updated: 2022/10/21 04:11:32 by shyrno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/header.hpp"

fd_set sstock, sready, rstock, rready;
int g_ctrl_called = 0;
int max_fd;
std::vector<std::string> ip_vec;
int done = 0;

int select_connection(int connection)
{
    struct timeval tv;
    int status = 0;

    tv.tv_sec = 1;
    tv.tv_usec = 0;
    fd_set tset, tstock, wset, wstock;
    FD_ZERO(&tstock);
    FD_ZERO(&wstock);
    FD_SET(connection, &tstock);
    FD_SET(connection, &wstock);
    while (!g_ctrl_called && status == 0)
    {
        FD_ZERO(&tset);
        FD_ZERO(&wset);
        tset = tstock;
        wset = wstock;
        usleep(2000);
        std::cout << "Select connection ... " << std::endl;
        if ((status = select(connection + 1, &tset, NULL, NULL, &tv)) < 0)
        {
            if (!g_ctrl_called)
                return printerr("Error with select ...");
            return (0);
        }
    }
    if (g_ctrl_called)
        return (0);
    return 1;
}

int select_connection_send(int connection)
{
    struct timeval tv;
    int status = 0;

    tv.tv_sec = 1;
    tv.tv_usec = 0;
    fd_set tset, tstock, wset, wstock;
    FD_ZERO(&tstock);
    FD_ZERO(&wstock);
    FD_SET(connection, &tstock);
    FD_SET(connection, &wstock);
    while (!g_ctrl_called && status == 0)
    {
        FD_ZERO(&tset);
        FD_ZERO(&wset);
        tset = tstock;
        wset = wstock;
        std::cout << "Select connection idk ... " << std::endl;
        if ((status = select(connection + 1, &tset, &wset, NULL, &tv)) < 0)
        {
            if (!g_ctrl_called)
                return printerr("Error with select ...");
            return (0);
        }
    }
    if (g_ctrl_called)
        return (0);
    return 1;
}


int setup(webServ & web, int backlog)
{
    FD_ZERO(&sready);
    FD_ZERO(&rready);
    for (unsigned long i = 0; i < web.getConf().getNbrServer(); i++)
    {
        if (!web.getSock()[i].setup(backlog, web.getConf().getConflist(i)))
            return 0;
        FD_SET(web.getSock()[i].getFd(), &sstock);
        FD_SET(web.getSock()[i].getFd(), &rstock);
        //ip_vec.push_back(web.getConf().getConflist(i).getAdress() + ":" + web.getConf().getConflist(i).getPort());
        if (web.getSock()[i].getFd() > max_fd)
            max_fd = web.getSock()[i].getFd();
        //}
    }
    // for(std::vector<std::string>::iterator it = ip_vec.begin(); it != ip_vec.end(); it++)
    //     std::cout << "Unique ip:" << *it << std::endl;
    return 1;
}

int error_handling(webServ & web)
{
    int j;
    if (!web.getConf().getNbrServer())
        printerr("Error : No server configured ...");
    for (unsigned long i = 0; i < web.getConf().getNbrServer(); i++)
    {
        j = -1;
        if (web.getConf().getAddress(i).empty() || web.getConf().getPort(i).empty())
            printerr("Error : No address or port configured ...");
        else
        {
            std::string check_address = web.getConf().getAddress(i);
            std::string check_port = web.getConf().getPort(i);
            while(check_port[++j])
                if (!isdigit(check_port[j]))
                    printerr("Error : Port must be numeric ...");
        }
        if (web.getConf().getServName(i).empty())
            web.getConf().getConflist(i).setServName("My Default Server");
        if (!web.getConf().getPath(i).compare("../"))
        {
            //std::cout << web.getConf().getServName(i) << std::endl;
            web.getConf().getConflist(i).setPath("root ./");
        }
    }
    j = -1;
    return 1;
}

int routine(webServ &web, std::string str, char *buffer, int ret, int i)
{
    if (!str.empty())
    {
        web.getReq().getInfo(web.getConnection(), str);
        std::string tmp = buffer;
        int sizeheader = tmp.find("\r\n\r\n") + 4;
        if (web.getReq().getBrutbody_fileno() != -1)
            web.getReq().Write_Brutbody(buffer + sizeheader, ret - sizeheader);
        else
            return printerr("Error with brutbody");
        while (!g_ctrl_called && web.getReq().getWrote() < atoi(web.getReq().getContentLength().data()) && web.getReq().getWrote() >= 0)
        {
            // if (!select_web.getConnection()(web.getConnection()))
            //      return 0;
            if ((ret = recv(web.getConnection(), buffer, BUFFER_SIZE - 1, 0)) > 0)
            {
                if (web.getReq().getBrutbody_fileno() != -1)
                    web.getReq().Write_Brutbody(buffer, ret);
            }
            else
                return printerr("Recv body -1 ...");
        }
        web.getRes().find_method(web, i);
        web.getRes().concat_response(web);
        long count = 0;
        for (unsigned long i = 0; i < web.getRes().getResponse().size(); i += count)
        {
            done = 1;
            if ((count = send(web.getConnection(), web.getRes().getResponse().data() + i, web.getRes().getResponse().size() - i, 0)) < 0)
                return printerr("Error with send ...");
            if (!count)
                break;
        }
        web.getCgi().setCGIBool(0);
        if (web.getReq().getBrutbody_fileno() != -1)
            fclose(web.getReq().getBrutBody());
        web.getRes().clear_info();
    }
    return 1;
}

int engine(webServ & web)
{
    char buffer[BUFFER_SIZE];
    std::string str = "";
    int ret;
    int i = 0;
    for(std::vector<Socket>::iterator it = web.getSock().begin(); it != web.getSock().end();it++)
    {
        if (FD_ISSET(web.getConnection(), &sready))
        {
            if ((ret = recv(web.getConnection(), buffer, sizeof(buffer) - 1, 0)) > 0)
            {
                buffer[ret] = '\0';
                str += buffer;
                if (!routine(web, str, buffer, ret, i))
                    return printerr("Error with routine ...");
            }
            else
                if (ret < 0)
                    return printerr("Recv -1 ...");
            //std::cout << "Ret : " << ret << std::endl;
            break;
        }
        if (FD_ISSET(it->getFd(), &sready))
        {
            
            memset(buffer, 0, BUFFER_SIZE);
            struct sockaddr client_address;
	        int addrlen = sizeof(sizeof(struct sockaddr_in));
            //std::cout << "Accept ... " << std::endl;
            if (web.getConnection() == -1)
            {
                if ((ret = accept(it->getFd(), (struct sockaddr*)&client_address, (socklen_t*)&addrlen)) < 0)
                    return printerr("Error with accept ...");
                web.setConnection(ret);
                if ((fcntl(web.getConnection(), F_SETFL, O_NONBLOCK)) < 0)
                    return printerr("Error with fcntl ...");
            // if (!select_connection(web.getConnection()))
                std::cout << "connection fd : " << web.getConnection() << std::endl;
                return 1;
            }
        }
    }
    return 1;
}

void ctrl_c(int sig)
{
	(void)sig;
    g_ctrl_called = 1;
}

int loopselect()
{
    int status = 0;
    struct timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    while(!g_ctrl_called && status == 0)
    {
        FD_ZERO(&sready);
        FD_ZERO(&rready);
        sready = sstock;
        rready = rstock;
        usleep(20);
        std::cout << "Loop Select ..." << std::endl;
        if ((status = select(max_fd + 1, &sready, &rready, NULL, &tv)) < 0)
        {
            if (!g_ctrl_called)
                return printerr("Error with select ...");
            return (0);
        }
    }
    if (g_ctrl_called)
        return 0;
    return 1;
}

int selecting(webServ & web)
{
    int status = 0;
    struct timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    while(!g_ctrl_called && status == 0)
    {
        FD_ZERO(&sready);
        FD_ZERO(&rready);
        sready = sstock;
        if (web.getConnection() != -1)
        {
            std::cout << "Set connection to sready...\n";
            FD_SET(web.getConnection(), &sready);
            if (!done)
                FD_SET(web.getConnection(), &rready);
            if (web.getConnection() >= max_fd)
                max_fd = web.getConnection();
        }
        for (int i = 0; i < 100; i++)
            if(FD_ISSET(i, &sready))
                std::cout << i << std::endl;
        for (int i = 0; i < 100; i++)
            if(FD_ISSET(i, &rready))
                std::cout << i << std::endl;
        // rready = rstock;
        //std::cout << "max fd : " << max_fd << std::endl;
        usleep(20);
        std::cout << "Loop Select ..." << std::endl;
        if ((status = select(FD_SETSIZE, &sready, &rready, NULL, &tv)) < 0)
        {
            if (!g_ctrl_called)
                return printerr("Error with select ...");
            return (0);
        }
        std::cout << "UH\n" << std::endl;
    }
    if (g_ctrl_called)
        return 0;
    return 1;
}

int main(int argc, char **argv, char **envp)
{
    FD_ZERO(&sstock);
    FD_ZERO(&sready);
    if (argc != 2)
        return printerr("Usage : ./Webserv [conf file]");
    webServ web(argv[1], envp);
	web.setServ_Root(envp);
    web.getCgi().set_transla_path(envp);
    if (!setup(web, 0))
    {
        web.cleave_info();
        return 0;
    }
    if (!error_handling(web))
        return 0;
    std::cout << "\n+++++++ Waiting for new connection ++++++++\n\n" << std::endl;
    signal(SIGINT, &ctrl_c);
    while(!g_ctrl_called)
    {
        if (!selecting(web) || (!engine(web)))
            break;
        //std::cout << "MH\n";
        if (done && web.getConnection() > 0)
        {
            //std::cout << "Close" << std::endl;
            close(web.getConnection());
            web.setConnection(-1);
            done = 0;
        }
    }
    web.cleave_info();
}
