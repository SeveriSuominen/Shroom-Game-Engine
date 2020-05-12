#pragma once
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <vector>

class ShroomString
{
public:
	 static void Split(const std::string& str, std::vector<std::string>& cont, char delim = ' ')
	 {
		 std::stringstream ss(str);
		 std::string token;
		 while (std::getline(ss, token, delim)) 
		 {
			 cont.push_back(token);
		 }
	 }
};
