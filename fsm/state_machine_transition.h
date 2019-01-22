/*************************************************************************/
/*  animation_node_state_machine.h                                       */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                      https://godotengine.org                          */
/*************************************************************************/
/* Copyright (c) 2007-2019 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2019 Godot Engine contributors (cf. AUTHORS.md)    */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#ifndef STATE_MACHINE_TRANSITION_H
#define STATE_MACHINE_TRANSITION_H

#include "core/resource.h"

class StateMachineTransition : public Resource {
	GDCLASS(StateMachineTransition, Resource);

public:
	enum SwitchMode {
		SWITCH_MODE_IMMEDIATE,
		SWITCH_MODE_SYNC,
		SWITCH_MODE_AT_END,
	};

private:
	SwitchMode switch_mode;
	bool auto_advance;
	StringName advance_condition;
	StringName advance_condition_name;
	float xfade;
	bool disabled;
	int priority;

	StringName from;
	StringName to;

protected:
	static void _bind_methods();


public:
	void set_from_string(const StringName &_new_from);
	StringName get_from_string() const;
	void set_to_string(const StringName &_new_to);
	StringName get_to_string() const;

	void set_switch_mode(SwitchMode p_mode);
	SwitchMode get_switch_mode() const;

	void set_auto_advance(bool p_enable);
	bool has_auto_advance() const;

	void set_advance_condition(const StringName &p_condition);
	StringName get_advance_condition() const;

	StringName get_advance_condition_name() const;

	void set_xfade_time(float p_xfade);
	float get_xfade_time() const;

	void set_disabled(bool p_disabled);
	bool is_disabled() const;

	void set_priority(int p_priority);
	int get_priority() const;

	StateMachineTransition();
};

VARIANT_ENUM_CAST(StateMachineTransition::SwitchMode)

#endif // ANIMATION_NODE_STATE_MACHINE_H
