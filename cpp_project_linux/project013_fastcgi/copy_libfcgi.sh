#!/bin/bash

# fcgi和fcgi++都需要建立软连接, 正常情况下只要在/usr/lib目录下建立链接就可以了，如果不行的话在/usr/lib64目录下也建立链接
ln -s -f /usr/local/lib/libfcgi.so.0  /usr/lib/libfcgi.so.0
ln -s -f /usr/local/lib/libfcgi.so.0  /usr/lib64/libfcgi.so.0
ln -s -f /usr/local/lib/libfcgi++.so.0  /usr/lib/libfcgi++.so.0
ln -s -f /usr/local/lib/libfcgi++.so.0  /usr/lib64/libfcgi++.so.0