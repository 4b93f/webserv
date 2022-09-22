/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   confData.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shyrno <shyrno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/08 05:45:59 by shyrno            #+#    #+#             */
/*   Updated: 2022/09/22 08:58:47 by shyrno           ###   ########.fr       */
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
        int parsing(std::string path);
        int getLocationNbr() const;
        int getAutoIndex() const;
        const location& LocationFinder(std::string str) const;
        const std::string& getAdress() const;
        std::string getPort() const;
        std::string getPath() const;
        std::string getServName() const;
        std::string getMethod() const;
        std::vector<std::string> getIndex() const;
        std::string getErrorPage() const;
        std::string getBodySize() const;
        const std::vector<std::string> & getRedir() const;
        const location & getLocation(int index) const;
        const location & getGoodLocation(std::string str) const;
        void print_info();
        void scrapData();
        void setAddress(std::string str);
        void setPath(std::string str);
        void setServName(std::string str);
        void setMethod(std::string str);
        void setIndex(std::string str);
        void setErrorPage(std::string str);
        void setBodySize(std::string str);
        void setAutoIndex(std::string str);
        void setRedir(std::string str);
        void complete_loc(int i);
    private:
        std::string address;
        std::string port;
        std::string path;
        std::string serv_name;
        std::string method;
        std::vector<std::string> index;
        std::string error_page;
        std::string body_size;
        int autoindex;
        unsigned long nbr_loc;
        std::vector<location> loc;
        std::vector<std::string> redir;
};

#endif
