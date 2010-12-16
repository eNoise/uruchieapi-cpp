/*
    Uruchie Forum API
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

#include "uforumapi.h"

UForumApi::UForumApi(const std::string& u, const std::string& p) : username(u), password(p)
{
	is_loggin = false;
	curl = new Curl();
	curl->setUrl(URUCHIE_API_LINK);
	Md5PasswordSet();
}

UForumApi::~UForumApi()
{
	delete curl;
	curl = 0;
}

std::string UForumApi::Md5Sum(const std::string& tomd5)
{
	unsigned char tmp_hash[MD5_DIGEST_LENGTH];
	MD5((const unsigned char*)tomd5.c_str(), tomd5.size(), tmp_hash);
	char buffer[10];
	std::string ret;
	for(int i=0; i <MD5_DIGEST_LENGTH; i++)
	{
		sprintf(buffer, "%02x", tmp_hash[i]);
		ret.append(buffer);
	}
	return ret;
}


void UForumApi::Md5PasswordSet(void)
{
	md5pass = Md5Sum(password);
}

void UForumApi::setPassword(const std::string& p)
{
	password = p;
	Md5PasswordSet();
}

void UForumApi::setUsername(const std::string& u)
{
	username = u;
}

void UForumApi::JsonEscape(std::string& string)
{
	boost::replace_all(string, "\\/", "/");
}

bool UForumApi::Login(void )
{
	curl->setPostArgs(
		std::string("module=forum") +
		"&action=login" +
		"&username=" + curl->urlencode(username) +
		"&password=" + curl->urlencode(md5pass)
	);
	json = curl->read();
	JsonEscape(json);
	if(json == "" || json.substr(0,1) != "{")
		return false; // not json
	
	boost::property_tree::ptree ptree, user_data;
	std::stringstream stream(json);
	boost::property_tree::json_parser::read_json(stream, ptree);
	if(ptree.get("authenticated", "false") == "false")
	{
		return false;
	}
	else if(ptree.get("authenticated", "") == "true")
	{
		user_data = ptree.get_child("user");
		userid = boost::lexical_cast<int>(user_data.get("userid", "0"));
		groupid = boost::lexical_cast<int>(user_data.get("usergroupid", "0"));
		is_loggin = true;
	}
	else
	{
		return false;
	}
	
	return true;
}

bool UForumApi::isLogin(void )
{
	return is_loggin;
}


void UForumApi::setThreadId(unsigned int tid)
{
	threadid = 0;
}

bool UForumApi::sendPost(const std::string& message, const std::string& title)
{
	if(!is_loggin)
		return false;
	
	if(threadid == 0)
		return false;
	
	curl->setPostArgs(
		std::string("module=forum") +
		"&action=reply" +
		"&username=" + curl->urlencode(username) +
		"&password=" + curl->urlencode(md5pass) +
		"&threadid=" + boost::lexical_cast<std::string>(threadid) +
		((title != "") ? "&title=" + curl->urlencode(title) : "") +
		"&content=" + curl->urlencode(message)
	);
	
	boost::property_tree::ptree ptree;
	std::stringstream stream(json);
	boost::property_tree::json_parser::read_json(stream, ptree);
	
	if(ptree.get("authenticated", "false") == "true" && ptree.get("sended", "false") == "true")
		return true;
	
	return false;
}


