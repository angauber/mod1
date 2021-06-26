#include "mod1Parser.hpp"

bool	Mod1Parser::parseFile(const std::string &path)
{
	std::ifstream infile(path);
	std::string line;

	if (infile.good() == false || infile.is_open() == false) {
		Error{} << "Could not read file:" << path.c_str();
		return false;
	}

	while (std::getline(infile, line)) {
		this->addPoints(line);
	}

	return this->getPointCount() > 0;
}

bool	Mod1Parser::addPoints(const std::string &line)
{
	std::istringstream iss(line);
	std::string token;
	std::vector<std::string> values;
	size_t start;
	size_t end;

	while(iss >> token) {
		start = token.find('(');
		end = token.find(')');

		if (start < end) {
			values = this->getValues(token.substr(start + 1, end - start - 1));

			if (values.size() != 3) {
				Error{} << "Could not parse points from `" << token.c_str() << "`";
				return false;
			}

			this->addPoint(Vector3 {
				std::stof(values[0]),
				std::stof(values[1]),
				std::stof(values[2]),
			});
		} else {
			Error{} << "Could not parse points from `" << token.c_str() << "`";
			return false;
		}
	}

	return true;
}

std::vector<std::string>	Mod1Parser::getValues(const std::string &str)
{
    std::vector<std::string> values;
    std::stringstream stream(str);
    std::string buffer;

    while (std::getline(stream, buffer, ',')) {
        values.push_back(buffer);
    }

    return values;
}
