#include "CoursesManager.h"
#include <assert.h>

//Builds a Course
Course::Course(int id,int numOfLectures):courseID(id),numOfLectures(numOfLectures),lectures(new lecture[numOfLectures]),zeroViewsListPointer(0){}

//Copy constructor for Course
Course::Course(const Course& course):courseID(course.courseID),numOfLectures(course.numOfLectures),lectures(new lecture[numOfLectures]),zeroViewsListPointer(course.zeroViewsListPointer){
    for (int i = 0; i<numOfLectures;i++)
        lectures[i] = course.lectures[i];
}

//Operator = for Course
Course& Course::operator=(const Course& course) {
    if (this == &course)
        return *this;
    delete[] lectures;
    courseID = course.courseID;
    numOfLectures = course.numOfLectures;
    zeroViewsListPointer = course.zeroViewsListPointer;
    lectures = new lecture[numOfLectures];
    for (int i = 0; i<numOfLectures;i++)
        lectures[i] = course.lectures[i];
    return *this;
}

//Deletes a course
Course::~Course(){
    delete[] lectures;
}

//Builds a CoursesManager
CoursesManager::CoursesManager():zeroViewsLectures(),courseTree(CompareCourse()),TCLTree(CompareLecture()),totalLectures(0){}

//Deletes CoursesManager
CoursesManager::~CoursesManager(){}

//Removes a course by removing all his lectures from both zeroViewsLectures and TCLTree and the course itself from courseTree
bool CoursesManager::RemoveCourse(int courseID){
    if(courseTree.head==0)
        return false;
    AVLTree<Course,CompareCourse>::Node* newNode; 
    if(!courseTree.findNode(courseTree.head,courseID,&newNode))
        return false;
    for (int i=0;i<newNode->info.numOfLectures;i++){
        if(newNode->info.lectures[i].tclP.time!=0)
            TCLTree.removeNode(newNode->info.lectures[i].tclP);
    }
    totalLectures -= newNode->info.numOfLectures;
    zeroViewsLectures.deleteNode(newNode->info.zeroViewsListPointer);
    courseTree.removeNode<int>(courseID);
    return true;
}

//Adds a course by adding all his lectures to zeroViewsLectures and the course itself to courseTree
bool CoursesManager::AddCourse(int courseID,int numOfLectures){
    Course newCourse (courseID,numOfLectures);
    if (!courseTree.insertNode(newCourse))
        return false;
    LinkedList<zeroViews> zeroViewsList (zeroViews(courseID,0));
    for(int i =1; i< numOfLectures; i++)
        zeroViewsList.insertNext(zeroViews(courseID,i));
    AVLTree<Course,CompareCourse>::Node* newNode; 
    courseTree.findNode(courseTree.head,newCourse,&newNode);
    if(newNode->getNextSmaller()==NULL)
        newNode->info.zeroViewsListPointer = zeroViewsLectures.insertNodeRightTo(zeroViewsList,NULL);
    else
        newNode->info.zeroViewsListPointer = zeroViewsLectures.insertNodeRightTo(zeroViewsList,newNode->getNextSmaller()->info.zeroViewsListPointer);
    totalLectures+=numOfLectures;

    LinkedList<zeroViews>::Node* it = newNode->info.zeroViewsListPointer->info.first;
    for (int i = 0;i<numOfLectures;i++){
        newNode->info.lectures[i].lectureP = it;
        it = it->right;
    }
    return true;
}

//Adds time to the requested lecture and adds the lecture to the TCLtree and removes from zeroViewsLectures if still exists there
bool CoursesManager::WatchClass(int courseID,int lectureID,int time,bool* lectureExists){
    if(courseTree.head==0)
        return false;
    AVLTree<Course,CompareCourse>::Node* newNode; 
    if(!courseTree.findNode(courseTree.head,courseID,&newNode))
        return false;
    if(newNode->info.numOfLectures<=lectureID){
        *lectureExists = false;
        return false;
    }
    if(newNode->info.lectures[lectureID].lectureP!=NULL){
        newNode->info.zeroViewsListPointer->info.deleteNode(newNode->info.lectures[lectureID].lectureP);
        newNode->info.lectures[lectureID].lectureP=NULL;
    }
    else{
        time += newNode->info.lectures[lectureID].tclP.time;
        TCLTree.removeNode(newNode->info.lectures[lectureID].tclP);
    }
    TCL newTCL (time,courseID,lectureID);
    TCLTree.insertNode(newTCL);
    newNode->info.lectures[lectureID].tclP = newTCL;
    return true;
}

//Returns the time of a lecture by pointer
bool CoursesManager::TimeViewed(int courseID,int lectureID,int* timeViewed,bool* lectureExists){
    if(courseTree.head==0)
        return false;
    AVLTree<Course,CompareCourse>::Node* newNode; 
    if(!courseTree.findNode(courseTree.head,courseID,&newNode))
        return false;
    if(newNode->info.numOfLectures<=lectureID){
        *lectureExists = false;
        return false;
    }
    if(newNode->info.lectures[lectureID].lectureP!=0)
        *timeViewed = 0;
    else
        *timeViewed = newNode->info.lectures[lectureID].tclP.time;
    return true;
}

//Returns most viewed lectures first from the TCLtree and if not enough from zeroViewsLectures by pointer
bool CoursesManager::GetMostViewedClasses(int numOfClasses, int *courses, int *classes){
    if(numOfClasses>totalLectures)
        return false;
    int left;
    TCL* bestCourses = new TCL[numOfClasses];
    left = TCLTree.getKLargest(numOfClasses,bestCourses);
    if(left!=0){
        LinkedList<LinkedList<zeroViews> >::Node* courseIterator = zeroViewsLectures.first;
        LinkedList<zeroViews>::Node* iterator;
        while(left>0&&courseIterator!=NULL){
            iterator = courseIterator->info.first;
            while(iterator!=NULL&&left>0){
                TCL temp (0,iterator->info.course,iterator->info.lecture);
                bestCourses[numOfClasses-left] = temp;
                left -= 1;
                iterator = iterator->right;
            }
            courseIterator = courseIterator->right;
        }
    }
    for (int i=0;i<numOfClasses;i++){
        courses[i]=bestCourses[i].course;
        classes[i]=bestCourses[i].lecture;
    }
    delete[] bestCourses;
    return true;
}
