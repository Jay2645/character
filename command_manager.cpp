#include "command_manager.h"
#include "core/os/input.h"

CommandManager::CommandManager() {
	connect("ready", this, "_ready");
}

void CommandManager::_bind_methods() {
	ClassDB::bind_method(D_METHOD("_ready"), &CommandManager::_ready);

	ClassDB::bind_method(D_METHOD("set_command_paths", "command_paths"), &CommandManager::set_command_paths);
	ClassDB::bind_method(D_METHOD("get_command_paths"), &CommandManager::get_command_paths);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "command_paths", PROPERTY_HINT_NONE, "Commands"), "set_command_paths", "get_command_paths");
}

void CommandManager::_ready() {
	commands.clear();
	for (int i = 0; i < command_paths.size(); i++) {
		CommandNode* _found_node = find_command_node(command_paths[i]);
		if (_found_node != NULL) {
			commands.push_back(_found_node);
		}
	}
	update_configuration_warning();
	print_line("Current command length: " + itos(commands.size()));
}

CommandNode* CommandManager::find_command_node(NodePath command_path) {
	if (has_node(command_path)) {
		return cast_to<CommandNode>(get_node(command_path));
	} else {
		return NULL;
	}
}

String CommandManager::get_configuration_warning() const {
	if (commands.size() == 0) {
		return String("You need to define some commands for the command manager to do anything!");
	}
	return String();
}

void CommandManager::set_command_paths(const Array &p_command_array) {
	command_paths.resize(p_command_array.size());
	commands.clear();
	for (int i = 0; i < command_paths.size(); i++) {
		command_paths.write[i] = p_command_array[i];
		CommandNode *_found_node = find_command_node(command_paths[i]);
		if (_found_node != NULL) {
			commands.push_back(_found_node);
		}
	}
	update_configuration_warning();
}

Array CommandManager::get_command_paths() const {
	Array out;
	for (int i = 0; i < command_paths.size(); i++) {
		out.push_back(command_paths[i]);
	}
	return out;
}
