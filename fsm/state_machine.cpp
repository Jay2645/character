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

#include "state_machine.h"

void StateMachine::add_node(const StringName &p_name, Ref<Resource> p_node, const Vector2 &p_position) {

	ERR_FAIL_COND(states.has(p_name));
	ERR_FAIL_COND(p_node.is_null());
	ERR_FAIL_COND(String(p_name).find("/") != -1);

	Ref<FSMState> state(memnew(FSMState));
	state->node = p_node;
	state->position = p_position;

	states[p_name] = state;

	emit_changed();
	emit_signal("tree_changed");

	p_node->connect("tree_changed", this, "_tree_changed", varray(), CONNECT_REFERENCE_COUNTED);
}

Ref<Resource> StateMachine::get_node(const StringName &p_name) const {

	ERR_FAIL_COND_V(!states.has(p_name), Ref<Resource>());

	return states[p_name]->node;
}

StringName StateMachine::get_node_name(const Ref<Resource> &p_node) const {
	for (Map<StringName, Ref<FSMState>>::Element *E = states.front(); E; E = E->next()) {
		if (E->get()->node == p_node) {
			return E->key();
		}
	}

	ERR_FAIL_V(StringName());
}

void StateMachine::get_child_nodes(List<Ref<FSMState>> *r_child_nodes) {
	Vector<StringName> nodes;

	for (Map<StringName, Ref<FSMState>>::Element *E = states.front(); E; E = E->next()) {
		nodes.push_back(E->key());
	}

	nodes.sort_custom<StringName::AlphCompare>();

	for (int i = 0; i < nodes.size(); i++) {
		r_child_nodes->push_back(states[nodes[i]]);
	}
}

bool StateMachine::has_node(const StringName &p_name) const {
	return states.has(p_name);
}
void StateMachine::remove_node(const StringName &p_name) {

	ERR_FAIL_COND(!states.has(p_name));

	{
		Ref<Resource> node = states[p_name]->node;
		node->disconnect("tree_changed", this, "_tree_changed");
	}

	states.erase(p_name);
	//path.erase(p_name);

	for (int i = 0; i < transitions.size(); i++) {
		if (transitions[i].is_null()) {
			continue;
		}
		if (transitions[i]->get_from_string() == p_name || transitions[i]->get_to_string() == p_name) {
			transitions.write[i]->disconnect("advance_condition_changed", this, "_tree_changed");
			transitions.remove(i);
			i--;
		}
	}

	if (start_node == p_name) {
		start_node = StringName();
	}

	if (end_node == p_name) {
		end_node = StringName();
	}

	/*if (playing && current == p_name) {
		stop();
	}*/

	emit_changed();
	emit_signal("tree_changed");
}

void StateMachine::rename_node(const StringName &p_name, const StringName &p_new_name) {

	ERR_FAIL_COND(!states.has(p_name));
	ERR_FAIL_COND(states.has(p_new_name));

	states[p_new_name] = states[p_name];
	states.erase(p_name);

	for (int i = 0; i < transitions.size(); i++) {
		if (transitions[i].is_null()) {
			continue;
		}
		if (transitions[i]->get_from_string() == p_name) {
			transitions.write[i]->set_from_string(p_new_name);
		}

		if (transitions[i]->get_to_string() == p_name) {
			transitions.write[i]->set_to_string(p_new_name);
		}
	}

	if (start_node == p_name) {
		start_node = p_new_name;
	}

	if (end_node == p_name) {
		end_node = p_new_name;
	}

	/*if (playing && current == p_name) {
		current = p_new_name;
	}*/

	//path.clear(); //clear path
	emit_signal("tree_changed");
}

void StateMachine::get_node_list(List<StringName> *r_nodes) const {

	List<StringName> nodes;
	for (Map<StringName, Ref<FSMState>>::Element *E = states.front(); E; E = E->next()) {
		nodes.push_back(E->key());
	}
	nodes.sort_custom<StringName::AlphCompare>();

	for (List<StringName>::Element *E = nodes.front(); E; E = E->next()) {
		r_nodes->push_back(E->get());
	}
}

