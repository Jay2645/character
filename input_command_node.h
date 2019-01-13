#ifndef INPUT_COMMAND_NODE
#define  INPUT_COMMAND_NODE

#include "command_node.h"

class InputCommandNode : public CommandNode
{
	GDCLASS(InputCommandNode, CommandNode);

protected:
	List<Ref<InputEvent> > valid_inputs;

public:
	String command_input;

public:
	InputCommandNode();

private:
	void _update_inputs();

protected:
	virtual void _ready() override;
	void _unhandled_input(Ref<InputEvent> p_event);
	static void _bind_methods();

public:
	virtual String get_configuration_warning() const;

	void set_input_event(const String &input);

	String get_input_event() const;
};
#endif
