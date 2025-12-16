#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cctype>
#include <string>

// Structure to hold course information
struct Course {
    std::string id;                      // e.g., "CS300"
    std::string title;                   // e.g., "Intro to Algorithms"
    std::vector<std::string> prerequisites; // e.g., {"CS200", "MATH120"}
};

// Binary Search Tree to store and manage Course objects
class CourseBST {
private:
    struct Node {
        Course course;
        Node* left;
        Node* right;
        Node(const Course& c) : course(c), left(nullptr), right(nullptr) {}
    };

    Node* root;

    // Recursively free all nodes
    void destroy(Node* node) {
        if (node != nullptr) {
            destroy(node->left);
            destroy(node->right);
            delete node;
        }
    }

    // Recursive insert helper
    void insert(Node*& node, const Course& course) {
        if (node == nullptr) {
            node = new Node(course);
            return;
        }
        if (course.id < node->course.id) {
            insert(node->left, course);
        } else if (course.id > node->course.id) {
            insert(node->right, course);
        } else {
            // If the course already exists, overwrite the data
            node->course = course;
        }
    }

    // Recursive in-order traversal (Left, Root, Right)
    void inOrder(Node* node) const {
        if (node != nullptr) {
            inOrder(node->left);
            std::cout << node->course.id << ", " << node->course.title << std::endl;
            inOrder(node->right);
        }
    }

    // Recursive search helper
    Course* search(Node* node, const std::string& id) const {
        if (node == nullptr) {
            return nullptr;
        }
        if (id == node->course.id) {
            return &(node->course);
        } else if (id < node->course.id) {
            return search(node->left, id);
        } else {
            return search(node->right, id);
        }
    }

public:
    CourseBST() : root(nullptr) {}

    ~CourseBST() {
        destroy(root);
    }

    // Remove all courses from the tree
    void Clear() {
        destroy(root);
        root = nullptr;
    }

    // Public insert wrapper
    void Insert(const Course& course) {
        insert(root, course);
    }

    // Public search wrapper
    Course* Search(const std::string& id) const {
        return search(root, id);
    }

    // Print all courses in alphanumeric order
    void PrintAllCourses() const {
        if (root == nullptr) {
            std::cout << "No courses loaded.\n";
            return;
        }
        inOrder(root);
    }

    bool IsEmpty() const {
        return root == nullptr;
    }
};

// Trim leading/trailing whitespace from a string
std::string trim(const std::string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    size_t end = s.find_last_not_of(" \t\r\n");
    if (start == std::string::npos) {
        return "";
    }
    return s.substr(start, end - start + 1);
}

// Convert a string to uppercase (for consistent course IDs)
std::string toUpper(const std::string& s) {
    std::string result = s;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return std::toupper(c); });
    return result;
}

// Load courses from a CSV file into the BST
// Expected format per line:
//   COURSE_ID,COURSE_TITLE,PREREQ_1,PREREQ_2,...
// Example:
//   CS200,Intro to CS
//   CS300,Data Structures,CS200
bool loadCoursesFromFile(const std::string& filename, CourseBST& bst) {
    std::ifstream inFile(filename);
    if (!inFile.is_open()) {
        std::cout << "ERROR: Could not open file '" << filename << "'.\n";
        return false;
    }

    bst.Clear();

    std::string line;
    while (std::getline(inFile, line)) {
        if (line.empty()) {
            continue; // skip blank lines
        }

        std::stringstream ss(line);
        std::string token;
        std::vector<std::string> tokens;

        // Split line on commas
        while (std::getline(ss, token, ',')) {
            tokens.push_back(trim(token));
        }

        if (tokens.size() < 2) {
            std::cout << "WARNING: Skipping malformed line: " << line << std::endl;
            continue;
        }

        Course c;
        c.id = toUpper(tokens[0]);
        c.title = tokens[1];

        // Any additional fields are prerequisites
        for (size_t i = 2; i < tokens.size(); ++i) {
            if (!tokens[i].empty()) {
                c.prerequisites.push_back(toUpper(tokens[i]));
            }
        }

        bst.Insert(c);
    }

    std::cout << "Courses successfully loaded from '" << filename << "'.\n";
    return true;
}

// Prompt for a course ID and print its title and prerequisites
void printCourseInfo(const CourseBST& bst) {
    std::cout << "Enter course number (e.g., CS300): ";
    std::string courseId;
    std::cin >> courseId;

    courseId = toUpper(courseId);

    Course* course = bst.Search(courseId);
    if (course == nullptr) {
        std::cout << "Course " << courseId << " not found.\n";
        return;
    }

    // Print the main course
    std::cout << course->id << ", " << course->title << std::endl;

    // Print prerequisites with titles
    if (course->prerequisites.empty()) {
        std::cout << "Prerequisites: None\n";
    } else {
        std::cout << "Prerequisites:\n";
        for (const std::string& preId : course->prerequisites) {
            Course* preCourse = bst.Search(preId);
            if (preCourse != nullptr) {
                std::cout << "  " << preCourse->id << ", " << preCourse->title << std::endl;
            } else {
                // In case the prereq code exists in file but not found for some reason
                std::cout << "  " << preId << " (course not found in catalog)\n";
            }
        }
    }
}

int main() {
    CourseBST bst;
    bool dataLoaded = false;

    int choice = 0;
    while (choice != 9) {
        std::cout << "\n===== Course Planner Menu =====\n";
        std::cout << "  1. Load Data Structure\n";
        std::cout << "  2. Print Course List\n";
        std::cout << "  3. Print Course Information\n";
        std::cout << "  9. Exit\n";
        std::cout << "Enter your choice: ";

        // Validate menu input
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "ERROR: Please enter a valid number.\n";
            continue;
        }

        switch (choice) {
        case 1: {
            std::cout << "Enter the file name to load: ";
            std::string filename;
            std::cin >> filename;
            if (loadCoursesFromFile(filename, bst)) {
                dataLoaded = true;
            } else {
                dataLoaded = false;
            }
            break;
        }
        case 2:
            if (!dataLoaded || bst.IsEmpty()) {
                std::cout << "Please load the data structure first (option 1).\n";
            } else {
                std::cout << "\nHere is a sample schedule:\n";
                bst.PrintAllCourses();
            }
            break;
        case 3:
            if (!dataLoaded || bst.IsEmpty()) {
                std::cout << "Please load the data structure first (option 1).\n";
            } else {
                printCourseInfo(bst);
            }
            break;
        case 9:
            std::cout << "Thank you for using the course planner.\n";
            break;
        default:
            std::cout << "ERROR: Invalid choice. Please select 1, 2, 3, or 9.\n";
            break;
        }
    }

    return 0;
}
