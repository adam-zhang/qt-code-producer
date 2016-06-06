#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>
#include <string.h>

using namespace std;

bool printHelp()
{
	cout << "Usage:\n"
		<< "\t--help show this message.\n"
		<< "\t--app generate files which can build a basic application.\n"
		<< "\t--gui generate files which can build a basic GUI application.\n"
		<< "\t--class generate two files for a basic structure of a class\n"
		<< "\t-c a short name for --class." 
		<< endl;
	return true;
}

bool generateApp()
{
	stringstream ss;
	ss << "#include <iostream>\n\n"
		<< "using namespace std;\n\n"
		<< "int main(int argc, char** argv);\n"
		<< "{\n"
		<< "\tcout << \"Hello World!\" << endl;\n"
		<< "}"
		<< endl;
	fstream file;
	file.open("appMain.cpp", ios_base::in|ios_base::out);
	if (file.good())
	{
		file << ss.str();
		file.close();
		return true;
	}
	return false;
}

bool analyse(int argc, char** argv)
{
	if (argc == 2 || argc == 3)
		return true;
	else
		return printHelp();
}

int main(int argc, char** argv)
{
	if (!analyse(argc, argv))
		return 0;
	if (argc == 2 && strcmp(argv[1],"--help") == 0)
		return printHelp();
	if (argc == 2 && strcmp(argv[1],"--app") == 0)
		return generateApp();
	//if (argc == 2 && strcmp(argv[1],"--gui") == 0)
	//	return generateGuiApp();
	//if (argc == 3 && (strcmp(argv[1],"--class") == 0 || strcmp(argv[1],"-c") == 0)
	//	return generateClass(argv[2]);
	return 0;
}
