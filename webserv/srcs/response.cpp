/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chly-huc <chly-huc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 00:42:01 by shyrno            #+#    #+#             */
/*   Updated: 2022/09/26 15:38:35 by chly-huc         ###   ########.fr       */
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
			stat_msg = "Non-Authoritative Information"; //Ce code de statut peut apparaître lorsqu’un proxy est utilisé.
                                                        // Cela signifie que le serveur proxy a reçu du serveur d’origine
                                                        // un code de statut 200 « Everything is OK », mais qu’il a modifié
                                                        // la réponse avant de la transmettre à votre navigateur.
			break;
		case 204:
			stat_msg = "No Content"; //Ce code signifie que le serveur a bien traité la requête,
                                     // mais qu’il ne retournera aucun contenu.
			break;
		case 205:
			stat_msg = "Reset Content"; //Comme un code 204, cela signifie que le serveur a traité la requête
                                        // mais ne va pas renvoyer de contenu.
                                        // Cependant, il faut également que votre navigateur réinitialise l’affichage du document.
			break;
		case 206:
			stat_msg = "Partial Content"; //Vous pouvez voir ce code de statut si votre client HTTP
                                          // (également appelé votre navigateur) utilise des « en-têtes de plage ».
                                          // Cela permet à votre navigateur de reprendre les téléchargements en pause,
                                          // ainsi que de diviser un téléchargement en plusieurs flux.
                                          // Un code 206 est envoyé lorsqu’un en-tête de plage fait que
                                          // le serveur n’envoie qu’une partie de la ressource demandée.
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
			stat_msg = "Multiple Choices";  //Parfois, il peut y avoir plusieurs ressources possibles
                                            // avec lesquelles le serveur peut répondre pour satisfaire
                                            // la demande de votre navigateur. Un code de statut 300
                                            // signifie que votre navigateur doit maintenant choisir entre les deux.
                                            // Cela peut se produire lorsque plusieurs extensions de type
                                            // de fichier sont disponibles, ou si le serveur subit une clarification
                                            // du sens des mots.
			break;
		case 301:
			stat_msg = "Moved Permanently"; //Ce code est délivré lorsqu’une page web ou une ressource a été remplacée
                                            // de manière permanente par une autre ressource.
                                            // Il est utilisé pour les redirection d’URL.
			break;
		case 302:
			stat_msg = "Found"; // Ce code est utilisé pour indiquer que
                                // la ressource demandée a été trouvée, mais pas à l’endroit où elle était attendue.
                                // Il est utilisé pour la redirection temporaire d’URL.
			break;
		case 303:
			stat_msg = "See Other"; //Essentiellement, un code 303 indique à votre navigateur
                                    // qu’il a trouvé la ressource demandée par l’intermédiaire de POST,
                                    // PUT ou DELETE. Toutefois, pour le récupérer via GET,
                                    // vous devez faire la requête appropriée pour une URL différente de celle
                                    // que vous avez utilisée précédemment.
			break;
		case 304:
			stat_msg = "Not Modified"; // Ce code indique au navigateur que les ressources stockées
                                        // dans le cache du navigateur n’ont pas changé.
                                        // Il est utilisé pour accélérer la livraison des pages web
                                        // en réutilisant les ressources précédemment téléchargées.
			break;
		case 305:
			stat_msg = "Use Proxy";
			break;
		case 306:
			stat_msg = "Switch Proxy";
			break;
		case 307:
			stat_msg = "Temporary Redirect"; //Ce code de statut a remplacé le 302 « Found »
                                            // comme action appropriée lorsqu’une ressource
                                            // a été temporairement déplacée vers une autre URL.
                                            // Contrairement au code d’état 302, il ne permet pas de modifier la méthode HTTP.
			break;
		case 308:
			stat_msg = "Permanent Redirect"; // Le code de statut 308 est le successeur du code 301 « Moved Permanently ».
                                            // Il ne permet pas de modifier la méthode HTTP et indique
                                            // que la ressource demandée est désormais située de façon permanente
                                            // sur une nouvelle URL.
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
			stat_msg = "Forbidden"; //Ce code est renvoyé lorsqu’un utilisateur tente d’accéder à quelque chose
                                    // qu’il n’a pas la permission de voir.
                                    // Par exemple, essayer d’accéder à un contenu protégé par un mot de passe
                                    // sans se connecter peut produire une erreur 403.
			break;
		case 404:
			stat_msg = "Not Found";
			break;
		case 405:
			stat_msg = "Method Not Allowed"; // Cela est généré lorsque le serveur
                                            // d’hébergement (serveur d’origine) prend en charge
                                            // la méthode reçue, mais pas la ressource cible.
			break;
		case 406:
			stat_msg = "Not Acceptable"; //La ressource demandée est capable de générer uniquement du contenu
                                        // qui n’est pas acceptable selon les en-têtes d’acceptation envoyés dans la requête.
			break;
		case 407:
			stat_msg = "Proxy Authentication Required";
			break;
		case 408:
			stat_msg = "Request Timeout";   // le serveur n’a pas reçu la requête complète envoyée par le navigateur.
                                            // Une des causes possibles pourrait être une congestion
                                            // entraînant la perte de paquets de données entre le navigateur et le serveur.
			break;
		case 409:
			stat_msg = "Conflict";  //Un code d’état 409 signifie que le serveur n’a pas pu traiter la requête de votre navigateur
                                    // parce qu’il y a un conflit avec la ressource concernée.
                                    // Cela se produit parfois en raison de multiples modifications simultanées.
			break;
		case 410:
			stat_msg = "Gone";  //Ce code est similaire au code 404 « Non trouvé »,
                                // sauf que le code 410 indique que l’état est attendu et permanent.
			break;
		case 411:
			stat_msg = "Length Required";   //Cela signifie que la ressource demandée exige que le client spécifie
                                            // une certaine longueur et qu’il ne l’a pas fait.
			break;
		case 412:
			stat_msg = "Precondition Failed";   // Votre navigateur incluait certaines conditions dans ses en-têtes de requête,
                                                // et le serveur ne répondait pas à ces spécifications.
			break;
		case 413:
			stat_msg = "Payload Too Large"; //Votre demande est plus importante que le serveur ne veut ou ne peut la traiter.
			break;
		case 414:
			stat_msg = "URI Too Long";  //C’est généralement le résultat d’une requête GET qui a été encodée
                                        // comme une chaîne de requête trop grande pour que le serveur puisse la traiter.
			break;
		case 415:
			stat_msg = "Unsupported Media Type";    //La requête comprend un type de média que le serveur ou la ressource ne prend pas en charge.
			break;
		case 416:
			stat_msg = "Range Not Satisfiable"; //Votre requête concernait une partie d’une ressource que le serveur ne peut pas retourner.
			break;
		case 417:
			stat_msg = "Expectation Failed";    //Le serveur n’est pas en mesure de répondre aux exigences spécifiées dans le champ d’en-tête attendu de la requête.
			break;
		case 418:
			stat_msg = "I'm a Teapot"; // Poisson d'avril
			break;
		case 421:
			stat_msg = "Misdirected Request";
			break;
		case 422:
			stat_msg = "Unprocessable Entity"; //La requête du client contient des erreurs sémantiques, et le serveur ne peut pas la traiter.
			break;
		case 423:
			stat_msg = "Locked";
			break;
		case 424:
			stat_msg = "Failed Dependency";
			break;
		case 425:
			stat_msg = "Too Early"; //Ce code est envoyé lorsque le serveur ne veut pas traiter une requête parce qu’elle peut être rejouée.
			break;
		case 426:
			stat_msg = "Upgrade Required"; //En raison du contenu du champ de l’en-tête de la demande de mise à niveau, le client doit passer à un protocole différent.
			break;
		case 428:
			stat_msg = "Precondition Required"; //Le serveur exige que des conditions soient spécifiées avant de traiter la requête.
			break;
		case 429:
			stat_msg = "Too Many Requests"; //Cela est généré par le serveur lorsque l’utilisateur a envoyé trop de requêtes dans un laps de temps donné.
			break;
		case 431:
			stat_msg = "Request Header Fields Too Large"; // L’opérateur du serveur a reçu une demande d’interdiction d’accès à la ressource que vous avez demandée
			break;
		case 451:
			stat_msg = "Unavailable For Legal Reasons";
			break;
		case 499:
			stat_msg = "Client Closed Request"; // Cette information est renvoyée par NGINX lorsque le client ferme la requête alors que Nginx est toujours en train de la traiter.
			break;
		case 500:
			stat_msg = "Internal Server Error"; // Un problème est survenu sur le serveur et la ressource demandée n’a pas été fournie.
			break;
		case 501:
			stat_msg = "Not Implemented";
			break;
		case 502:
			stat_msg = "Bad Gateway";   //indique que le serveur, tout en agissant comme une passerelle ou un proxy,
                                        // a reçu une réponse non valide d’un serveur entrant auquel il a accédé en tentant de répondre à la requête.
			break;
		case 503:
			stat_msg = "Service Unavailable"; //La requête ne peut pas être complétée à ce stade.
			break;
		case 504:
			stat_msg = "Gateway Timeout";   // Il s’agit du code renvoyé lorsque deux serveurs sont impliqués
                                            // dans le traitement d’une requête et que le premier serveur attend la réponse du second.
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
        case 509:
            stat_msg = "Bandwidth Limit Exceeded"; //signifie que votre site web utilise plus de bande passante que ce que votre hébergeur autorise.
            break;
		case 510:
			stat_msg = "Not Extended";
			break;
		case 511:
			stat_msg = "Network Authentication Required";   // Ce code de statut est envoyé lorsque le réseau que vous essayez d’utiliser
                                                            // nécessite une forme d’authentification avant d’envoyer votre requête au serveur.
			break;
        case 521:
            stat_msg = "Web Server Is Down"; // Cela signifie que votre navigateur web a pu se connecter avec succès à Cloudflare,
                                                // mais que Cloudflare n’a pas pu se connecter au serveur web d’origine.
            break;
        case 525:
            stat_msg = "SSL Handshake Failed"; // signifie que le serveur et le navigateur n’ont pas pu établir une connexion sécurisée.
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
	{
        if (isspace(str[i]))
            count++;
		count++;
	}
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
        if (!web.getbool_redir().first.empty() && !web.getbool_redir().second.empty())
        {
            std::cout << web.getReq().getUrl() << std::endl;
            web.getRes().setStatus(atoi(web.getbool_redir().first.c_str()));
        }
		setContentLenght();
		if (web.getMax_body_size() > 0 && web.getMax_body_size() < atoi(content_lenght.c_str()))
		{
			content_lenght = itoa(web.getMax_body_size());
			std::cout << "tHINLGE" << std::endl;
		}
		
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
	{
		setStatus(100);
		concat_response(web);
	}
	else
	{
		std::vector<std::pair<std::string, std::string> > post(post_arg(web.getReq().getBody(), nbr));
		post_exe(web, post, conf);
//		MethodGet(web, conf);
	}
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
	if (remove(fullpath.c_str()) == 0)
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

void Response::concat_response(webServ & web)
{
    setStatMsg();
    if (status == 301 || status == 302)
        full_response = version + ' ' + itoa(status) + ' ' + stat_msg + '\n' + "Location : " + web.getbool_redir().second;
    else
	    full_response = version + ' ' + itoa(status) + ' ' + stat_msg + '\n' + "Content-Type: " + content_type + '\n' + "Content-Lenght: " + content_lenght + "\n\n" + body;
	//std::cout << "header response  ========" << std::endl << std::endl <<  version + ' ' + itoa(status) + ' ' + stat_msg + '\n' + "Content-Type: " + content_type + '\n' + "Content-Lenght: " + content_lenght + "\n" << std::endl;
    //std::cout << "full_response ---------------------------" << std::endl << std::endl << full_response << std::endl << std::endl;
    web.del_redir();
}

std::string Response::getResponse() const
{
    return full_response;
}

std::string Response::getContentLenght() const
{
    return content_lenght;
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
	if (getContentType().find("image") != std::string::npos)
		content_lenght = itoa(body.size());
	else
		content_lenght = itoa(how_many(body));
	
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
