#include <TimerOne.h>
#include <Wire.h>
#include <MultiFuncShield.h>

int hoursClock = 0;               // время часов
float minClock = 0;
int secClock = 0;
unsigned int milsecClock = 0;

int hoursSetClock = 0;            // переменные для установки времени
float minSetClock = 0;

int hoursAlarm = 0;               // время будильника
float minAlarm = 0;

int hoursSetAlarm = 0;            // переменные для установки будильника
float minSetAlarm = 0;

bool alarmActivated = false;          // состояние будильника и часов
bool clockActivated = false;

enum displayModes                 // различные режимы работы дисплея
{
  MODE_CLOCK,
  MODE_SET_CLOCK_HOURS,
  MODE_SET_CLOCK_MINUTES,
  MODE_ALARM,
  MODE_SET_ALARM_HOURS,
  MODE_SET_ALARM_MINUTES
};

volatile byteDisplayMode = MODE_CLOCK;  // переменная текущего режима работы дисплея

void clock()  // функция хода времени часов
{
  if (clockActivated)
  {
    milsecClock++;
    if (milsecClock == 1000)
    {
      milsecClock = 0;
      secClock++;
    }
    if (secClock == 60)
    {
      secClock = 0;
      minClock += 0.01;
    }
    if (minClock > 0.59)
    {
      minClock = 0;
      hourseClock++;
    }
    if (hoursClock == 24)
    {
      hoursClock = 0;
    }
  }
}

void setup() {
  Timer1.initialize();          // создание объекта Timer1
  MFS.userInterrupt = clock();    // установка прерываний функция clock
  MFS.initialize(&Timer1);      // инициализация платы объектом Timer1
}

void displayTime(int hours, float minutes)  // функция отображения времени
{
  float time = hourse + minutes;
  MFS.write(time, 2);
}



void loop() {
  byte button = MFS.getButton();  // переменная, принимающая текущее состояние нажатий кнопок

  switch (displayMode)
  {
    case MODE_CLOCK:  // режим отображения времени часов   
      MFS.blinkDisplay(DIGIT_ALL, OFF);
      if (button == BUTTON_1_PRESSED) displayMode = MODE_SET_CLOCK_HOURS;
      else
      {
        if (button == BUTTON_3_PRESSED) displayMode = MODE_ALARM;
      }
      if (alarmActivated) MFS.writeLeds(LED_4, ON);
      else MFS.writeLeds(LED_4, OFF);
      if (alarmActivated && hoursAlarm == hoursClock && minAlarm == minClock)
      {
        MFS.beep();
        if (button == BUTTON_2_PRESSED) alarmActivated = false;
      }
      displayTime(hoursClock, minClock);
    
    case MODE_SET_CLOCK_HOURS:  // режим настройки времени часов, выбор количества часов
      MFS.blinkDisplay(DIGIT_1 | DIGIT_2, ON);
      if (button == BUTTON_3_PRESSED)
      {
        MFS.blinkSidplay(DIGIT_1 | DIGIT_2, OFF);
        displayMode = MODE_SET_CLOCK_MINUTES;
      }
      else
      {
        if (button == BUTTON_2_PRESSED)
        {
          hourseSetClock++;
          if (hourseSetClock > 23) hourseSetClock = 0;
        }
      }
      displayTime(hourseSetClock, minSetClock);
    
    case MODE_SET_CLOCK_MINUTES:  // режим настроки времени часов, выбор количества минут
      MFS.blinkDisplay(DIGIT_3, DIGIT_4, ON);
      if (button == BUTTON_3_PRESSED)
      {
        MFS.blinkDisplay(DIGIT_3, DIGIT_4, OFF);
        hoursClock = hourseSetClock;
        minClock = minSetClock;
        displayMode = MODE_CLOCK;
      }
      else
      {
        if (button == BUTTON_2_PRESSED)
        {
          minSetClock += 0.1;
          if (minSetClock > 0.59) minSetClock = 0;
        }
      }
      displayTime(hoursSetClock, minSetClock);
    
    case MODE_ALARM:  // режим отображения времени будильника
      MFS.writeLeds(LED_ALL, ON);
      if (button == BUTTON_3_PRESSED)
      {
        MFS.writeLeds(LED_ALL, OFF);
        displayMode = MODE_SET_ALARM_HOURS;
      }
      else
      {
        if (button == BUTTON_2_PRESSED)
        {
          MFS.writeLeds(LED_ALL, OFF);
          displayMode = MODE_CLOCK;
        }
      }
      displayTime(hoursAlarm, minAlarm);
    
    case MODE_SET_ALARM_HOURS:  // режим настройки времени будильника, выбор количества часов
      MFS.blinkDisplay(DIGIT_1 | DIGIT_2, ON);
      MFS.writeLeds(LED_1, ON);

      if (button == BUTTON_1_PRESSED)
      {
        MFS.blinkDisplay(DIGIT_1 | DIGIT_2, OFF);
        displayMode = MODE_SET_ALARM_MINUTES;
      }
      else
      {
        if (button == BUTTON_2_PRESSED)
        {
          hoursSetAlarm++;
          if (hoursSetAlarm > 23) hoursSetAlarm = 0;
        }
        else
        {
          if (button == BUTTON_3_PRESSED)
          {
            MFS.blinkDisplay(DIGIT_1 | DIGIT_2, OFF);
            MFS.writeLeds(LED_1, OFF);
          }
        }
      }
      displayTime(hoursSetAlarm, minSetAlarm);

    case MODE_SET_ALARM_MINUTES:  // режим настройки будильника, выбор количества минут
      MFS.blinkDisplay(DIGIT_3 | DIGIT_4, ON);
      if (button == BUTTON_1_PRESSED)
      {
        MFS.blinkDisplay(DIGIT_3 | DIGIT_4, OFF);
        MFS.writeLeds(LED_1, OFF);
        hoursAlarm = hoursSetAlarm;
        minAlarm = minSetAlarm;
        displayMode = MODE_CLOCK;
      }
      else
      {
        if (button == BUTTON_2_PRESSED)
        {
          minSetAlarm += 0.01;
          if (minSetAlarm > 0.59) minSetAlarm = 0;
        }
      }
      displayTime(hoursSetAlarm, minSetAlarm);
  }
}
