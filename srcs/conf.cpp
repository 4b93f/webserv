/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conf.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chly-huc <chly-huc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 05:49:14 by shyrno            #+#    #+#             */
/*   Updated: 2022/09/27 19:16:38 by chly-huc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "conf.hpp"

Conf::Conf()
{
    nbr_serv = 0;
}

Conf::~Conf()
{
    confList.clear();
    std::cout << "Conf destructor called" << std::endl;
}

Conf::Conf(const Conf & other)
{
	nbr_serv = other.getNbrServer();
	confList.resize(nbr_serv, confData());
	confList = other.getVectorConflist();
}

Conf& Conf::operator=(const Conf& other)
{
	confList = other.getVectorConflist();
	nbr_serv = other.getNbrServer();
	return *this;
}

void Conf::print_info()
{
    for(long i = 0; i < nbr_serv; i++)
    {
        //std::cout << std::endl <<"[SERVER " << i + 1 << "]" << std::endl << std::endl; 
        (confList)[i].print_info();
    }
}

std::string Conf::getAddress(int index) const 
{
    return (confList)[index].getAdress();
}

std::string Conf::getPort(int index) const 
{
    return (confList)[index].getPort();
}

std::string Conf::getPath(int index) const 
{
    return (confList)[index].getPath();
}
std::string Conf::getServName(int index) const 
{
    return (confList)[index].getServName();  
}
std::string Conf::getMethod(int index) const 
{
    return (confList)[index].getMethod();     
}
std::vector<std::string> Conf::getIndex(int index) const 
{
    return (confList)[index].getIndex();
}

long Conf::getNbrServer() const
{
    return nbr_serv;
}

confData & Conf::getConflist(int index)  
{
    return (confList)[index];
}

int Conf::parsing(std::string path)
{
    if (!(nbr_serv = (confList)[0].parsing(path)) || nbr_serv == -1)
        return 0;
    confList.resize(nbr_serv);
    for (long i = 0; i < nbr_serv; i++)
        if (!((confList)[i].scrapData()))
            return 0;
    print_info();
    return 1;
}

const std::vector<confData> Conf::getVectorConflist() const
{
	return confList;
}
