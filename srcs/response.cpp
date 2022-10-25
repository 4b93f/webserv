/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shyrno <shyrno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 00:42:01 by shyrno            #+#    #+#             */
/*   Updated: 2022/10/21 23:24:57 by shyrno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "response.hpp"

Response::Response():content_length("0")
{
	
}

Response::~Response()
{
	std::cout << "Response destructor called" << std::endl;
}

Response::Response(const Response & other)
{
    *this = other;
}

Response &Response::operator=(Response const & other)
{
	version = other.getVersion();
	status = other.getStatus();
	setStatMsg();
	content_type = other.getContentType();
	body = other.getBody();
	full_response = other.getFullResponse();
	version = other.getVersion();
	status = other.getStatus();
	setStatMsg();
	content_type = other.getContentType();
	full_response = other.getFullResponse();
	return (*this);
}

void Response::find_method(webServ & web, int i)
{
	clear_info();
	version = web.getReq().getVersion();
	std::string loc = location_exe(web.getConf().getConflist(i), web.getReq().getUrl());
	std::string method;
    if (!loc.empty())
	{
		method = web.getConf().getConflist(i).getGoodLocation(loc).getMethod();
		web.setErrorPage(web.getConf().getConflist(i).getGoodLocation(loc).getErrorPage());
	}
	else
	{
		method = web.getConf().getConflist(i).getMethod();
		web.setErrorPage(web.getConf().getConflist(i).getErrorPage());
	}
    if (web.getReq().getMethod() == "GET" && method.find("GET") != std::string::npos)
        MethodGet(web, web.getConf().getConflist(i));
    else if (web.getReq().getMethod() == "DELETE" && method.find("DELETE") != std::string::npos)
        MethodDel(web, web.getConf().getConflist(i));
    else if (web.getReq().getMethod() == "POST" && method.find("POST") != std::string::npos)
        MethodPost(web, web.getConf().getConflist(i));
	else
	{
		setBody(error_parse(405));
        setStatus(405);
        setStatMsg();
	}
}

void Response::setStatus(int status) // Todo : make the status code work
{
    this->status = status;
	setStatMsg();
}

void Response::setStatMsg()
{
	switch(status)
	{
		// case 100:
		// 	stat_msg = "Continue";
		// 	break;
		// case 101:
		// 	stat_msg = "Switching protocols";
		// 	break;
		// case 102:
		// 	stat_msg = "Processing";
		// 	break;
		// case 103:
		// 	stat_msg = "Early Hints";
		// 	break;
		case 200:
			stat_msg = "OK";
			break;
		// case 201:
		// 	stat_msg = "Created";
		// 	break;
		// case 202:
		// 	stat_msg = "Accepted";
		// 	break;
		// case 203:
		// 	stat_msg = "Non-Authoritative Information";
		// 	break;
		// case 204:
		// 	stat_msg = "No Content";
		// 	break;
		case 205:
			stat_msg = "Reset Content";
			break;
		// case 206:
		// 	stat_msg = "Partial Content";
		// 	break;
		// case 207:
		// 	stat_msg = "Multi-Status";
		// 	break;
		// case 208:
		// 	stat_msg = "Already Reported";
		// 	break;
		// case 226:
		// 	stat_msg = "IM Used";
		// 	break;
		// case 300:
		// 	stat_msg = "Multiple Choices";
		// 	break;
		case 301:
			stat_msg = "Moved Permanently";
			break;
		// case 302:
		// 	stat_msg = "Found";
		// 	break;
		// case 303:
		// 	stat_msg = "See Other";
		// 	break;
		// case 304:
		// 	stat_msg = "Not Modified";
		// 	break;
		// case 305:
		// 	stat_msg = "Use Proxy";
		// 	break;
		// case 306:
		// 	stat_msg = "Switch Proxy";
		// 	break;
		// case 307:
		// 	stat_msg = "Temporary Redirect";
		// 	break;
		// case 308:
		// 	stat_msg = "Permanent Redirect";
		// 	break;
		// case 400:
		// 	stat_msg = "Bad Request";
		// 	break;
		// case 401:
		// 	stat_msg = "Unauthorized";
		// 	break;
		// case 402:
		// 	stat_msg = "Payment Required";
		// 	break;
		case 403:
			stat_msg = "Forbidden";
			break;
		case 404:
			stat_msg = "Not Found";
			break;
		case 405:
			stat_msg = "Method Not Allowed";
			break;
		// case 406:
		// 	stat_msg = "Not Acceptable";
		// 	break;
		// case 407:
		// 	stat_msg = "Proxy Authentication Required";
		// 	break;
		// case 408:
		// 	stat_msg = "Request Timeout";
		// 	break;
		// case 409:
		// 	stat_msg = "Conflict";
		// 	break;
		// case 410:
		// 	stat_msg = "Gone";
		// 	break;
		// case 412:
		// 	stat_msg = "Precondition Failed";
		// 	break;
		case 413:
			stat_msg = "Request entity too large";
			break;
		case 414:
			stat_msg = "URI Too Long";
			break;
		// case 416:
		// 	stat_msg = "Range Not Satisfiable";
		// 	break;
		// case 417:
		// 	stat_msg = "Expectation Failed";
		// 	break;
		// case 418:
		// 	stat_msg = "I'm a Teapot";
		// 	break;
		// case 421:
		// 	stat_msg = "Misdirected Request";
		// 	break;
		// case 422:
		// 	stat_msg = "Unprocessable Entity";
		// 	break;
		// case 423:
		// 	stat_msg = "Locked";
		// 	break;
		// case 424:
		// 	stat_msg = "Failed Dependency";
		// 	break;
		// case 425:
		// 	stat_msg = "Too Early";
		// 	break;
		// case 426:
		// 	stat_msg = "Upgrade Required";
		// 	break;
		// case 428:
		// 	stat_msg = "Precondition Required";
		// 	break;
		// case 429:
		// 	stat_msg = "Too Many Requests";
		// 	break;
		case 431:
			stat_msg = "Request Header Fields Too Large";
			break;
		// case 451:
		// 	stat_msg = "Unavailable For Legal Reasons";
		// 	break;
		case 500:
			stat_msg = "Internal Server Error";
			break;
		// case 501:
		// 	stat_msg = "Not Implemented";
		// 	break;
		// case 502:
		// 	stat_msg = "Bad Gateway";
		// 	break;
		// case 503:
		// 	stat_msg = "Service Unavailable";
		// 	break;
		// case 504:
		// 	stat_msg = "Gateway Timeout";
		// 	break;
		case 505:
			stat_msg = "HTTP Version Not Supported";
			break;
		// case 506:
		// 	stat_msg = "Variant Also Negotiates";
		// 	break;
		// case 507:
		// 	stat_msg = "Insufficient Storage";
		// 	break;
		// case 508:
		// 	stat_msg = "Loop Detected";
		// 	break;
		// case 510:
		// 	stat_msg = "Not Extended";
		// 	break;
		// case 511:
		// 	stat_msg = "Network Authentication Required";
		// 	break;
		default:
			stat_msg = "Unknown";
			break;
	}
  }

void Response::setContentType()
{
	content_type = "text/html";
}

int Response::setContentType(std::string fullpath)
{
	if (fullpath.rfind('.') != std::string::npos)
	{
		std::string type = fullpath.substr(fullpath.rfind(".") + 1, fullpath.size());
		if (type == "html" || type == "htm")
			content_type = "text/html";
        else if (type == "gif")
			content_type = "image/gif";
		else if (type == "css")
			content_type = "text/css";
		else if (type == "js")
			content_type = "text/javascript";
		else if (type == "jpeg" || type == "jpg")
			content_type = "image/jpeg";
		else if (type == "png")
			content_type = "image/png";
		else if (type == "bmp")
			content_type = "image/bmp";
		else if (type == "ico")
			content_type = "image/png";
		else if (type == "php" || type == "py")
        {
            content_type = "text/html";
			return 1;
        }
		else if (type == "ico")
			content_type = "image/png";
		else
			content_type = "text/plain";
	}
	else
		content_type = "text/plain";
	return 0;
}

int how_many(std::string str)
{
    int count = 0;
    int i = -1;
    while(str[++i])
	{
        if (isspace(str[i]))
            count++;
		count++;
	}
    return count;
}
 
void Response::MethodGet(webServ & web, confData & conf)
{
	std::string loc = location_exe(conf, web.getReq().getUrl());
    version = web.getReq().getVersion();
	setStatus(200);
	if (loc.empty())
	{
		if (atoll(conf.getBodySize().data()) > 0 && atoll(conf.getBodySize().data()) < web.getReq().getWrote())
			return web.getRes().setStatus(413);
	}
	else if (atoll(conf.getGoodLocation(loc).getBodySize().data()) > 0 && atoll(conf.getGoodLocation(loc).getBodySize().data()) < web.getReq().getWrote())
		return web.getRes().setStatus(413);
	if (setContentType(web.getReq().getUrl()) == 0)
	{
		body = readfile(web, conf, web.getReq().getUrl());
        if (!web.getbool_redir().first.empty() && !web.getbool_redir().second.empty())
            web.getRes().setStatus(atoi(web.getbool_redir().first.data()));
		setContentLength();
		web.setMax_body_size(is_bodySized(web,conf));
	}
	else
	{
		if (loc.empty())
			if (conf.getCGI() == 0)
				return web.getRes().setStatus(500);
		if (conf.getGoodLocation(loc).getCGI() == 0)
			return web.getRes().setStatus(500);
		web.getCgi().run_api(web, conf);
	}

}

void Response::MethodPost(webServ & web, confData & conf)
{
    int nbr = atoi(web.getReq().getContentLength().data());
	std::string loc = location_exe(conf, web.getReq().getUrl());
	if (loc.empty())
	{
		if (atoll(conf.getBodySize().data()) > 0 && atoll(conf.getBodySize().data()) < web.getReq().getWrote())
			return web.getRes().setStatus(413);
	}
	else if (atoll(conf.getGoodLocation(loc).getBodySize().data()) > 0 && atoll(conf.getGoodLocation(loc).getBodySize().data()) < web.getReq().getWrote())
		return web.getRes().setStatus(413);
    if (setContentType(web.getReq().getUrl()) == 1)
		web.getCgi().run_api(web, conf);
    else if (!nbr)
	{
		setStatus(100);
		setContentType(web.getReq().getUrl());
		concat_response(web);
	}
	else
	{
		if (loc.empty())
		{
			if (conf.getCGI() == 0)
				return web.getRes().setStatus(500);
		}
		if (conf.getGoodLocation(loc).getCGI() == 0)
			return web.getRes().setStatus(500);
		std::vector<std::pair<std::string, std::string> > post(post_arg(web.getReq().getBody(), nbr));
		setStatus(205);
		post_exe(web, post, conf, nbr);
		post.clear();
	}
}

void Response::MethodDel(webServ&  web, confData& conf)
{
	std::string url(web.getReq().getUrl());
    std::string fullpath;
    std::string loc = location_exe(conf, url);
    if (conf.LocationExist(loc))
        fullpath = conf.getGoodLocation(loc).getPath() + url;
	if (remove(fullpath.data()) == 0)
	{
		setStatus(200);
		setContentType();
		setBody("File " + fullpath + " deleted successfully\n");
	}
	else
	{
		setStatus(404);
		setBody("");
	}
}

void Response::concat_response(webServ & web)
{
    if (web.getMax_body_size() > 0 && atoi(content_length.data()) > web.getMax_body_size() && web.getMax_body_size() < web.getReq().getWrote())
	{
		setStatus(413);
		setStatMsg();
		setContentType(".html");
		body = CreateErrorPage(web, 413);
		setContentLength();
		full_response = version + ' ' + itoa(status) + ' ' + stat_msg + '\n' + "Content-Type: " + content_type + '\n' + "Content-Length: " + content_length + "\n\n" + body;
	}
	if (status == 403 || status == 404 || status == 405 || status == 500 || status == 413)
	{
		setStatMsg();
		setContentType(".html");
		body = CreateErrorPage(web, status);
		setContentLength();
		full_response = version + ' ' + itoa(status) + ' ' + stat_msg + '\n' + "Content-Type: " + content_type + '\n' + "Content-Length: " + content_length + "\n\n" + body;
	}
    else if (status == 301)
        full_response = version + ' ' + itoa(status) + ' ' + stat_msg + '\n' + "Location : " + web.getbool_redir().second;
    else
	{
		if (!web.getCgi().getCGIBool())
	    	full_response = version + ' ' + itoa(status) + ' ' + stat_msg + '\n' + "Content-Type: " + content_type + '\n' + "Content-Length: " + content_length + "\n\n" + body;
		else
			full_response = version + ' ' + itoa(status) + ' ' + stat_msg + '\n' + "Content-Length: " + content_length + body;
	}
    web.del_redir();
	web.setErrorPage("");
}

std::string Response::getResponse() const 
{
    return full_response;
}

std::string Response::getContentLength() const 
{
    return content_length;
}

size_t Response::getBodySize() const
{
    return getBody().size();
}
std::string Response::getContentType() const
{
    return content_type;
}

std::string Response::getBody() const 
{
    return body;
}

std::string Response::getFullResponse() const 
{
    return full_response;
}

std::string Response::getVersion() const
{
	return version;
}

void	Response::seterrorpage()
{
	std::ifstream errorpage;
	std::string tmp;
	std::string path("www/error/" + itoa(status) + ".html");
	errorpage.open(path.data());
	if (errorpage.is_open())
	{
		body.clear();
		while(getline(errorpage, tmp))
			body += tmp;
		errorpage.close();
	}
	else
		setBody("");
	setContentLength();
}

void  Response::setContentLength()
{
	body.shrink_to_fit();
	content_length = itoa(body.size());
}

void Response::setBody(std::string str)
{
	body = str;
	setContentLength();
}

int Response::getStatus() const
{
	return status;
}

void Response::clear_info()
{
    version = "";
    status = 0;
    stat_msg = "";
    content_type = "";
    content_length = "";
    body = "";
    full_response = "";
}

void  Response::set_FullResponse(std::string str)
{
	full_response = str;
}