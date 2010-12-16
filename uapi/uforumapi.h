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

#ifndef UFORUMAPI_H
#define UFORUMAPI_H

#include <string>
#include "curl.h"
#include <openssl/md5.h>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/property_tree/json_parser.hpp>

#define URUCHIE_API_VERSION "0.1"
#define URUCHIE_API_LINK "http://uruchie.org/api.php"

class UForumApi
{
private:
  std::string json;
  std::string username;
  std::string password;
  std::string md5pass;
  unsigned int userid;
  unsigned int groupid;
  unsigned int threadid;
  bool is_loggin;
  Curl* curl;

  std::string Md5Sum(const std::string& tomd5);
  void Md5PasswordSet(void);
  void JsonEscape(std::string& string);
public:
  UForumApi(const std::string& u, const std::string& p);
  ~UForumApi();
  void setPassword(const std::string& p);
  void setUsername(const std::string& u);
  bool Login(void);
  bool isLogin(void);
  void setThreadId(unsigned int tid);
  bool sendPost(const std::string& message, const std::string& title = "");
  
};


#endif // UFORUMAPI_H