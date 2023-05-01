#include <Keypad.h>
#include <EEPROM.h>
#include <Servo.h>
#include <LiquidCrystal.h>

Servo lock;
int pos = 0;

const byte numRows = 4;
const byte numCols = 4;

char keymap[numRows][numCols] = {
  {'1', '2', '3', 'A'}, 
  {'4', '5', '6', 'B'}, 
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[numRows] = {12, 10, 9, 8};
byte colPins[numCols] = {7, 6, A0, A1};

Keypad keypad_lock= Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);

String password = "";

const int rs = 13, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  lock.attach(A2);
  lcd.begin(16,2);
  Serial.begin(9600);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Crie uma senha");
  int x = 0;
  char first_key;
  while(true) {
    first_key = keypad_lock.getKey();
    if(first_key) {
      if (first_key == '#'){
        break;
      }
      if (first_key == 'D'){
        x -= 1;
        password.remove(x);
        lcd.setCursor(x, 1);
        lcd.print(" ");
      } else {
        lcd.setCursor(x, 1);
        lcd.print('x');
        password += first_key;
        x += 1;
      }
    }
  }
  lcd.clear();
  menu();
}

void servo(){
  for (pos = 0; pos<= 180; pos +=1) {
    lock.write(pos);
    delay(50);
  }
  for (pos = 180; pos >= 0; pos -= 1) {
    lock.write(pos);
    delay(50);
  }
}

void create_password() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Digite a senha:");
  int x = 0;
  String password_2 = "";
  char new_key;
  while(true) {
    new_key = keypad_lock.getKey();
    if(new_key) {
      if(new_key == '*') {
        return;
      }
      if (new_key == '#'){
        break;
      }
      if (new_key == 'D'){
        x -= 1;
        password_2.remove(x);
        lcd.setCursor(x, 1);
        lcd.print(" ");
      } else {
        lcd.setCursor(x, 1);
        lcd.print("X");
        password_2 += new_key;
        x += 1;
      }
    }

  }
  if (password_2.equalsIgnoreCase(password)) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Destravando.");
    servo();
    lcd.clear();
    menu();
  } else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Senha Incorreta.");
    delay(4000);
    lcd.clear();
    menu();
  }
}
void menu() {
  lcd.setCursor(0, 0);
  lcd.print("Bem-vindo :D");
  lcd.setCursor(0, 1);
  lcd.print("1. Desbloquear");
}
void loop() {
  char menu_key;
  menu_key = keypad_lock.getKey();
  if (menu_key == '1') {
    create_password();
  }

}