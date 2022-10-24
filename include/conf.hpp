/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conf.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chly-huc <chly-huc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 05:49:12 by shyrno            #+#    #+#             */
/*   Updated: 2022/09/27 18:54:03 by chly-huc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONF_HPP
#define CONF_HPP

#include "header.hpp"
#include <vector>

class confData;

class Conf
{
    public:
        Conf();
        ~Conf();
        Conf(const Conf & other);
		Conf& operator=(const Conf& other);
        void print_info();
        int parsing(std::string path);
        
        confData& getConflist(int index);
        long getNbrServer() const;
        std::string getPort(int index) const;
        std::string getPath(int index) const;
        std::string getMethod(int index) const;
        std::string getAddress(int index) const;
        std::string getServName(int index) const;
        std::vector<std::string> getIndex(int index) const;
		const std::vector <confData> getVectorConflist() const;
        
    private:
        long nbr_serv;
        std::vector<confData> confList;
};

#endif

