#ifndef COMMAND_H
#define COMMAND_H

#include "scene/main/node.h"

class CommandNode : public Node {
	GDCLASS(CommandNode, Node);

public:
	String command_name;
};

#endif
