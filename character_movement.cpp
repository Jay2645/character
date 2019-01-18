#include "character_movement.h"

CharacterMovement::CharacterMovement() {
	JUMP_SPEED = 18.0f;
	GRAVITY = Vector3(0.0, -24.8, 0.0);
	MAX_SPEED = 20.0f;
	MAX_SLOPE = 40.0f;
	ACCELERATION = 4.5f;
	MAX_LOOK_UP = 70.0f;
	MOUSE_SENSITIVITY = 0.5f;
	INVERT_Y = false;

	_move_body = NULL;
	_camera = NULL;
	_x_rotator = NULL;

	connect("ready", this, "_movement_ready");
}

void CharacterMovement::_movement_ready() {
	if (has_node(camera_path)) {
		_camera = cast_to<Camera>(get_node(camera_path));
	}
	if (has_node(x_rotator_path)) {
		_x_rotator = cast_to<Spatial>(get_node(x_rotator_path));
	}
	if (has_node(move_body_path)) {
		_move_body = cast_to<KinematicBody>(get_node(move_body_path));
	}
}

void CharacterMovement::_bind_methods() {
	ClassDB::bind_method(D_METHOD("_movement_ready"), &CharacterMovement::_movement_ready);

	ClassDB::bind_method(D_METHOD("add_character_movement", "movement"), &CharacterMovement::add_character_movement);
	ClassDB::bind_method(D_METHOD("add_camera_movement", "movement"), &CharacterMovement::add_camera_movement);

	ClassDB::bind_method(D_METHOD("set_camera_path", "camera_path"), &CharacterMovement::set_camera_path);
	ClassDB::bind_method(D_METHOD("get_camera_path"), &CharacterMovement::get_camera_path);
	ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "camera_path", PROPERTY_HINT_NODE_PATH_VALID_TYPES, "Camera"), "set_camera_path", "get_camera_path");

	ClassDB::bind_method(D_METHOD("set_x_rotator_path", "rotation_helper"), &CharacterMovement::set_x_rotator_path);
	ClassDB::bind_method(D_METHOD("get_x_rotator_path"), &CharacterMovement::get_x_rotator_path);
	ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "rotation_helper", PROPERTY_HINT_NODE_PATH_VALID_TYPES, "Spatial"), "set_x_rotator_path", "get_x_rotator_path");

	ClassDB::bind_method(D_METHOD("set_move_body_path", "move_body"), &CharacterMovement::set_move_body_path);
	ClassDB::bind_method(D_METHOD("get_move_body_path"), &CharacterMovement::get_move_body_path);
	ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "move_body", PROPERTY_HINT_NODE_PATH_VALID_TYPES, "KinematicBody"), "set_move_body_path", "get_move_body_path");

	ClassDB::bind_method(D_METHOD("set_gravity", "move_body"), &CharacterMovement::set_gravity);
	ClassDB::bind_method(D_METHOD("get_gravity"), &CharacterMovement::get_gravity);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "gravity"), "set_gravity", "get_gravity");

	ClassDB::bind_method(D_METHOD("set_jump_speed", "jump_speed"), &CharacterMovement::set_jump_speed);
	ClassDB::bind_method(D_METHOD("get_jump_speed"), &CharacterMovement::get_jump_speed);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "jump_speed"), "set_jump_speed", "get_jump_speed");

	ClassDB::bind_method(D_METHOD("set_max_speed", "max_speed"), &CharacterMovement::set_max_speed);
	ClassDB::bind_method(D_METHOD("get_max_speed"), &CharacterMovement::get_max_speed);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "max_speed"), "set_max_speed", "get_max_speed");

	ClassDB::bind_method(D_METHOD("set_acceleration", "acceleration"), &CharacterMovement::set_acceleration);
	ClassDB::bind_method(D_METHOD("get_acceleration"), &CharacterMovement::get_acceleration);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "acceleration"), "set_acceleration", "get_acceleration");

	ClassDB::bind_method(D_METHOD("set_deceleration", "deceleration"), &CharacterMovement::set_deceleration);
	ClassDB::bind_method(D_METHOD("get_deceleration"), &CharacterMovement::get_deceleration);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "deceleration"), "set_deceleration", "get_deceleration");

	ClassDB::bind_method(D_METHOD("set_max_slope", "max_slope"), &CharacterMovement::set_max_slope);
	ClassDB::bind_method(D_METHOD("get_max_slope"), &CharacterMovement::get_max_slope);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "max_slope"), "set_max_slope", "get_max_slope");

	ClassDB::bind_method(D_METHOD("set_max_look_up", "max_look_up"), &CharacterMovement::set_max_look_up);
	ClassDB::bind_method(D_METHOD("get_max_look_up"), &CharacterMovement::get_max_look_up);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "max_look_up"), "set_max_look_up", "get_max_look_up");
}

