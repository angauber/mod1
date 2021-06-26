#ifndef _MOD1_PARSER_HPP_
#define _MOD1_PARSER_HPP_

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

#include <Magnum/Magnum.h>
#include <Magnum/Math/Vector3.h>

using namespace Magnum;

class Mod1Parser
{
	public:
		bool	parseFile(const std::string &path);

	private:
		virtual void				addPoint(Vector3 point) = 0;
		virtual size_t				getPointCount() const = 0;
		bool						addPoints(const std::string &line);
		std::vector<std::string> 	getValues(const std::string &str);
};

#endif
