#include "register_types.h"

#include "character/character.h"
#include "character/character_movement.h"

#include "damage/damage.h"
#include "damage/damageable.h"

#include "fsm/state.h"
#include "fsm/state_machine.h"
#include "fsm/state_machine_transition.h"
#include "fsm/state_machine_node.h"

void register_character_types() {

	ClassDB::register_class<Character>();
	ClassDB::register_class<CharacterMovement>();

	ClassDB::register_class<Damage>();
	ClassDB::register_class<Damageable>();

	ClassDB::register_class<FSMState>();
	ClassDB::register_class<StateMachine>();
	ClassDB::register_class<StateMachineTransition>();
	ClassDB::register_class<StateMachineNode>();
}

void unregister_character_types() {
}
