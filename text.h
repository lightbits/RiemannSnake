#ifndef TEXT_H
#define TEXT_H
#include <sstream>
#include <string>

class Text
{
public:
	Text() : stream()
	{

	}

	Text &operator=(const std::string &str_)
	{
		stream.str(str_);
		return *this;
	}

	std::string get_string() const
	{
		return stream.str();
	}

	template <typename T>
	Text &operator<<(const T &rhs)
	{
		stream<<rhs;
		return *this;
	}

	template <typename T>
	Text &operator+(const T &rhs)
	{
		stream<<rhs;
		return *this;
	}
private:
	std::stringstream stream;
};

#endif