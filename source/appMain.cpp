#include <vector>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <fstream>
#include <string.h>
#include <cctype>

using namespace std;

int printHelp()
{
	cout << "Usage:\n"
		<< "\t--help show this message.\n"
		//<< "\t--app generate files which can build a basic application.\n"
		//<< "\t--gui generate files which can build a basic GUI application.\n"
		<< "\t--class className : generate two files for a class without base class.\n"
		<< "\t--class className baseClassName : generate two files for a class with base class."
		//<< "\t--qt-class generate two files for a class which base class must be a class in Qt.\n"
		//<< "\t--baseclass generate two files for a class without base class.\n"
		<< endl;
	return 0;
}

string getUppercase(const string& className)
{
	string uppercase(className);
	transform(uppercase.begin(), uppercase.end(), uppercase.begin(),::toupper);
	return "__" + uppercase + "__H";
}

string generateClassHeaderFile(const string& className)
{
	string uppercase = getUppercase(className);
	return "#ifndef " + uppercase + "\n" +
		"#define " + uppercase + "\n\n" +
		"class " + className + "\n"
		"{\n"
		"public:\n"
		"\t" + className + "();\n"
		"\t~" + className + "();\n"
		"}\n"
		"#endif//" + uppercase + "\n";
}

string generateClassHeaderFile(const string& className, const string& baseClassName)
{
	string uppercase = getUppercase(className);
	return "#ifndef " + uppercase + "\n"
		"#define " + uppercase + "\n\n"
		"#include \"" + baseClassName + ".h\"\n\n"
		"class " + className + "\n"
		"{\n"
		"public:\n"
		"\t" + className + "();\n"
		"\t~" + className + "();\n"
		"#endif//" + uppercase + "\n";

}

string generateClassCppFile(const string& className)
{
	return "#include \"" + className + ".h\"\n\n" +
		className + "::" + className + "(){}\n" +
		className + "::~" + className + "() {}\n";

}

string generateClassCppFile(const string& className, const string& baseClassName)
{
	return "#include \"" + className + ".h\"\n\n" +
		className + "::" + className + "()\n"
		"\t:" + baseClassName + "()\n"
		"\t{}\n" +
		className + "::~" + className + "(){}\n";
}

bool generateApp(const string& /*projectName*/)
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
	ofstream file;
	file.open(fileName.c_str());
	if (file.good())
	{
		file << content;
		file.close();
		return;
	}
}

string getGuiMainContent()
{
	return "#include <QApplication>\n"
		"#include \"MainWindow.h\"\n\n"
		"int main(int argc, char** argv)\n"
		"{\n"
		"\tQApplication a(argc, argv);\n"
		"\tMainWindow w;\n"
		"w.show();\n"
		"return a.exec();\n"
		"}";
}

string getMainWindowHeaderContent()
{
	stringstream ss;
	ss << "#ifndef __MAIN_WINDOW__H\n"
	      "#define __MAIN_WINDOW__H\n\n"
	      "#include <QMainWindow>\n\n"
	      "class MainWindow : public QMainWindow\n"
	      "{\n"
	      "\tQ_OBJECT\n"
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

string consoleMakeLists(const string projectName)
{
	return "cmake_minimum_required(VERSION 2.8)\n"
		"project(\"" + projectName + ")\n"
		"set(sources appMain.cpp\n"
		"\t)\n"
		"add_executable(" + projectName + " {sources})\n";

}

int generateGuiApp(const string& projectName)
{
	string mainContent = getGuiMainContent();
	writeContent("appMain.cpp", mainContent);
	string guiHeaderContent = getMainWindowHeaderContent();
	writeContent("MainWindow.h", guiHeaderContent);
	string guiWindowContent = getGuiMainWindowContent();
	writeContent("MainWindow.cpp", guiWindowContent);
	//if (projectName.size() == 0)
	//	writeContent("CMakeLists.txt", guiMakeLists("test"));
	//else
	//	writeContent("CMakeLists.txt", guiMakeLists(projectName));
}




int generateClass(const string& className)
{
	string headerFile = className + ".h";
	string cppFile = className + ".cpp";
	writeContent(headerFile, generateClassHeaderFile(className));
       	writeContent(cppFile, generateClassCppFile(className));
	return 0;
}

int generateClass(const string& className, const string& baseClassName)
{
	string headerFile = className + ".h";
	string cppFile = className + ".cpp";
	writeContent(headerFile, generateClassHeaderFile(className, baseClassName));
	writeContent(cppFile, generateClassCppFile(className, baseClassName));
	return 0;
}

int process(const string&)
{

}

int process(const string& arg1, const string& arg2)
{
	if (arg1 == "--class")
		return generateClass(arg2);	
	return -1;
}

int process(const string& arg1, const string& arg2, const string& arg3)
{
	if (arg1 == "--class")
		return generateClass(arg2, arg3);
	return -1;
}

int main(int argc, char** argv)
{
	if (argc == 1)
		return printHelp();
	if (argc == 2)
		return process(argv[1]);
	if (argc == 3)
		return process(argv[1], argv[2]);
	if (argc == 4)
		return process(argv[1], argv[2], argv[3]);
	return 0;
}
