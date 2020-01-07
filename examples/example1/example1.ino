#include <task.h>
#include <supervisor.h>
void send_msg(void) ;

Supervisor supervisor("example1 supervisor");
Task task1(send_msg);

void setup() {
  Serial.begin(9600);
    while (!Serial) {
    ; 
  }
  supervisor.addTask(task1.setPriority(P_HIGH));
}

void loop() {
  // put your main code here, to run repeatedly:
    supervisor.execute();
}


void send_msg(void*) {
  int id = random(1,65536);
  Serial.println("Heartbeat msg: "+String(id,HEX));
}
