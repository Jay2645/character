
#ifndef CHARACTER_H
#define CHARACTER_H

#include "scene/3d/camera.h"
#include "scene/3d/collision_shape.h"
#include "scene/3d/physics_body.h"
#include "scene/3d/spatial.h"

#include "character_movement.h"
#include "modules/command/command_manager.h"

class Character : public KinematicBody {
	GDCLASS(Character, KinematicBody);

protected:
	CommandManager *_command_manager;
	CharacterMovement *_movement;

public:
	NodePath command_manager_path;
	NodePath character_movement_path;

public:
	Character();

protected:
	static void _bind_methods();

	void _character_ready();
	void _notification(int p_notification);

	void _physics_process(float delta_time);

public:
	void set_command_manager_path(const NodePath &new_path);
	NodePath get_command_manager_path() const;
	void set_character_movement_path(const NodePath &new_path);
	NodePath get_character_movement_path() const;

	CharacterMovement *get_character_movement() const;
};

#endif
