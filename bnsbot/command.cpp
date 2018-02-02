#include "command.h"
#include <stdio.h>

bot::Command::Command() {
}

bot::Command::~Command() {
	printf("Freed a command :)\n");
}

/*bot::Command::~Command() {
	printf("Freed a command :)\n");
}*/

bool bot::Command::Execute() {
	return false;
}
