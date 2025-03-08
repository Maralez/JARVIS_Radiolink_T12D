#include <ServoSmooth.h>

#define CH8 2   // Канал для клешни
#define CH4 12  // Канал для поворота клешни
#define CH3 3   // Канал для поднятия/опускания клешни

#define SERVO_CLAW A0   // Серво 1: Клешня
#define SERVO_ROTATE A1 // Серво 2: Поворот клешни
#define SERVO_LIFT A2   // Серво 3: Подъём/опускание

ServoSmooth clawServo;   // Объект для управления клешнёй
ServoSmooth rotateServo; // Объект для поворота клешни
ServoSmooth liftServo;   // Объект для подъёма/опускания

#define DEADZONE 10   // Гистерезис (мертвая зона)
#define SPEED 200     // Скорость движения сервоприводов (0-255)
#define ACCEL 0.5     // Ускорение (0-1)

void setup() {
  // Инициализация сервоприводов
  clawServo.attach(SERVO_CLAW);
  rotateServo.attach(SERVO_ROTATE);
  liftServo.attach(SERVO_LIFT);

  // Настройка скорости и ускорения
  clawServo.setSpeed(SPEED);
  clawServo.setAccel(ACCEL);

  rotateServo.setSpeed(SPEED);
  rotateServo.setAccel(ACCEL);

  liftServo.setSpeed(SPEED);
  liftServo.setAccel(ACCEL);

  // Чтение значений с пульта
  int ch8 = pulseIn(CH8, HIGH, 25000);  // Клешня
  int ch4 = pulseIn(CH4, HIGH, 25000);  // Поворот клешни
  int ch3 = pulseIn(CH3, HIGH, 25000);  // Подъем/опускание

  // Преобразование значений в углы
  int clawAngle = map(ch8, 1065, 1889, 0, 180);    // Клешня
  int rotateAngle = map(ch4, 1074, 1898, 0, 180);  // Поворот клешни
  int liftAngle = map(ch3, 1074, 1898, 17, 130);   // Подъем/опускание

  // Ограничение углов
  clawAngle = constrain(clawAngle, 0, 180);
  rotateAngle = constrain(rotateAngle, 0, 180);
  liftAngle = constrain(liftAngle, 17, 130);

  // Установка начальных позиций
  clawServo.setTargetDeg(clawAngle);   // Установка клешни
  rotateServo.setTargetDeg(90);        // Поворот клешни в центр
  liftServo.setTargetDeg(liftAngle);   // Установка подъема/опускания

  // Небольшая задержка для стабилизации
  delay(500);
}

void loop() {
  // Чтение значений с пульта
  int ch8 = pulseIn(CH8, HIGH, 25000);  // Клешня
  int ch4 = pulseIn(CH4, HIGH, 25000);  // Поворот клешни
  int ch3 = pulseIn(CH3, HIGH, 25000);  // Подъем/опускание

  // Преобразование значений в углы
  int clawAngle = map(ch8, 1065, 1889, 0, 180);    // Клешня
  int rotateAngle = map(ch4, 1074, 1898, 0, 180);  // Поворот клешни
  int liftAngle = map(ch3, 1074, 1898, 17, 130);   // Подъем/опускание

  // Ограничение углов
  clawAngle = constrain(clawAngle, 0, 180);
  rotateAngle = constrain(rotateAngle, 0, 180);
  liftAngle = constrain(liftAngle, 17, 130);

  // Управление сервоприводами
  static int lastClaw = clawAngle, lastRotate = rotateAngle, lastLift = liftAngle;

  if (abs(clawAngle - lastClaw) > DEADZONE) {
    clawServo.setTargetDeg(clawAngle);
    lastClaw = clawAngle;
  }

  if (abs(rotateAngle - lastRotate) > DEADZONE) {
    rotateServo.setTargetDeg(rotateAngle);
    lastRotate = rotateAngle;
  }

  if (abs(liftAngle - lastLift) > DEADZONE) {
    liftServo.setTargetDeg(liftAngle);
    lastLift = liftAngle;
  }

  // Обновление состояния сервоприводов
  clawServo.tick();
  rotateServo.tick();
  liftServo.tick();

  delay(20);
}
