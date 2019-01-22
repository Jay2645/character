
#ifndef FSM_STATE_H
#define FSM_STATE_H

#include "core/resource.h"

class FSMState : public Resource {
	GDCLASS(FSMState, Resource);

public:
	StringName name;
	Ref<Resource> node;
	Vector2 position;

public:
	FSMState();

};

#endif
