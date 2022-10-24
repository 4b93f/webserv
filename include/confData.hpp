/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   confData.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chly-huc <chly-huc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/08 05:45:59 by shyrno            #+#    #+#             */
/*   Updated: 2022/09/27 19:16:08 by chly-huc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFDATA_HPP
#define CONFDATA_HPP

#include "header.hpp"

class location;

class confData
{
    public:
        confData();
        ~confData();
        confData(const confData & other);
		confData& operator=(const confData& other);
        void print_info();
        int scrapData();
        int scrapLocation();
        void complete_loc(int i);
        int parsing(std::string path);
        int LocationExist(std::string str);
        void clear_info();
        const location& LocationFinder(std::string str) const;
        
        int getAutoIndex() const;
        int getLocationNbr() const;
        std::string getPort() const;
        std::string getPath() const;
        std::string getMethod() const;
        std::string getBodySize() const;
        std::string getServName() const;
        std::string getErrorPage() const;
        int getCGI() const;
        std::string getCGIPath() const;
        const std::string& getAdress() const;
        std::vector<std::string> getIndex() const;
        const location & getLocation(int index) const;
        const std::vector<std::string> & getRedir() const;
        const location & getGoodLocation(std::string str) const;
        
        void setPath(std::string str);
        void setIndex(std::string str);
        void setRedir(std::string str);
        void setMethod(std::string str);
        void setAddress(std::string str);
        void setServName(std::string str);
        void setBodySize(std::string str);
        void setErrorPage(std::string str);
        void setAutoIndex(std::string str);
        void setCGI(std::string str);
        void setCGIPath(std::string str);
    private:
        int autoindex;
        int cgi;
        std::string port;
        std::string path;
        std::string method;
        std::string address;
        std::string serv_name;
        std::string body_size;
        std::string cgi_upload_path;
        unsigned long nbr_loc;
        std::string error_page;
        std::vector<location> loc;
        std::vector<std::string> index;
        std::vector<std::string> redir;
};

#endif