bool StateMachine::has_transition(const StringName &p_from, const StringName &p_to) const {

	for (int i = 0; i < transitions.size(); i++) {
		if (transitions[i].is_null()) {
			continue;
		}
		if (transitions[i]->get_from_string() == p_from && transitions[i]->get_to_string() == p_to)
			return true;
	}
	return false;
}

int StateMachine::find_transition(const StringName &p_from, const StringName &p_to) const {

	for (int i = 0; i < transitions.size(); i++) {
		if (transitions[i].is_null()) {
			continue;
		}
		if (transitions[i]->get_from_string() == p_from && transitions[i]->get_to_string() == p_to)
			return i;
	}
	return -1;
}

void StateMachine::add_transition(const Ref<StateMachineTransition> &p_transition) {

	if (p_transition.is_null()) {
		transitions.push_back(p_transition);
		return;
	}
	ERR_FAIL_COND(!states.has(p_transition->get_from_string()));
	ERR_FAIL_COND(!states.has(p_transition->get_to_string()));
	ERR_FAIL_COND(p_transition->get_from_string() == p_transition->get_to_string());

	for (int i = 0; i < transitions.size(); i++) {
		ERR_FAIL_COND(transitions[i]->get_from_string() == p_transition->get_from_string() && transitions[i]->get_to_string() == p_transition->get_to_string());
	}

	transitions.push_back(p_transition);
	transitions.write[transitions.size() - 1]->connect("advance_condition_changed", this, "_tree_changed", varray(), CONNECT_REFERENCE_COUNTED);
}

Ref<StateMachineTransition> StateMachine::get_transition(int p_transition) const {
	ERR_FAIL_INDEX_V(p_transition, transitions.size(), Ref<StateMachineTransition>());
	return transitions[p_transition];
}
StringName StateMachine::get_transition_from(int p_transition) const {

	ERR_FAIL_INDEX_V(p_transition, transitions.size(), StringName());
	ERR_FAIL_COND_V(transitions[p_transition].is_null(), StringName());
	return transitions[p_transition]->get_from_string();
}
StringName StateMachine::get_transition_to(int p_transition) const {

	ERR_FAIL_INDEX_V(p_transition, transitions.size(), StringName());
	ERR_FAIL_COND_V(transitions[p_transition].is_null(), StringName());
	return transitions[p_transition]->get_to_string();
}

int StateMachine::get_transition_count() const {

	return transitions.size();
}
void StateMachine::remove_transition(const StringName &p_from, const StringName &p_to) {

	for (int i = 0; i < transitions.size(); i++) {
		if (transitions[i].is_null()) {
			continue;
		}
		if (transitions[i]->get_from_string() == p_from && transitions[i]->get_to_string() == p_to) {
			transitions.write[i]->disconnect("advance_condition_changed", this, "_tree_changed");
			transitions.remove(i);
			return;
		}
	}

	/*if (playing) {
		path.clear();
	}*/
}

void StateMachine::remove_transition_by_index(int p_transition) {

	ERR_FAIL_INDEX(p_transition, transitions.size());
	ERR_FAIL_COND(transitions[p_transition].is_null());
	transitions.write[p_transition]->disconnect("advance_condition_changed", this, "_tree_changed");
	transitions.remove(p_transition);
	/*if (playing) {
		path.clear();
	}*/
}

void StateMachine::set_start_node(const StringName &p_node) {

	ERR_FAIL_COND(p_node != StringName() && !states.has(p_node));
	start_node = p_node;
}

String StateMachine::get_start_node() const {

	return start_node;
}

void StateMachine::set_end_node(const StringName &p_node) {

	ERR_FAIL_COND(p_node != StringName() && !states.has(p_node));
	end_node = p_node;
}

