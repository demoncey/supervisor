#define WITH_BUILDER 1
#include <task.h>
#include <supervisor.h>
void send_msg_low(void*);
void send_msg_high(void*);
void send_msg_once(void*);

Supervisor supervisor("example1 supervisor");
Task task1("task1", send_msg_high);
Task task2("task2", send_msg_high);
Task task3("task3", send_msg_low);

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ;
  }
  supervisor.addTask(task1.setPriority(P_HIGH));
  supervisor.addTask(task2.setPriority(P_HIGH));
  supervisor.addTask(task3.setPriority(P_LOW));
}

void loop() {
  // put your main code here, to run repeatedly:
  supervisor.execute();
  delay(2000);
}


void send_msg_low(void*) {
  int id = random(1, 65536);
  Task * temp = new Task("TEMPORARY TASK",send_msg_once);
  supervisor.addTask(temp->setPriority(P_HIGH).makeOnce());
  Serial.println("XXXXXXXXXXXXXXXX LOW : " + String(id, HEX));
  
}


void send_msg_high(void*) {
  int id = random(1, 65536);
  Serial.println("YYYYYYYYYYYYYYY  HIGH : " + String(id, HEX));
}



void send_msg_once(void*) {
  Serial.println("DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD");
  Serial.println("DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD");
  Serial.println("DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD");
}
