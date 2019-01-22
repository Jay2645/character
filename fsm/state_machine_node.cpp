#include "state_machine_node.h"
#include "state_machine.h"

StateMachineNode::StateMachineNode() {
}

void StateMachineNode::_fsm_ready() {
}

void StateMachineNode::_bind_methods() {
}

void StateMachineNode::set_state_machine(Ref<StateMachine> _new_fsm) {
	_state_machine = _new_fsm;
}

Ref<StateMachine> StateMachineNode::get_state_machine() const {
	return _state_machine;
}

bool StateMachineNode::can_switch_state(StringName _new_state) const {
	return _state_machine->has_transition(_current_state, _new_state);
}

void StateMachineNode::switch_state(StringName _new_state) {
	_current_state = _new_state;
}