String StateMachine::get_end_node() const {

	return end_node;
}

void StateMachine::set_graph_offset(const Vector2 &p_offset) {
	graph_offset = p_offset;
}

Vector2 StateMachine::get_graph_offset() const {
	return graph_offset;
}

void StateMachine::set_transitions(const Array &trans) {
	_set("transitions", trans);
}

Array StateMachine::get_transitions() const {
	Variant trans;
	_get("transitions", trans);
	return trans;
}

String StateMachine::get_caption() const {
	return "StateMachine";
}

void StateMachine::_notification(int p_what) {
}

Ref<Resource> StateMachine::get_child_by_name(const StringName &p_name) {
	return get_node(p_name);
}

float StateMachine::blend_node(const StringName &p_sub_path, Ref<Resource> p_node, float p_time, bool p_seek, float p_blend, bool p_optimize) {
	return 1.0f;
}

bool StateMachine::_set(const StringName &p_name, const Variant &p_value) {

	String name = p_name;
	if (name.begins_with("states/")) {
		String node_name = name.get_slicec('/', 1);
		String what = name.get_slicec('/', 2);

		if (what == "node") {
			Ref<Resource> anode = p_value;
			if (anode.is_valid()) {
				add_node(node_name, p_value);
			}
			return true;
		}

		if (what == "position") {

			if (states.has(node_name)) {
				states[node_name]->position = p_value;
			}
			return true;
		}
	} else if (name == "transitions") {
		Array trans = p_value;
		for (int i = 0; i < trans.size(); i++) {
			Ref<StateMachineTransition> transition = trans[i];
			add_transition(transition);
		}
		return true;
	} else if (name == "start_node") {
		set_start_node(p_value);
		return true;
	} else if (name == "end_node") {
		set_end_node(p_value);
		return true;
	} else if (name == "graph_offset") {
		set_graph_offset(p_value);
		return true;
	}

	return false;
}

bool StateMachine::_get(const StringName &p_name, Variant &r_ret) const {

	String name = p_name;
	if (name.begins_with("states/")) {
		String node_name = name.get_slicec('/', 1);
		String what = name.get_slicec('/', 2);

		if (what == "node") {
			if (states.has(node_name)) {
				r_ret = states[node_name]->node;
				return true;
			}
		}

		if (what == "position") {

			if (states.has(node_name)) {
				r_ret = states[node_name]->position;
				return true;
			}
		}
	} else if (name == "transitions") {
		Array trans;
		trans.resize(transitions.size());

		for (int i = 0; i < transitions.size(); i++) {
			trans[i] = transitions[i];
		}

		r_ret = trans;
		return true;
	} else if (name == "start_node") {
		r_ret = get_start_node();
		return true;
	} else if (name == "end_node") {
		r_ret = get_end_node();
		return true;
	} else if (name == "graph_offset") {
		r_ret = get_graph_offset();
		return true;
	}

	return false;
}
void StateMachine::_get_property_list(List<PropertyInfo> *p_list) const {

	List<StringName> names;
	for (Map<StringName, Ref<FSMState>>::Element *E = states.front(); E; E = E->next()) {
		names.push_back(E->key());
	}
	names.sort_custom<StringName::AlphCompare>();

	for (List<StringName>::Element *E = names.front(); E; E = E->next()) {
		String name = E->get();
		p_list->push_back(PropertyInfo(Variant::OBJECT, "states/" + name + "/node", PROPERTY_HINT_RESOURCE_TYPE, "Resource", PROPERTY_USAGE_NOEDITOR));
		p_list->push_back(PropertyInfo(Variant::VECTOR2, "states/" + name + "/position", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NOEDITOR));
	}

	p_list->push_back(PropertyInfo(Variant::ARRAY, "transitions", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NOEDITOR));
	p_list->push_back(PropertyInfo(Variant::STRING, "start_node", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NOEDITOR));
	p_list->push_back(PropertyInfo(Variant::STRING, "end_node", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NOEDITOR));
	p_list->push_back(PropertyInfo(Variant::VECTOR2, "graph_offset", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NOEDITOR));
}

