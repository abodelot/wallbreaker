#ifndef INIPARSER_HPP
#define INIPARSER_HPP

/**
 * Simple INI file parser
 * @author Alexandre Bodelot <alexandre.bodelot@gmail.com>
 * @date 3 November 2011
 */

#include <string>
#include <map>
#include <sstream>

/**
 * Read and write INI files
 */
class IniParser
{
public:
	IniParser();

	/**
	 * Load an INI document
	 * @param filename: path to document
	 * @return true if document loaded
	 */
	bool load(const std::string& filename);
	bool load(const char* filename);

	/**
	 * Save INI document to a file
	 * @param filename: path to document
	 * @return true if document saved
	 */
	bool save(const std::string& filename);
	bool save(const char* filename);

	/**
	 * Set internal cursor on a section
	 * The section will be created if it doesn't exist yet.
	 * @param section_name: unique [identifier] in the INI file
	 */
	void seek_section(const std::string& section_name);

	/**
	 * Read a value from the current section
	 * @param key: value identifier
	 * @param default_value: returned value if key not found
	 * @return value stored if key found, otherwise default_value
	 */
	template <class T>
	T get(const std::string& key, const T& default_value) const;
	const std::string& get(const std::string& key, const std::string& default_value = "") const;

	/**
	 * Write a value
	 * @param key: value identifer. If key already exists, value will be overridden
	 * @param value: value to write
	 */
	template <class T>
	void set(const std::string& key, const T& value);

private:
	void clear_string(std::string& line) const;

	typedef std::map<std::string, std::string> Section;
	typedef std::map<std::string, Section> SectionMap;

	SectionMap sections_;
	Section* cursor_;
};


template <class T>
T IniParser::get(const std::string& key, const T& default_value) const
{
	std::istringstream iss(get(key));
	T value;
	if (iss >> value)
		return value;
	return default_value;
}


template <class T>
void IniParser::set(const std::string& key, const T& value)
{
	if (cursor_ != NULL)
	{
		std::ostringstream oss;
		oss << value;
		(*cursor_)[key] = oss.str();
	}
}

#endif // INIPARSER_HPP
