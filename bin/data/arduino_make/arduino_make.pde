#include <Servo.h>
#include "claws.h"




void setup(){
	rightClaw.setup();
	

}

void loop(){
	rightClaw.start();
	

	for(int i=0; i<4; i++){
			rightClaw.manual("OPEN");
			delay(1 *1000);
			rightClaw.manual("CLOSE");
			delay(1 *1000);
	}
	rightClaw.end();
	

}
