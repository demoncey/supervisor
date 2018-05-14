#ifndef supervisor_h
#define supervisor_h
#include "Arduino.h"
#include "task.h"


#define SERIAL_LOGGER(name, ptr,msg) (Serial)?Serial.println((name)+":"+String((ptr),HEX)+" "+(msg)):0
#define SERIAL_LOGGER1(msg) (Serial)?Serial.println((msg)):0
#define GET_HEX_PTR(ptr) (uint16_t)(ptr)
#define ARGS void *args


//sprintf(buffer, "Task %u flashing %u times.", taskNum, count);
//https://stackoverflow.com/questions/20059673/print-out-value-of-stack-pointer

class Supervisor
{
	public:
		Supervisor(String name);
		void addTask(Task& task);
		void addTask(Task* task);
		void deleteTask(Task& task);
		void execute();
		void suspendAll();
		void resumeAll();
		//first argument always this hidden
		Supervisor& operator +(Task&  task){
			this->addTask(task);
			return *this;
		};
		Supervisor& operator +(Task*  task){
			this->addTask(task);
			return *this;
		};
	private:
		String name;
		//task chain
		Task  *first,*last;	
};




#endif
