#define CH1 4  // Поворот (лево/право)
#define CH2 5  // Движение (вперед/назад)

#define IN1 6  // Левый двигатель (направление 1)
#define IN2 7  // Левый двигатель (направление 2)
#define IN3 8  // Правый двигатель (направление 1)
#define IN4 9  // Правый двигатель (направление 2)
#define ENA 10 // ШИМ для левого двигателя
#define ENB 11 // ШИМ для правого двигателя

void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  //Serial.begin(9600);  // Для отладки
}

void loop() {
  int ch1 = pulseIn(CH1, HIGH, 25000);  // Ось X (поворот)
  int ch2 = pulseIn(CH2, HIGH, 25000);  // Ось Y (вперед/назад)

  // Преобразуем сигнал в диапазон -255..255
  int speedY = map(ch2, 1085, 1900, -255, 255); // Движение вперед/назад
  int speedX = map(ch1, 1085, 1900, -255, 255); // Поворот влево/вправо

  // Ограничиваем значения диапазоном -255..255
  speedY = constrain(speedY, -255, 255);
  speedX = constrain(speedX, -255, 255);

  // Нейтральное положение с мертвой зоной (±20)
  if (abs(speedY) < 20) speedY = 0;
  if (abs(speedX) < 20) speedX = 0;

  // Вывод значений для отладки
  //Serial.print("ch1: ");
  //Serial.print(ch1);
  //Serial.print(" | ch2: ");
  //Serial.print(ch2);
  //Serial.print(" | speedX: ");
  //Serial.print(speedX);
  //Serial.print(" | speedY: ");
  //Serial.println(speedY);

 // Управление движением
  if (speedY != 0) {
    if (speedY > 0) {
      //Serial.print(" | speedY: ");
      //Serial.println(speedY);
      controlSteering(LOW, HIGH, HIGH, LOW, abs(speedY)); // Движение назад
    } else {
      //Serial.print(" | speedY: ");
      //Serial.println(speedY);
      controlSteering(HIGH, LOW, LOW, HIGH, abs(speedY)); // Движение вперед
    }
  } else if (speedX != 0) {
    if (speedX > 0) {
      //Serial.print(" | speedX: ");
      //Serial.println(speedX);
      controlSteering(HIGH, LOW, HIGH, LOW, abs(speedX)); // Поворот вправо
    } else {
      //Serial.print(" | speedX: ");
      //Serial.println(speedX);
      controlSteering(LOW, HIGH, LOW, HIGH, abs(speedX)); // Поворот влево
    }
  } else {
    controlSteering(LOW, LOW, LOW, LOW, 0); // Стоим на месте
  }
  delay(20);
}

void controlSteering(int in1, int in2, int in3, int in4, int speed) {
  digitalWrite(IN1, in1);
  digitalWrite(IN2, in2);
  digitalWrite(IN3, in3);
  digitalWrite(IN4, in4);
  analogWrite(ENA, speed); // Скорость левого двигателя
  analogWrite(ENB, speed); // Скорость правого двигателя
}
