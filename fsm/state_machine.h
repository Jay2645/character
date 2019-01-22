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

#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include "core/resource.h"

#include "state.h"
#include "state_machine_transition.h"

class StateMachine : public Resource {

	GDCLASS(StateMachine, Resource);

private:
	Map<StringName, Ref<FSMState> > states;
	Vector<Ref<StateMachineTransition> > transitions;

	StringName start_node;
	StringName end_node;

	Vector2 graph_offset;

	void _tree_changed();

protected:
	void _notification(int p_what);
	static void _bind_methods();

	bool _set(const StringName &p_name, const Variant &p_value);
	bool _get(const StringName &p_name, Variant &r_ret) const;
	void _get_property_list(List<PropertyInfo> *p_list) const;

public:
	void add_node(const StringName &p_name, Ref<Resource> p_node, const Vector2 &p_position = Vector2());
	void add_state(const StringName &p_name, Ref<FSMState> state);

	Ref<Resource> get_node(const StringName &p_name) const;
	void remove_node(const StringName &p_name);
	void rename_node(const StringName &p_name, const StringName &p_new_name);
	bool has_node(const StringName &p_name) const;
	StringName get_node_name(const Ref<Resource> &p_node) const;
	void get_node_list(List<StringName> *r_nodes) const;

	void set_node_position(const StringName &p_name, const Vector2 &p_position);
	Vector2 get_node_position(const StringName &p_name) const;

	virtual void get_child_nodes(List<Ref<FSMState> > *r_child_nodes);

	bool has_transition(const StringName &p_from, const StringName &p_to) const;
	int find_transition(const StringName &p_from, const StringName &p_to) const;
	void add_transition(const Ref<StateMachineTransition> &p_transition);
	Ref<StateMachineTransition> get_transition(int p_transition) const;
	StringName get_transition_from(int p_transition) const;
	StringName get_transition_to(int p_transition) const;
	int get_transition_count() const;
	void remove_transition_by_index(int p_transition);
	void remove_transition(const StringName &p_from, const StringName &p_to);

	void set_start_node(const StringName &p_node);
	String get_start_node() const;

	void set_end_node(const StringName &p_node);
	String get_end_node() const;

	void set_graph_offset(const Vector2 &p_offset);
	Vector2 get_graph_offset() const;

	void set_transitions(const Array &trans);
	Array get_transitions() const;

	virtual String get_caption() const;

	virtual Ref<Resource> get_child_by_name(const StringName &p_name);

	virtual float blend_node(const StringName &p_sub_path, Ref<Resource> p_node, float p_time, bool p_seek, float p_blend, bool p_optimize = true);

	StateMachine();
};

#endif // ANIMATION_NODE_STATE_MACHINE_H
