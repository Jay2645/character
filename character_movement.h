#ifndef MOVEMENT_H
#define MOVEMENT_H

#include "core/math/vector3.h"
#include "scene/3d/camera.h"
#include "scene/3d/physics_body.h"
#include "scene/main/node.h"

class CharacterMovement : public Node {
	GDCLASS(CharacterMovement, Node);

protected:
	Vector2 _camera_look_dir;
	Vector3 _move_input;
	Vector3 _dir;
	Vector3 _vel;
	Spatial *_x_rotator;
	Camera *_camera;
	KinematicBody *_move_body;

public:
	NodePath camera_path;
	NodePath x_rotator_path;
	NodePath move_body_path;

	float JUMP_SPEED;
	float MAX_SPEED;
	float ACCELERATION;
	float DECELERATION;
	float MAX_SLOPE;
	float MAX_LOOK_UP;
	Vector3 GRAVITY;

	// @TODO: Move this to settings class
	float MOUSE_SENSITIVITY;
	bool INVERT_Y;

public:
	CharacterMovement();

protected:
	void _movement_ready();
	static void _bind_methods();

public:
	void add_character_movement(Vector3 movement);
	void add_camera_movement(Vector2 movement);
	void process_movement(float delta_time);

	virtual bool can_jump();

	void set_camera_path(const NodePath &new_path);
	NodePath get_camera_path() const;
	void set_x_rotator_path(const NodePath &new_path);
	NodePath get_x_rotator_path() const;
	void set_move_body_path(const NodePath &new_path);
	NodePath get_move_body_path() const;

	void set_jump_speed(const float& new_jump_speed);
	float get_jump_speed() const;
	void set_max_speed(const float &new_max_speed);
	float get_max_speed() const;
	void set_acceleration(const float &new_acceleration);
	float get_acceleration() const;
	void set_deceleration(const float &new_deceleration);
	float get_deceleration() const;
	void set_max_slope(const float &new_slope);
	float get_max_slope() const;
	void set_max_look_up(const float &new_look_up);
	float get_max_look_up() const;
	void set_gravity(const Vector3 &new_gravity);
	Vector3 get_gravity() const;
};

#endif
