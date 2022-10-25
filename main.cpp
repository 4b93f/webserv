/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shyrno <shyrno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/28 02:20:16 by shyrno            #+#    #+#             */
/*   Updated: 2022/10/24 16:05:05 by chly-huc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/header.hpp"

fd_set sstock, sready, rstock, rready;
int g_ctrl_called = 0;
std::string str = "";

int setup(webServ & web)
{
    FD_ZERO(&sready);
    FD_ZERO(&rready);
    for (long i = 0; i < web.getConf().getNbrServer(); i++)
    {
        if (!web.getSock()[i].setup(web.getConf().getConflist(i)))
            return 0;
        FD_SET(web.getSock()[i].getFd(), &sstock);
        FD_SET(web.getSock()[i].getFd(), &rstock);
    }
    return 1;
}

int error_handling(webServ & web)
{
    int j;
    if (!web.getConf().getNbrServer())
        return printerr("Error : No server configured ...");
    for (long i = 0; i < web.getConf().getNbrServer(); i++)
    {
        j = -1;
        if (web.getConf().getAddress(i).empty() || web.getConf().getPort(i).empty())
            return printerr("Error : No address or port configured ...");
        else if (web.getConf().getPort(i).size() > 4 || atoi(web.getConf().getPort(i).data()) < 0)
            return printerr("Error with port ...");
        else
        {
            std::string check_address = web.getConf().getAddress(i);
            std::string check_port = web.getConf().getPort(i);
            while(check_port[++j])
                if (!isdigit(check_port[j]))
                    return printerr("Error : Port must be numeric ...");
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

int routine(webServ &web, std::string str, char *buffer, int ret)
{
    if (!str.empty())
    {
        web.getReq().getInfo(str);
        std::string tmp = buffer;
        int sizeheader = tmp.find("\r\n\r\n") + 4;
        if (web.getReq().getBrutbody_fileno() != -1)
        {
            if (!(web.getReq().Write_Brutbody(buffer + sizeheader, ret - sizeheader)))
                return 0;
        }
        else
            return printerr("Error with brutbody ...");
        while (!g_ctrl_called && web.getReq().getWrote() < atoi(web.getReq().getContentLength().data()) && web.getReq().getWrote() >= 0)
        {
            if ((ret = recv(web.getConnection(), buffer, BUFFER_SIZE - 1, 0)) > 0)
            {
                if (web.getReq().getBrutbody_fileno() != -1)
                    if (!(web.getReq().Write_Brutbody(buffer, ret)))
                        return 0;
            }
            else
                return printerr("Recv : -1 on body ...");
        }
    }
    return 1;
}

int sending(webServ &web, std::string str, int i)
{

    if (!str.empty())
    {
        web.getRes().find_method(web, i);
        web.getRes().concat_response(web);
        long count = 0;
        for (unsigned long i = 0; i < web.getRes().getResponse().size(); i += count)
        {
            if ((count = send(web.getConnection(), web.getRes().getResponse().data() + i, web.getRes().getResponse().size() - i, 0)) < 0)
                printerr("Error with send ...");
            if (!count || count == -1)
                break;
        }
        web.getCgi().setCGIBool(0);
        web.getRes().clear_info();
        web.getReq().clear_info();
    }
    return 1;
}

int engine(webServ & web, int *step)
{
    char buffer[BUFFER_SIZE];
    int ret = 0;
    // int nbsock = 0;
    // for (std::vector<Socket>::iterator it = web.getSock().begin(); it != web.getSock().end();it++)
    // {
    //     if (FD_ISSET(it->getFd(), &sready))
    //         nbsock++;
    // }
    std::vector<Socket>::iterator it = web.getSock().begin();
    while(it != web.getSock().end() && it->read == true)
    {
        it++;
    }
    // if (it->read == 0 && !FD_ISSET(it->getFd(), &sready) && !FD_ISSET(web.getConnection(), &rready))
    //     it->read = true;
    std::cout << "i buccle in pos : " << it - web.getSock().begin() << " and step = " << *step << std::endl;
    if (it == web.getSock().end())
    {
        // exit(0);
        for(std::vector<Socket>::iterator its = web.getSock().begin(); its != web.getSock().end();its++)
            its->read = false;
        return 1;
    }
    // for(std::vector<Socket>::iterator it = web.getSock().begin(); it != web.getSock().end();it++)
    // {
        // std::cout << "test" << std::endl;
        // while((it != web.getSock().end() && it->read == 1 )|| (it != web.getSock().end() && !FD_ISSET(it->getFd(), &sready)))
        //     it++;
        // std::cout << "i buccle in pos : " << it - web.getSock().begin() << " and step = " << *step << std::endl;
        // if (it == web.getSock().end())
        // {
        //     for(std::vector<Socket>::iterator its = web.getSock().begin(); its != web.getSock().end();its++)
        //         its->read = 0;
        //     return 1;
        // }
        if (FD_ISSET(web.getConnection(), &rready) && *step == 2)
        {
            if (!sending(web, str, it - web.getSock().begin()))
                return 0;
            *step = 3;
            it->read = true;
            // exit (0);
            return 1;
        }
        else if (FD_ISSET(web.getConnection(), &sready) && *step == 1)
        {
            if ((ret = recv(web.getConnection(), buffer, sizeof(buffer) - 1, 0)) > 0)
            {
                buffer[ret] = '\0';
                str += buffer;
                if (!routine(web, str, buffer, ret))
                    return 0;
                *step = 2;
            }
            else if (ret == 0)
            {
                close(web.getConnection());
                *step = 0;
            }
            else
                return web.cleave_info("Error with recv : -1", GO);
            return 1;
        }
        else if (it != web.getSock().end() && FD_ISSET(it->getFd(), &sready) && *step == 0)
        {
            memset(buffer, 0, BUFFER_SIZE);
            struct sockaddr client_address;
	        int addrlen = sizeof(sizeof(struct sockaddr_in));
            if (web.getConnection() == -1)
            {
                if ((ret = accept(it->getFd(), (struct sockaddr*)&client_address, (socklen_t*)&addrlen)) < 0)
                    return printerr("Error with accept ...");
                web.setConnection(ret);
                if ((fcntl(web.getConnection(), F_SETFL, O_NONBLOCK)) < 0)
                    return printerr("Error with fcntl ...");
                *step = 1;
                return 1;
            }
        }
        else
            it->read = true;
    // }
    return 1;
}

void ctrl_c(int sig)
{
	(void)sig;
    g_ctrl_called = 1;
}

int selecting(webServ & web, int *step)
{
    int status = 0;
    struct timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    while(!g_ctrl_called && status == 0)
    {
        FD_ZERO(&sready);
        FD_ZERO(&rready);
        if (*step == 0)
            sready = sstock;
        if (*step == 1)
        {
            sready = sstock;
            if (web.getConnection() != -1)
                FD_SET(web.getConnection(), &sready);
        }
        if (*step == 2)
            FD_SET(web.getConnection(), &rready);
        usleep(2000);
        std::cout << "Loop Select ..." << std::endl;
        if ((status = select(FD_SETSIZE, &sready, &rready, NULL, &tv)) < 0)
        {
            if (!g_ctrl_called)
                return printerr("Error with select ...");
            return (0);
        }
        if (status == 0)
            std::cout << "Timeout ... Select Retry ..." << std::endl;
    }
    if (g_ctrl_called)
        return 0;
    return 1;
}

int main(int argc, char **argv, char **envp)
{
    // int max_fd = 0;
    int step = 0;
    FD_ZERO(&sstock);
    FD_ZERO(&sready);
    if (argc != 2)
        return printerr("Usage : ./Webserv [conf file]");
    webServ web(argv[1], envp);
	web.setServ_Root(envp);
    if (!setup(web))
        return web.cleave_info("Error with setup ...", GO);
    if (!web.getCgi().set_transla_path(envp))
    {
        std::cout << "Error No executable found for cgi : cgi not available" << std::endl;
        web.setCgi_state(0);
    }
    if (!error_handling(web))
        web.cleave_info("Error with setup ...", STOP);
    std::cout << "\n+++++++ Waiting for new connection ++++++++\n\n" << std::endl;
    signal(SIGINT, &ctrl_c);
    while(!g_ctrl_called)
    {
        if (!selecting(web, &step) || (!engine(web, &step)))
            break;
        if (step == 3 && web.getConnection() > -1)
        {
            close(web.getConnection());
            web.setConnection(-1);
            step = 0;
            str = "";
        }
    }
    web.cleave_info("Bye Bye\n", GO);
}
