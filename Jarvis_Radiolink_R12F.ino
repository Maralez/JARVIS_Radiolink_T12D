#include <VarSpeedServo.h>

#define CH1 4   // Поворот танка(лево/право)
#define CH2 5   // Движение танка (вперед/назад)
#define CH3 12  // Подъем/опускание клешни
#define CH4 2   // Хват клешни
#define CH8 13  // Поворот клешни

#define IN1 6   // Левый двигатель (направление 1)
#define IN2 7   // Левый двигатель (направление 2)
#define IN3 8   // Правый двигатель (направление 1)
#define IN4 9   // Правый двигатель (направление 2)
#define ENA 10  // ШИМ для левого двигателя
#define ENB 11  // ШИМ для правого двигателя

#define SERVO_CLAW A0   // Серво 1: Клешня
#define SERVO_ROTATE A1 // Серво 2: Поворот клешни
#define SERVO_LIFT A2   // Серво 3: Подъем/опускание

VarSpeedServo clawServo;
VarSpeedServo rotateServo;
VarSpeedServo liftServo;

#define DEADZONE 10  // Гистерезис (мертвая зона)
#define SPEED 50     // Скорость движения сервоприводов

void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  clawServo.attach(SERVO_CLAW);
  rotateServo.attach(SERVO_ROTATE);
  liftServo.attach(SERVO_LIFT);

  // Устанавливаем стартовые позиции сервоприводов
  //clawServo.write(180, SPEED);   // Клешня закрыта
  rotateServo.write(95, SPEED);  // Поворот клешни в нейтральное положение
  //liftServo.write(20, SPEED);    // Подъем клешни в минимальное положение
}

void loop() {
  // Читаем сигналы от пульта
  int ch1 = pulseIn(CH1, HIGH, 25000);
  int ch2 = pulseIn(CH2, HIGH, 25000);
  int ch3 = pulseIn(CH3, HIGH, 25000);
  int ch4 = pulseIn(CH4, HIGH, 25000);
  int ch8 = pulseIn(CH8, HIGH, 25000);

  int speed = map(ch2, 1000, 2000, 0, 255);

  // Управление движением
  if (ch1 > 1500) {
    controlSteering(HIGH, LOW, HIGH, LOW, speed);
  } else if (ch1 < 1450) {
    controlSteering(LOW, HIGH, LOW, HIGH, speed);
  } else if (ch2 < 1450) {
    controlSteering(HIGH, LOW, LOW, HIGH, speed);
  } else if (ch2 > 1500) {
    controlSteering(LOW, HIGH, HIGH, LOW, speed);
  } else {
    controlSteering(LOW, LOW, LOW, LOW, 0);
  }

  // Управление сервоприводами
  int clawAngle   = map(ch4, 1000, 2000, 20, 180);
  int rotateAngle = map(ch8, 1000, 2000, 0, 180);
  int liftAngle   = map(ch3, 1000, 2000, 17, 160); // Подъем ограничен до 160

  clawAngle   = constrain(clawAngle, 20, 180);
  rotateAngle = constrain(rotateAngle, 0, 180);
  liftAngle   = constrain(liftAngle, 17, 160);

  static int lastClaw = 180, lastRotate = 95, lastLift = 20;

  if (abs(clawAngle - lastClaw) > DEADZONE) {
    clawServo.write(clawAngle, SPEED);
    lastClaw = clawAngle;
  }

  if (abs(rotateAngle - lastRotate) > DEADZONE) {
    rotateServo.write(rotateAngle, SPEED);
    lastRotate = rotateAngle;
  }

  if (abs(liftAngle - lastLift) > DEADZONE) {
    liftServo.write(liftAngle, SPEED);
    lastLift = liftAngle;
  }

  delay(20);
}

void controlSteering(int in1, int in2, int in3, int in4, int speed) {
  digitalWrite(IN1, in1);
  digitalWrite(IN2, in2);
  digitalWrite(IN3, in3);
  digitalWrite(IN4, in4);
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
}
