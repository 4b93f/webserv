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

int routine(webServ &web, char *buffer, int *step, int serv)
{
    int ret;
    if ((ret = recv(web.getConnection(), buffer, BUFFER_SIZE - 1, 0)) > 0)
    {
        if (web.getReq().getBrutbody_fileno() != -1)
            if (!(web.getReq().Write_Brutbody(buffer, ret)))
                return 0;
    }
    else
        return printerr("Recv : -1 on body ...");
    if (!g_ctrl_called && web.getReq().getWrote() >= atoll(web.getReq().getContentLength().data()) && web.getReq().getWrote() >= 0)
    {
        web.getRes().find_method(web, serv);
        web.getRes().concat_response(web);
        *step = 3;
    }
    return 1;
}

int sending(webServ &web, std::string str, int* step, std::vector<Socket>::iterator it)
{
    static unsigned long int i = 0;
    if (!str.empty())
    {
        // web.getRes().find_method(web, i);
        // web.getRes().concat_response(web);
        long count = 0;
        // for (unsigned long i = 0; i < web.getRes().getResponse().size(); i += count)
        // {
            if ((count = send(web.getConnection(), web.getRes().getResponse().data() + i, web.getRes().getResponse().size() - i, 0)) < 0)
                printerr("Error with send ...");
            //std::cout << "sending : " << count << "|" << web.getRes().getResponse().data() + i << std::endl;
            if (!count || count == -1)
                return 1;
            i +=count;
        // }
        if (i >= web.getRes().getResponse().size())
        {
            i = 0;
            web.getCgi().setCGIBool(0);
            web.getRes().clear_info();
            web.getReq().clear_info();
            *step = 4;
            it->read = true;
        }
    }
    return 1;
}

int engine(webServ & web, int *step)
{
    char buffer[BUFFER_SIZE];
    int ret = 0;
    std::vector<Socket>::iterator it = web.getSock().begin();
    while(it != web.getSock().end() && it->read == true)
        it++;
    if (it == web.getSock().end())
    {
        // exit(0);
        for(std::vector<Socket>::iterator its = web.getSock().begin(); its != web.getSock().end();its++)
            its->read = false;
        return 1;
    }
    if (FD_ISSET(web.getConnection(), &sready) && *step == 2)
        return routine(web, buffer, step, it - web.getSock().begin());
    if (FD_ISSET(web.getConnection(), &rready) && *step == 3)
    {
        if (!sending(web, str, step, it))
                return 0;
        return 1;
        }
        else if (FD_ISSET(web.getConnection(), &sready) && *step == 1)
        {
            if ((ret = recv(web.getConnection(), buffer, sizeof(buffer) - 1, 0)) > 0)
            {
                buffer[ret] = '\0';
                str += buffer;
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
                    if (web.getReq().getWrote() < atoll(web.getReq().getContentLength().data()))
                        *step = 2;
                    else
                    {
                        web.getRes().find_method(web, it - web.getSock().begin());
                        web.getRes().concat_response(web);
                        *step = 3;
                    }
                }      
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
    tv.tv_sec = 3;
    tv.tv_usec = 0;
    while(!g_ctrl_called && status == 0)
    {
        FD_ZERO(&sready);
        FD_ZERO(&rready);
        if (*step == 0)
            sready = sstock;
        if (*step == 1 || *step == 2)
        {
            sready = sstock;
            if (web.getConnection() != -1)
                FD_SET(web.getConnection(), &sready);
        }
        if (*step == 3)
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
        {
            *step = 0;
            std::cout << "Timeout ... Select Retry ..." << std::endl;
        }
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
        if (step == 4 && web.getConnection() > -1)
        {
            close(web.getConnection());
            web.setConnection(-1);
            step = 0;
            str = "";
        }
    }
    web.cleave_info("Bye Bye\n", GO);
}
