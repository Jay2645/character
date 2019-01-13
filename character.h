
#ifndef CHARACTER_H
#define CHARACTER_H

#include "scene/3d/camera.h"
#include "scene/3d/collision_shape.h"
#include "scene/3d/physics_body.h"
#include "scene/3d/spatial.h"

#include "command_manager.h"

class Character : public KinematicBody {
	GDCLASS(Character, KinematicBody);

protected:
	CollisionShape *_body_collision;
	CollisionShape *_feet_collision;

	Spatial *_rotation_helper;
	Spatial *_model_base;
	Camera *_character_camera;
	CommandManager *_command_manager;

public:
	Character();
	~Character();

protected:
	static void _bind_methods();

	void _ready();
};

#endif
