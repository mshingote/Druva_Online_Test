#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<vector>
#include <cctype>
#include <string>
#include <algorithm>
#include<map>

//constants
#define NO_OF_FIELD_SEPARATORS 2
#define FIELD_SEPARATOR_SYMBOL ','
#define INPUT_FILE "input.txt"
#define FILE_READ_MODE "r"
#define DELIMETER ","

class OutdatedAPIs
{
private:
	class Api_Details
	{
	public:
		std::string App_Name;
		std::string Api_Name;
		std::string Api_Version;
	};
	static std::map<std::string, std::string> ApiName_LatestVersion;
	static std::vector<Api_Details> Api_Details_Collection;
	std::vector<std::string> split(const std::string& str)
	{
		const std::string& delim = DELIMETER;
		std::vector<std::string> tokens;
		size_t prev = 0, pos = 0;
		do
		{
			pos = str.find(delim, prev);
			if (pos == std::string::npos) pos = str.length();
			std::string token = str.substr(prev, pos - prev);
			if (!token.empty()) tokens.push_back(token);
			prev = pos + delim.length();
		} while (pos < str.length() && prev < str.length());
		return tokens;
	}
	inline std::string trim(const std::string &s)
	{
		auto wsfront = std::find_if_not(s.begin(), s.end(), [](int c) {return std::isspace(c); });
		auto wsback = std::find_if_not(s.rbegin(), s.rend(), [](int c) {return std::isspace(c); }).base();
		return (wsback <= wsfront ? std::string() : std::string(wsfront, wsback));
	}
public:
	void fill(std::string &line)
	{
		Api_Details details;
		std::vector<std::string> tokens = split(line);
		details.App_Name = trim(tokens[0]);
		details.Api_Name = trim(tokens[1]);
		details.Api_Version = trim(tokens[2]);
		ApiName_LatestVersion[details.Api_Name] = std::max(ApiName_LatestVersion[details.Api_Name], details.Api_Version);
		Api_Details_Collection.emplace_back(details);
	}
	std::vector<std::string> Get_Older_App_Collection()
	{
		std::vector<std::string> older_apps;
		std::map<std::string, bool> found;
		for (Api_Details& app : Api_Details_Collection)
		{
			if (!found[app.App_Name] && app.Api_Version != ApiName_LatestVersion[app.Api_Name])
			{
				older_apps.emplace_back(app.App_Name);
				found[app.App_Name] = true;
			}
		}
		found.clear();
		return older_apps;
	}
	~OutdatedAPIs()
	{
		ApiName_LatestVersion.clear();
		Api_Details_Collection.clear();
	}
};
std::map<std::string, std::string> OutdatedAPIs::ApiName_LatestVersion;
std::vector<OutdatedAPIs::Api_Details> OutdatedAPIs::Api_Details_Collection;

int main()
{
	freopen(INPUT_FILE, FILE_READ_MODE, stdin);
	std::string line;
	OutdatedAPIs Api_Info;

	while (std::getline(std::cin, line))
	{
		if (NO_OF_FIELD_SEPARATORS == std::count(line.begin(), line.end(), FIELD_SEPARATOR_SYMBOL))
		{
			Api_Info.fill(line);
		}
	}
	std::vector<std::string> Older_Apps = Api_Info.Get_Older_App_Collection();
	for (std::string& app_name : Older_Apps)
	{
		std::cout << app_name << std::endl;
	}

	return 0;
}