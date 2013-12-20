#include <iostream>
#include <fstream>

#include "IniParser.hpp"

#define TOKEN_COMMENT       ';'
#define TOKEN_START_SECTION '['
#define TOKEN_END_SECTION   ']'
#define TOKEN_SEPARATOR     '='


static void trim_string(std::string& str)
{
	const char* WHITESPACES = " \t\n\r\0xb";
	size_t first = str.find_first_not_of(WHITESPACES);
	if (first != std::string::npos)
	{
		size_t last = str.find_last_not_of(WHITESPACES);
		str = str.substr(first, last - first + 1);
	}
}


IniParser::IniParser():
	cursor_(NULL)
{
}


bool IniParser::load(const std::string& filename)
{
	return load(filename.c_str());
}


bool IniParser::load(const char* filename)
{
	std::ifstream file(filename);
	if (file)
	{
		std::string line;
		Section* section = NULL;
		while (std::getline(file, line))
		{
			// Ignore comments and empty lines
			trim_string(line);
			if (line.size() > 0 && line[0] != TOKEN_COMMENT)
			{
				// Look for sections
				if (line[0] == TOKEN_START_SECTION)
				{
					// Extract section name
					size_t index = line.find(TOKEN_END_SECTION);
					line = line.substr(1, index - 1);
					section = &sections_[line];
				}
				else if (section != NULL)
				{
					size_t index = line.find(TOKEN_SEPARATOR);
					if (index != std::string::npos)
					{
						// Store key:value in the current section
						std::string key = line.substr(0, index);
						trim_string(key);
						std::string value = line.substr(index + 1);
						trim_string(value);
						(*section)[key] = value;
					}
					else
					{
						std::cerr << "[IniParser] line '" << line << "' ignored: missing '" << TOKEN_SEPARATOR << "' token" << std::endl;
					}
				}
				else
				{
					std::cerr << "[IniParser] line '" << line << "' ignored: outside of a section" << std::endl;
				}
			}
		}
		file.close();
		return true;
	}
	std::cerr << "[IniParser] cannot open file: " << filename << std::endl;
	return false;
}


bool IniParser::save(const std::string& filename) const
{
	return save(filename.c_str());
}


bool IniParser::save(const char* filename) const
{
	std::ofstream file(filename);
	if (file)
	{
		// For each section
		for (SectionMap::const_iterator it = sections_.begin(); it != sections_.end(); ++it)
		{
			file << TOKEN_START_SECTION << it->first << TOKEN_END_SECTION << std::endl;
			const Section& section = it->second;
			// For each value
			for (Section::const_iterator it2 = section.begin(); it2 != section.end(); ++it2)
			{
				file << it2->first << " " << TOKEN_SEPARATOR << " " << it2->second << std::endl;
			}
			file << std::endl;
		}
		file.close();
		return true;
	}
	std::cerr << "[IniParser] cannot write file: " << filename << std::endl;
	return false;
}


void IniParser::seek_section(const std::string& section_name)
{
	cursor_ = &sections_[section_name];
}


const std::string& IniParser::get(const std::string& key, const std::string& default_value) const
{
	if (cursor_ != NULL)
	{
		Section::const_iterator it = cursor_->find(key);
		return it != cursor_->end() ? it->second : default_value;
	}
	return default_value;
}
