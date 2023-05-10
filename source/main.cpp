#include "GUI/main_menu.h"

int main(int argc, char** argv)
{
	if (argc == 1)
	{
		std::cout << "Not enough arguments\nAttach a .txt" << std::endl;
		//return 0;
	}

	std::string program_name;
	std::string txt_file_name;
	std::string type;

	if (argc > 1)
	{
		program_name = argv[0];
		txt_file_name = argv[1];
	}

	MainMenu main_menu(txt_file_name/*"C:/Users/ass/source/repos/Visual-Vibration-Tracking-V2/docs/input.txt"*/);
}