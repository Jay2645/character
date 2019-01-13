#ifndef COMMAND_MANAGER_H
#define COMMAND_MANAGER_H

#include "scene/main/node.h"
#include "command_node.h"
#include "core/vector.h"
#include "core/set.h"


// A CommandManager is a node which runs every physics tick (by default 60 times a second)
// and executes all the active commands that get passed into it.
//
// This is useful for player input -- the player can give a "move forward" command by
// pressing a button. A command somewhere along the line consumes the move forward input
// and notifies this manager that it is active and wants to be ticked ASAP.
// This manager will then tick that command every physics frame until the player stops
// giving input, at which point the command unregisters itself from the "active input" list.
//
// This is also useful for AI -- a pathfinding algorithm can give an AI a heading. The AI
// turns this heading into the same commands that the player uses when they press a button --
// in effect giving the AI an artificial joystick. The AI's character can then issue commands
// just as if they were a player.
//
// This also has other uses. For example, a replay system can save every single command that
// every CommandManager gets. When the player wants to re-watch the game, the replay system
// can take over and issue the appropriate commands to each command manager. Assuming
// everything else is deterministic, the game will play out in the exact same way.
//
// Another possible use is in multiplayer over a network. A client can simply tell the server
// what commands it is issuing. The server does those commands and gets back to the client
// to tell them the "real" output from their commands.
class CommandManager : public Node {
	GDCLASS(CommandManager, Node);

private:
	const StringName EXECUTE_COMMAND_NAME = StringName("execute_command");

protected:
	Object *_command_reciever;

public:
	CommandManager();

protected:
	void _notification(int p_notification);
	static void _bind_methods();

	void _physics_process(float delta_time);

	void _execute_all_commands(float delta_time);

public:
	void set_reciever(Object *reciever);
	void add_listener(CommandNode *listener, StringName execute_command);
	void remove_listener(CommandNode *listener, StringName execute_command);
};
#endif
