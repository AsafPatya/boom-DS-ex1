#ifndef TCL_H_
#define TCL_H_

//Struct TCL represents: Time, Course, Lecture used for printing in order most watched lectures 
struct TCL
{
	int time;
	int course;
	int lecture;
	TCL(int time,int course,int lecture):time(time),course(course),lecture(lecture){}
	TCL():time(0),course(0),lecture(0){}
};

// Compares lectures first by time, than courseID, than lectureID
class CompareLecture{
public:
	int operator()(TCL tcl1,TCL tcl2)
	{
		if(tcl1.time!=tcl2.time)
            return tcl1.time-tcl2.time;
        if(tcl1.course!=tcl2.course)
            return tcl2.course-tcl1.course;
        return tcl2.lecture-tcl1.lecture;
	}
};

#endif
