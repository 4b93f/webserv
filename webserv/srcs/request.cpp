/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chly-huc <chly-huc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 01:02:50 by shyrno            #+#    #+#             */
/*   Updated: 2022/08/09 18:32:44 by chly-huc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/header.hpp"

Request::Request()
{
    
}

Request::~Request()
{

}

Request::Request(const Request & other)
{
    *this = other;
}
            
void Request::getInfo(int connection)
{
    char **req, **req2;
    int ret;
    char buff[10000];
    if ((ret = recv(connection, buff, sizeof(buff), 0)) < 0)
        printerr("Error with recv ...");
    if (ret == 0)
        printerr("Error with recv : Connection close ...");
    std::cout << "---------------------------\n" << buff << "\n-------------------------\n";
    req = ft_split(buff, '\n');
    req2 = ft_split(req[0], ' ');
    if (!strcmp(req2[0],"GET") || !strcmp(req2[0],"POST") || !strcmp(req2[0],"DELETE"))
        method = req2[0];
    url = req2[1];
    version = req2[2];
    version.resize(strlen(version.c_str()) - 1);
    std::string str;
    std::string searched = "Accept: ";
    for (int i = 0; req[i]; i++)
    {
        str = req[i];
        if (!str.substr(0, searched.size()).compare(searched))
        {
            type_data = str.substr(searched.size(), str.find(',') - searched.size());
            std::cout << "i found that type of request::  " << type_data << std::endl;
            break ;
        }
    }
    return;
}

std::string Request::getUrl()
{
    return url;
}

std::string Request::getMethod()
{
    return method;
}
std::string Request::getVersion()
{
    return version;
}
std::string Request::getHeader()
{
    return header;
}
std::string Request::gettype_data()
{
    return type_data;
}
