#!/bin/bash


chmod 755 ./configure
./configure --prefix=/usr/local/nginx \
	--without-http_memcached_module \
	--without-http_empty_gif_module \
	--without-http_auth_basic_module \
	--without-http_map_module \
	--without-http_browser_module \
	--without-mail_pop3_module \
	--without-mail_imap_module \
	--without-mail_smtp_module \
	--add-module=mymodule/ngx_http_handle_module \
	--with-ipv6 \
	--with-cc-opt='-D_FILE_OFFSET_BITS=64'
	#--with-debug --with-cc-opt='-g -pg -O0 -D_FILE_OFFSET_BITS=64'

make -j8
