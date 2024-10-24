#!/bin/bash

gcc -o m_server m_server.c -lfcgi
spawn-fcgi -p 8080 ./m_server
service nginx start
/bin/bash