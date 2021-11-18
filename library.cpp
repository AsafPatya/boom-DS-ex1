#include "library.h"
#include "CoursesManager.h"
#include <exception>

//Library checks if the input is valid and runs the correct function from CourseManager

void *Init() {
    try{
        CoursesManager *DS = new CoursesManager ();
        return (void*)DS;
    }catch(int e){
        return NULL;
    }
}

StatusType AddCourse(void *DS, int courseID, int numOfClasses) {
    if(DS==NULL||courseID<=0||numOfClasses<=0)
        return INVALID_INPUT;
    try{
        if (!((CoursesManager *)DS)-> AddCourse(courseID, numOfClasses))
            return FAILURE;
    }catch(int e){
        return ALLOCATION_ERROR;
    }
    return SUCCESS;
}

StatusType RemoveCourse(void *DS, int courseID){
    try{
        if(DS==NULL||courseID<=0)
            return INVALID_INPUT;
        if (!((CoursesManager *)DS)-> RemoveCourse(courseID))
            return FAILURE;
        return SUCCESS;
    }catch(int e){
        return ALLOCATION_ERROR;
    }
}

StatusType WatchClass(void *DS, int courseID, int classID, int time){
    try{
        if(DS==NULL||courseID<=0||classID<0||time<=0)
            return INVALID_INPUT;
        bool lectureExists = true;
        bool* lectureExistsP =  &lectureExists;
        bool success = ((CoursesManager *)DS)-> WatchClass(courseID,classID,time,lectureExistsP);
        if(!(lectureExists))
            return INVALID_INPUT;
        if(!success)
            return FAILURE;
        return SUCCESS;
    }catch(int e){
        return ALLOCATION_ERROR;
    }
}

StatusType TimeViewed(void *DS, int courseID, int classID, int *timeViewed){
    if(DS==NULL||courseID<=0||classID<0)
        return INVALID_INPUT;
    try{
        bool lectureExists = true;
        bool* lectureExistsP = &lectureExists;
        bool success = ((CoursesManager *)DS)-> TimeViewed(courseID,classID,timeViewed,lectureExistsP);
        if(!lectureExists)
            return INVALID_INPUT;
        if(!success)
            return FAILURE;
    }catch(int e){
        return ALLOCATION_ERROR;
    }
    return SUCCESS;
}

StatusType GetMostViewedClasses(void *DS, int numOfClasses, int *courses, int *classes){
    try{
        if(DS==NULL||numOfClasses<=0)
            return INVALID_INPUT;
        if (!((CoursesManager *)DS)-> GetMostViewedClasses(numOfClasses,courses,classes))
            return FAILURE;
        return SUCCESS;
    }catch(int e){
        return ALLOCATION_ERROR;
    }
}

void Quit(void** DS){
    delete (*(CoursesManager **)DS);
    *DS = NULL;
}