#!/usr/bin/env python
import BaseHTTPServer
import CGIHTTPServer
import cgitb; cgitb.enable()

server = BaseHTTPServer.HTTPServer

handler = CGIHTTPServer.CGIHTTPRequestHandler
handler.cgi_directories = ["/cgi-bin"]

server_address = ("", 8000)

httpd = server(server_address, handler)
httpd.serve_forever()
