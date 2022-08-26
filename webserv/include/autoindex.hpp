/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autoindex.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chly-huc <chly-huc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/26 17:45:15 by chly-huc          #+#    #+#             */
/*   Updated: 2022/08/18 22:12:15 by chly-huc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AUTOINDEX_HPP
#define AUTOINDEX_HPP

#include "../include/header.hpp"

class Autodex
{
    public:
        Autodex();
        ~Autodex();
        Autodex(const Autodex & other);
		Autodex& operator=(const Autodex& other);
        Autodex(std::string path, confData & conf);
        std::string create_dex(webServ & web, confData & conf, std::string url);
        std::string getIndexStr() const;
    private:
        std::string index_str;
};

#endif