void CharacterMovement::add_character_movement(Vector3 movement) {
	_move_input += movement;
}

void CharacterMovement::add_camera_movement(Vector2 movement) {
	_camera_look_dir += movement;
}

void CharacterMovement::process_movement(float delta_time) {
	if (_move_body == NULL) {
		return;
	}
	if (_camera == NULL) {
		return;
	}
	if (_x_rotator == NULL) {
		return;
	}
	_dir.zero();

	Vector3 _normalized_movement = _move_input.normalized();
	if (_move_input.y > 0.0 && can_jump()) {
		_vel.y = JUMP_SPEED;
	}
	_move_input.zero();

	Transform _base_tfm = _camera->get_global_transform();

	_dir += -_base_tfm.basis.get_axis(2).normalized() * _normalized_movement.z;
	_dir += _base_tfm.basis.get_axis(0).normalized() * _normalized_movement.x;


	_dir.y = 0.0f;
	_dir.normalize();

	_vel += GRAVITY * delta_time;
	Vector3 _horizontal_velocity = _vel;
	_horizontal_velocity.y = 0.0;

	float _accel = _dir.dot(_horizontal_velocity) > 0 ? ACCELERATION : DECELERATION;
	_horizontal_velocity = _horizontal_velocity.linear_interpolate(_dir * MAX_SPEED, ACCELERATION * delta_time);
	_vel.x = _horizontal_velocity.x;
	_vel.z = _horizontal_velocity.z;
	_vel = _move_body->move_and_slide(_vel, Vector3(0.0, 1.0, 0.0), false, 4, Math::deg2rad(MAX_SLOPE));

	_x_rotator->rotate_x(Math::deg2rad(_camera_look_dir.y * MOUSE_SENSITIVITY));
	int _inverted_y = INVERT_Y ? 1 : -1;
	_move_body->rotate_y(Math::deg2rad(_camera_look_dir.x * MOUSE_SENSITIVITY * _inverted_y));

	_camera_look_dir.x = 0.0f;
	_camera_look_dir.y = 0.0f;

	Vector3 _camera_rot = _x_rotator->get_rotation_degrees();
	_camera_rot.x = CLAMP(_camera_rot.x, -1 * MAX_LOOK_UP, MAX_LOOK_UP);
	_x_rotator->set_rotation_degrees(_camera_rot);
}

bool CharacterMovement::can_jump() {
	if (_move_body == NULL) {
		return false;
	}
	return _move_body->is_on_floor();
}

void CharacterMovement::set_camera_path(const NodePath &new_path) {
	camera_path = new_path;
	if (has_node(camera_path)) {
		_camera = cast_to<Camera>(get_node(camera_path));
	}
}

NodePath CharacterMovement::get_camera_path() const {
	return camera_path;
}

void CharacterMovement::set_x_rotator_path(const NodePath &new_path) {
	x_rotator_path = new_path;
	if (has_node(x_rotator_path)) {
		_x_rotator = cast_to<Spatial>(get_node(x_rotator_path));
	}
}

NodePath CharacterMovement::get_x_rotator_path() const {
	return x_rotator_path;
}

void CharacterMovement::set_move_body_path(const NodePath &new_path) {
	move_body_path = new_path;
	if (has_node(move_body_path)) {
		_move_body = cast_to<KinematicBody>(get_node(move_body_path));
	}
}

NodePath CharacterMovement::get_move_body_path() const {
	return move_body_path;
}

void CharacterMovement::set_jump_speed(const float &new_jump_speed) {
	JUMP_SPEED = new_jump_speed;
}

float CharacterMovement::get_jump_speed() const {
	return JUMP_SPEED;
}

void CharacterMovement::set_max_speed(const float &new_max_speed) {
	MAX_SPEED = new_max_speed;
}

float CharacterMovement::get_max_speed() const {
	return MAX_SPEED;
}

void CharacterMovement::set_acceleration(const float &new_acceleration) {
	ACCELERATION = new_acceleration;
}

float CharacterMovement::get_acceleration() const {
	return ACCELERATION;
}

void CharacterMovement::set_deceleration(const float &new_deceleration) {
	DECELERATION = new_deceleration;
}

float CharacterMovement::get_deceleration() const {
	return DECELERATION;
}

void CharacterMovement::set_max_slope(const float &new_slope) {
	MAX_SLOPE = new_slope;
}

float CharacterMovement::get_max_slope() const {
	return MAX_SLOPE;
}

void CharacterMovement::set_max_look_up(const float &new_look_up) {
	MAX_LOOK_UP = new_look_up;
}

float CharacterMovement::get_max_look_up() const {
	return MAX_LOOK_UP;
}

void CharacterMovement::set_gravity(const Vector3 &new_gravity) {
	GRAVITY = new_gravity;
}

Vector3 CharacterMovement::get_gravity() const {
	return GRAVITY;
}
