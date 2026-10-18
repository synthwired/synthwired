#include "strings.h"

#include <cctype>

void to_uppercase(char *text)
{
	for (char *character = text; *character != '\0'; character++) {
		*character = static_cast<char>(std::toupper(static_cast<unsigned char>(*character)));
	}
}
