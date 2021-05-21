/*
        More Details in : https://github.com/emirhanbilge/ArduinoLCDandClock  
        Thank You
        Note:My teacher, I tried to explain as much as possible and our time was limited, so I put the circuit diagram and details here.
*/

#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int TIME_SCREEN_CLOCK = 2000;
int TIME_SCREEN_INFO = 1000;
String TIME_STRING = "2021.05.31 - 23:59:55";
String language = "TUR";

int month, day, year, hour, minutes, second;

String daysofWeekTUR[] = {"CAR", "PER", "CUM", "CMT", "PAZ", "PZT", "SAL"};
String daysofWeekENG[] = {"WED", "THU", "FRI", "SAT", "SUN", "MON", "TUE"};

int monthsLongs[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

unsigned long lastMilliSecond = 0;
unsigned long currentMilisend = 0;
unsigned long timeScreenMiliSecond = 0;

void dateParser(String currentDate)
{
    int i = 0;
    String monthh, dayy, yearr, hourr, minutess, seconds; // Firstly store String
    int variableIndex = 0;                                // The value I hold to understand which variable is worth
    while (i < 21)
    {
        if (currentDate[i] == '.' || currentDate[i] == ' ' || currentDate[i] == '-' || currentDate[i] == ':') // In the case of these variables, no additions should be made.
        {
            i++;
            variableIndex++;
        }
        else
        {
            if (variableIndex == 0) // The first variable -year- we will read
            {
                yearr += currentDate[i++];
            }
            else if (variableIndex == 1) // The second variable -month- we will read
            {
                monthh += currentDate[i++];
            }
            else if (variableIndex == 2) // The third variable -day- we will read
            {
                dayy += currentDate[i++];
            }
            else if (variableIndex == 5) // The fourth variable -hour- we will read
            {
                hourr += currentDate[i++];
            }
            else if (variableIndex == 6) // The fifth variable -minutes- we will read
            {
                minutess += currentDate[i++];
            }
            else
            {
                seconds += currentDate[i++]; // The sixth variable -second- we will read
            }
        }
    }
    // I keep them as integers so that we can make calculations easier.
    year = yearr.toInt();
    month = monthh.toInt();
    day = dayy.toInt();
    hour = hourr.toInt();
    minutes = minutess.toInt();
    second = seconds.toInt() - 1;
}
String makeDoubleString(int param) //I wrote this function to add a leading 0 for values less than 10
{
    if (param < 10) //If it's less than 10, I add 0 in front
    {
        return "0" + String(param);
    }
    return String(param); //otherwise it already has 2 digits
}
String EbbDayCalculator(int inputYear, int inputMonth, int inputDay)
{
    /* For the calculation, I have based on Wednesday, January 1, 2020.
    I have it calculated how many days have passed after January 1, 2020, and I find the name of that 
    day with the 7% transaction. This percentage is the first index on Wednesday, in which I hold the days above.
    */
    int baseDay = (inputYear - 2020) * 365; // First I directly calculate how many days have passed by multiplying the year difference by 365.
    if (baseDay != 0)                       // If it is greater than 2020, I am directly adding the leap year in 2020.
    {
        baseDay += 1;
    }
    else if (baseDay == 0 && inputMonth >= 2 && inputDay > 28) // If it is in 2020 but greater than February 28, then there is this condition for collecting the leap year as well.
    {
        baseDay += 1;
    }
    int monthdif = inputMonth - 1;     //I have to calculate how many months have passed the year in which the date given here. minus 1 bacause arrey index!!
    for (int i = 0; i < monthdif; i++) //I calculate the days passed in those months.
    {
        baseDay += monthsLongs[i];
    }
    baseDay += inputDay - 1; // How many days progressed on the given date.
    baseDay %= 7;            // I find the array address in my days with% 7 of the large value.
    if (language == "TUR")   // I will return it according to the language difference
    {
        return daysofWeekTUR[baseDay];
    }
    return daysofWeekENG[baseDay]; //I will return it according to the language difference
}
void updateClockandCalender()
{
    if (second > 59) // If greater than 60 seconds, a new minute must be added and seconds must be reset
    {
        second = 0;
        minutes += 1;
    }
    if (minutes > 59) // If greater than 60 minutes, a new hour must be added and minutes must be reset
    {
        hour += 1;
        minutes = 0;
    }
    if (hour > 23) //If greater than 24 hours, a new day must be added and hour must be reset
    {
        day += 1;
        hour = 0;
    }

    if (day > monthsLongs[month - 1]) //  If it exceeds the day of the current month, switch to the new month and set the date as 1 of the month.
    {
        if (year == 2020 && month == 2 && day == 29) //Here is a very special situation, February 29, 2020, there is a control for the only situation where it could happen.
        {
            day = 29;
        }
        else
        {
            day = 1;
            month += 1;
        }
    }
    if (month > 12) // If it exceeds 12 months, the new year should be started and the month should be updated.
    {
        year += 1;
        month = 1;
    }
}
String dateType()
{
    if (language == "ENG") //Checking AM / PM status when English is selected
    {
        if (hour > 11)
        {
            return " PM";
        }
        return " AM";
    }
    return ""; // Nothing should return when Turkish is selected.
}

void setup()
{
    lcd.begin(16, 2);        // lcd starting
    dateParser(TIME_STRING); //The function I calculate the analysis of the entered date in the first set up
                             //DisplayClockandDate();   //To start showing the entered time and date directly at the first moment.
    if (language == "ENG")
    {
        language = "TUR";
    }
    else
    {
        language = "ENG";
    }
    pinMode(6, INPUT); // for button
}
int clockInfo = 0;

void DispInformation() /// The structure I created to show the information on LCD
{
    lcd.setCursor(0, 0);
    if (language == "ENG")
    {
        lcd.print("HH:MM:SS 12H LNG");
        lcd.setCursor(0, 1);
        lcd.print("DD.MM.YYYY   DOW");
    }
    else
    {
        lcd.print("SA:DA:SN 24H DIL");
        lcd.setCursor(0, 1);
        lcd.print("GG.AA.YYYY   GUN");
    }
}

void DisplayClockandDate() // The part with the necessary adjustments and pressing commands to print the clock display
{
    lcd.setCursor(0, 0);
    if (language == "ENG")
    {
        if (hour % 12 == 0)
        {
            lcd.print(makeDoubleString(12) + ":" + makeDoubleString(minutes) + ":" + makeDoubleString(second) + dateType());
        }
        else
        {
            lcd.print(makeDoubleString(hour % 12) + ":" + makeDoubleString(minutes) + ":" + makeDoubleString(second) + dateType());
        }
    }
    else
    {
        lcd.print(makeDoubleString(hour) + ":" + makeDoubleString(minutes) + ":" + makeDoubleString(second));
    }
    lcd.setCursor(13, 0);
    lcd.print(language);
    lcd.setCursor(0, 1);
    lcd.print(makeDoubleString(day) + "." + makeDoubleString(month) + "." + String(year));
    lcd.setCursor(13, 1);
    lcd.print(EbbDayCalculator(year, month, day));
}

bool switchFunction = true;
volatile boolean currentButtonValue = false; // if you press button it change
volatile boolean lastButtonValue = false;

void loop()
{
    currentMilisend = millis();                // read current time
    currentButtonValue = digitalRead(6);       //read button value
    if (lastButtonValue != currentButtonValue) // checking last value . Press the button to make a single change.
    {
        if (currentButtonValue == HIGH) // Pressing button
        {
            if (language == "ENG") // change language
            {
                language = "TUR";
            }
            else
            {
                language = "ENG";
            }
            switchFunction = 1; // When the language is changed, I directly set the control variable to 1 so that the time is displayed first.
        }
    }
    lastButtonValue = currentButtonValue;                     // When the button is pressed, I keep the old value and when the button is pressed continuously, I add a value to make it change 1 time.
    if (currentMilisend - lastMilliSecond >= TIME_SCREEN_INFO) //I'll update the seconds of my watch with the smallest value equal to about 1 second over the last time I held.
    {
        ++second; // I increase the second because 1 second has passed
        updateClockandCalender();
        if (switchFunction) // I will switch between the switchFunction variable and the time-information screen at 2 second intervals.
        {
            DisplayClockandDate();
            TIME_SCREEN_CLOCK = 2000;
        }
        else
        {
            DispInformation();
            TIME_SCREEN_CLOCK = 1000;
        }
        lastMilliSecond = currentMilisend; // Now I will update the time I kept the old and check it to be 1 second again.
    }
    if (currentMilisend - timeScreenMiliSecond >= TIME_SCREEN_CLOCK)
    {
        lcd.setCursor(9, 0);
        lcd.print("   ");                       // To delete AM-PM texts that will be caused by language change on the screen
        switchFunction = !switchFunction;       // I print the clock and screen information by taking the opposite of the value of this variable in periods of 2 seconds.
        timeScreenMiliSecond = currentMilisend; // I update the old value again to see if 2 seconds have passed.
    }
    // To update times, for example to switch to a new clock, to switch to a new day, to a year
}

/*
References :
    I only used the default connections on tinkercad and the default pins provided by the library in the setup of the circuit.
    tinkercad.com
    https://www.arduino.cc/en/Reference/LiquidCrystal
*/