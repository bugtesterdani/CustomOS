#include "log/log.h"
#include "parser/elfParser.h"

#include <string>

int main(int argc, char** argv)
{
	if (argc == 1) {
		LOG::Error("Missing arguments!\n1. asm File\n2. elf dir");
		return 1;
	} else if (argc > 3) {
		LOG::Error("To many arguments!\n1. asm File\n2. elf dir");
		return 1;
	}

	const char* srcFile = argv[1];
	const char* srcDir = argv[2];
	//LOG::Debug(srcFile);

	Parser::ElfParser ok("out/");
	ok.setSrc(srcDir);
	if(!ok.parse()) {
		LOG::Error("Failed to parse!");
		return 1;
	}

	for (auto const& [key, val] : ok.getParsedReadElf())
    {
		LOG::Info(std::string("Parsed .efl file: " + key).c_str());
		for (auto const& [key1, val1] : val)
    	{
			LOG::Info(std::string("Func: " + key1 + ", Addr: " + val1).c_str());
		}
	}

	return 0;
}
