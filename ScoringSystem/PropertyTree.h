#pragma once
#pragma once
#include <iostream>
#include <string>
//Used for creating date and time object
#include "boost\date_time\posix_time\posix_time.hpp"
//The two library below is used for reading and parsing XML file
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>
//Used for create, delete...directories
#include <boost/filesystem.hpp>

class PropertyTree {
	friend class Submission;
	//Main tree
	boost::property_tree::ptree pt;
	//Directory of XML file
	std::string Directory;
	friend int NoOfChild(boost::property_tree::ptree&);
	void _CreateDir(std::string, boost::property_tree::ptree const&, bool);
public:
	void setDir(std::string); //Xác định vị trí file XML chính của folder ScoringSystem
	std::string getDir(); //Trả về vị trí file XML chính của folder ScoringSystem
	void CreateDirFromXML(); //Tạo folder Submission và Testcase từ file XML
	void InsertDir(std::string, std::string);//Insert directory and update XML file, bao gồm MSSV và date time dưới dạng string
	void UpdateDir(std::string, std::string); //update propertyTree and XML file
	void DeleteDir(std::string, std::string);//delete directory and update XML file, bao gồm MSSV và date time dưới dạng string
	void InsertStudent(std::string);
	void DeleteStudent(std::string);
	std::string PosixTimeToFolderName(boost::posix_time::ptime);
};

