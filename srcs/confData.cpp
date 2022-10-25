/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   confData.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shyrno <shyrno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/08 05:46:00 by shyrno            #+#    #+#             */
/*   Updated: 2022/10/21 23:13:17 by shyrno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "confData.hpp"

std::string data_split;
std::string data;

confData::confData()
{
    nbr_loc = 0;
    autoindex = 0;
    cgi = 0;
}

confData::~confData()
{
    loc.clear();
    index.clear();
    redir.clear();
    std::cout << "ConfData destructor called" << std::endl;
}

confData::confData(const confData & other): autoindex(other.getAutoIndex()), port(other.getPort()), path(other.getPath()), method(other.getMethod()), address(other.getAdress()) , serv_name(other.getServName()) , body_size(other.getBodySize()),  nbr_loc(other.getLocationNbr()), error_page(other.getErrorPage()), loc(nbr_loc), index(other.getIndex())
{
	for (unsigned long i = 0; i < nbr_loc; i++)
		(loc)[i] = other.getLocation(i);
}

confData& confData::operator=(const confData& other)
{
	port = other.getPort();
	path = other.getPath();
	index = other.getIndex();
	method = other.getMethod();
	address = other.getAdress();
	serv_name = other.getServName();
	body_size = other.getBodySize();
	autoindex = other.getAutoIndex();
	nbr_loc = other.getLocationNbr();
	error_page = other.getErrorPage();
	loc.clear();
	for (unsigned long i = 0; i < nbr_loc; i++)
		loc.push_back(other.getLocation(i));
	return *this;
}

const std::string& confData::getAdress() const
{
    return address;
}

std::string confData::getPort() const 
{
    return port;
}
std::string confData::getPath() const 
{
    return path;
}

std::string confData::getServName() const 
{
    return serv_name;
}

std::string confData::getMethod() const 
{
    return method;
}

std::vector<std::string> confData::getIndex() const 
{
    return index;
}

std::string confData::getErrorPage() const 
{
    return error_page;
}

std::string confData::getBodySize() const 
{
    return body_size;
}

std::string confData::getCGIPath() const 
{
    return cgi_upload_path;
}

int confData::getLocationNbr() const 
{
    return nbr_loc;
}

int confData::getAutoIndex() const 
{
    return autoindex;
}

int confData::getCGI() const 
{
    return cgi;
}

const location & confData::getLocation(int index) const
{
    return (loc)[index];
}

const location & confData::getGoodLocation(std::string str) const 
{
    for (unsigned long i = 0; i < nbr_loc; i++)
        if (!(loc)[i].getLocation_name().compare(str))
            return (loc)[i];
    return (loc)[0];
}

const std::vector<std::string> & confData::getRedir() const
{
    return redir;
}

const location& confData::LocationFinder(std::string str) const
{
    for (unsigned long i = 1; i < nbr_loc; i++)
    {
		std::string name = str.substr(0, loc[i].getLocation_name().size());
        if (!(loc)[i].getLocation_name().compare(name))
            return (loc)[i];
	}
    return (loc)[0];
}

int confData::LocationExist(std::string str)
{
    for (unsigned long i = 0; i < nbr_loc; i++)
		if (!loc[i].getLocation_name().compare(str))
            return 1;
    return 0;
}

void confData::setAddress(std::string str)
{
    std::vector<std::string> tmp;
    remove_spaces(str);
    address = str.substr(strlen("listen "), str.size());
    if (address.empty() || address.find(":") == std::string::npos)
        return;
	splitstring(address, tmp, ':');
    address = tmp[0];
    port = tmp[1];
	port = port.substr(0, port.size() - 1);
}

void confData::setPath(std::string str)
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

void confData::setServName(std::string str)
{
    remove_spaces(str);
    serv_name = str.substr(strlen("server_name "), str.size());
    if (serv_name.empty())
        return;
    serv_name.resize(serv_name.size() - 1);
}
void confData::setMethod(std::string str)
{
    remove_spaces(str);
    method = str.substr(strlen("methods "), str.size());
    if (method.empty())
        return;
    method.resize(method.size() - 1);
}

void confData::setErrorPage(std::string str)
{
    remove_spaces(str);
    error_page = str.substr(strlen("error_page "), str.size());
    if (error_page.empty())
        return;
    error_page.resize(error_page.size() - 1);
}

void confData::setIndex(std::string str)
{
    std::string tmp;
    remove_spaces(str);
    str = str.substr(str.find(" "), str.size());
    while(str.find(" ") != std::string::npos)
    {
        tmp = str.substr(0, str.find(" "));
        index.push_back(tmp);
        if (str.empty())
            return;
        str = str.substr(str.find(" ") + 1, str.size());
    }
	str = str.substr(0, str.size() - 1);
	str = str.substr(0, str.size() - 1);
    index.push_back(str);
	for (unsigned long i = 0; i < index.size(); i++)
	{
		//std::cout << "Index is |" << index[i] << "|" << std::endl;
	}
    if (index.back().empty())
        return;
}

void confData::setAutoIndex(std::string str)
{
    remove_spaces(str);
    if (!str.compare("autoindex on;"))
        autoindex = 1;
    else if (!str.compare("autoindex off;"))
        autoindex = 0;
}

void confData::setCGI(std::string str)
{
    remove_spaces(str);
    if (!str.compare("cgi on;"))
        cgi = 1;
    else if (!str.compare("cgi off;"))
        cgi = 0;
}

void confData::setCGIPath(std::string str)
{
    remove_spaces(str);
    cgi_upload_path = str.substr(strlen("cgi_upload_path "), str.size());
    if (cgi_upload_path.empty())
        return;
    cgi_upload_path.resize(cgi_upload_path.size() - 1);
}

void confData::setBodySize(std::string str)
{
    remove_spaces(str);
    body_size = str.substr(strlen("client_max_body_size "), str.size());
    if (body_size.empty())
        return;
    body_size.resize(body_size.size() - 1);
}

void confData::setRedir(std::string str)
{
    std::string tmp;
    remove_spaces(str);
    str = str.substr(str.find(" ") + 1, str.size());
    while(str.find(" ") != std::string::npos)
    {
        tmp = str.substr(0, str.find(" "));
        if (tmp.find("^") != std::string::npos)
        {
            if (!tmp.compare("^"))
                tmp = "/";
            else
                tmp.erase(0, 1);
        }
        redir.push_back(tmp);
        if (str.empty())
            return;
        str = str.substr(str.find(" ") + 1, str.size());
    }
	str = str.substr(0, str.size() - 1);
    redir.push_back(str);
    if (redir.back().empty())
        return;
}

int confData::parsing(std::string path)
{
    std::ifstream fd;
    std::stringstream buff;
    fd.open(path.data(), std::ifstream::in);
    if (!fd)
        return -1;
    buff << fd.rdbuf();
    data = buff.str();
    if (check_error(data))
        return -1;
    check_quote(data);
    return check_server_nbr(data, "server {");
}

void confData::print_info()
{
    std::cout << "[Default]" << std::endl;
    if (!address.empty())
        std::cout << "Address->         " << "[" << address << "]" << std::endl;
    if (!port.empty())
        std::cout << "Port->            " << "[" << port << "]" << std::endl;
    if (!path.empty())
        std::cout << "Path->            " << "[" << path << "]" << std::endl;
    if (!serv_name.empty())
        std::cout << "Serv_name->       " << "[" << serv_name << "]" << std::endl;
    else
        std::cout << "Serv_name->       " << "[..........]" << std::endl;
    if (!index.empty())
    {
        std::cout << "Index->           ";
        for (unsigned long i = 0;i < index.size(); i++)
            std::cout << "[" << index[i] << "]";
        std::cout << std::endl;
    }
    if (!redir.empty())
    {
        std::cout << "redir->           ";
        for (unsigned long i = 0;i < redir.size(); i++)
            std::cout << "[" << redir[i] << "]";
        std::cout << std::endl;
    }
    if (!method.empty())
        std::cout << "Method->          " << "[" << method << "]" << std::endl;
    if (!cgi_upload_path.empty())
        std::cout << "cgi_upload_path->          " << "[" << cgi_upload_path << "]" << std::endl;
    if (!error_page.empty())
        std::cout << "Error_page->      " << "[" << error_page << "]" << std::endl;
    if (!body_size.empty())
        std::cout << "Body->            " << "[" << body_size << "]" << std::endl;
    std::cout << "Autoindex->       " << "[" << autoindex << "]" << std::endl;
    std::cout << "Cgi->             " << "[" << cgi << "]" << std::endl;
    for (unsigned long x = 0; x < nbr_loc; x++)
        (loc)[x].print_info();
    std::cout << "--------------------------------------" << std::endl;
}

std::string location_str(std::string str)
{
    std::string tmp;
    
    if (str.find("location"))
        str = str.substr(str.find("location"), str.size());
    while(1)
    {
        tmp += str.substr(0, str.find("\n"));
        tmp += "\n";
        str = str.substr(str.find("\n") + 1, str.size());
        if (tmp.find("}") == std::string::npos)
            continue;
        else    
            break;
    }
    return tmp;
}

int confData::scrapData()
{
    std::string tmp;
    std::string cpy_data(data);
    tmp = data.substr(0, data.find("\n"));
    if (!tmp.find("server {"))
        data = data.substr(data.find("\n") + 1, data.size());
    else
        return 1;
    while (!data.empty())
    {   
        tmp = data.substr(0, data.find("\n"));
        std::cout << "ah : "<<tmp << std::endl;
        if (!tmp.find("server {") || tmp.empty() ||tmp.find("{") != std::string::npos || tmp.find("location") != std::string::npos)
            break;
        data = data.substr(data.find("\n") + 1, data.size());
        if (tmp.size() >= 1 && tmp[tmp.size() - 1] != ';')
            return printerr("Error with conf file syntax ...");
        else if (tmp.find("listen")!= std::string::npos)
            setAddress(tmp);
        else if (tmp.find("server_name")!= std::string::npos)
            setServName(tmp);
        else if (tmp.find("root")!= std::string::npos)
            setPath(tmp);
        else if (tmp.find("methods")!= std::string::npos)
            setMethod(tmp);
        else if (tmp.find("error_page")!= std::string::npos)
            setErrorPage(tmp);
        else if (tmp.find("autoindex")!= std::string::npos)
            setAutoIndex(tmp);
        else if (tmp.find("cgi_upload_path")!= std::string::npos)
            setCGIPath(tmp);
        else if (tmp.find("cgi")!= std::string::npos)
            setCGI(tmp);
        else if (tmp.find("index")!= std::string::npos)
            setIndex(tmp);
        else if (tmp.find("client_max_body_size")!= std::string::npos)
            setBodySize(tmp);
        else if (tmp.find("return") != std::string::npos)
            setRedir(tmp);
        else
            return printerr("your config file ...");
    }
    if (!scrapLocation())
        return 0;
    return 1; 
}

int confData::scrapLocation()
{
    std::string cpy_data(data);
    
    std::cout << "data is == " << data << std::endl;
    if (data.find("server {") != std::string::npos)
        data = data.substr(data.find("server {"), data.size());
    if (path.empty())
        path = "./";
    nbr_loc = check_location_nbr(cpy_data, "location ");
    std::cout << "Nbr of location : " << nbr_loc << std::endl;
    loc.reserve(nbr_loc);
    loc.resize(nbr_loc);
    for(unsigned long x = 0; x < nbr_loc; x++)
        complete_loc(x);
    for (unsigned long x = 0; x < nbr_loc; x++)
    {
        if (!((loc)[x].scrapData(location_str(cpy_data))))
            return 0;
        cpy_data = cpy_data.substr(cpy_data.find("location") + 1, cpy_data.size());
    }
    return 1;
}

void confData::complete_loc(int i)
{
    std::vector<std::string> vec;
    if (loc[i].getPath().empty())
        loc[i].edit_info("path", getPath(), vec);
        
    if (loc[i].getMethod().empty())
        loc[i].edit_info("method", getMethod(), vec);

    if (loc[i].getErrorPage().empty())
        loc[i].edit_info("error_page", getErrorPage(), vec); 
        
    if (loc[i].getAutoIndex() != getAutoIndex())
        loc[i].edit_info("autoindex", itoa(getAutoIndex()), vec); 
        
    if (loc[i].getIndex().empty())
            loc[i].edit_info("index", "", getIndex());
    
    if (loc[i].getBodySize().empty())
        loc[i].edit_info("body_size", getBodySize(), vec);
    
    if (loc[i].getCGI() != getCGI())
        loc[i].edit_info("cgi", itoa(getCGI()), vec);

    if (loc[i].getCGIPath().empty())
        loc[i].edit_info("cgi_upload_path", getCGIPath(), vec); 

    if (loc[i].getRedir().empty())
        loc[i].edit_info("redir", "", getRedir());
}

void clear_info()
{
    
}

int confData::check_error(std::string data)
{
    std::vector<std::string> lines;
    splitstring(data, lines, '\n');
    int flag_serv = 0;
    int flag_location = 0;
    for (unsigned long i = 0; i < lines.size(); i++)
    {
        std::cout << flag_serv << "hihhi : "<<lines[i] << std::endl;
        remove_spaces(lines[i]);
        if (lines[i].empty())
            continue;
        else if (lines[i].find("server {") != std::string::npos && flag_serv == 0)
        {
            if (lines[i].find('{') == std::string::npos && i + 1 < lines.size() && lines[i + 1].find('{') != std::string::npos)
                i++;
            flag_serv = 1;
        }
        else if (lines[i].find("location") != std::string::npos && flag_serv == 1 && flag_location == 0)
        {
            if (lines[i].find('{') == std::string::npos && i + 1 < lines.size() && lines[i + 1].find('{') != std::string::npos)
                i++;
            flag_location = 1;
        }
        else if (lines[i].find('}') != std::string::npos)
        {
            if (flag_location == 1)
                flag_location = 0;
            else if (flag_serv == 1)
                flag_serv = 0;
        }
        else if (lines[i][lines[i].size() - 1] != ';' && flag_serv == 1)
            return 1;
    }
    return 0;
}