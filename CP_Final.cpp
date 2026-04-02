#include <iostream>
#include <fstream>
#include <sstream>      // to break down strings
#include <vector>       // for dynamic arrays
#include <map>          // to store students by their ID
#include <cmath>
#include <iomanip>
#include <algorithm>
#include <numeric>      // for functions like accumulate

using namespace std; 


const int NUM_SUBJECTS = 6;
const string SUBJECT_NAMES[6] = {"CP", "ENA", "DE", "ISL", "I&CP", "MRC"};
const int CREDIT_HOURS[6] = {4, 4, 3, 2, 2, 1};

struct individual_student          // stores all info about ONE student like id, name, marks, attendance, grades and gpa. we'll use it inside a map to handle many students
{                       
    string cmsid;
    string fullname;
    vector<double> allmarks;
    vector<double> attendance;
    vector<string> finalgrades;
    double finalgpa = 0.0;
};


map<string, individual_student> students; // we handle multiple students using this mapping, first we used struct & now mapping


string subject_grade(double mark, double mean, double stddev, double attendance) // this is a simple function that assigns gardes using standard deviation method
{
    if (attendance < 75.0) 
    {
        return "XF";
    }
    if (mark >= mean + 1.0 * stddev) 
    {
        return "A";
    } 
    else if (mark >= mean + 0.5 * stddev) 
    {
        return "B+";
    } 
    else if (mark >= mean) 
    {
        return "B";
    } 
    else if (mark >= mean - 0.5 * stddev)
    {
        return "C+";
    } 
    else if (mark >= mean - 1.0 * stddev) 
    {
        return "C";
    } 
    else if (mark >= mean - 2.0 * stddev)
    {
        return "D";
    } 
    else 
        return "F";
}


double subject_gpa(const string &grade) // another simple function., which converts grades to gpa
{
    if (grade == "A") 
    {
        return 4.0;
    } 
    else if (grade == "B+") 
    {
        return 3.5;
    } 
    else if (grade == "B") 
    {
        return 3.0;
    } 
    else if (grade == "C+") 
    {
        return 2.5;
    } 
    else if (grade == "C") 
    {
        return 2.0;
    } 
    else if (grade == "D") 
    {
        return 1.0;
    } 
    else 
        return 0.0;
}

double mean(const vector<double> &data) //calculation of mean by stroing data through array (using vector technique) and then dividing by size
{
    double sum = 0;
    size_t index = 0;

    if (!data.empty()) 
    {
        do 
        {
            sum += data[index];
            index++;
        } while (index < data.size());
    }

    return data.empty() ? 0.0 : sum / data.size();
}


double standard_deviatiom(const vector<double> &data, double mean) //  standard deviation formula applied using while condition 
{
    if (data.empty()) 
    {
        return 0.0;
    }
    
    double sumOfSquares = 0;
    int dataSize = data.size();  

    int index = 0;
    while (index < dataSize) 
    {
        double difference = data[index] - mean;
        double squaredDifference = pow(difference, 2);
        
        sumOfSquares += squaredDifference;
        index++;
    }
    double variance = sumOfSquares / dataSize;
    return sqrt(variance);
}

void clean(string &val) // a very important function, used for removing all whitespace characters (spaces, tabs, newlines, etc.) from the entire string.
{
    val.erase(remove_if(val.begin(), val.end(), ::isspace), val.end());
} 

/* remove_if rearranges the string so that all non whitespace characters are moved to the front, 
and returns an iterator to the new logical end.
val.erase removes the "garbage" left behind after the remove_if */

string trimOnly(const string& str) // removes leading and trailing whitespace like spaces, tabs, newlines
{
    size_t first = str.find_first_not_of(" \t\n\r"); // Finds the position of the first character that is not a whitespace, tab, newline, or carriage return
    size_t last = str.find_last_not_of(" \t\n\r");   // gives the position where the useful data ends.
    if (first == string::npos || last == string::npos) // it means no valid characters were found, so it returns an empty string.
        return "";
    return str.substr(first, (last - first + 1));
} 

