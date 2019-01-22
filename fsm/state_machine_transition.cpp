/*************************************************************************/
/*  animation_node_state_machine.cpp                                     */
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

#include "state_machine_transition.h"

void StateMachineTransition::set_from_string(const StringName &_new_from) {
	from = _new_from;
}


StringName StateMachineTransition::get_from_string() const {
	return from;
}


void StateMachineTransition::set_to_string(const StringName &_new_to) {
	to = _new_to;
}


StringName StateMachineTransition::get_to_string() const {
	return to;
}

void StateMachineTransition::set_switch_mode(SwitchMode p_mode) {

	switch_mode = p_mode;
}

StateMachineTransition::SwitchMode StateMachineTransition::get_switch_mode() const {

	return switch_mode;
}

void StateMachineTransition::set_auto_advance(bool p_enable) {
	auto_advance = p_enable;
}

bool StateMachineTransition::has_auto_advance() const {
	return auto_advance;
}

void StateMachineTransition::set_advance_condition(const StringName &p_condition) {
	String cs = p_condition;
	ERR_FAIL_COND(cs.find("/") != -1 || cs.find(":") != -1);
	advance_condition = p_condition;
	if (cs != String()) {
		advance_condition_name = "conditions/" + cs;
	} else {
		advance_condition_name = StringName();
	}
	emit_signal("advance_condition_changed");
}

StringName StateMachineTransition::get_advance_condition() const {
	return advance_condition;
}

StringName StateMachineTransition::get_advance_condition_name() const {
	return advance_condition_name;
}

void StateMachineTransition::set_xfade_time(float p_xfade) {

	ERR_FAIL_COND(p_xfade < 0);
	xfade = p_xfade;
	emit_changed();
}

float StateMachineTransition::get_xfade_time() const {
	return xfade;
}

void StateMachineTransition::set_disabled(bool p_disabled) {
	disabled = p_disabled;
	emit_changed();
}

bool StateMachineTransition::is_disabled() const {
	return disabled;
}

void StateMachineTransition::set_priority(int p_priority) {
	priority = p_priority;
	emit_changed();
}

int StateMachineTransition::get_priority() const {
	return priority;
}

void StateMachineTransition::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_switch_mode", "mode"), &StateMachineTransition::set_switch_mode);
	ClassDB::bind_method(D_METHOD("get_switch_mode"), &StateMachineTransition::get_switch_mode);

	ClassDB::bind_method(D_METHOD("set_auto_advance", "auto_advance"), &StateMachineTransition::set_auto_advance);
	ClassDB::bind_method(D_METHOD("has_auto_advance"), &StateMachineTransition::has_auto_advance);

	ClassDB::bind_method(D_METHOD("set_advance_condition", "name"), &StateMachineTransition::set_advance_condition);
	ClassDB::bind_method(D_METHOD("get_advance_condition"), &StateMachineTransition::get_advance_condition);

	ClassDB::bind_method(D_METHOD("set_to_string", "name"), &StateMachineTransition::set_to_string);
	ClassDB::bind_method(D_METHOD("get_to_string"), &StateMachineTransition::get_to_string);

	ClassDB::bind_method(D_METHOD("set_from_string", "name"), &StateMachineTransition::set_from_string);
	ClassDB::bind_method(D_METHOD("get_from_string"), &StateMachineTransition::get_from_string);

	ClassDB::bind_method(D_METHOD("set_xfade_time", "secs"), &StateMachineTransition::set_xfade_time);
	ClassDB::bind_method(D_METHOD("get_xfade_time"), &StateMachineTransition::get_xfade_time);

	ClassDB::bind_method(D_METHOD("set_disabled", "disabled"), &StateMachineTransition::set_disabled);
	ClassDB::bind_method(D_METHOD("is_disabled"), &StateMachineTransition::is_disabled);

	ClassDB::bind_method(D_METHOD("set_priority", "priority"), &StateMachineTransition::set_priority);
	ClassDB::bind_method(D_METHOD("get_priority"), &StateMachineTransition::get_priority);

	ADD_PROPERTY(PropertyInfo(Variant::INT, "switch_mode", PROPERTY_HINT_ENUM, "Immediate,Sync,AtEnd"), "set_switch_mode", "get_switch_mode");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "auto_advance"), "set_auto_advance", "has_auto_advance");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "advance_condition"), "set_advance_condition", "get_advance_condition");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "from"), "set_from_string", "get_from_string");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "to"), "set_to_string", "get_to_string");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "xfade_time", PROPERTY_HINT_RANGE, "0,240,0.01"), "set_xfade_time", "get_xfade_time");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "priority", PROPERTY_HINT_RANGE, "0,32,1"), "set_priority", "get_priority");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "disabled"), "set_disabled", "is_disabled");

	BIND_ENUM_CONSTANT(SWITCH_MODE_IMMEDIATE);
	BIND_ENUM_CONSTANT(SWITCH_MODE_SYNC);
	BIND_ENUM_CONSTANT(SWITCH_MODE_AT_END);

	ADD_SIGNAL(MethodInfo("advance_condition_changed"));
}

StateMachineTransition::StateMachineTransition() {

	switch_mode = SWITCH_MODE_IMMEDIATE;
	auto_advance = false;
	xfade = 0;
	disabled = false;
	priority = 1;
	to = "";
	from = "";
}
