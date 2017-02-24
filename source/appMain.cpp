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
		<< "\t--class generate two files for a class with base class.\n"
		<< "\t--qclass generate two files for a class which base class must be a class in Qt.\n"
		<< "\t--baseclass generate two files for a class without base class.\n"
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
	if (argc == 2 || argc == 3 || argc == 4)
		return true;
	else
		return printHelp();
}

void writeContent(const string& fileName, const string& content)
{
	fstream file;
	file.open(fileName.c_str(), ios_base::in);
	if (file.good())
	{
		file << content;
		file.close();
		return;
	}
}

string getGuiMainContent()
{
	stringstream ss;
	ss << "#include <QApplication>\n"
		"#include \"MainWindow.h\"\n\n"
		"int main(int argc, char** argv)\n"
		"{\n"
		"\tQApplication a(argc, argv);\n"
		"\tMainWindow w;"
		"w.show();"
		"return a.exec();"
		"}" << endl;
	return ss.str();
}

string getMainWindowHeaderContent()
{
	stringstream ss;
	ss << "#ifndef __MAIN_WINDOW__H\n"
	      "#define __MAIN_WINDOW__H\n\n"
	      "#include <QMainWindow>\n\n"
	      "class MainWindow : public QMainWindow\n"
	      "{\n"
	      "public:\n"
	      "\tMainWindow();\n"
	      "\t~MainWindow();\n"
	      "}\n"
	      "#endif//__MAIN_WINDOW__H" << endl;
	return ss.str();
}

string getGuiMainWindowContent()
{
	return "#include \"MainWindow.h\"\n\n"
		"MainWindow::MainWindow() {}\n\n"
		"MainWindow::~MainWindow() {}";

}

int generateGuiApp()
{
	string mainContent = getGuiMainContent();
	writeContent("appMain.cpp", mainContent);
	string guiHeaderContent = getMainWindowHeaderContent();
	writeContent("MainWindow.h", guiHeaderContent);
	string guiWindowContent = getGuiMainWindowContent();
	writeContent("MainWindow.cpp", guiWindowContent);
}

int main(int argc, char** argv)
{
	if (!analyse(argc, argv))
		return 0;
	if (argc == 2 && strcmp(argv[1],"--help") == 0)
		return printHelp();
	if (argc == 2 && strcmp(argv[1],"--app") == 0)
		return generateApp();
	if (argc == 2 && strcmp(argv[1],"--gui") == 0)
		return generateGuiApp();
	//if (argc == 3 && (strcmp(argv[1],"--class") == 0 || strcmp(argv[1],"-c") == 0)
	//	return generateClass(argv[2]);
	return 0;
}
