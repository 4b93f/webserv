# webserv

`webserv` is a lightweight HTTP web server built in C++. The project aims to implement a functional web server from scratch, following the HTTP/1.1 specification. This project is part of the 42 school curriculum and serves as an introduction to network programming, socket management, and handling concurrent client connections in a server environment.

## Table of Contents

- [webserv](#webserv)
	- [Table of Contents](#table-of-contents)
	- [About the Project](#about-the-project)
	- [Features](#features)
	- [HTTP Protocol](#http-protocol)
		- [HTTP Response Codes](#http-response-codes)
	- [Getting Started](#getting-started)
		- [Prerequisites](#prerequisites)
		- [Installation](#installation)

## About the Project

The `webserv` project involves creating a web server that can handle HTTP requests and serve static files. It requires understanding and implementing parts of the HTTP/1.1 protocol, managing multiple connections, parsing HTTP requests, and responding appropriately based on request methods like GET, POST, and DELETE.

## Features

- **HTTP/1.1 compliance**: Supports basic HTTP methods (GET, POST, DELETE).
- **Static file serving**: Serve static files from a specified directory.
- **Directory listing**: Automatically generates a directory listing if an index file is not found.
- **CGI support**: Execute server-side scripts using the Common Gateway Interface (CGI).
- **Configurable server**: Use configuration files to set server parameters like ports, root directories, error pages, etc.
- **Error handling**: Proper error handling for common HTTP errors (404, 500, 403, etc.).
- **Concurrent client handling**: Manage multiple simultaneous client connections using multiplexing (select or poll).
- **Logging**: Basic request and error logging.

## HTTP Protocol

This server implements a subset of the HTTP/1.1 protocol:

- **GET**: Retrieve information from the server (can be a file or generated response).
- **POST**: Send data to the server (often from a form) for processing.
- **DELETE**: Delete a specified resource on the server.

### HTTP Response Codes

`webserv` handles several standard HTTP response codes, such as:

- `200 OK`
- `404 Not Found`
- `403 Forbidden`
- `500 Internal Server Error`
- `405 Method Not Allowed`
- `413 Payload Too Large`

## Getting Started

### Prerequisites

- **g++** or any C++98 compliant compiler.
- **Make** build tool.
- Basic understanding of socket programming and HTTP/1.1.

### Installation

1. **Clone the repository:**

   ```bash
   git clone https://github.com/4b93f/webserv.git
   cd webserv
