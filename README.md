This C++ program is a Student Grading and Management System that processes academic data from CSV files using a relative grading model.

Uses a `struct` and `std::map` to store student IDs, names, marks, and attendance for 6 core subjects.
 
Assigns letter grades based on the class **Mean** and **Standard Deviation** rather than fixed percentages.

Automatically assigns an **"XF"** grade if a student's attendance falls below 75%.

Converts letter grades to points (e.g., $A = 4.0$) and calculates a weighted GPA based on specific credit hours.

Includes functions to "sanitize" input by removing extra spaces or tabs from the CSV data.

Generates a full GPA report sorted from highest to lowest.

Provides an interactive menu to look up detailed "report cards" by entering a 6-digit CMS ID.
