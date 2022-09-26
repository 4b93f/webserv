/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chly-huc <chly-huc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 01:02:50 by shyrno            #+#    #+#             */
/*   Updated: 2022/09/24 16:48:50 by chly-huc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "request.hpp"

Request::Request()
{
    
}

Request::~Request()
{
    type_data.clear();
}

Request::Request(const Request & other):method(other.getMethod()), url(other.getUrl()), version(other.getVersion()), header(other.getHeader()), body(other.getBody()), type_data(other.getDataType())
{
}
            
Request& Request::operator=(const Request& other)
{
	method = other.getMethod();
	url = other.getUrl();
	version = other.getVersion();
	header = other.getHeader();
	body = other.getBody();
	type_data = other.getDataType();
	return *this;

}

void Request::getInfo(int connection)
{
    std::vector<std::string> req, req2, req3;
    int ret;
    char buff[10000];
    if ((ret = recv(connection, buff, sizeof(buff), 0)) < 0)
        printerr("Error with recv ...");
    std::string str_temp = buff;
    std::cout << "oof" << std::endl;
	if (ret == 0)
       printerr("Error with recv : Connection close ...");
    std::cout << "---------------------------\n" << buff << "\n-------------------------\n";
 //   req = ft_split(buff, '\n');
 //   req2 = ft_split(req[0], ' ');
    splitstring(buff, req, '\n');
	splitstring(req[0], req2, ' ');
    if (!req2[0].compare("GET") || !req2[0].compare("POST") || !req2[0].compare("DELETE"))
        method = req2[0];
    url = req2[1];
    version = req2[2];
    version.resize(strlen(version.c_str()) - 1);
	type_data.clear();
	std::string str;
    std::string searched = "Accept: ";
    for (unsigned long i = 0; i < req.size(); i++)
    {
        str = req[i];
        if (!str.substr(0, searched.size()).compare(searched))
        {
			splitstring(str.substr(searched.size(), str.size()), type_data, ',');
            break ;
        }
    }
    int index;
    std::string tmp = str_temp;
    while(tmp.find("\n") != std::string::npos)
    {
        print("wtf");
        std::string tmp_2;
        index = tmp.find("\n");
        tmp_2 = tmp.substr(0, index);
        if(tmp_2.empty())
        {
            print("tmp is empty");
            break;
        }
        header += tmp_2 + "\n";
        tmp = tmp.substr(index + 1, tmp.size());
    }
    body =  tmp;
    print("STR_TEMP : \n");
    print(str_temp);
    print("INDEX : \n");
    std::cout << index << std::endl;
    print("HEADER : \n");
    print(header);
    print("HEADER END \n");
    print("BODY : \n");
    print(body);
    print("BODY END \n");
    return;
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
