#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE
#include <DabbleESP32.h>
// Define os pinos para os motores
const int rightMotorPin1 = 13;
const int rightMotorPin2 = 12;
const int leftMotorPin1 = 27;
const int leftMotorPin2 = 26;
const int leftPWM = 18;
const int rightPWM = 19;
const int PWM_FREQ = 500;     // Recall that Arduino Uno is ~490 Hz. Official ESP32 example uses 5,000Hz
const int PWM_RESOLUTION = 8; // We'll use same resolution as Uno (8 bits, 0-255) but ESP32 can go up to 16 bits
 
static int app_cpu = 0;
int rightMotorSpeed = 0;
int leftMotorSpeed = 0;
int pwmLeft = 0;
int pwmRight = 0;
 
int control1 = false;
int control2 = false;
 
void task1(void *argp) {
  while(1){
    if(control1 == true){
       Dabble.processInput();
       if (GamePad.isDownPressed()) {
        control2 = true;
       }
    }
    /*
     if sensor ativa{
      rotateMotor(0, 0);
     */
    if(control2 == true && control1 == true){
      rotateMotor(0, 0);
      control1 = false;
    }
  }
}
 
void rotateMotor(int rightMotorSpeed, int leftMotorSpeed)
{
  // Motor direito
  if (rightMotorSpeed < 0) {
    digitalWrite(rightMotorPin1, LOW);
    digitalWrite(rightMotorPin2, HIGH);
  } else if (rightMotorSpeed > 0) {
    digitalWrite(rightMotorPin1, HIGH);
    digitalWrite(rightMotorPin2, LOW);
  } else {
    digitalWrite(rightMotorPin1, LOW);
    digitalWrite(rightMotorPin2, LOW);
  }
  // Motor esquerdo
  if (leftMotorSpeed < 0) {
    digitalWrite(leftMotorPin1, LOW);
    digitalWrite(leftMotorPin2, HIGH);
  } else if (leftMotorSpeed > 0) {
    digitalWrite(leftMotorPin1, HIGH);
    digitalWrite(leftMotorPin2, LOW);
  } else {
    digitalWrite(leftMotorPin1, LOW);
    digitalWrite(leftMotorPin2, LOW);
  }
}
void setup()
{
  // Inicializa os pinos dos motores como saídas
  pinMode(rightMotorPin1, OUTPUT);
  pinMode(rightMotorPin2, OUTPUT);
  pinMode(leftMotorPin1, OUTPUT);
  pinMode(leftMotorPin2, OUTPUT);
  ledcSetup(0, PWM_FREQ, PWM_RESOLUTION);
  ledcSetup(1, PWM_FREQ, PWM_RESOLUTION);
  ledcAttachPin(leftPWM, 0);
  ledcAttachPin(rightPWM, 1);
  // Inicializa a comunicação Bluetooth
  Dabble.begin("OdeioProjetointegrador");
 
  BaseType_t rc;
  unsigned priority = 0;
  TaskHandle_t h1;
 
  app_cpu = xPortGetCoreID();
 
  delay(2000); // Allow USB init time
 
  vTaskPrioritySet(nullptr,3);
  priority = uxTaskPriorityGet(nullptr);
  assert(priority == 3);
 
  printf("\ntaskcreate.ino:\n");
  printf("loopTask priority is %u.\n",
    priority);
 
  rc = xTaskCreatePinnedToCore(
    task1,
    "task1",
    2000,     // Stack size
    nullptr,
    2,        // Priority
&h1,      // Task handle
    app_cpu   // CPU
  );
  assert(rc == pdPASS);
  // delay(1);
  printf("Task1 created.\n");
 
  vTaskPrioritySet(h1,3);
}
void HalfMoon(){
  pwmLeft = 255;
  pwmRight = 255;
  ledcWrite(0, pwmLeft);
  ledcWrite(1, pwmRight);
  control1 = true;
  rightMotorSpeed = 255;
  leftMotorSpeed = -255;
  if(control2 == false){
  rotateMotor(rightMotorSpeed, leftMotorSpeed);
  }
  delay(100);
  rightMotorSpeed = 255;
  leftMotorSpeed = 255;
  if(control2 == false){
  rotateMotor(rightMotorSpeed, leftMotorSpeed);
  }
  delay(400);
  rightMotorSpeed = -255;
  leftMotorSpeed = 255;
  if(control2 == false){
  rotateMotor(rightMotorSpeed, leftMotorSpeed);
  }
  delay(350);
  rightMotorSpeed = 255;
  leftMotorSpeed = 255;
  if(control2 == false){
  rotateMotor(rightMotorSpeed, leftMotorSpeed);
  }
  delay(400);
  rightMotorSpeed = 0;
  leftMotorSpeed = 0;
  rotateMotor(rightMotorSpeed, leftMotorSpeed);
  control2 = false;
}

void loop()
{
  Dabble.processInput(); // Processa a entrada do gamepad

  // Ajusta a velocidade dos motores com base na entrada do gamepad
  if (GamePad.isUpPressed()) {
    rightMotorSpeed = 255;
    leftMotorSpeed = 255;
    pwmLeft = 255;
    pwmRight = 255;
  } else if (GamePad.isDownPressed()) {
    rightMotorSpeed = -255;
    leftMotorSpeed = -255;
    pwmLeft = 255;
    pwmRight = 255;
  } else if (GamePad.isLeftPressed()) {
    rightMotorSpeed = 255;
    leftMotorSpeed = -255;
    pwmLeft = 255;
    pwmRight = 255;
  } else if (GamePad.isRightPressed()) {
    rightMotorSpeed = -255;
    leftMotorSpeed = 255;
    pwmLeft = 255;
    pwmRight = 255;
  } else if(GamePad.isSquarePressed()){
    HalfMoon();
   }else{
    rightMotorSpeed = 0;
    leftMotorSpeed = 0;
    pwmLeft = 255;
    pwmRight = 255;
  }

  // Aplica a velocidade aos motores
  ledcWrite(0, pwmLeft);
  ledcWrite(1, pwmRight);
  rotateMotor(rightMotorSpeed, leftMotorSpeed);
}