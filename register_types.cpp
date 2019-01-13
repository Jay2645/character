#include "register_types.h"

#include "character.h"
#include "command_manager.h"
#include "command_node.h"

void register_character_types() {

	ClassDB::register_class<Character>();
	ClassDB::register_class<CommandManager>();
	ClassDB::register_class<CommandNode>();

}

void unregister_character_types() {

}

