#pragma once
#include "PropertyTree.h"
#include "Submission.h"
#include <boost/regex.hpp>

using namespace boost::filesystem;
using boost::property_tree::ptree;

#define TIME_LIMIT 3000 //3 seconds
const std::string ProjectPath = "D:\\ScoringSystem\\ScoringSystem\\";

void CreateObjectFiles(std::string);
std::string GetFileName(std::string str);
std::vector<std::string> GetArrayOfFile(boost::filesystem::path path, std::string extension);
std::string GetFileNameWithoutExtension(std::string str);
std::vector<std::string> GetArrayOfFileAddress(boost::filesystem::path path); //this return the address of file
std::string LinkObjectFiles(std::string);
void Process(std::string);
std::string GetExtension(std::string file);
void CreateXMLFile(std::string);
void WriteXMLUltil(ptree& pt, std::string path);
bool HasExtension(std::string fileName);

