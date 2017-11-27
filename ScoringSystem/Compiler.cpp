#include "Compiler.h"
void CreateObjectFiles(std::string Path)
{
	std::string tempPath = Path + "source\\";
	std::vector<std::string> cppFiles = GetArrayOfFile(Path + "source\\", "cpp");
	//if the folder is not in C:\, we must change the disk by adding "D:"
	std::string write_log_command = "D: && cd " + tempPath + " && g++";
	for (int i = 0; i < cppFiles.size(); i++)
		write_log_command += " " + cppFiles[i];
	write_log_command += " 2>log.txt ";
	system(write_log_command.c_str());

	//when linking, file log.txt is generated in source dir,
	//so we have to move it to the output dir
	//copy log file to log dir
	boost::filesystem::copy_file(tempPath + "log.txt", Path + "log\\log.txt", copy_option::overwrite_if_exists);
	//delete log.txt file in source dir
	boost::filesystem::wpath log(tempPath + "log.txt");
	if (boost::filesystem::exists(log))
		boost::filesystem::remove(log);
	else
		throw "file does not exist";

	//when linking, the compiler automatically generates a default exe file a.exe,
	//we have to remove it
	//delete a.exe file in source dir
	boost::filesystem::wpath exe(tempPath + "a.exe");
	if (boost::filesystem::exists(exe))
		boost::filesystem::remove(exe);


	std::string create_object_command = "D: && cd " + tempPath + " && g++ -c ";
	for (int i = 0; i < cppFiles.size(); i++)
	{
		std::string cmd = create_object_command + cppFiles[i] + " -o " + Path + "build\\" + (GetFileNameWithoutExtension(cppFiles[i])) + ".o";
		system(cmd.c_str());
	}
}

std::string GetFileName(std::string str)
{
	int i = str.size() - 1;
	int lastIndex = -1, secondIndex;
	while (str[i])
	{
		if (str[i] == '\"')
			lastIndex = i;
		if (str[i] == '\\' || str[i] == '/')
		{
			secondIndex = i;
			break;
		}
		i--;
	}
	return str.substr(secondIndex + 1, lastIndex - 1 - secondIndex);
}

std::vector<std::string> GetArrayOfFile(boost::filesystem::path path, std::string extension)
{
	directory_iterator it{ path };
	std::vector<std::string> files;
	while (it != directory_iterator{})
	{
		std::ostringstream oss;
		oss << *it;
		std::string fileName = GetFileName(oss.str());
		int i = fileName.size() - 1;
		while (fileName[i] != '.')
			i--;
		if (fileName.substr(i + 1, fileName.size() - 1 - i) == extension)
			files.push_back(fileName);
		*it++;
	}
	return files;
}

std::string GetFileNameWithoutExtension(std::string str)
{
	int i = str.size() - 1;
	while (str[i])
	{
		if (str[i] == '.')
			break;
		i--;
	}
	return str.substr(0, i);
}

std::vector<std::string> GetArrayOfFileAddress(boost::filesystem::path path) //this return the address of file
{
	directory_iterator it{ path };
	std::vector<std::string> files;
	while (it != directory_iterator{})
	{
		std::ostringstream oss;
		oss << *it;
		std::string temp = oss.str();
		std::string fileName = temp.substr(1, temp.size() - 2); //remove double quote from file name 
		files.push_back(fileName);
		*it++;
	}
	return files;
}

std::string LinkObjectFiles(std::string Path)
{
	boost::filesystem::path tempPath = Path + "build\\";
	directory_iterator it{ tempPath };
	std::vector<std::string> oFiles = GetArrayOfFile(tempPath, "o");

	std::string link_command = "D: && cd " + tempPath.string() + " && g++ -o program ";
	for (int i = 0; i < oFiles.size(); i++)
		link_command += " " + oFiles[i];

	system(link_command.c_str());

	return tempPath.string() + "program.exe";
}

