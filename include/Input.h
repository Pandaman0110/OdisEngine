#ifndef INPUT_H
#define INPUT_H

#include <vector>
#include <string>
#include <map>

#include <GLFW/glfw3.h>

namespace OdisEngine
{
	class InputMap
	{
	public:
		InputMap();

		void addEvent (std::string action, int event);
		void eraseEvent (std::string action, int event);
		void removeEvents (std::string action);

		void addAction (std::string action);
		void eraseAction (std::string action);

		std::vector<std::string> getActions() const;

	private:
		//std::multimap<std::string, >

	};



	class Input
	{
	public:
		Input();

		void pollInputs();

		bool isActionPressed(std::string action) const;
		bool isActionRelease(std::string action) const;
		bool isActionDown(std::string action) const;

	private:
		InputMap input_map;

	};

}

#endif