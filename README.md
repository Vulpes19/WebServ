# WebServ - A C++ HTTP Web Server
## Description
WebServ is a C++ HTTP web server designed as part of the 42 common core. It porvides support for handling HTTP requests using GET, POST and DELETE methods, and it includes a CGI (Common Gateway Interface) implementation for Python and PHP. Additionally, our webServ can handle cookies for session management.
## Features
- Supports HTTP GET, POST and DELETE methods.
- Supports directory listing.
- Supports server names.
- CGI support for Python and PHP scripts.
- Cookies handling for session management.
## Table of Contents
1. [Installation](#Installation)
2. [Usage](#Usage)
3. [Configuration](#Configuration)
4. [Acknowledgments](#Acknowledgments)
## Installation
To get started, follow these steps:
1. Clone the repository:  
  `git clone https://github.com/Vulpes19/webServ.git`
2. Build the web server:  
  `make`  
notice: This web server doesn't support Windows. It's advised to use only mac OS if you wanna test it as Linux systems might cause some problems)  
3. Start the server:  
   `./webServ config_file.conf`  
## Usage
### Basic Usage
To use webServ, open a web browser and navigate to `http://localhost:8080`. You can use Postman or `curl` to make HTTP requests programmatically.
### Configuration
You can configure webServ by creating or using a config from the examples, notice that the file should end with `.conf`. Modify settings like the server port, server name, document root, or CGI script directory to suit your needs.
## Acknowledgments
We would like to acknowledge the contributions of our team members to this project:  
- **[Ayman Baioumy](https//github.com/Vulpes19)**:  
  - Implemented core HTTP server functionality.
  - Worked on CGI integration for Python and PHP.
  - Handled cookie support.
- **[Mohamed Amine Baioumy](https://github.com/0xDopamine)**:  
  - Worked on the configuration file and request parsing.
  - Assisted with project setup and testing.
