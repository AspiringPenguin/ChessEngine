#include "core.h"

std::vector<std::string> split(std::string string, std::string delimiter) {
	auto sections = std::vector<std::string>();

	int pos = 0;
	int dl = delimiter.length();
	int l = string.length() - dl + 1;

	std::string substr;

	for (int i = 0; i < l; i++) {
		substr = string.substr(i, dl);
		if (substr == delimiter) {
			sections.push_back(string.substr(pos, i - pos));
			pos = i + 1;
		}
	}

	sections.push_back(string.substr(pos, string.length() - pos));

	return sections;
}