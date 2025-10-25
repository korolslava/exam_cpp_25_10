#include <iostream>
#include <map>
#include <set>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <nlohmann/json.hpp>
using json = nlohmann::json;
using namespace std;

class Student
{
public:
	map<string, set<map<string, int>>> grades;
	Student() = default;

	void add_grade(const string& subject, const string& assessment, int score)
	{
		auto& subject_set = grades[subject];
		auto it = find_if(subject_set.begin(), subject_set.end(),
			[&assessment](const map<string, int>& m) {
				return m.find(assessment) != m.end();
			});
		if (it != subject_set.end())
		{
			subject_set.erase(it);
		}
		subject_set.insert({ {assessment, score} });
	}

	void avg_grade(const string& subject)
	{
		auto it = grades.find(subject);
		if (it == grades.end())
		{
			cout << "No grades for subject: " << subject << endl;
			return;
		}
		int total = 0;
		int count = 0;
		for (const auto& assessment_map : it->second)
		{
			for (const auto& [assessment, score] : assessment_map)
			{
				total += score;
				count++;
			}
		}
		if (count == 0)
		{
			cout << "No assessments for subject: " << subject << endl;
			return;
		}
		double avg = static_cast<double>(total) / count;
		cout << "Average grade for " << subject << ": " << avg << endl;
	}

	void save_to_file(const string& filename)
	{
		json j;
		for (const auto& [subject, assessments] : grades)
		{
			for (const auto& assessment_map : assessments)
			{
				for (const auto& [assessment, score] : assessment_map)
				{
					j[subject].push_back({ {assessment, score} });
				}
			}
		}
		ofstream file(filename);
		if (file.is_open())
		{
			file << j.dump(4);
			file.close();
		}
		else
		{
			cerr << "Could not open file for writing: " << filename << endl;
		}
	}

	void load_from_file(const string& filename)
	{
		ifstream file(filename);
		if (file.is_open())
		{
			json j;
			file >> j;
			for (auto& [subject, assessments] : j.items())
			{
				for (const auto& assessment_map : assessments)
				{
					for (const auto& [assessment, score] : assessment_map.items())
					{
						add_grade(subject, assessment, score);
					}
				}
			}
			file.close();
		}
		else
		{
			cerr << "Could not open file for reading: " << filename << endl;
		}
	}

	void save_to_json(json& j) const
	{
		for (const auto& [subject, assessments] : grades)
		{
			for (const auto& assessment_map : assessments)
			{
				for (const auto& [assessment, score] : assessment_map)
				{
					j[subject].push_back({ {assessment, score} });
				}
			}
		}
	}

	void load_from_json(const json& j)
	{
		for (auto& [subject, assessments] : j.items())
		{
			for (const auto& assessment_map : assessments)
			{
				for (const auto& [assessment, score] : assessment_map.items())
				{
					add_grade(subject, assessment, score);
				}
			}
		}
	}

	void calculate_three_best_students(const vector<Student>& students)
	{
		multimap<double, const Student*, greater<double>> avg_map;
		for (const auto& student : students)
		{
			double total = 0;
			int count = 0;
			for (const auto& [subject, assessments] : student.grades)
			{
				for (const auto& assessment_map : assessments)
				{
					for (const auto& [assessment, score] : assessment_map)
					{
						total += score;
						count++;
					}
				}
			}
			if (count > 0)
			{
				double avg = total / count;
				avg_map.insert({ avg, &student });
			}
		}
		int rank = 1;
		for (const auto& [avg, student_ptr] : avg_map)
		{
			if (rank > 3) break;
			cout << "Rank " << rank << ": Average Grade = " << avg << endl;
			rank++;
		}
	}

	void save_honor_students(const vector<Student>& students, const string& filename)
	{
		json j;
		for (const auto& student : students)
		{
			double total = 0;
			int count = 0;
			for (const auto& [subject, assessments] : student.grades)
			{
				for (const auto& assessment_map : assessments)
				{
					for (const auto& [assessment, score] : assessment_map)
					{
						total += score;
						count++;
					}
				}
			}
			if (count > 0)
			{
				double avg = total / count;
				if (avg > 90)
				{
					json student_json;
					student.save_to_json(student_json);
					j.push_back(student_json);
				}
			}
		}
		ofstream file(filename);
		if (file.is_open())
		{
			file << j.dump(4);
			file.close();
		}
		else
		{
			cerr << "Could not open file for writing: " << filename << endl;
		}
	}
};


int main()
{
	int choice;
	Student student;
	do
	{
		cout << "Menu:\n";
		cout << "1. Add Grade\n";
		cout << "2. Average Grade\n";
		cout << "3. Save to File\n";
		cout << "4. Load from File\n";
		cout << "5. Top 3 Students\n";
		cout << "6. Show honor students\n";
		cout << "0. Exit\n";
		cout << "Enter your choice: ";
		cin >> choice;
		switch (choice)
		{
		case 1:
		{
			string subject, assessment;
			int score;
			cout << "Enter subject: ";
			cin >> subject;
			cout << "Enter assessment type: ";
			cin >> assessment;
			cout << "Enter score: ";
			cin >> score;
			student.add_grade(subject, assessment, score);
			break;
		}
		case 2:
		{
			string subject;
			cout << "Enter subject: ";
			cin >> subject;
			student.avg_grade(subject);
			break;
		}
		case 3:
		{
			string filename = "file.json";
			cout << "Saving to default file: " << filename << endl;
			student.save_to_file(filename);
			break;
		}
		case 4:
		{
			string filename = "file.json";
			cout << "Loading from default file: " << filename << endl;
			student.load_from_file(filename);
			break;
		}
		case 5:
		{
			int n;
			cout << "Enter number of students: ";
			cin >> n;
			vector<Student> students(n);
			for (int i = 0; i < n; ++i)
			{
				string filename = "file.json";
				cout << "Loading student " << (i + 1) << " from default file: " << filename << endl;
				students[i].load_from_file(filename);
			}
			student.calculate_three_best_students(students);
			break;
		}
		case 6:
		{
			int n;
			cout << "Enter number of students: ";
			cin >> n;
			vector<Student> students(n);
			for (int i = 0; i < n; ++i)
			{
				string filename = "file.json";
				cout << "Loading student " << (i + 1) << " from default file: " << filename << endl;
				students[i].load_from_file(filename);
			}
			string honor_filename = "file2.json";
			cout << "Saving honor students to default file: " << honor_filename << endl;
			student.save_honor_students(students, honor_filename);
			break;
		}
		case 0:
			cout << "Exiting...\n";
			break;
		default:
			cout << "Invalid choice. Please try again.\n";
			break;
		}
	} while (choice != 0);

	return 0;
}