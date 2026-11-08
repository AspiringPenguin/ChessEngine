#include "fileHandling.h"

namespace files{
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
}