#include "core.h"

#include <fstream>
#include <iostream>

std::string loadFile(std::string fileName) {
	std::ifstream reader(fileName);

	if (!reader) {
		std::cerr << "Error opening file " << fileName << std::endl;
		return "";
	}
	else {
		std::string line; //For reading the file

		std::string contents = ""; //To return

		bool skip = true;

		while (!reader.eof()) {
			getline(reader, line);
			if (!skip) {
				contents += "\n";
			}
			else {
				skip = false;
			}
			contents += line;
		}
		reader.close();
		return contents;
	}
}

void writeToFile(std::string fileName, std::string contents) {
	std::ofstream writer(fileName);
	if (!writer) {
		std::cerr << "Error opening file " << fileName << std::endl;
		return;
	}
	writer << contents;
	writer.close();
}



std::vector<std::string> split(std::string string, std::string delimiter) {
	auto sections = std::vector<std::string>();

	size_t pos = 0;
	size_t dl = delimiter.length();
	size_t l = string.length() - dl + 1;

	std::string substr;

	for (size_t i = 0; i < l; i++) {
		substr = string.substr(i, dl);
		if (substr == delimiter) {
			sections.push_back(string.substr(pos, i - pos));
			pos = i + 1;
		}
	}

	sections.push_back(string.substr(pos, string.length() - pos));

	return sections;
}

bool sameColor(square sq1, square sq2) {
	return (((sq1 >> 3) + (sq1 & 0b111)) % 2) == (((sq2 >> 3) + (sq2 & 0b111)) % 2);
}
bool sameColor(unsigned char sq1, unsigned char sq2) {
	return (((sq1 >> 3) + (sq1 & 0b111)) % 2) == (((sq2 >> 3) + (sq2 & 0b111)) % 2);
}

square stringToSquare(std::string sq) {
	return square((sq[0] - 97) + (sq[1] - 49) * 8);
}