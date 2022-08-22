/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chly-huc <chly-huc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/20 12:02:42 by chly-huc          #+#    #+#             */
/*   Updated: 2022/08/22 12:46:56 by chly-huc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/header.hpp"

location::location()
{
    location_name = "";
    path = "";
    method = "";
    autoindex = 0;
}

location::~location()
{

}

location::location(const location & other)
{
    
}

std::string location::getPath()
{
    return path;
}

std::vector<std::string> location::getIndex()
{
    return index;
}

std::string location::getMethod()
{
    return method;   
}

std::string location::getLocation_name()
{
    return location_name;   
}

std::string location::getErrorPage()
{
    return location_name;   
}

std::string location::getBodySize()
{
    return body_size;   
}

int location::getAutoIndex()
{
    return autoindex;
}


void location::setLocation_name(std::string str)
{
    remove_spaces(str);
    location_name = str.substr(str.find("location") + strlen("location "), str.size());
}

void location::setPath(std::string str)
{
    remove_spaces(str);
    path = str.substr(strlen("root "), str.size());
    if (path.empty())
    {
        path = "./";
        return;
    }
    path.resize(path.size() - 1);
}
void location::setMethod(std::string str)
{
    remove_spaces(str);
    method = str.substr(strlen("methods "), str.size());
    if (method.empty())
        return;
    method.resize(method.size() - 1);
}

void location::setIndex(std::string str)
{
    std::string tmp;
    remove_spaces(str);
    str = str.substr(str.find(" ") + 1, str.size());
    while(str.find(" ") != std::string::npos)
    {
        tmp = str.substr(0, str.find(" "));
        index.push_back(tmp);
        str = str.substr(str.find(" ") + 1, str.size());
    }
    str.pop_back();
    index.push_back(str);
    if (index.back().empty())
        return;
    index.resize(index.size() - 1);
}

void location::setErrorPage(std::string str)
{
    remove_spaces(str);
    error_page = str.substr(strlen("error_page "), str.size());
    if (error_page.empty())
        return;
    error_page.resize(error_page.size() - 1); 
}

void location::setBodySize(std::string str)
{
    remove_spaces(str);
    body_size = str.substr(strlen("client_max_body_size "), str.size());
    if (body_size.empty())
        return;
    body_size.resize(body_size.size() - 1);
}

void location::setAutoIndex(std::string str)
{
    remove_spaces(str);
    std::cout << "str : " << str << std::endl;
    if (!str.compare("autoindex on;"))
        autoindex = 1;
    else if (!str.compare("autoindex off;"))
        autoindex = 0;
}

void location::print_info()
{
    int i = -1;
    if (!location_name.empty())
        std::cout << "[location " << location_name << "]" << std::endl;
    if (!path.empty())
        std::cout << "Path->            " << "[" << path << "]" << std::endl;
    if (!method.empty())
        std::cout << "Method->          " << "[" << method << "]" << std::endl;
    if (!index.empty())
    {
        std::cout << "Index->           ";
        while (++i <= index.size())
            std::cout << "[" << index[i] << "]";
        std::cout << std::endl;
    }
    if (!error_page.empty())
        std::cout << "Error_page->      " << "[" << error_page << "]" << std::endl;
    if (!body_size.empty())
        std::cout << "Body_size->       " << "[" << body_size << "]" << std::endl;
    std::cout << "AutoIndex->       " << "[" << autoindex << "]" << std::endl << std::endl;
}
int location::scrapData(std::string data, int i)
{
    (void)data;
    (void)i;
    
    std::string tmp;
    tmp = data.substr(0, data.find("\n"));
    setLocation_name(tmp);
    autoindex = 0;
    while (!data.empty())
    {
        tmp = data.substr(0, data.find("\n"));
        data = data.substr(data.find("\n") + 1, data.size());
        if (tmp.find("{") != std::string::npos || tmp.find("location")!= std::string::npos)
            continue;
        if (tmp.find("}") != std::string::npos)
            break;
        else if (tmp.back() != ';')
            printerr("Error with conf file syntax ...");
        else if (tmp.find("root")!= std::string::npos)
            setPath(tmp);
        else if (tmp.find("methods")!= std::string::npos)
            setMethod(tmp);
        else if (tmp.find("error_page")!= std::string::npos)
            setErrorPage(tmp);
        else if (tmp.find("autoindex")!= std::string::npos)
            setAutoIndex(tmp);
        else if (tmp.find("index")!= std::string::npos)
            setIndex(tmp);
        else if (tmp.find("client_max_body_size")!= std::string::npos)
            setBodySize(tmp);
        else
            printerr("Something is wrong with your config file ...");
    }
    if (path.empty())
        path = "./";
    return 0;
}