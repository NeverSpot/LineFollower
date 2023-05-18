// variables for ir
const int ir_pin[8] = {0,1,2,3,4,5,6,7};
int ir[8] = {0, 0, 0, 0, 0,0,0,0};
int air[8] = {0, 0, 0, 0, 0,0,0,0};

// variables for motor
int motorL_pin1 = 8;
int motorL_pin2 = 9;
int motorR_pin1 = 10;
int motorR_pin2 = 11;

// variables for pid
int P,I,D,lastError;
float kp=0.1,ki=1,kd=1;
const int maxspeedL=200,maxspeedR=200;
const int basespeed=155;
int motorspeedL=basespeed,motorspeedR=basespeed;

void setup() {
    Serial.begin(9600);
    for (int i = 0; i < 8; i++) {
        pinMode(ir_pin[i], INPUT);
    }
    pinMode(motorL_pin1, OUTPUT);
    pinMode(motorL_pin2, OUTPUT);
    pinMode(motorR_pin1, OUTPUT);
    pinMode(motorR_pin2, OUTPUT);
};

int read_ir() {
    int v1, v2;
    for (int i = 0; i < 8; i++) {
        v1 += i * ir[i];
        v2 += ir[i] ;
        ir[i] = digitalRead(ir_pin[i]);
        air[i] = analogRead(ir_pin[i]);
    }
    return v1/v2;
}

void loop() {
    pid();

    if (ir[0] == 0 && ir[1] == 0 && ir[2] == 0 && ir[3] == 1 && ir[4] == 1 && ir[5] == 0 && ir[6] == 0 && ir[7] == 0) {
        setspeed(155,155);
    }

    if (ir[0] == 1 && ir[1] == 1 && ir[2] == 1 && ir[3] == 1 && ir[4] == 1 && ir[5] == 0 && ir[6] == 0 && ir[7] == 0){
        setspeed(0,155);
    }
    if (ir[0] == 0 && ir[1] == 0 && ir[2] == 0 && ir[3] == 1 && ir[4] == 1 && ir[5] == 1 && ir[6] == 1 && ir[7] == 1){
        setspeed(155,0);
    }

    if (ir[0] == 0 && ir[1] == 0 && ir[2] == 0 && ir[3] == 0 && ir[4] == 0 && ir[5] == 0 && ir[6] == 0 && ir[7] == 0){
        setspeed(0,0);
    }
};

void setspeed(int lspeed,int rspeed){
    digitalWrite(motorL_pin2, LOW);
    digitalWrite(motorR_pin2, LOW);
    digitalWrite(motorL_pin1, HIGH);
    digitalWrite(motorR_pin1, HIGH);


    analogWrite(motorL_pin1, lspeed);
    analogWrite(motorR_pin1, rspeed);
}
void pid(){
    //to be coded
    int position=read_ir();

    //pid control
    int error = 3500-position;
    P=error;
    I=I+error;
    D = error - lastError;
    lastError = error;

    int motorspeed = kp*P + ki*I + kd*D;
    motorspeedL= basespeed+motorspeed;
    motorspeedR= basespeed-motorspeed;

    if (motorspeedL > maxspeedL) {
        motorspeedL = maxspeedL;
    }
    if (motorspeedR > maxspeedR) {
        motorspeedR = maxspeedR;
    }
    if (motorspeedL < 0) {
        motorspeedL = 0;
    }
    if (motorspeedR < 0){
        motorspeedR = 0;
    }
    setspeed(motorspeedL,motorspeedR);
}
