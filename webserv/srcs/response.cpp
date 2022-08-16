/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chly-huc <chly-huc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 00:42:01 by shyrno            #+#    #+#             */
/*   Updated: 2022/08/11 15:43:45 by chly-huc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/header.hpp"

Response::Response():content_lenght("0")
{
	    
}

Response::~Response()
{

}

Response::Response(const Response & other)
{
    *this = other;
}

Response &Response::operator=(Response const & other)
{
	return (*this);
}

void Response::find_method(Request & req, confData & conf)
{
    std::cout << req.getMethod() << std::endl;
    if (req.getMethod() == "GET")
        getMethod(req, conf);
    if (req.getMethod() == "DELETE")
        delMethod();
    if (req.getMethod() == "POST")
        postMethod();
}

int Response::setStatus(Request& req, confData& conf) // Todo : make the status code work
{
    if (conf.findLocation(req.getUrl()) >= 0)
        return 200;
	else
		return 404;
}

std::string Response::setStatMsg()
{
    if (true)
        return "OK";
}

std::string Response::setContentType()
{
    if (true)
        return "text/html";
}

int how_many(std::string str)
{
    int count = 0;
    int i = -1;
    while(str[++i])
        if(isspace(str[i]))
            count++;
    std::cout << count << std::endl;
    return count;
}
 
void Response::getMethod(Request & req, confData & conf)
{
    version = req.getVersion();
    status = setStatus(req, conf);
    stat_msg = setStatMsg();
    std::cout << "test ------------------" << std::endl << std::endl;
    body = readHTML(conf, req.getUrl(), goodIndex(conf, req.getUrl()));
   // exit(0);
    content_lenght = itoa(body.size() + how_many(body));
    content_type = req.gettype_data();
    std::cout << "here -------------------:" << std::endl << std::endl;
}

void Response::postMethod()
{
    
}

void Response::delMethod()
{

}

void Response::concat_response()
{
    std::cout << "content_length  ======== " << content_lenght << std::endl;
    full_response = version + ' ' + itoa(status) + ' ' + stat_msg + '\n' + "Content-Type: " + content_type + '\n' + "Content-Lenght: " + content_lenght + "\n\n" + body;
}

std::string Response::getResponse()
{
    return full_response;
}

std::string Response::getContentLenght()
{
    return content_lenght;
}

std::string Response::getContentType()
{
    return content_type;
}

std::string Response::getBody()
{
    return body;
}

std::string Response::getFullResponse()
{
    return full_response;
}

void Response::init()
{
    
}
