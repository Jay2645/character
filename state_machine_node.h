#ifndef STATE_MACHINE_NODE_H
#define STATE_MACHINE_NODE_H

#include "scene/main/node.h"


class StateMachine;
class StateMachineNode : public Node {
	GDCLASS(StateMachineNode, Node);

public:
	StateMachineNode();

protected:
	Ref<StateMachine> _state_machine;
	StringName _current_state;

protected:
	void _fsm_ready();
	static void _bind_methods();

public:
	void set_state_machine(Ref<StateMachine> _new_fsm);
	Ref<StateMachine> get_state_machine() const;

	bool can_switch_state(StringName _new_state) const;
	void switch_state(StringName _new_state);
};
#endif
