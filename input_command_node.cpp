#include "input_command_node.h"
#include "core/input_map.h"
#include "core/os/input.h"
#include "scene/main/scene_tree.h"

InputCommandNode::InputCommandNode() {
	set_process_unhandled_input(true);
}


void InputCommandNode::_update_inputs() {
	Map<StringName, InputMap::Action> action_map = InputMap::get_singleton()->get_action_map();
	if (action_map.has(command_input)) {
		valid_inputs = action_map[command_input].inputs;
	} else {
		valid_inputs.clear();
	}
}

void InputCommandNode::_ready() {
	_update_inputs();
	// Parent ready class implements updating the warning
	CommandNode::_ready();
}

void InputCommandNode::_unhandled_input(Ref<InputEvent> p_event) {
	for (int i = 0; i < valid_inputs.size(); i++) {
		if (p_event->shortcut_match(valid_inputs[i])) {
			if (p_event->is_pressed()) {
				add_command();
			} else {
				remove_command();
			}
			SceneTree::get_singleton()->set_input_as_handled();
			break;
		}
	}
}

void InputCommandNode::_bind_methods() {
	ClassDB::bind_method(D_METHOD("_unhandled_input"), &InputCommandNode::_unhandled_input);

	ClassDB::bind_method(D_METHOD("set_input_event", "input"), &InputCommandNode::set_input_event);
	ClassDB::bind_method(D_METHOD("get_input_event"), &InputCommandNode::get_input_event);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "command_input"), "set_input_event", "get_input_event");
}

String InputCommandNode::get_configuration_warning() const {
	if (valid_inputs.size() == 0) {
		return String("No valid inputs found!");
	}
	return CommandNode::get_configuration_warning();
}

void InputCommandNode::set_input_event(const String &input) {
	command_input = input;
	_update_inputs();
	update_configuration_warning();
}

String InputCommandNode::get_input_event() const {
	return command_input;
}
