/*
 * Typical pin layout used:
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
 * SPI SS      SDA(SS)      10            53        D10        10               10
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
 */

#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h> // Библиотека для RFID

#define SS_PIN 10                 // Определение пина SDA
#define RST_PIN 9                 // Определение пина RST
MFRC522 mfrc522(SS_PIN, RST_PIN); // Создание экземпляра класса MFRC522

void setup()
{
  Serial.begin(9600);                                       // Инициализация последовательного соединения и задание скорости передачи данных в бит/с(бод)
  SPI.begin();                                              // Инициализация шины SPI
  mfrc522.PCD_Init();                                       // Инициализация MFRC522
  Serial.println("Approximate your card to the reader..."); // Вывод надписи "Приложите вашу карту к плате"
  Serial.println();                                         // Пропуск строки
}
void loop()
{
  // Поиск новых карт:
  if (!mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }
  // Выбор одну из карт:
  if (!mfrc522.PICC_ReadCardSerial())
  {
    return;
  }
  // Показание UID на мониторе порта:
  Serial.print("UID tag :"); // Вывод надписи "UID tag: "
  String content = "";
  // Вывод UID тега в шестнадцатеричном формате
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();           // Пропуск строки
  Serial.print("Message : "); // Вывод надписи "Message: "
  content.toUpperCase();      // Преобразование в верхний регистр, если это возможно
  // Номер UID тега, которому разрешён доступ
  if (content.substring(1) == "96 E1 33 53")
  {
    Serial.println("Authorized access"); // Вывод надписи "Доступ открыт"
    Serial.println();                    // Пропуск строки
    delay(3000);                         // Задержка 3с
  }

  else
  {
    Serial.println("Access denied"); // Вывод надписи "Запрос на доступ отклонен"
    delay(3000);                     // Задержка 3с
  }
}
