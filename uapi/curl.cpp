/*
    Pichi XMPP (Jabber) Bot.
    Copyright (C) 2010  Alexey Kasyanchuk (deg@uruchie.org)

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

*/

#include "curl.h"

Curl::Curl()
{
	curl = curl_easy_init();

	curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errorBuffer);  
	curl_easy_setopt(curl, CURLOPT_HEADER, 0);  
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);  
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, Curl::writer);  
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
	
	postargs = new std::string();
}

Curl::~Curl()
{
	curl_easy_cleanup(curl);
	delete postargs;
}

int Curl::writer(char* data, size_t size, size_t nmemb, std::string* buf)
{
	int rslt = 0;
	buf->append(data, size * nmemb);
	rslt = size * nmemb;
	return rslt;
}

std::string Curl::readurl(const std::string& u)
{
	setUrl(u);
	return read();
}

std::string Curl::read(void )
{
	buffer.clear();
	result = curl_easy_perform(curl);
	if (result == CURLE_OK)
		return buffer;
	else
		return std::string();
}

void Curl::setUrl(const std::string& u)
{
	url = u;
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
}

void Curl::setReferer(const std::string& u)
{
	curl_easy_setopt(curl, CURLOPT_REFERER, u.c_str());
}

void Curl::setAuth(const std::string& user, const std::string& pass)
{
	curl_easy_setopt(curl, CURLOPT_USERPWD, (user + ":" + pass).c_str());
}

void Curl::setPostArgs(const std::string& args)
{
	*postargs = args;
	curl_easy_setopt(curl, CURLOPT_POST, 1);  
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postargs->data());
}

const std::string Curl::urlencode(const std::string& u) const
{
	return curl_easy_escape(curl, u.c_str(), u.length());
}

const std::string Curl::urldecode(const std::string& u) const
{
	int size;
	return curl_easy_unescape(curl, u.c_str(), u.length(), &size);
}

