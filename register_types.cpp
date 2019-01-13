#include "register_types.h"

#include "character.h"
#include "command_manager.h"
#include "command_node.h"
#include "input_command_node.h"

void register_character_types() {

	ClassDB::register_class<Character>();
	ClassDB::register_class<CommandManager>();
	ClassDB::register_class<CommandNode>();
	ClassDB::register_class<InputCommandNode>();

}

void unregister_character_types() {

}

