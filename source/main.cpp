#include "GUI/main_menu.h"

int main(int argc, char** argv)
{
	if (argc == 1)
	{
		std::cout << "Not enough arguments\nAttach a .txt" << std::endl;
		return 0;
	}

	std::string program_name;
	std::string txt_file_name;
	std::string type;

	if (argc > 1)
	{
		program_name = argv[0];
		txt_file_name = argv[1];
		type = argv[2];
	}
	int code = std::stoi(type);

	MainMenu main_menu(txt_file_name);
}