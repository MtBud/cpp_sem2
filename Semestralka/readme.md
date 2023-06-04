For my semester work, I chose an HTTP server.
Server is able to handle basic HTTP requests. It supports two types of methods: GET and POST and HTTP protocol
version 1.1. It does not support any version lower than that, therefore all requests for lower versions will
be rejected. Server only supports one client at a time and the client will stay connected until terminating
the TCP connection or sending HTTP request with the header "Connection: close".

The method GET can be used for a wole number of things. First and foremost, it can send static files over the
network. It supports all popular file formats, although they have to be specified in the configuration file.
The GET method can also be used for viewing directory contents, executing scripts and remote server shutdown.

The method POST supports only one function, that being sending static files to the server. All popular formats are
supported. A default path for "post" has to be specified in the configuration file. This path will be used for
all client uploads. For uploads outside the default path, the client has to authenticate himself as and administrator.
For the ability to upload, client has to send the required headers, mainly: "Content-Length" and "Content-Type".

Server supports basic authorization using the "Authorization: Basic ..." HTTP header. An admin password has to be
specified in the config file. Authorization is needed for accessing "restricted" paths specified in the config file.

Server supports whole range of responses, not only limited to 200 and 404. Other used response codes are
400 Bad Request, 408 Request Timeout, 500 Internal Server Error and more.

Server supports logging to console and a file specified in config. Server logs miscelaneous messages, and
both recieved and sent message headers and request lines.

Server can be configured from config.json located in /src . More about configuration in CConfig