void Process(std::string Path)
{
	HANDLE hProcess;
	HANDLE hThread;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	DWORD dwProcessId = 0;
	DWORD dwThreadId = 0;
	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));
	BOOL bCreateProcess = NULL;
	DWORD exitCode;

	std::string path = Path + "build\\program.exe";

	bCreateProcess = CreateProcess(
		NULL,
		const_cast<char*>(path.c_str()),
		NULL,
		NULL,
		FALSE,
		0,
		NULL,
		NULL,
		&si,
		&pi);

	std::this_thread::sleep_for(std::chrono::milliseconds(TIME_LIMIT)); //sleep on this thread, 
																		//while process is executed in another thread

	if (bCreateProcess == FALSE)
		std::cout << "Error " << GetLastError() << std::endl;
	else
	{
		WaitForSingleObject(pi.hProcess, 0);
		// Get the exit code.
		auto result = GetExitCodeProcess(pi.hProcess, &exitCode);
		// Close the handles.
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);

		if (!result)
			throw "Could not get process's exit code";
		else
		{
			ofstream file;
			file.open(Path + "score.txt");

			if (exitCode != 0)  //exit code returns 0 if process terminates before the time_limit does
			{
				std::cout << "exceed time limit" << std::endl;
				if (OpenProcess(PROCESS_ALL_ACCESS, TRUE, pi.dwProcessId) != NULL)
					TerminateProcess(OpenProcess(PROCESS_ALL_ACCESS, TRUE, pi.dwProcessId), 0);

				//mark student's output here
				file << 0;
				//mark student's output here
			}
			else
			{
				//because the output file is generated in the project folder (the scroring system)
				//we have to move it to the student's folder
				boost::filesystem::copy_file(ProjectPath + "output.txt", Path + "output\\output.txt", copy_option::overwrite_if_exists);

				boost::filesystem::wpath output(ProjectPath + "output.txt");
				boost::filesystem::remove(output);

				//mark student's output here
				file << 10;
				//mark student's output here
			}

			file.close();
		}
	}

} //function that need to use // //function

std::string GetExtension(std::string file)
{
	int i = file.size() - 1;
	while (file[i])
	{
		if (file[i] == '.')
			break;
		i--;
	}
	if (i < 0)
		throw "no ext";
	else
		return file.substr(i + 1, file.size() - i - 1);

}

void CreateXMLFile(std::string Path)
{
	using boost::property_tree::ptree;
	ptree pt;
	std::vector<std::string> files = GetArrayOfFileAddress(Path);
	for each (std::string file in files)
	{
		std::string fileName = GetFileName(file);
		if (HasExtension(fileName))
		{
			if (GetExtension(fileName) != "xml")
				pt.put(GetFileNameWithoutExtension(fileName), GetExtension(fileName));
		}
		else
		{
			pt.put(fileName, "");
			WriteXMLUltil(pt, file);
		}
	}

	std::string xmlFileName;
	int i = Path.size() - 1, lastIndex = -1, secondIndex;
	while (Path[i])
	{
		if (Path[i] == '\\' || Path[i] == '/')
		{
			if (lastIndex == -1)
				lastIndex = i;
			else
			{
				secondIndex = i;
				break;
			}
		}
		i--;
	}
	xmlFileName = Path.substr(secondIndex + 1, lastIndex - secondIndex - 2);

	boost::property_tree::write_xml(Path + "\\" + xmlFileName + ".xml",
		pt,
		std::locale(),
		boost::property_tree::xml_writer_make_settings< std::string >(' ', 4));	//write_xml(".xml", pt, std::locale(), boost::property_tree::xml_writer_settings<char>('\t', 1));
}

void WriteXMLUltil(ptree& pt, std::string path)
{
	std::vector<std::string> files = GetArrayOfFileAddress(path);

	if (files.size() == 0)
		return;

	for each (std::string file in files)
	{
		std::string fileName = GetFileName(file);
		if (HasExtension(fileName))
			pt.get_child(GetFileName(path)).put(GetFileNameWithoutExtension(fileName), GetExtension(fileName));
		else
		{
			pt.get_child(GetFileName(path)).put(GetFileNameWithoutExtension(fileName), "");
			WriteXMLUltil(pt, file);
		}
	}
}

bool HasExtension(std::string fileName)
{
	int i = 0;
	while (fileName[i])
	{
		if (fileName[i] == '.')
			return true;
		i++;
	}
	return false;
};



