#include "register_types.h"

#include "state_machine_node.h"
#include "character.h"
#include "character_movement.h"

void register_character_types() {

	ClassDB::register_class<StateMachineNode>();
	ClassDB::register_class<Character>();
	ClassDB::register_class<CharacterMovement>();
}

void unregister_character_types() {

}

