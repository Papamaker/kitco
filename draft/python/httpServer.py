#!/usr/bin/python
 
import BaseHTTPServer
import CGIHTTPServer
 
PORT = 8888
server_address = ("", PORT)

server = BaseHTTPServer.HTTPServer
handler = CGIHTTPServer.CGIHTTPRequestHandler
handler.cgi_directories = ["/"]
print "Serveur actif sur le port :", PORT

httpd = server(server_address, handler)
httpd.serve_forever()
