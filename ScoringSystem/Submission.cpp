#include "Submission.h"
#include "Compiler.h"

StudentSubmission & StudentSubmission::operator=(StudentSubmission & SS)
{
	MSSV = SS.MSSV;
	Time = SS.Time;
	Location = SS.Location;
	return *this;
}

Submission::Submission(std::string Address) {
	pTree.setDir(Address);
	//Check existence of XML file
	if (!exists(boost::filesystem::path(pTree.getDir())))
		throw("XML File does not exist!");
	pTree.CreateDirFromXML();
	MaxSize = DEFAULT_MAX_SUBMISSION;
	size = 0;
}

bool Submission::IsEmpty()
{
	return size == 0;
}

PropertyTree Submission::GetFolderStructure()
{
	return pTree;
}

StudentSubmission * Submission::Scan() {
	using boost::property_tree::ptree;
	//quét folders
	WIN32_FIND_DATA StudentList;
	HANDLE hfind;
	//tạo string dẫn đến folder Submission
	std::string spath = pTree.getDir().substr(0, pTree.getDir().size() - 11);
	spath += "Submission/*";
	int i = 0;
	//Loop đọc các folder mã số sinh viên
	hfind = FindFirstFile(spath.data(), &StudentList);
	do {
		if (StudentList.dwFileAttributes == 16) {
			if (i > 1) {
				boost::property_tree::ptree list = pTree.pt.begin()->second.get_child(StudentList.cFileName); //cây chứa lần nộp CŨ của 1 sinh viên
				std::string submissionTurns = spath.substr(0, spath.size() - 1) + StudentList.cFileName + "/*";
				WIN32_FIND_DATA SubmissionList;
				HANDLE hfind2 = FindFirstFile(submissionTurns.data(), &SubmissionList);
				//đọc các lần nộp của sinh viên
				int g = 0;
				do {
					if (SubmissionList.dwFileAttributes == 16) {
						if (g > 1) {
							ptree::assoc_iterator idt = list.find(SubmissionList.cFileName);
							if ((idt == list.not_found())) { //không có ->update cây và file XMl
															 //1.Update file XML và cây property_tree
								pTree.InsertDir(StudentList.cFileName, SubmissionList.cFileName);
							}
						}
					}
					++g;
				} while (FindNextFile(hfind2, &SubmissionList));
			}
		}
		++i;
	} while (FindNextFile(hfind, &StudentList));
	FindClose(hfind);
	system("pause");
	return nullptr;
}

void Submission::ScoreStudent() {
	if (size == 0) {

		StudentSubmission newStudent;
		newStudent.MSSV = "1000001";
		newStudent.Location = "D:\\ScoringSystem\\ScoringSystem\\ScoringSystem\\Submission\\1000001\\2017-04-21_04~43~34,000\\";
		//newStudent.Time = boost::posix_time::time_from_string("2017-04-21_04~43~34,000");
		//chặn cho array thêm vào lúc này
		{
			//std::lock_guard<std::mutex> ScoreLock(lock);
			//Submit->pop();
			//--size;
		}
		//Update property tree and XML
		//pTree.UpdateDir(newStudent.MSSV, pTree.PosixTimeToFolderName(newStudent.Time));
		/*******************/
		//compiling and scoring student
		CreateObjectFiles(newStudent.Location);
		LinkObjectFiles(newStudent.Location);
		Process(newStudent.Location);
		CreateXMLFile(newStudent.Location);
		/*****************************/

	}
}

void Submission::AddStudents() {
	//1. nhận tín hiệu xem bên C# client có gửi code hok và khi nào tạm ngừng, nếu có thì delay đến khi nhận được tín hiệu
	//2. đọc file xml chứa sanh sách các sinh viên vừa mới nộp bài
	//3. Chờ tín hiệu bên ScoreStudent có hoàn tất chưa, nếu hoàn tất rồi thì bắt đầu thực hiện việc insert file mới nộp vào array Submit
	//nhớ khóa mutex để tránh các hàm khác xâm nhập vào array
	while (true) {
		//std::unique_lock<std::mutex> lk(lock);
		//flag.wait(lk, !IsEmpty());
		//A. Lấy student
		std::ifstream newTurn("/ScoringSystem/NewSubmission.txt");
		std::string TheTurn;
		//lấy 1 dòng trong file nộp mới
		while (std::getline(newTurn, TheTurn)) {
			//A1. Lấy thông tin lần nộp của 1 sinh viên
			StudentSubmission NewOne;
			std::string TimeInString = TheTurn.substr(TheTurn.find('/') + 1, TheTurn.size() - TheTurn.find('/'));
			NewOne.MSSV = TheTurn.substr(0, TheTurn.find('/'));
			NewOne.Location = "/ScoringSystem/Submission/" + NewOne.MSSV + "/" + TimeInString;
			NewOne.Time = boost::posix_time::time_from_string(TimeInString);
			//A2. Cập nhật cây và file XML
			pTree.UpdateDir(NewOne.MSSV, TimeInString);
			//A3. Đưa vào array
			//caution! Nếu array  bị full thì đợi đến khi
			while (size == MaxSize) { //CẦN ĐƯỢC IMPLEMENT!!!!
									  //Thực hiện việc chờ đến khi nào có chỗ thì tiếp tục
									  //lúc này mở khóa cho ScoreStudent tiếp tục được chạy

			}
			//khóa mutex lại

			//Đưa thông tin mới vào array và reheapup
			Submit->push(NewOne);
			++size;
			//trả condition_variable và unlock mutex
		}
		//blah blah somehow
		//B. Thêm vào array Submit

	}
}

PropertyTree Submission::GetPropertyTree()
{
	return pTree;
}
