#ifndef COMMAND_MANAGER_H
#define COMMAND_MANAGER_H

#include "scene/main/node.h"
#include "command_node.h"
#include "core/vector.h"

class CommandManager : public Node {
	GDCLASS(CommandManager, Node);

protected:
	Vector<CommandNode*> commands;

public:
	Vector<NodePath> command_paths;

public:
	CommandManager();

protected:
	static void _bind_methods();
	void _ready();

	CommandNode* find_command_node(NodePath command_path);

public:
	virtual String get_configuration_warning() const override;

	void set_command_paths(const Array &p_command_array);
	Array get_command_paths() const;
};
#endif
