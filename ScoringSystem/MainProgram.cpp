#include "Submission.h"
#include "Compiler.h"

int main() {
	Submission test("D:/ScoringSystem/ScoringSystem/ScoringSystem/Student.XML");
	//std::cout << PosixTimeToFolderName(boost::posix_time::microsec_clock::local_time());  current time
	//std::thread ADD(&Submission::AddStudents,&test);
	//std::thread SCORE(&Submission::ScoreStudent,&test);
	////ADD.join();
	//SCORE.join();
	test.ScoreStudent();
}