void input_aggregates(const string &filename) // Function to read aggregate student data (ID, Name, Marks) from a CSV file
{
 
    ifstream file(filename);   // Open the CSV file
    if (!file.is_open()) 
    {
        cout << "Error opening aggregates file: " << filename << endl;
        return;
    }

    string line;

    getline(file, line); // used so that the header line is skipped


    while (getline(file, line))    // Read each line of the file with one stduent per line
    {
        stringstream ss(line); // stringstream lets us break the line into smaller parts (like ID, name, marks).
        string id, name, token;

        getline(ss, id, ',');        // ss will be used to pull out values separated by commas.
        getline(ss, name, ',');

        clean(id);      // Remove all whitespace from ID

   
        trimOnly(name);     // Remove leading spaces from name (to preserve inner spaces)

        // Create a new individual_student object
        individual_student s;
        s.cmsid = id;
        s.fullname = name;
        s.allmarks.resize(NUM_SUBJECTS);           // Prepare space for subject marks
        s.attendance.resize(NUM_SUBJECTS, 0.0); // Set initial attendance to 0.0 for all subjects
        s.finalgrades.resize(NUM_SUBJECTS);          // Placeholder for grades. Prepare space to store grades for all subjects.
        s.finalgpa = 0.0;                             // GPA initially 0

        // Read and store marks for each subject
        for (int i = 0; i < NUM_SUBJECTS; i++) 
        {
            if (getline(ss, token, ',')) 
            {
                clean(token); // Remove all whitespace from mark

                try 
                {
                    s.allmarks[i] = stod(token); // Convert string to double
                } 
                catch (...) 
                {
                    s.allmarks[i] = 0.0; // If conversion fails, assign 0
                    cout << "Error parsing mark '" << token << "' for student " << id << " subject " << i << endl;
                }
            }
        }

        // Add the student to the map using ID as the key
        students[s.cmsid] = s;
    }

    // Close the file after reading
    file.close();

    // Print how many students were loaded
    cout << "Read " << students.size() << " students from aggregates file." << endl;
}

void input_attendence(const string &filename) 
{
    ifstream file(filename);
    if (!file.is_open()) 
    {
        cout << "Error opening attendance file: " << filename << endl;
        return;
    }

    string line;
    getline(file, line); // used for skipping header

    while (getline(file, line)) 
    {
        stringstream ss(line);
        string id, name, token; //token stores each subject’s attendance value temporarily
        
        getline(ss, id, ',');
        getline(ss, name, ',');
        
        // Clean ID
        clean(id);
        
        if (students.find(id) == students.end()) //This checks: Is this student ID present in the students map? If not found, that means the student is missing from the aggregates file.
        {
            cout << "Student with ID " << id << " not found in aggregates." << endl;
            continue; //Print a warning, then skip this student’s attendance (don't crash or stop the program).
        }

        individual_student &s = students[id]; //Get a reference to the student object in the map. We use & to avoid copying the object; changes will directly affect the original.

        for (int i = 0; i < NUM_SUBJECTS; ++i) // Read attendance
        {
            if (getline(ss, token, ',')) 
            {
                clean(token);
                try 
                {
                    s.attendance[i] = stod(token); /*Try to convert the token (string) into a double using stod (string to double).
                                                    Store it in the student's attendance array at subject i */
                } 
                catch (...) //If the token is invalid (e.g. empty or non-numeric), set attendance to 0.0.
                {
                    s.attendance[i] = 0.0;
                    cout << "Error parsing attendance for student " << id << " subject " << i << endl;
                }
            }
        }
    }

    file.close();
    cout << "Processed attendance data." << endl;
}


//this is the function that i am going to use to gather all the students' aggregates in a list. 
void everyone_grades() 
{
    int subj=0;
    while (subj<NUM_SUBJECTS) 
    {
        vector<double>subjectMarks; //this is the list which will store each students' aggregate in each subject  
        auto it1=students.begin();
        while(it1!=students.end()) 
        {
            subjectMarks.push_back(it1->second.allmarks[subj]);
            ++it1;
        }
        double mean_val=mean(subjectMarks); //using the mean function made previously for the mean of the subject 
        double stdDev=standard_deviatiom(subjectMarks, mean_val); //using the standard deviation functiom made previously for the standard dev of the subject
        cout<<SUBJECT_NAMES[subj] << "  Mean: " << mean_val << ", Std Dev: " << stdDev << "\n";

        auto it2=students.begin();
        while(it2 != students.end()) 
        {
            individual_student& s=it2->second; 
            s.finalgrades[subj]=subject_grade(s.allmarks[subj], mean_val, stdDev, s.attendance[subj]); //sing the subject_grade function made previously to store the grade for each student in the subject
            ++it2;
        }

        ++subj;//same process will be repeaating for each  and every subject 
    }
}


