#include "log/log.h"
#include "parser/elfParser.h"
#include "parser/asmParser.h"

#include <string>
#include <csignal>

static Parser::ElfParser* elfParser;
static Parser::ASMParser* asmParser;

void cleanup()
{
	if(elfParser) {
		delete elfParser;
   	}

   	if(asmParser) {
		delete asmParser;
   	}
}

void signalHandler(int signum) {

   cleanup();
   exit(signum);  
}

int main(int argc, char** argv)
{
	if (argc < 4) {
		LOG::Error("Missing arguments!\n1. asm File\n2. elf dir\n3. Out dir");
		return 1;
	} else if (argc > 4) {
		LOG::Error("To many arguments!\n1. asm File\n2. elf dir\n3. Out dir");
		return 1;
	}

	const char* srcFile = argv[1];
	const char* srcDir = argv[2];
	const char* outDir = argv[3];

	signal(SIGINT, signalHandler);
	signal(SIGTERM, signalHandler);

	elfParser = new Parser::ElfParser;
	elfParser->setSrc(srcDir);
	elfParser->setOutDir(outDir);
	if(!elfParser->parse()) {
		LOG::Error("Failed to parse .elf files!");
		return 1;
	}

	asmParser = new Parser::ASMParser(elfParser->getParsedReadElf());
	asmParser->setSrc(srcFile);
	asmParser->setOutDir(outDir);
	if(!asmParser->parse()) {
		LOG::Error("Failed to parse .asm files");
		return 1;
	}

	cleanup();

	return 0;
}
