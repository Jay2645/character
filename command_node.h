#ifndef COMMAND_NODE_H
#define COMMAND_NODE_H

#include "scene/main/node.h"

class CommandManager;

class CommandNode : public Node {
	GDCLASS(CommandNode, Node);
	friend CommandManager;

protected:
	CommandManager *_manager;
	const StringName EXECUTE_COMMAND_NAME = StringName("_execute_command");

public:
	NodePath command_manager_path;

public:
	CommandNode();
	~CommandNode();

protected:
	virtual void _ready();
	static void _bind_methods();
	virtual void _execute_command(Object *commanded_object, float delta_time);

public:
	virtual String get_configuration_warning() const override;

	void add_command();
	void remove_command();

	void set_command_manager_path(const NodePath& new_path);
	NodePath get_command_manager_path() const;
};
#endif