void StateMachine::set_node_position(const StringName &p_name, const Vector2 &p_position) {
	ERR_FAIL_COND(!states.has(p_name));
	states[p_name]->position = p_position;
}

Vector2 StateMachine::get_node_position(const StringName &p_name) const {

	ERR_FAIL_COND_V(!states.has(p_name), Vector2());
	return states[p_name]->position;
}

void StateMachine::_tree_changed() {
	emit_signal("tree_changed");
}

void StateMachine::_bind_methods() {

	ClassDB::bind_method(D_METHOD("add_node", "name", "node", "position"), &StateMachine::add_node, DEFVAL(Vector2()));
	ClassDB::bind_method(D_METHOD("get_node", "name"), &StateMachine::get_node);
	ClassDB::bind_method(D_METHOD("remove_node", "name"), &StateMachine::remove_node);
	ClassDB::bind_method(D_METHOD("rename_node", "name", "new_name"), &StateMachine::rename_node);
	ClassDB::bind_method(D_METHOD("has_node", "name"), &StateMachine::has_node);
	ClassDB::bind_method(D_METHOD("get_node_name", "node"), &StateMachine::get_node_name);

	ClassDB::bind_method(D_METHOD("set_node_position", "name", "position"), &StateMachine::set_node_position);
	ClassDB::bind_method(D_METHOD("get_node_position", "name"), &StateMachine::get_node_position);

	ClassDB::bind_method(D_METHOD("has_transition", "from", "to"), &StateMachine::has_transition);
	ClassDB::bind_method(D_METHOD("add_transition", "transition"), &StateMachine::add_transition);
	ClassDB::bind_method(D_METHOD("get_transition", "idx"), &StateMachine::get_transition);
	ClassDB::bind_method(D_METHOD("get_transition_from", "idx"), &StateMachine::get_transition_from);
	ClassDB::bind_method(D_METHOD("get_transition_to", "idx"), &StateMachine::get_transition_to);
	ClassDB::bind_method(D_METHOD("get_transition_count"), &StateMachine::get_transition_count);
	ClassDB::bind_method(D_METHOD("remove_transition_by_index", "idx"), &StateMachine::remove_transition_by_index);
	ClassDB::bind_method(D_METHOD("remove_transition", "from", "to"), &StateMachine::remove_transition);

	ClassDB::bind_method(D_METHOD("set_start_node", "name"), &StateMachine::set_start_node);
	ClassDB::bind_method(D_METHOD("get_start_node"), &StateMachine::get_start_node);

	ClassDB::bind_method(D_METHOD("set_end_node", "name"), &StateMachine::set_end_node);
	ClassDB::bind_method(D_METHOD("get_end_node"), &StateMachine::get_end_node);

	ClassDB::bind_method(D_METHOD("set_graph_offset", "offset"), &StateMachine::set_graph_offset);
	ClassDB::bind_method(D_METHOD("get_graph_offset"), &StateMachine::get_graph_offset);

	ClassDB::bind_method(D_METHOD("set_transitions", "transitions"), &StateMachine::set_transitions);
	ClassDB::bind_method(D_METHOD("get_transitions"), &StateMachine::get_transitions);

	ClassDB::bind_method(D_METHOD("_tree_changed"), &StateMachine::_tree_changed);

	ADD_PROPERTY(PropertyInfo(Variant::STRING, "start_node"), "set_start_node", "get_start_node");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "end_node"), "set_end_node", "get_end_node");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "offset"), "set_graph_offset", "get_graph_offset");
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "transitions"), "set_transitions", "get_transitions");
}

StateMachine::StateMachine() {
}
