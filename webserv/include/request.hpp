/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chly-huc <chly-huc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 01:02:47 by shyrno            #+#    #+#             */
/*   Updated: 2022/08/06 20:21:29 by chly-huc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "header.hpp"

class Request
{
    public:
        Request();
        ~Request();
        Request(const Request & other);
        void getInfo(int connection);
        std::string getUrl();
        std::string getMethod();
        std::string getVersion();
        std::string getHeader();
        std::string getBody();
        std::string gettype_data();
        
    private:
        std::string method;
        std::string url;
        std::string version;
        std::string header;
        std::string body;
        std::string type_data;
};

#endif