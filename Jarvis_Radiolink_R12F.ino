#include <VarSpeedServo.h>

#define CH8 2   // Канал для клешни
#define CH4 12  // Канал для поворота клешни
#define CH3 3   // Канал для поднятия/опускания клешни (исправлено)

#define SERVO_CLAW A0   // Серво 1: Клешня
#define SERVO_ROTATE A1 // Серво 2: Поворот клешни
#define SERVO_LIFT A2   // Серво 3: Подъём/опускание

VarSpeedServo clawServo;   // Объект для управления клешнёй
VarSpeedServo rotateServo; // Объект для поворота клешни
VarSpeedServo liftServo;   // Объект для подъёма/опускания

#define DEADZONE 10   // Гистерезис (мертвая зона)
#define SPEED 20     // Скорость движения сервоприводов (увеличено)

void setup() {
  clawServo.attach(SERVO_CLAW);
  rotateServo.attach(SERVO_ROTATE);
  liftServo.attach(SERVO_LIFT);

  // Устанавливаем стартовые позиции
  clawServo.write(180, SPEED);  
  rotateServo.write(95, SPEED);
  liftServo.write(17, SPEED); // Нижнее положение (минимум 20)
}

void loop() {
  int ch8 = pulseIn(CH8, HIGH, 25000);
  int ch4 = pulseIn(CH4, HIGH, 25000);
  int ch3 = pulseIn(CH3, HIGH, 25000);

  int clawAngle   = map(ch8, 1000, 2000, 0, 180);
  int rotateAngle = map(ch4, 1000, 2000, 0, 180);
  int liftAngle   = map(ch3, 1000, 2000, 17, 150); 

  clawAngle   = constrain(clawAngle, 20, 180);
  rotateAngle = constrain(rotateAngle, 0, 180);
  liftAngle   = constrain(liftAngle, 17, 150); // Ограничение подъёма

  static int lastClaw = 180, lastRotate = 95, lastLift = 17;

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
