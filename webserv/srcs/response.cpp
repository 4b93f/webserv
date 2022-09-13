/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: simonwautelet <simonwautelet@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 00:42:01 by shyrno            #+#    #+#             */
/*   Updated: 2022/09/13 18:22:27 by swautele         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "response.hpp"

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
	version = other.getVersion();
	status = other.getStatus();
	setStatMsg();
	content_type = other.getContentType();
	body = other.getBody();
	full_response = other.getFullResponse();
	return (*this);
}

void Response::find_method(webServ & web, int i)
{
//std::cout << "find_method = " << web.getReq().getMethod() << std::endl;
//std::cout << "Method available = " << web.getConf().getConflist(i).LocationFinder(web.getReq().getUrl()).getMethod() << std::endl;
//std::cout << "In location : " << web.getConf().getConflist(i).LocationFinder(web.getReq().getUrl()).getLocation_name() << std::endl;
//std::cout << "With URL : " << web.getReq().getUrl() << std::endl;
	version = web.getReq().getVersion();
    if (web.getReq().getMethod() == "GET" && web.getConf().getConflist(i).LocationFinder(web.getReq().getUrl()).getMethod().find("GET") != std::string::npos)
        MethodGet(web, web.getConf().getConflist(i));
    else if (web.getReq().getMethod() == "DELETE" && web.getConf().getConflist(i).LocationFinder(web.getReq().getUrl()).getMethod().find("DELETE") != std::string::npos)
        delMethod(web, web.getConf().getConflist(i));
    else if (web.getReq().getMethod() == "POST" && web.getConf().getConflist(i).LocationFinder(web.getReq().getUrl()).getMethod().find("POST") != std::string::npos)
        MethodPost(web, web.getConf().getConflist(i));
	else
	{
		std::cout << "Method forbidden" << std::endl;
		setStatus(405);
		setContentType();
		setBody("");
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
		case 100:
			stat_msg = "Continue";
			break;
		case 101:
			stat_msg = "Switching protocols";
			break;
		case 102:
			stat_msg = "Processing";
			break;
		case 103:
			stat_msg = "Early Hints";
			break;
		case 200:
			stat_msg = "OK";
			break;
		case 201:
			stat_msg = "Created";
			break;
		case 202:
			stat_msg = "Accepted";
			break;
		case 203:
			stat_msg = "Non-Authoritative Information";
			break;
		case 204:
			stat_msg = "No Content";
			break;
		case 205:
			stat_msg = "Reset Content";
			break;
		case 206:
			stat_msg = "Partial Content";
			break;
		case 207:
			stat_msg = "Multi-Status";
			break;
		case 208:
			stat_msg = "Already Reported";
			break;
		case 226:
			stat_msg = "IM Used";
			break;
		case 300:
			stat_msg = "Multiple Choices";
			break;
		case 301:
			stat_msg = "Moved Permanently";
			break;
		case 302:
			stat_msg = "Found";
			break;
		case 303:
			stat_msg = "See Other";
			break;
		case 304:
			stat_msg = "Not Modified";
			break;
		case 305:
			stat_msg = "Use Proxy";
			break;
		case 306:
			stat_msg = "Switch Proxy";
			break;
		case 307:
			stat_msg = "Temporary Redirect";
			break;
		case 308:
			stat_msg = "Permanent Redirect";
			break;
		case 400:
			stat_msg = "Bad Request";
			break;
		case 401:
			stat_msg = "Unauthorized";
			break;
		case 402:
			stat_msg = "Payment Required";
			break;
		case 403:
			stat_msg = "Forbidden";
			break;
		case 404:
			stat_msg = "Not Found";
			break;
		case 405:
			stat_msg = "Method Not Allowed";
			break;
		case 406:
			stat_msg = "Not Acceptable";
			break;
		case 407:
			stat_msg = "Proxy Authentication Required";
			break;
		case 408:
			stat_msg = "Request Timeout";
			break;
		case 409:
			stat_msg = "Conflict";
			break;
		case 410:
			stat_msg = "Gone";
			break;
		case 411:
			stat_msg = "Length Required";
			break;
		case 412:
			stat_msg = "Precondition Failed";
			break;
		case 413:
			stat_msg = "Payload Too Large";
			break;
		case 414:
			stat_msg = "URI Too Long";
			break;
		case 415:
			stat_msg = "Unsupported Media Type";
			break;
		case 416:
			stat_msg = "Range Not Satisfiable";
			break;
		case 417:
			stat_msg = "Expectation Failed";
			break;
		case 418:
			stat_msg = "I'm a Teapot";
			break;
		case 421:
			stat_msg = "Misdirected Request";
			break;
		case 422:
			stat_msg = "Unprocessable Entity";
			break;
		case 423:
			stat_msg = "Locked";
			break;
		case 424:
			stat_msg = "Failed Dependency";
			break;
		case 425:
			stat_msg = "Too Early";
			break;
		case 426:
			stat_msg = "Upgrade Required";
			break;
		case 428:
			stat_msg = "Precondition Required";
			break;
		case 429:
			stat_msg = "Too Many Requests";
			break;
		case 431:
			stat_msg = "Request Header Fields Too Large";
			break;
		case 451:
			stat_msg = "Unavailable For Legal Reasons";
			break;
		case 500:
			stat_msg = "Internal Server Error";
			break;
		case 501:
			stat_msg = "Not Implemented";
			break;
		case 502:
			stat_msg = "Bad Gateway";
			break;
		case 503:
			stat_msg = "Service Unavailable";
			break;
		case 504:
			stat_msg = "Gateway Timeout";
			break;
		case 505:
			stat_msg = "HTTP Version Not Supported";
			break;
		case 506:
			stat_msg = "Variant Also Negotiates";
			break;
		case 507:
			stat_msg = "Insufficient Storage";
			break;
		case 508:
			stat_msg = "Loop Detected";
			break;
		case 510:
			stat_msg = "Not Extended";
			break;
		case 511:
			stat_msg = "Network Authentication Required";
			break;
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
//std:: cout << "i received fullpath =========== " << fullpath << std::endl;
	if (fullpath.rfind('.') != std::string::npos)
	{
		std::string type = fullpath.substr(fullpath.rfind(".") + 1, fullpath.size());
//std::cout << "looking for type of file with = " << type << std::endl;
		if (type == "html")
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
		else if (type == "php")
			return 1;
		else
			content_type = "text/plain";
	}
	else
		content_type = "text/plain";
	return 0;
//std::cout << "content_type = " << content_type << std::endl;
}

int how_many(std::string str)
{
    int count = 0;
    int i = -1;
    while(str[++i])
        if(isspace(str[i]))
            count++;
//std::cout << count << std::endl;
    return count;
}
 
void Response::MethodGet(webServ & web, confData & conf)
{
    version = web.getReq().getVersion();
	setStatus(200);
	if (setContentType(web.getReq().getUrl()) == 0)
	{
		body = readHTML(web, conf, web.getReq().getUrl());
		setContentLenght();
	}
	else
	{
		run_api(web, conf);
		std::cout << "made my way to api" << std::endl;
	}
}

void Response::MethodPost(webServ & web, confData & conf)
{
//std::cout << "POST\n";
    int nbr = post_element_nbr(web.getReq().getBody());
    if (!nbr)
        printerr("Error: Body doesnt have arguement ...");        
    std::vector<std::pair<std::string, std::string> > post(post_arg(web.getReq().getBody(), nbr));
    post_exe(web, post, conf);
    MethodGet(web, conf);
}

void Response::delMethod(webServ&  web, confData& conf)
{
std::cout << "DELETE Method " << std::endl;
	std::string url(web.getReq().getUrl());
    location loc = conf.LocationFinder(url);
    std::string fullpath(loc.getPath() + url.substr(loc.getLocation_name().size(), url.size()));
	std::cout << "url = " << url << std::endl << "loc = " << loc.getLocation_name() << std::endl << "fullpath = " << fullpath << std::endl;
	if (loc.getLocation_name().empty())
        url = conf.getPath() + url.substr(1, url.size());
    else
    {
        fullpath = loc.getPath() + url.substr(loc.getLocation_name().size(), url.size());
        url = url.substr(1, url.size());
    }
	if(remove(fullpath.c_str()) == 0 )
	{
std::cout << "File " << fullpath << " deleted successfully" << std::endl;
		setStatus(200);
		setContentType();
		setBody("File " + fullpath + " deleted successfully\n");
	}
	else
	{
std::cout << "File could'nt be deleted fullpath was : " << fullpath << std::endl;
		setStatus(404);
		setBody("");
		}
}

void Response::concat_response()
{
	 std::cout << "header response  ========" << std::endl << std::endl <<  version + ' ' + itoa(status) + ' ' + stat_msg + '\n' + "Content-Type: " + content_type + '\n' + "Content-Lenght: " + content_lenght + "\n" << std::endl;
	full_response = version + ' ' + itoa(status) + ' ' + stat_msg + '\n' + "Content-Type: " + content_type + '\n' + "Content-Lenght: " + content_lenght + "\n\n" + body;
//	std::cout << "full_response ---------------------------" << std::endl << std::endl << full_response << std::endl << std::endl;
}

std::string Response::getResponse() const
{
    return full_response;
}

std::string Response::getContentLenght() const
{
    return content_lenght;
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

void Response::init()
{
    
}

void	Response::seterrorpage()
{
	std::ifstream errorpage;
	std::string tmp;
	std::string path("www/error/" + itoa(status) + ".html");
	std::cout << "Try to set errorpage : " << path << std::endl;
	errorpage.open(path.c_str());
	if (errorpage.is_open())
	{
		body.clear();
		while(getline(errorpage, tmp))
		{
			body += tmp;
		}
		errorpage.close();
	}
	else
	{
		body = "";
	}
	setContentLenght();
}

void  Response::setContentLenght()
{
	content_lenght = itoa(body.size());
}

void Response::setBody(std::string str)
{
	body = str;
	setContentLenght();
}

int Response::getStatus() const
{
	return status;
}
