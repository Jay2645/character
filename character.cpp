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
	if (get_child_count() > 0) {
		return;
	}

	const String _body_collision_name = String("Body_Collision");
	const String _feet_collision_name = String("Feet_Collision");
	const String _rotation_helper_name = String("Rotation_Helper");
	const String _model_base_name = String("Model");
	const String _character_camera_name = String("Camera");
	const String _command_manager_name = String("Command_Manager");

	Node *owner = get_owner();
	if (owner == NULL) {
		owner = this;
	}
	_body_collision = memnew(CollisionShape);
	_body_collision->set_name(_body_collision_name);
	add_child(_body_collision);
	_body_collision->set_owner(owner);

	_feet_collision = memnew(CollisionShape);
	_feet_collision->set_name(_feet_collision_name);
	add_child(_feet_collision);
	_feet_collision->set_owner(owner);

	_rotation_helper = memnew(Spatial);
	_rotation_helper->set_name(_rotation_helper_name);
	add_child(_rotation_helper);
	_rotation_helper->set_owner(owner);

	_model_base = memnew(Spatial);
	_model_base->set_name(_model_base_name);
	_rotation_helper->add_child(_model_base);
	_model_base->set_owner(owner);

	_character_camera = memnew(Camera);
	_character_camera->set_name(_character_camera_name);
	_rotation_helper->add_child(_character_camera);
	_character_camera->set_owner(owner);

	_command_manager = memnew(CommandManager);
	_command_manager->set_name(_command_manager_name);
	add_child(_command_manager);
	_command_manager->set_owner(owner);
}