void overall_gpa() 
{
    map<string, individual_student>::iterator it=students.begin();//here we go thru each of the students from start to finish

    
    while (it!=students.end()) 
    {
        individual_student& s=it->second;  

        double totalPoints=0;
        int totalCredits=0;

        int i=0;
        
        while (i<NUM_SUBJECTS) {
            string grade=s.finalgrades[i]; 
            double gp=subject_gpa(grade); //using the function we just made we convert a letter to a grad, B+ -> 3.5

            totalPoints+=gp * CREDIT_HOURS[i]; //here total points are calculated via this formula
            totalCredits+=CREDIT_HOURS[i];     //same goes for total credits which will add for each subject

            i++;
        }

        if (totalCredits>0)
            s.finalgpa=totalPoints/totalCredits;
        else
            s.finalgpa=0.0; //so that we dont have a divide by zero error if the total credit hours are listed as zerp 

        ++it; 
    }
}



bool compare_students(const pair<string, individual_student>& a,const pair<string,individual_student>& b) 
{
    return a.second.finalgpa > b.second.finalgpa; //this function compares 2 students so that we can output gpa in proper order
}
void individual_report() 
{
    vector<pair<string, individual_student>> sortedStudents(students.begin(), students.end()); //the student map could not be sorted so we created a vector sortedStudents
    sort(sortedStudents.begin(), sortedStudents.end(), compare_students); //sorted using our compare students function(meaning by gpa)
    cout << "\n--- GPA Report ---\n";
    cout <<left<<setw(10) <<"CMS ID"<< setw(25) <<"Name"; //simple formatting of the output   

    for (int i = 0; i < NUM_SUBJECTS; ++i) 
    {
        cout << setw(6) << SUBJECT_NAMES[i];
    }

    cout << setw(6)<<"GPA"<< "\n";

    
    for (int i=0;i<sortedStudents.size(); ++i) 
    {
        pair<string, individual_student> studentEntry=sortedStudents[i];//pair is used here, first vlue is the string CMS id and the second is  the whole student object
        individual_student s = studentEntry.second;
        cout<<left<<setw(10)<<s.cmsid
             <<setw(25)<< s.fullname;
        for (int j = 0; j < NUM_SUBJECTS; ++j)//Print their grades
        {
            cout << setw(6)<<s.finalgrades[j];
        }
        cout<<fixed<<setprecision(2)<<setw(6)<<s.finalgpa<<"\n"; //Print their GPA
    }
}

void main_output_table(const string& cmsId) 
{
    
    if (students.find(cmsId)==students.end()) 
    {
        cout << "\nStudent with CMS ID " << cmsId << " not found.\n"; //if the student does not exist(future proofing our code)
        return;
    }
    const individual_student& s = students[cmsId]; //s will be used like our variable to access cms ID and full nme
    cout <<"\n=== Student Details ===\n";
    cout <<"CMS ID: " <<s.cmsid<< "\n";
    cout <<"Name:   " <<s.fullname<< "\n";
    cout <<"\n";
    cout <<left<<setw(7)<<"Subject"<<setw(10)<<" Marks "<<setw(15)<<"Attendance (%)"<<setw(7)<<"Grade"<<"\n";
    cout<<string(39, '-')<<"\n";
    for (int i=0; i<NUM_SUBJECTS;++i) 
    {
        cout <<left<<setw(7)<<SUBJECT_NAMES[i]<<fixed<<setprecision(2)<<setw(10)<< s.allmarks[i]<< setw(15)<<s.attendance[i]<<setw(7)<<s.finalgrades[i]<<"\n";
    } 
    cout<<"\nCredit Hours: "<<accumulate(CREDIT_HOURS, CREDIT_HOURS+NUM_SUBJECTS,0)<<"\n";
    cout<<"GPA: "<<fixed<<setprecision(2)<<s.finalgpa<<"\n";
}

void student_search() 
{
    string input;
    while (true) 
    {
        cout<<"\n=== Student Lookup Menu ===\n";
        cout<<"Enter a 6-digit CMS ID to view student details (or '0' to quit): ";
        cin>> input;
        input.erase(remove_if(input.begin(),input.end(),::isspace),input.end()); //like trim and clean this is used to remove whitespace
        if (input == "0"){
            cout << "Exiting student lookup menu.\n";
            break;
        }
        // Validate: must be exactly 6 digits
        if (input.length()!= 6||!all_of(input.begin(),input.end(),::isdigit)) {
            cout<<"Invalid CMS ID! Please enter a 6-digit number only.\n";
            continue;
        }
        // Proceed to display student info
        main_output_table(input);
    }
}

int main() 
{
    input_aggregates("Aggregates_All.csv");  
    input_attendence("Attendence_All.csv");  
    everyone_grades();
    overall_gpa();
    individual_report();
    cout <<"\nPress Enter to continue to student lookup:";
    cin.ignore(); 
    student_search();
    return 0;
}
