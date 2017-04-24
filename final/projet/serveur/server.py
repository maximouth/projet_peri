#!/usr/bin/python3
 
import http.server

#import BaseHTTPServer
#import CGIHTTPServer
#import cgitb; cgitb.enable()
 
PORT = 8300
server_address = ("", PORT)

server = http.server.HTTPServer
handler = http.server.CGIHTTPRequestHandler

#server = BaseHTTPServer.HTTPServer

#handler = CGIHTTPServer.CGIHTTPRequestHandler
handler.cgi_directories = ['/', '/cgi-bin']
print("Serveur actif sur le port :", PORT)

httpd = server(server_address, handler)
httpd.serve_forever()

