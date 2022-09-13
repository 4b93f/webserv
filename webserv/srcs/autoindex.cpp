/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autoindex.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shyrno <shyrno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/26 17:45:19 by chly-huc          #+#    #+#             */
/*   Updated: 2022/09/13 21:34:08 by shyrno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "autoindex.hpp"

Autodex::Autodex()
{
    std::cout << "Autoindex constructor " << std::endl;
}

Autodex::~Autodex()
{
            
}

Autodex::Autodex(const Autodex & other):index_str(other.getIndexStr())
{
//    *this = other;
}

Autodex& Autodex::operator=(const Autodex& other)
{
	index_str = other.getIndexStr();
	return *this;
}

std::string Autodex::create_dex(webServ & web, confData & conf, std::string url)
{
    DIR *dir;
    
    struct dirent *ent;
    struct stat info;
    std::ofstream out;
    std::string path;
    std::string tmp;
    std::string urlz(url);
    int i = -1;
//    int autodex = 0;

    std::cout << "autoindex" << std::endl;
    std::cout << "--------------AUTOINDEX CREATION---------------" << std::endl;
    if (web.getReq().getUrl().size() > 1)
        path = conf.getPath();
    else
        path = conf.getPath();
    if (path.rfind("/") == path.size() - 1)
        path = path.substr(0, path.size() - 1);
    if (url.rfind("/") == url.size() - 1)
        url = url.substr(0, url.size() - 1);
    if (url[0] == '.' && url[1] == '/')
        url.erase(0, 2);
    if (stat(path.c_str(), &info) != 0)
        // std::cout << "AH\n";
    std::cout << "!!!!!!!!!!!!!!PATH == " << path << std::endl;
    std::cout << "*---------------------------------------------*url == " << url << std::endl;
    while(++i < conf.getLocationNbr())
    {
        if (!conf.getLocation(i).getLocation_name().compare(path))
        {
            // std::cout << "---- " << conf.getLocation(i).getLocation_name() << std::endl;
            // std::cout << "---- " << conf.getLocation(i).getAutoIndex() << std::endl;
           /* autodex = */conf.getLocation(i).getAutoIndex();
            path = conf.getGoodLocation(path).getPath();
            break;
        }
    }
    if ((dir = opendir(url.c_str())) != NULL)
    {
		std::cout << "i begin to write autoindex " << std::endl;
        tmp += "<html>";
		tmp += "<head><title>Index of";
		tmp += url + "</title></head>\n";
		tmp += "<body>";
		tmp += "<h1>Index of ";
		tmp += url + "</h1><hr><pre>";
        if (url.find("/") == std::string::npos)
            url = "";
        else
            url = url.substr(url.find("/"), url.size());
        std::cout << url << std::endl;
        while ((ent = readdir(dir)) != NULL)
        {
            if (strcmp(ent->d_name, "."))
            {
                    std::cout << "---- " << url << std::endl;
                    std::cout << "---- " << path << std::endl;
                    tmp += "<a href=";
                    if (!strcmp(ent->d_name, ".."))
                    {
                        tmp += url.substr(0, url.rfind("/"));
                        tmp += "/";
                        tmp += ">";
                        tmp += ent->d_name;
                        tmp += " </a>\n";
                    }
                    else
                    {
                        if (!url.empty())
                        {
                            tmp += url;
                            tmp += "/";
                        }
                        // std::cout << "Hyperlink " << path + "/" + ent->d_name << std::endl;
                        tmp += ent->d_name;
                        tmp += ">";
                        tmp += ent->d_name;
                        tmp += " </a>\n";
                    }
                    // std::cout << "NAME : " <<ent->d_name << std::endl;
            }
        }
        closedir(dir);
        tmp += "</pre><hr></body>";
		tmp += "</html>";
    }
    index_str = tmp;
    std::cout << index_str << std::endl;
    web.getRes().setContentType(".html");
    return index_str;
}

std::string Autodex::getIndexStr() const
{
    return index_str;
}

/*
<p>You can reach Michael at:</p>

<ul>
  <li><a href="https://example.com">Website</a></li>
  <li><a href="mailto:m.bluth@example.com">Email</a></li>
  <li><a href="tel:+123456789">Phone</a></li>
</ul>
*/
