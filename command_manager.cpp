#include "command_manager.h"
#include "core/os/input.h"

CommandManager::CommandManager() {
	set_physics_process(true);
}

void CommandManager::_notification(int p_notification) {
	if (p_notification == NOTIFICATION_PHYSICS_PROCESS) {
		_physics_process(get_physics_process_delta_time());
	}
	Node::_notification(p_notification);
}

void CommandManager::_bind_methods() {
	ADD_SIGNAL(MethodInfo("execute_command"));
}

void CommandManager::_physics_process(float delta_time) {
	_execute_all_commands(delta_time);
}

void CommandManager::_execute_all_commands(float delta_time) {
	if (_command_reciever == NULL) {
		print_error("Command reciever is null!");
		return;
	}

	emit_signal(EXECUTE_COMMAND_NAME, _command_reciever, delta_time);
}

void CommandManager::set_reciever(Object *reciever) {
	_command_reciever = reciever;
}

void CommandManager::add_listener(CommandNode *listener, StringName execute_command) {
	if (!is_connected(EXECUTE_COMMAND_NAME, listener, execute_command)) {
		print_line("Connecting to manager.");
		connect(EXECUTE_COMMAND_NAME, listener, execute_command);
	}
}

void CommandManager::remove_listener(CommandNode *listener, StringName execute_command) {
	if (is_connected(EXECUTE_COMMAND_NAME, listener, execute_command)) {
		print_line("Disconnecting from manager.");
		disconnect(EXECUTE_COMMAND_NAME, listener, execute_command);
	}
}
