#include "Input.h"

using namespace OdisEngine;

Input::Input()
{

}

void Input::pollInputs()
{
	glfwPollEvents();
}
