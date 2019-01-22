#include "character.h"
#include "scene/3d/camera.h"
#include "scene/3d/collision_shape.h"
#include "scene/3d/spatial.h"

Character::Character() {
	connect("ready", this, "_character_ready");
	set_physics_process(true);

	_command_manager = NULL;
	_movement = NULL;
}

void Character::_character_ready() {
	if (has_node(command_manager_path)) {
		_command_manager = cast_to<CommandManager>(get_node(command_manager_path));
		if (_command_manager != NULL) {
			_command_manager->set_reciever(this);
		}
	}
	if (has_node(character_movement_path)) {
		_movement = cast_to<CharacterMovement>(get_node(character_movement_path));
	}
}

void Character::_notification(int p_notification) {
	if (p_notification == NOTIFICATION_PHYSICS_PROCESS) {
		_physics_process(get_physics_process_delta_time());
	}
	KinematicBody::_notification(p_notification);
}

void Character::_physics_process(float delta_time) {
	if (_command_manager != NULL) {
		_command_manager->execute_all_commands(delta_time);
	}
	if (_movement != NULL) {
		_movement->process_movement(delta_time);
	}
}

void Character::_bind_methods() {
	ClassDB::bind_method(D_METHOD("_character_ready"), &Character::_character_ready);

	ClassDB::bind_method(D_METHOD("set_command_manager_path", "command_paths"), &Character::set_command_manager_path);
	ClassDB::bind_method(D_METHOD("get_command_manager_path"), &Character::get_command_manager_path);
	ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "command_manager_path", PROPERTY_HINT_NODE_PATH_VALID_TYPES, "CommandManager"), "set_command_manager_path", "get_command_manager_path");

	ClassDB::bind_method(D_METHOD("set_character_movement_path", "movement_path"), &Character::set_character_movement_path);
	ClassDB::bind_method(D_METHOD("get_character_movement_path"), &Character::get_character_movement_path);
	ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "character_movement_path", PROPERTY_HINT_NODE_PATH_VALID_TYPES, "CharacterMovement"), "set_character_movement_path", "get_character_movement_path");

	ClassDB::bind_method(D_METHOD("get_character_movement"), &Character::get_character_movement);
}

void Character::set_command_manager_path(const NodePath &new_path) {
	command_manager_path = new_path;
	if (has_node(command_manager_path)) {
		_command_manager = cast_to<CommandManager>(get_node(command_manager_path));
	}
}

NodePath Character::get_command_manager_path() const {
	return command_manager_path;
}

void Character::set_character_movement_path(const NodePath &new_path) {
	character_movement_path = new_path;
	if (has_node(character_movement_path)) {
		_movement = cast_to<CharacterMovement>(get_node(character_movement_path));
	}
}

NodePath Character::get_character_movement_path() const {
	return character_movement_path;
}

CharacterMovement *Character::get_character_movement() const {
	return _movement;
}
