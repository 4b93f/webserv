/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chly-huc <chly-huc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/28 02:23:39 by shyrno            #+#    #+#             */
/*   Updated: 2022/08/11 16:16:49 by chly-huc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/header.hpp"

std::pair<std::string, std::string> find_base_location(confData & conf, std::string url)
{
	for(int x = 0; x < conf.getLocationNbr(); x++)
		if (!conf.getLocation(x).getLocation_name().compare("/"))
			return std::make_pair(url.substr(1, url.size()), conf.getLocation(x).getPath());	
	return std::make_pair(url.substr(1, url.size()), conf.getPath());
}

std::pair<std::string, std::string> goodIndex(confData & conf, std::string url)
{
	int i = -1;
	int index = 0;
	if (!url.compare("/"))
		return std::make_pair(url, conf.getPath());		
	else
	{	
		std::string urlz = url;
		if ((index = urlz.rfind('/')) == 0)
			return find_base_location(conf, urlz);
		urlz = urlz.substr(0, index);
		
		while(++i < conf.getLocationNbr())
		{
			if (!urlz.compare(conf.getLocation(i).getLocation_name()))
			{
				if(!conf.getPath().empty())
					return std::make_pair(urlz,conf.getLocation(i).getPath());
				else
					return std::make_pair(urlz, conf.getPath());
			}
		}
	}
	return std::make_pair("", "");
}

std::string readHTML(confData & conf, std::string req_file, std::pair<std::string, std::string> index) // Need to be change, actually disgusting
{
	int j;
    int autodex = 0;
    struct stat info;
	std::string buff;
	std::string ret;
    std::cout << req_file << " & " << index.first << " & " << index.second << std::endl;
    int inde = conf.findLocation(req_file);
    std::cout << " inde ========= " << inde << std::endl;
  //int inde  = 1;
    std::cout << conf.getLocation(inde).getPath() + req_file << std::endl;
    std::ifstream infile;
    infile.open(conf.getLocation(inde).getPath() + req_file);
    if (infile.is_open())
    {
	while (getline(infile, buff))
		ret += buff + "\n";
    }
   // std::cout << "ret ========== " <<  ret << std::endl; 
    return (ret);
	//std::string tmp(file);
    // if (index.first.empty())
	// 	printerr("Error with index in the conf file ... ");
    // j = -1;
    // //std::cout << "FILE : " << file << std::endl;
    // while(++j < conf.getLocationNbr())
    // {
	// 	if (!conf.getLocation(j).getLocation_name().compare(tmp) && conf.getLocation(j).getAutoIndex())
    //     {
    //         tmp = conf.getLocation(j).getPath() + "/" + conf.getLocation(j).getIndex();
    //         //std::cout << "TMP == " << tmp << std::endl;
    //         tmp = tmp.substr(0, tmp.size());
    //         autodex = 1;
    //     }
    // }
    // if (!autodex)   
    // {   
    //     std::cout << "!!!" << std::endl;
    //     j = tmp.rfind('/');
    //     tmp = file.substr(j + 1, tmp.size());
    //     //std::cout << "index.second == "<< index.second << std::endl;
    //     j = -1;
    //     while(tmp.empty() && ++j < conf.getLocationNbr())
    //         if (conf.getLocation(j).getPath() == index.second)
    //             tmp = index.second.substr(0, index.second.size()) + "/" + conf.getLocation(j).getIndex();
    //     if(!index.second.empty())
    //         tmp = index.second.substr(0, index.second.size()) + "/" + tmp;
    // }
    // std::cout << "TMP == " << tmp << std::endl;

    // if (stat(tmp.c_str(), &info) != 0)
    //     std::cout << "AH\n";
    // if (info.st_mode & S_IFDIR)
    //     tmp = "www/error/403.html";
	// std::ifstream fd (tmp);
    // if (!fd.is_open())
    //     printerr("Error with file opening ... (ReadHTML)");
	// if (!fd.good())
	// 	printerr("File not found ... ");
    // buff << fd.rdbuf();
    // //std::cout << buff.str() << std::endl;
    // return buff.str();
  //  exit(0);
}

std::string itoa(int a)
{
    std::string ss="";   //create empty string
    while(a)
    {
        int x=a%10;
        a/=10;
        char i='0';
        i=i+x;
        ss=i+ss;      //append new character at the front of the string!
    }
    return ss;
}

int printerr(const char *str)
{
    std::cerr << str << std::endl;
    exit(1);
}

static char	**malloc_free(char **str)
{
	int	i;

	i = -1;
	while (str[++i])
		free(str[i]);
	free(str);
	return (NULL);
}

static int	count_words(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i])
	{
		while (str[i] && (str[i] == c))
			i++;
		if (str[i++] && str[i] != c)
			while (str[i] && str[i] != c)
				i++;
	}
	return (i);
}

static char	*malloc_word(char *str, char c)
{
	char	*word;
	int		i;

	i = 0;
	while (str[i] && str[i] != c)
		i++;
	word = (char*)malloc(sizeof(char) * (i + 1));
	if (!word)
		return (NULL);
	i = -1;
	while (str[++i] && str[i] != c)
		word[i] = str[i];
	word[i] = '\0';
	return (word);
}

char	**ft_split(char *str, char charset)
{
	int		i;
	char	**tab;

	i = 0;
	if (!str)
		return (NULL);
	tab = (char**)malloc(sizeof(char *) * (count_words(str, charset) + 1));
	if (!tab)
		return (NULL);
	while (*str)
	{
		while (*str && (*str == charset))
			str++;
		if (*str && *str != charset)
		{
			tab[i++] = malloc_word(str, charset);
			if (!tab)
				return (malloc_free(tab));
			while (*str && *str != charset)
				str++;
		}
	}
	tab[i] = NULL;
	return (tab);
}

int check_quote(std::string str) 
{
	int i = -1;
	int open = 0;
	int close = 0;
	while (str[++i])
	{
		if (str[i] == '{')
			open++;
		if (str[i] == '}')
			close++;
	}
	if (close != open)
		return printerr ("Error with configuration file  ...");
	return 1;
}

int check_server_nbr(std::string str, std::string to_find)
{
	int count = 0;
	int find = 0;
	if (str.empty() || to_find.empty())
		return 0;
	while(1)
	{
		count = str.find(to_find, count);
		if (count >= strlen(str.c_str()) - 1 || count == -1)
			break;
		count += 6;
		find += 1;
	}
	return find;
}

int	words(std::string c, std::string str)
{
	int	i = 0, j = 0, nb = 0;
	
	while (str[i])
	{
		if(str[i] && str[i] == c[j])
		{
			while(str[i] && str[i] == c[j])
			{
				i++;
				j++;
			}
			if (c[j] == '\0')
				nb++;
			j = 0;
		}
		i++;
	}
	return (nb);
}

char	*next_string(std::string str , std::string c)
{
	char	*word;
	int		i;
	int	j = 0;
	int stock = 0;

	i = -1;
	while(str[++i])
	{
		if(str[i] && str[i] == c[j] && i > 0)
		{
			while(str[i] && str[i] == c[j])
			{
				i++;
				j++;
			}
			if (c[j] == '\0')
			{
				stock = i - strlen(c.c_str());
				i -= j;
				break;
			}
			j = 0;
		}
		stock = i + 1;
	}
	word = (char*)malloc(sizeof(char) * (i + 1));
	if (!word)
		return (NULL);
	i = -1;
	while (str[++i] && i < stock)
		word[i] = str[i];
	word[i] = '\0';
	return (word);
}

char	**server_split(std::string str, std::string strset)
{
	// maybe in c++
	int		i;
	int j  = 0;
	char	**tab;

	i = 0;
	int nbword = words(strset, str);
	tab = (char**)malloc(sizeof(char *) * nbword + 1);
	if (!tab)
		return (NULL);
	while (i < nbword)
	{
		if (i > 0)	
			str = str.substr(strlen(tab[i - 1]));
		tab[i] = next_string(str, strset);
		if (!tab)
			return (malloc_free(tab));
		i++;
	}
	tab[i] = NULL;
	return (tab);
}

void print_tab(char **tab)
{
    int i = 0;
    while(tab[i])
    {
        std::cout << "[i]-> " << i <<  tab[i] << std::endl;
        i++;
    }
}

void print(std::string str)
{
    std::cout << str <<std::endl;
}


char *strnstr(const char *s, const char *find, size_t slen)
{
	char c, sc;
	size_t len;

	if ((c = *find++) != '\0') {
		len = strlen(find);
		do {
			do {
				if (slen-- < 1 || (sc = *s++) == '\0')
					return (NULL);
			} while (sc != c);
			if (len > slen)
				return (NULL);
		} while (strncmp(s, find, len) != 0);
		s--;
	}
	return ((char *)s);
}

void remove_spaces(std::string &str)
{
    int i = 0;
    int j = str.size() - 1;
    while (str[i])
    {
        while(isspace(str[i]))
            i++;
        while(isspace(str[j]))
            j--;
        str = str.substr(i, j);
        break;
    }
}
