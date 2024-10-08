#pragma warning(disable : 4996)
#include <iostream>
#include <string>
#include <vector>
using namespace std;

enum DateCompareResults { Before = -1, Equal = 0, After = 1 };

struct stDuration
{
	int days, hours, mins, secs;
};

struct stDate
{
	short Year, Month, Day;
};

struct stPeriod
{
	stDate StartDate, EndDate;
};

string ReadString(string Message)
{
	string s;
	cout << Message;
	getline(cin >> ws, s);
	return s;
}

int ReadNumber(string massage)
{
	int num = 0;
	cout << endl << massage << endl;
	cin >> num;

	return num;
}

short ReadYear()
{
	short year = 0;
	cout << "Enter a Year: " << endl;
	cin >> year;

	return year;
}

short ReadMonth()
{
	short month = 0;
	while (month < 1 || month >12)
	{
		cout << "Enter a Month: (1 - 12)" << endl;
		cin >> month;
	}
	return month;
}

short ReadDay()
{
	short day = 0;
	while (day < 1 || day > 31)
	{
		cout << "Enter a Day: (1 - 31)" << endl;
		cin >> day;
	}
	return day;
}

string NumberToText(int num)
{
	if (num == 0)
		return "";
	else if (num >= 1 && num <= 19)
	{
		string arr[] = { "", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten", "Eleven", "Twelve", "Thirteen", "Fourteen", "Fifteen", "Sixteen", "Seventeen", "Eighteen", "Nineteen" };

		return arr[num];
	}
	else if (num >= 20 && num <= 99)
	{
		string arr[] = { "" , "", "Twenty", "Thirty", "Forty", "Fifty", "Sixty", "Seventy", "Eighty", "Ninety" };

		return arr[num / 10] + " " + NumberToText(num % 10);
	}
	else if (num >= 100 && num <= 199)
	{
		return "One Hundred " + NumberToText(num % 100);
	}
	else if (num >= 200 && num <= 999)
	{
		return NumberToText(num / 100) + " Hundreds " + NumberToText(num % 100);
	}
	else if (num >= 1000 && num <= 1999)
	{
		return "One Thousand " + NumberToText(num % 1000);
	}
	else if (num >= 2000 && num <= 999999)
	{
		return NumberToText(num / 1000) + " Thousands " + NumberToText(num % 1000);
	}
	else if (num >= 1000000 && num <= 1999999)
	{
		return "One Million " + NumberToText(num % 1000000);
	}
	else if (num >= 2000000 && num <= 999999999)
	{
		return NumberToText(num / 1000000) + " Millions " + NumberToText(num % 1000000);
	}
	else if (num >= 1000000000 && num <= 1999999999)
	{
		return "One Billion " + NumberToText(num % 1000000000);
	}
	else
	{
		return NumberToText(num / 1000000000) + " Billions " + NumberToText(num % 1000000000);
	}


}

bool isLeapYear(int year)
{
	//leap year can be (divided by 400) or  (divided by 4 but not divided by 100)
	
	/*if (year % 400 == 0)
		return true;
	if (year % 4 == 0 && year % 100 != 0)
		return true;

	return false;*/

	return ((year % 400 == 0) || (year % 4 == 0 && year % 100 != 0)); // algorithm in one line
}

stDuration Calculate_DaysHoursMinsSecs_InYear(int year)
{
	stDuration d;
	
	d.days = 365;
	if (isLeapYear(year))
		d.days = 366;
	
	d.hours = d.days * 24;
	d.mins = d.hours * 60;
	d.secs = d.mins * 60;

	return d;
}

short NumberOfDaysInYear(short year)
{
	return isLeapYear(year) ? 366 : 365;
}

short NumberOfHoursInYear(int year)
{
	return NumberOfDaysInYear(year) * 24;
}

int NumberOfMinsInYear(int year)
{
	return NumberOfHoursInYear(year) * 60;
}

int NumberOfSecsInYear(int year)
{
	return NumberOfMinsInYear(year) * 60;
}

short NumberOfDaysInMonth_oldFunc(short month, short year)
{// less optimized
	if (month == 2)
	{
		return isLeapYear(year) ? 29 : 28;
	}

	short arr31Days[] = { 1,3,5,7,8,10,12 }; //months that have 31 days

	for (short i : arr31Days)
	{
		if (month == i)
			return 31;
	}
	
	return 30;
}

short NumberOfDaysInMonth(short month, short year)
{// More optimized function // in 2 lines

	short arrNumberOfDays[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };

	return isLeapYear(year) && month == 2 ? 29 : arrNumberOfDays[month - 1]; // my solution

	//return (month == 2) ? (isLeapYear(year) ? 29 : 28) : arrNumberOfDays[month - 1]; // abo hadhoud solution

	//all func in one line//
	/*return month == 2 ? (isLeapYear(year) ? 29 : 28) : (month == 4 || month == 6 || month == 9 || month == 11 ? 30 : 31);*/
}

short NumberOfHoursInMonth(short month, short year)
{
	return NumberOfDaysInMonth(month, year) * 24;
}

int NumberOfMinsInMonth(short month, short year)
{
	return NumberOfHoursInMonth(month, year) * 60;
}

int NumberOfSecsInMonth(short month, short year)
{
	return NumberOfMinsInMonth(month, year) * 60;
}

short DayOfWeekOrder(short y, short m, short d)
{
	short a = (14 - m) / 12;
	y = y - a;
	m = m + 12 * a - 2;
	
	return (d + y + (y / 4) - (y / 100) + (y / 400) + ((31 * m) / 12)) % 7;

	// will return the Day Index //
	// Sunday = 0, Monday = 1, Tuesday = 2, Wednesday = 3, Thursday = 4, Friday = 5, Saturday = 6 // 
}

string WeekDayName(short DayIndex)
{
	string arrWeekDays[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
	return arrWeekDays[DayIndex];
}

void PrintWeekDay(short year, short month, short day)
{
	cout << "\nDate     : " << day << "/" << month << "/" << year;
	cout << "\nDay Index: " << DayOfWeekOrder(year, month, day);
	cout << "\nDay Name : " << WeekDayName(DayOfWeekOrder(year, month, day));
}

string MonthShortName(short MonthNumber)
{
	string MonthsList[12] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
	return MonthsList[MonthNumber - 1];
}

void PrintMonthCalendar(short year, short month)
{
	int CurrentDay = DayOfWeekOrder(year, month, 1); // 1 = first day of the month
	int NumberOfDays = NumberOfDaysInMonth(month, year); // get number of days in that month

	printf("\n  _______________________%s_______________________\n", MonthShortName(month).c_str());
	cout << "\n     Sun    Mon    Tue    Wed    Thu    Fri    Sat \n";

	int i;
	for (i = 0; i < CurrentDay; i++)
		printf("       "); //print the first space before day number 1

	for(int j = 1; j <= NumberOfDays ; j++)
	{
		printf("%7d", j); //print day number

		if (++i == 7)
		{
			i = 0;
			printf("\n"); //move to the next line every 7 numbers
		}

	}

	printf("\n  _________________________________________________\n");


}

void PrintYearCalendar(short year)
{
	printf("\n     -------------------------------------------\n");
	printf("\n                   Year %d Calendar              \n", year);
	printf("\n     -------------------------------------------\n");

	for (int month = 1; month <= 12; month++)
	{
		PrintMonthCalendar(year, month);
		cout << endl;
	}
}

short CalculateDaysFromTheBeginningOfYear(short year, short month, short day)
{
	short TotalDays = day; //number of days enterd
	
	for (short i = 1; i < month; i++)
	{
		TotalDays += NumberOfDaysInMonth(i, year); // number of days in each month 
	}

	return TotalDays;
}

stDate GetDateFromDayOrderInYear(short year, short DayOrder)
{
	stDate date;

	date.Year = year;
	date.Month = 1;


	short MonthDays = 0, RemainingDays = DayOrder;

	while (true)
	{
		MonthDays = NumberOfDaysInMonth(date.Month, date.Year);

		if (RemainingDays > MonthDays)
		{
			RemainingDays -= MonthDays;
			date.Month++;
		}
		else
		{
			date.Day = RemainingDays;
			break;
		}
	}

	return date;
}

stDate ReadFullDate()
{
	stDate date;

	date.Day = ReadDay();
	date.Month = ReadMonth();
	date.Year = ReadYear();

	return date;
}

stDate AddDaysToDate(stDate Date, int DaysToAdd)
{
	short RemainingDays=DaysToAdd + CalculateDaysFromTheBeginningOfYear(Date.Year, Date.Month, Date.Day);

	Date.Month = 1; //start adding from the beginning of the month
	short MonthDays = 0;

	while (true)
	{
		MonthDays = NumberOfDaysInMonth(Date.Month, Date.Year);

		if (RemainingDays > MonthDays)
		{
			RemainingDays -= MonthDays;
			Date.Month++;

			if (Date.Month > 12) {
				Date.Month = 1; //restart the year
				Date.Year++;
			}
		}
		else
		{
			Date.Day = RemainingDays;
			break;
		}

	}

	return Date;
}

bool IsDate1BeforeDate2(stDate date1, stDate date2)
{ //one line of code
	return (date1.Year < date2.Year) ? true : (date1.Year > date2.Year) ? false : (date1.Month < date2.Month) ? true : (date1.Month > date2.Month) ? false : (date1.Day < date2.Day) ? true : false;
}

bool IsDate1EqualsDate2(stDate date1, stDate date2)
{
	return (date1.Year == date2.Year) ? ((date1.Month == date2.Month) ? ((date1.Day == date2.Day) ? true : false) : false) : false;

}

bool isLastDayInMonth(stDate date)
{
	return (date.Day == NumberOfDaysInMonth(date.Month, date.Year));
}

bool isLastMonthInYear(short month)
{
	return (month == 12);
}

stDate IncreaseDateByOneDay(stDate date)
{
	if (isLastDayInMonth(date))
	{
		date.Day = 1;
		if (isLastMonthInYear(date.Month))
		{
			date.Month = 1;
			date.Year++;
		}
		else
			date.Month++;
	}
	else
		date.Day++;

	return date;
}

int GetDiffrenceInDays_oldFunc(stDate date1, stDate date2, bool IncludeEndDay = false)
{
	int DaysCounter = 0;
	while (IsDate1BeforeDate2(date1, date2))
	{
		DaysCounter++;
		date1 = IncreaseDateByOneDay(date1);
	}
	return IncludeEndDay ? ++DaysCounter : DaysCounter;

}

stDate GetSystemDate()
{
	stDate Date;

	time_t t = time(0);
	tm* now = localtime(&t); 
	
	Date.Year = now->tm_year + 1900;   
	Date.Month = now->tm_mon + 1;    
	Date.Day = now->tm_mday;

	return Date;

}

void SwapDates(stDate& date1, stDate& date2)
{
	stDate temp;

	temp.Year = date1.Year;
	temp.Month = date1.Month;
	temp.Day = date1.Day;

	date1.Year = date2.Year;
	date1.Month = date2.Month;
	date1.Day = date2.Day;

	date2.Year = temp.Year;
	date2.Month = temp.Month;
	date2.Day = temp.Day;


}

int GetDiffrenceInDays(stDate date1, stDate date2, bool IncludeEndDay = false)
{
	int DaysCounter = 0;
	int SwapFlagValue = 1;

	if (!IsDate1BeforeDate2(date1, date2))
	{
		SwapDates(date1, date2);

		SwapFlagValue = -1;
	}

	while (IsDate1BeforeDate2(date1, date2))
	{
		DaysCounter++;
		date1 = IncreaseDateByOneDay(date1);
	}
	return IncludeEndDay ? ++DaysCounter * SwapFlagValue : DaysCounter * SwapFlagValue;
}

stDate IncreaseDateByXDays(stDate date, short xDays)
{
	for (int i = 0; i < xDays; i++)
	{
		date = IncreaseDateByOneDay(date);
	}

	return date;
}

stDate IncreaseDateByOneWeek(stDate date)
{
	return date = IncreaseDateByXDays(date, 7);
}

stDate IncreaseDateByXWeeks(stDate date, short xWeeks)
{
	for (int i = 0; i < xWeeks; i++)
	{
		date = IncreaseDateByOneWeek(date);
	}

	return date;
}

stDate IncreaseDateByOneMonth(stDate date)
{
	if (isLastMonthInYear(date.Month))	
	{
			date.Month = 1;
			date.Year++;
	}
	else
		date.Month++;

	//importent
	//last check day in date should not exceed max days in the current month
	//example if date is 31/1/2022 increasing one month should not be 31/2/2022, it should be 28/2/2022

	short NumberOfDaysInCurrentMonth = NumberOfDaysInMonth(date.Month, date.Year);

	if (date.Day > NumberOfDaysInCurrentMonth)
	{
		date.Day = NumberOfDaysInCurrentMonth;
	}

	return date;
}

stDate IncreaseDateByXMonths(stDate date, short xMonths)
{
	for (int i = 0; i < xMonths; i++)
	{
		date = IncreaseDateByOneMonth(date);
	}

	return date;
}

stDate IncreaseDateByOneYear(stDate date)
{
	date.Year++;
	return date;
}

stDate IncreaseDateByXYears(stDate date, short xYears)
{
	for (int i = 0; i < xYears; i++)
	{
		date = IncreaseDateByOneYear(date);
	}

	return date;
}

stDate IncreaseDateByXYearsFaster(stDate date, short xYears)
{
	date.Year += xYears;
	return date;
}

stDate IncreaseDateByOneDecade(stDate date)
{
	date.Year += 10;
	return date;
}

stDate IncreaseDateByXDecades(stDate date, short xDecades)
{
	for (int i = 0; i < xDecades; i++)
	{
		date = IncreaseDateByOneDecade(date);
	}

	return date;
}

stDate IncreaseDateByXDecadesFaster(stDate date, short xDecades)
{
	date.Year += (xDecades * 10);
	return date;
}

stDate IncreaseDateByOneCentury(stDate date)
{
	date.Year += 100;
	return date;
}

stDate IncreaseDateByOneMillennium(stDate date)
{
	date.Year += 1000;
	return date;
}

stDate DecreaseDateByOneDay(stDate date)
{
	if (date.Day == 1)
	{
		if (date.Month == 1)
		{
			date.Month = 12;
			date.Day = 31;
			date.Year--;
		}
		else
		{
			date.Month--;
			date.Day = NumberOfDaysInMonth(date.Month, date.Year);
		}
	}
	else
		date.Day--;

	return date;
}

stDate DecreaseDateByXDays(stDate date, short xDays)
{
	for (int i = 0; i < xDays; i++)
	{
		date = DecreaseDateByOneDay(date);
	}

	return date;
}

stDate DecreaseDateByOneWeek(stDate date)
{
	return date = DecreaseDateByXDays(date, 7);
}

stDate DecreaseDateByXWeeks(stDate date, short xWeeks)
{
	for (int i = 0; i < xWeeks; i++)
	{
		date = DecreaseDateByOneWeek(date);
	}

	return date;
}

stDate DecreaseDateByOneMonth(stDate date)
{
	if (date.Month == 1)
	{
		date.Month = 12;
		date.Year--;
	}
	else
		date.Month--;

	//importent
	//last check day in date should not exceed max days in the current month
	//example if date is 31/3/2022 decreasing one month should not be 31/2/2022, it should be 28/2/2022

	short NumberOfDaysInCurrentMonth = NumberOfDaysInMonth(date.Month, date.Year);

	if (date.Day > NumberOfDaysInCurrentMonth)
	{
		date.Day = NumberOfDaysInCurrentMonth;
	}

	return date;
}

stDate DecreaseDateByXMonths(stDate date, short xMonths)
{
	for (int i = 0; i < xMonths; i++)
	{
		date = DecreaseDateByOneMonth(date);
	}

	return date;
}

stDate DecreaseDateByOneYear(stDate date)
{
	date.Year--;
	return date;
}

stDate DecreaseDateByXYears(stDate date, short xYears)
{
	for (int i = 0; i < xYears; i++)
	{
		date = DecreaseDateByOneYear(date);
	}

	return date;
}

stDate DecreaseDateByXYearsFaster(stDate date, short xYears)
{
	date.Year -= xYears;
	return date;
}

stDate DecreaseDateByOneDecade(stDate date)
{
	date.Year -= 10;
	return date;
}

stDate DecreaseDateByXDecades(stDate date, short xDecades)
{
	for (int i = 0; i < xDecades; i++)
	{
		date = DecreaseDateByOneDecade(date);
	}

	return date;
}

stDate DecreaseDateByXDecadesFaster(stDate date, short xDecades)
{
	date.Year -= (xDecades * 10);
	return date;
}

stDate DecreaseDateByOneCentury(stDate date)
{
	date.Year -= 100;
	return date;
}

stDate DecreaseDateByOneMillennium(stDate date)
{
	date.Year -= 1000;
	return date;
}

short DayOfWeekOrder(stDate date)
{
	//this is overload function (i changed parameter)//

	return DayOfWeekOrder(date.Year, date.Month, date.Day);

}

short CalculateDaysFromTheBeginningOfYear(stDate date)
{
	//overload func
	return CalculateDaysFromTheBeginningOfYear(date.Year, date.Month, date.Day);
}

bool IsEndOfWeek(stDate date)
{
	return (DayOfWeekOrder(date) == 6);
		
}

bool IsWeekEnd(stDate date)
{
	//weekends are Fri & Sat
	short DayIndex = DayOfWeekOrder(date);
	return (DayIndex == 5 || DayIndex == 6);
}

bool IsBusinessDay(stDate date)
{
	return !(IsWeekEnd(date));
}

short DaysUntilTheEndOfWeek(stDate date)
{
	return 6 - DayOfWeekOrder(date); //substract from index 6
}

short DaysUntilTheEndOfMonth(stDate date)
{
	//my solution
	return NumberOfDaysInMonth(date.Month, date.Year) - date.Day;


	//-----------Abo Hadhoud Solution-----------//
	//stDate EndOfMonthDate;
	//
	//EndOfMonthDate.Day = NumberOfDaysInMonth(date.Month, date.Year);
	//EndOfMonthDate.Month = date.Month;
	//EndOfMonthDate.Year = date.Year;
	//
	//return GetDiffrenceInDays(date, EndOfMonthDate);

}

short DaysUntilTheEndOfYear(stDate date)
{
	//my solution
	if (isLeapYear(date.Year))
		return 366 - CalculateDaysFromTheBeginningOfYear(date);
	else
		return 365 - CalculateDaysFromTheBeginningOfYear(date);

	////-----------Abo Hadhoud Solution-----------//
	//stDate EndOfYearDate;
	//
	//EndOfYearDate.Day = 31;
	//EndOfYearDate.Month = 12;
	//EndOfYearDate.Year = date.Year;
	//
	//return GetDiffrenceInDays(date, EndOfYearDate);
}

short CalculateVacationDays(stDate StartDate, stDate EndDate)
{
	short VacationDaysCounter = 0;
	
	while (IsDate1BeforeDate2(StartDate, EndDate))
	{
		if (IsBusinessDay(StartDate))
			VacationDaysCounter++;
		
		StartDate = IncreaseDateByOneDay(StartDate);
	}
	return VacationDaysCounter;

}

stDate CalculateVacationReturnDate(stDate StartDate, short VacationDuration)
{

	for (int i = 0; i < VacationDuration; i++)
	{
		if (IsBusinessDay(StartDate))
			StartDate = IncreaseDateByOneDay(StartDate);
		else
		{
			StartDate = IncreaseDateByOneDay(StartDate);
			i--;
		}
	}

	return StartDate;

	////Abo Hadhoud Solution // longer //

	//short WeekEndCounter = 0;

	////in case the data  is weekend keep adding one day util you reach business day
	////we get rid of all weekends before the first business day
	//while (IsWeekEnd(StartDate))
	//{
	//	StartDate = IncreaseDateByOneDay(StartDate);
	//}

	////here we increase the vacation dates to add all weekends to it.

	//for (short i = 1; i <= VacationDuration + WeekEndCounter; i++)
	//{
	//	if (IsWeekEnd(StartDate))
	//		WeekEndCounter++;

	//	StartDate = IncreaseDateByOneDay(StartDate);
	//}

	////in case the return date is weekend keep adding one day util you reach business day
	//while (IsWeekEnd(StartDate))
	//{
	//	StartDate = IncreaseDateByOneDay(StartDate);
	//}

	//return StartDate;
}

bool IsDate1AfterDate2(stDate date1, stDate date2)
{ 
	return (!IsDate1BeforeDate2(date1, date2) && !IsDate1EqualsDate2(date1, date2));
}

DateCompareResults CompareDates(stDate date1, stDate date2)
{
	return IsDate1BeforeDate2(date1, date2) ? Before : IsDate1EqualsDate2(date1, date2) ? Equal : After;
}

short CompareDatesWithoutENUMS(stDate Date1, stDate Date2)
{
	return IsDate1AfterDate2(Date1, Date2) - IsDate1BeforeDate2(Date1, Date2); //will return number not enum
}

stPeriod ReadPeriod()
{
	stPeriod Period;

	cout << "\Date1:\n";
	Period.StartDate = ReadFullDate();

	cout << "\nDate2:\n";
	Period.EndDate = ReadFullDate();

	return Period;
}

bool IsOverlapPeriods(stPeriod period1, stPeriod period2)
{
	//short easy solution
	if (CompareDates(period1.EndDate, period2.StartDate) == Before
		||
		CompareDates(period2.EndDate, period1.StartDate) == Before)
		return false; //don't overlap
	else
		return true; //overlap


	// My Solution // long //

	//while (CompareDates(period1.StartDate, period2.StartDate) == Before)
	//{	
	//	period1.StartDate = IncreaseDateByOneDay(period1.StartDate);
	//	if (CompareDates(period1.StartDate, period2.StartDate) == Equal)
	//		return true;
	//	if (CompareDates(period1.StartDate, period1.EndDate) == Equal)
	//		break;
	//}
	//
	//while (CompareDates(period1.StartDate, period2.StartDate) == After)
	//{
	//	period2.StartDate = IncreaseDateByOneDay(period2.StartDate);
	//	if (CompareDates(period2.StartDate, period1.StartDate) == Equal)
	//		return true;
	//	if (CompareDates(period2.StartDate, period2.EndDate) == Equal)
	//		break;
	//}
	//
	//return false;

}

short GetPeriodLength(stPeriod period, bool IncludeEndDay = false)
{
	return GetDiffrenceInDays(period.StartDate, period.EndDate, IncludeEndDay);
}

bool IsDateWithinPeriod(stPeriod period ,stDate date)
{
	return !(CompareDates(date, period.StartDate) == Before || CompareDates(date, period.EndDate) == After);

	// My Solution // Bad //
	//short PeriodLength = GetPeriodLength(period, true);
	//for (int i = 0; i < PeriodLength; i++)
	//{
	//	if (CompareDates(period.StartDate, date) == Equal)
	//		return true;

	//	period.StartDate = IncreaseDateByOneDay(period.StartDate);
	//}
	//return false;

}

int CountOverlapDaysInPeriods(stPeriod period1, stPeriod period2)
{
	int Period1Length = GetPeriodLength(period1, true);
	int Period2Length = GetPeriodLength(period2, true);
	int OverlapDaysCounter = 0;

	if (IsOverlapPeriods(period1, period2) == false)
		return OverlapDaysCounter; //return 0

	if (Period1Length < Period2Length)
	{
		while (IsDate1BeforeDate2(period1.StartDate, period1.EndDate))
		{
			if (IsDateWithinPeriod(period2, period1.StartDate))
				OverlapDaysCounter++;

			period1.StartDate = IncreaseDateByOneDay(period1.StartDate);
		}
	}
	else
	{
		while (IsDate1BeforeDate2(period2.StartDate, period2.EndDate))
		{
			if (IsDateWithinPeriod(period1, period2.StartDate))
				OverlapDaysCounter++;

			period2.StartDate = IncreaseDateByOneDay(period1.StartDate);
		}
	}
	
	return OverlapDaysCounter;

}

bool IsValidDate(stDate date)
{
	if (date.Month > 12)
		return false;

	if (NumberOfDaysInMonth(date.Month, date.Year) < date.Day)
		return false;

	return true;

	//the logic // Abo Hadhoud Function //
	//if (date.Day < 1 || date.Day>31) 
	//	return false; 
	//if (date.Month < 1 || date.Month>12)
	//	return false; 

	//if (date.Month == 2) { 

	//	if (isLeapYear(date.Year)) {
	//		if (date.Day > 29) 
	//			return false; 
	//	} 
	//	else {
	//		if (date.Day > 28)
	//			return false; 
	//	} 
	//}
	//short DaysInMonth = NumberOfDaysInMonth(date.Month, date.Year);
	//
	//if (date.Day > DaysInMonth) 
	//	return false;
	//
	//return true;

}

vector <string> SplitString(string s, string separator)
{
	//func from course 7
	//separator is the same as delimeter
	vector <string> vStringWords;
	string word;
	short position = 0;
	while ((position = s.find(separator)) != s.npos)
	{
		word = s.substr(0, position);
		if (word != "")
			vStringWords.push_back(word);
		s.erase(0, position + separator.length());
	}
	if (s != "")
		vStringWords.push_back(s);
	return vStringWords;
}

stDate StringToDate(string date)
{
	stDate Date;
	vector <string> vDate;

	vDate = SplitString(date, "/");

	Date.Day = stoi(vDate[0]);
	Date.Month = stoi(vDate[1]);
	Date.Year = stoi(vDate[2]);

	return Date;
}

string DateToString(stDate date)
{
	return to_string(date.Day) + "/" + to_string(date.Month) + "/" + to_string(date.Year);
	//return example: 6/8/2004
}

string FormatDate_Mysolution(stDate date, short formatID)
{
	switch (formatID)
	{
	case 1:
		return to_string(date.Day) + "/" + to_string(date.Month) + "/" + to_string(date.Year);
	case 2:
		return to_string(date.Year) + "/" + to_string(date.Month) + "/" + to_string(date.Day);
	case 3:
		return to_string(date.Month) + "/" + to_string(date.Day) + "/" + to_string(date.Year);
	case 4:
		return to_string(date.Day) + "-" + to_string(date.Month) + "-" + to_string(date.Year);
	case 5:
		return to_string(date.Year) + "-" + to_string(date.Month) + "-" + to_string(date.Day);
	default:
		return "Day:" + to_string(date.Day) + ", Month:" + to_string(date.Month) + ", Year:" + to_string(date.Month);
	}
}

string ReplaceWordUsingBuiltInFunction(string s, string WordToReplace, string NewWord) //using built-in funcation
{
	int WordPosition = s.find(WordToReplace);
	if (WordPosition == s.npos)
		return "\nWord Not Found\n";
	while (WordPosition != s.npos)//use while to check for multiple words
	{
		s.replace(WordPosition, WordToReplace.length(), NewWord);// built-in funcation
		WordPosition = s.find(WordToReplace); //check if there is another word
	}
	return s;
}
string FormatDate(stDate date, string DateFormat = "dd/mm/yyyy")
{
	string FormattedDateString = "";

	FormattedDateString = ReplaceWordUsingBuiltInFunction(DateFormat, "dd", to_string(date.Day));
	FormattedDateString = ReplaceWordUsingBuiltInFunction(FormattedDateString, "mm", to_string(date.Month));
	FormattedDateString = ReplaceWordUsingBuiltInFunction(FormattedDateString, "yyyy", to_string(date.Year));

	return FormattedDateString;

}

int main()
{
	stDate Date1;
	string DateString = ReadString("Enter Date: (DD/MM/YYYY): ");

	Date1 = StringToDate(DateString);

	cout << FormatDate(Date1) << endl;
	cout << FormatDate(Date1, "mm/dd/yyyy") << endl;
	cout << FormatDate(Date1, "yyyy/mm/dd") << endl;
	cout << FormatDate(Date1, "Day = dd - Month = mm - Year = yyyy") << endl;



	cout << endl;
	return 0;
}
