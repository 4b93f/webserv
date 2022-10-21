/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shyrno <shyrno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 01:02:50 by shyrno            #+#    #+#             */
/*   Updated: 2022/10/21 03:42:14 by shyrno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "request.hpp"

#define BUFF_SIZE 10000

Request::Request()
{
    brutbody = tmpfile();
}

Request::~Request()
{
    type_data.clear();
    std::cout << "Request destructor called" << std::endl;
}

Request::Request(const Request & other):method(other.getMethod()), url(other.getUrl()), version(other.getVersion()), header(other.getHeader()), body(other.getBody()), type_data(other.getDataType())
{
    brutbody = tmpfile();
}
   
Request& Request::operator=(const Request& other)
{
	method = other.getMethod();
	url = other.getUrl();
	version = other.getVersion();
	header = other.getHeader();
	body = other.getBody();
	type_data = other.getDataType();
    brutbody = tmpfile();
	return *this;
}

int Request::getInfo(int connection, std::string string)
{
    (void)connection;
    //std::cout << "|" << string << "|" << std::endl;
    clear_info();
    std::vector<std::string> req, req2;

    //string = buff;
 	splitstring(string, req, '\n');
	splitstring(req[0], req2, ' ');
    if (!req2[0].compare("GET") || !req2[0].compare("POST") || !req2[0].compare("DELETE"))
        method = req2[0];
    url = req2[1];
    version = req2[2];
    version.resize(version.size() - 1);
	type_data.clear();
    _search_info(req, string);
    //std::cout  << " ici =====================================" << std::endl << body << std::endl;
    // char* temp = NULL;
    // if (string.find("\r\n\r\n") != std::string::npos)
    // {
    //     temp = (char*)string.data();
    //     temp += string.find("\r\n\r\n") + 4;
    // }
    // char* temp2 = to_char(content_length);
    // if (temp)
    //     Write_Brutbody(temp, string.size() - (string.find("\r\n\r\n") + 4));
    // delete[] temp2;
    if (url.find("?") != std::string::npos)
    {
        query_s = url.substr(url.find("?") + 1, url.size());
        url = url.substr(0 ,url.find("?"));
        //std::cout << " BUT " << query_s << std::endl;
    }
    clean_header();
    //std::cout << std::endl << "Header is ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::" << std::endl << header << std::endl << "-----------------------------------------------------------------------" << std::endl;
    //std::cout << "BODY IS ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::" << std::endl << body << std::endl << "--------------------------------------------------------------" << std::endl;
    req.clear();
    req2.clear();
    return 1;
}

void Request::_search_info(std::vector<std::string> req, std::string buff)
{
    
    std::string str;
    std::string searched = "Accept: ";
    for (unsigned long i = 0; i < req.size(); i++)
    {
        if (!searched.empty())
            break;
        str = req[i];
        str.resize(str.size() - 1);
        if (!str.substr(0, searched.size()).compare(searched))
        {
			splitstring(str.substr(searched.size(), str.size()), type_data, ',');
            break ;
        }
    }
    content_length = search_value_vect(req, "Content-Length:");
    content_type = search_value_vect(req, "Content-Type:");
    if (content_length.empty())
        content_length = "0";
    else
        content_length.resize(content_length.size() - 1);
    int index;
    while(buff.find("\n") != std::string::npos)
    {
        std::string tmp_2;
        index = buff.find("\n");
        tmp_2 = buff.substr(0, index);
        if (tmp_2.empty())
        {
            print("Body , or not has been found");
            break;
        }
        header += tmp_2 + "\n";
        buff = buff.substr(index + 1, buff.size());
    }
    if (!content_length.empty())
        body = buff;
    
}

std::string Request::getUrl() const 
{
    return url;
}

std::string Request::getMethod() const 
{
    return method;
}

std::string Request::getVersion() const 
{
    return version;
}

std::string Request::getHeader() const 
{
    return header;
}

const std::vector<std::string>&	Request::getDataType() const
{
	return type_data;
}

std::string Request::getBody() const 
{
    return body;
}

std::string Request::getContentType() const 
{
    return content_type;
}

std::string Request::getContentLength() const
{
    return content_length;
}

void Request::clear_info()
{
    header = "";
    url = "";
    body = "";
    version = "";
    query_s = "";

    if (brutbody->_fileno > 0)
        fclose(brutbody);
    brutbody = tmpfile();
    wrote = 0;
    std::cout << "out" << std::endl;
}

std::string Request::getQuery_string()
{
    return query_s;
}
int Request::getBrutbody_fileno()
{
    int fd = -1;
    //std::cout << "!\n";
    if (brutbody)
    {
        fd = fileno(brutbody);
        if (fd == -1)
            printerr("Error with brutbody");
    }
    return fd;
}

void    Request::clean_header()
{
    unsigned long decal = 0;
    for (unsigned long i = 0; i + decal < header.size(); i++)
    {
        if (header[i + decal] == '\r')
            decal++;
        if (decal)
            header[i] = header[i + decal];
    }
    header.resize(header.size() - decal);
    if (header[header.size() - 2] == '\n' && header[header.size() - 1] == '\n')
        header.resize(header.size() - 1);
    // DEBUG SHOULD NEVER DISPLAY
    for (unsigned long i = 0; i < header.size(); i++)
    {
        if (header[i] == '\r')
            std::cout << "ERROR WITH HEADER CLEANING " << header.size() - i  << std::endl;
    }
}

int    Request::getWrote() const
{
    return wrote;
}

void        Request::add_Wrote(int read)
{
    if (read > 0)
        wrote += read;
}

void        Request::Write_Brutbody(char* buff, int size)
{
    //std::cout << getBrutbody_fileno() << std::endl;
    if ((write(getBrutbody_fileno(), buff, size) < 0))
        printerr("Error with write..");
    add_Wrote(size);
}

FILE * Request::getBrutBody()
{
    return brutbody;
}