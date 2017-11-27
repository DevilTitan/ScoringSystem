#pragma once
#include "PropertyTree.h"
#include <Windows.h>
#include <vector>
//used for calculating time and multithreading
#include <chrono>
#include <mutex>
#include <thread>
#include <queue>
//for compiling data

#define DEFAULT_MAX_SUBMISSION 10000

//struct 1 lần nộp bài của 1 sinh viên bất kỳ
struct StudentSubmission {
	std::string MSSV; //Student's ID
	boost::posix_time::ptime Time; //get from Location
	std::string Location;// current address of file group in computer (date time format)
	StudentSubmission& operator=(StudentSubmission& SS);
};

class Submission {
	//an array of students, each contain a list of file group student submitted
	std::queue<StudentSubmission> *Submit;
	//current property tree
	PropertyTree pTree;
	//current size of array above
	int size;
	//maximum size of heap
	int MaxSize;
	//mutex for thread blocking
	std::mutex lock;
	//condition variable for waiting between AddStudents and ScoreStudents
	std::condition_variable flag;
public:
	//constructor
	Submission(std::string Address);
	//public methods
	bool IsEmpty();
	PropertyTree GetFolderStructure();
	StudentSubmission* Scan(); //scan new submissions from student and update property tree
	void ScoreStudent();
	void AddStudents();
	//getpTree
	PropertyTree GetPropertyTree();
};

//functions for compiling
