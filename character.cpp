#include "character.h"
#include "command_manager.h"
#include "scene/3d/camera.h"
#include "scene/3d/collision_shape.h"
#include "scene/3d/spatial.h"

Character::Character() {
	connect("ready", this, "_ready");
}

Character::~Character() {
}

void Character::_bind_methods() {
	ClassDB::bind_method(D_METHOD("_ready"), &Character::_ready);
}

void Character::_ready() {
	const String _body_collision_name = String("Body_Collision");
	const String _feet_collision_name = String("Feet_Collision");
	const String _rotation_helper_name = String("Rotation_Helper");
	const String _model_base_name = String("Model");
	const String _character_camera_name = String("Camera");
	const String _command_manager_name = String("Command_Manager");

	if (get_child_count() > 0) {
		for (int i = 0; i < get_child_count(); i++) {
			Node *child = get_child(i);
			if (child->get_name() == _body_collision_name) {
				_body_collision = cast_to<CollisionShape>(child);
			} else if (child->get_name() == _feet_collision_name) {
				_feet_collision = cast_to<CollisionShape>(child);
			} else if (child->get_name() == _rotation_helper_name) {
				_rotation_helper = cast_to<Spatial>(child);
			} else if (child->get_name() == _command_manager_name) {
				_command_manager = cast_to<CommandManager>(child);
			}
		}
		if (_rotation_helper != NULL) {
			for (int i = 0; i < _rotation_helper->get_child_count(); i++) {
				Node *child = _rotation_helper->get_child(i);
				if (child->get_name() == _model_base_name) {
					_model_base = cast_to<Spatial>(child);
				} else if (child->get_name() == _character_camera_name) {
					_character_camera = cast_to<Camera>(child);
				}
			}
		}
	}

	Node *owner = get_owner();
	if (owner == NULL) {
		owner = this;
	}

	if (_body_collision == NULL) {
		_body_collision = memnew(CollisionShape);
		_body_collision->set_name(_body_collision_name);
		add_child(_body_collision);
		_body_collision->set_owner(owner);
	}

	if (_feet_collision == NULL) {
		_feet_collision = memnew(CollisionShape);
		_feet_collision->set_name(_feet_collision_name);
		add_child(_feet_collision);
		_feet_collision->set_owner(owner);
	}

	if (_rotation_helper == NULL) {
		_rotation_helper = memnew(Spatial);
		_rotation_helper->set_name(_rotation_helper_name);
		add_child(_rotation_helper);
		_rotation_helper->set_owner(owner);
	}

	if (_model_base == NULL) {
		_model_base = memnew(Spatial);
		_model_base->set_name(_model_base_name);
		_rotation_helper->add_child(_model_base);
		_model_base->set_owner(owner);
	}

	if (_character_camera == NULL) {
		_character_camera = memnew(Camera);
		_character_camera->set_name(_character_camera_name);
		_rotation_helper->add_child(_character_camera);
		_character_camera->set_owner(owner);
	}

	if (_command_manager == NULL) {
		_command_manager = memnew(CommandManager);
		_command_manager->set_name(_command_manager_name);
		add_child(_command_manager);
		_command_manager->set_owner(owner);
		_command_manager->set_reciever(this);
	}
}
