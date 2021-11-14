#ifndef COURSE_H_
#define COURSE_H_

#include "TCL.h"
#include "list.h"
#include "AVLTree.h"

//Struct zeroViews used for the list containing only lectures with zero views
struct zeroViews
{
	int course;
	int lecture;
    zeroViews():course(0),lecture(0){}
	zeroViews(int course,int lecture):course(course),lecture(lecture){}
};

//Struct lecture used as an array in Course to find each lecture's time in O(1)
struct lecture
{
	LinkedList<zeroViews>::Node* lectureP;
	TCL tclP;
    lecture():lectureP(0),tclP(){}
};

//Struct Course implements a course with a pointer to his list of zeroViews
struct Course
{
    int courseID;
    int numOfLectures;
    lecture* lectures;
    LinkedList<LinkedList<zeroViews> >::Node* zeroViewsListPointer;
    Course(int id,int numOfLectures);
    Course():courseID(0),numOfLectures(0),lectures(0),zeroViewsListPointer(0){}
    Course(const Course& course);
    Course& operator=(const Course& course);
    ~Course();
};

//Class CourseManager manages all the data structure we need
class CoursesManager{
public:
    //Compares courses by thier ID
    class CompareCourse{
        public:
	    int operator()(Course c1,Course c2){
            return c1.courseID-c2.courseID;
        }
        int operator()(int courseID,Course c){
            return courseID-c.courseID;
        }
    };

    LinkedList<LinkedList<zeroViews> > zeroViewsLectures; //A list of lists for each course of lectures with zero views 

    AVLTree<Course,CompareCourse> courseTree; //AVLTree containing all the courses

    AVLTree<TCL,CompareLecture> TCLTree; //AVLTree containing all the lectures with more than zero views

    int totalLectures; //Sum of all the lectures in all courses

    CoursesManager();

    ~CoursesManager();

    bool AddCourse(int courseID,int numOfLectures);

    bool RemoveCourse(int courseID);
    
    bool WatchClass(int courseID,int lectureID,int time,bool* lectureExists);

    bool TimeViewed(int courseID,int lectureID,int* timeViewed,bool* lectureExists);

    bool GetMostViewedClasses(int numOfClasses, int *courses, int *classes);
};
#endif