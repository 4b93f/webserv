/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autoindex.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chly-huc <chly-huc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/26 17:45:15 by chly-huc          #+#    #+#             */
/*   Updated: 2022/09/13 18:27:06 by swautele         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AUTOINDEX_HPP
#define AUTOINDEX_HPP

#include "header.hpp"
class webServ;

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
