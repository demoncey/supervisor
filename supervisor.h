#ifndef supervisor_h
#define supervisor_h
#include "Arduino.h"
#include <SoftwareSerial.h> 
#include "task.h"


#define MAX_POOL 10

class Supervisor
{
	//friend class Task;
	public:
		Supervisor(String name);
		void addTask(Task& task);
		void deleteTask(Task& task);
		void execute();
		void suspendAll();
		void resumeAll();
	private:
		String name;
		void is_com(String msg);
		//task chain
		Task  *first,*last;
		
		
};




#endif
