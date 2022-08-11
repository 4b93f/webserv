/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conf.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shyrno <shyrno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 05:49:14 by shyrno            #+#    #+#             */
/*   Updated: 2022/08/08 02:30:57 by shyrno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/header.hpp"

Conf::Conf()
{
    nbr_serv = 0;
    confList = new std::vector<confData>(1);
}

Conf::~Conf()
{
    delete confList;
}

Conf::Conf(const Conf & other)
{
    *this = other;
}

void Conf::print_info()
{
    for(int i = 0; i < nbr_serv; i++)
        (*confList)[i].print_info();
}

std::string Conf::getAddress(int index)
{
    return (*confList)[index].getAdress();
}

std::string Conf::getPort(int index)
{
    return (*confList)[index].getPort();
}

std::string Conf::getPath(int index)
{
    return (*confList)[index].getPath();
}
std::string Conf::getServName(int index)
{
    return (*confList)[index].getServName();  
}
std::string Conf::getMethod(int index)
{
    return (*confList)[index].getMethod();     
}
std::string Conf::getIndex(int index)
{
    return (*confList)[index].getIndex();
}

int Conf::getNbrServer()
{
    return nbr_serv;
}

confData & Conf::getConflist(int index)
{
    return (*confList)[index];
}

void Conf::parsing(char *path)
{
   
    if (!(nbr_serv = (*confList)[0].parsing(path)))
        printerr("Error with parsing, 0 server found ...");
    delete this->confList;
    confList = new std::vector<confData>(nbr_serv);
    for (int i = 0; i < nbr_serv; i++)
        (*confList)[i].scrapData();
    print_info();
}