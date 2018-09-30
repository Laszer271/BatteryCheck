#include <iostream>
#include <windows.h>
#include <ctime>
#include <fstream>
#include <string>



void currentTime(std::ostream& str = std::cout, int i = 11);

void showBatteryStatus(double i, std::ostream& str = std::cout);

int option(const unsigned opt, std::ostream& str = std::cout, std::istream& istr = std::cin);

int main()
{
	bool program = true;
	while (program)
	{
		std::cout << "1. Check battery status and show results in a console \n";
		std::cout << "2. Check battery status and save results to a file \n";
		std::cout << "3. Exit\n" << std::endl;

		switch (option(3))
		{
		case 1:
			std::cout << "\nWhat intervals (in seconds) should be\nbetween each check of the status of the battery: ";
			double sec;
			std::cin >> sec;

			system("cls");
			std::cout << "You can exit the program at all times\nby clicking X in top-right corner\n";
			std::cout << "or by clicking Ctrl+C on your keyboard\n" << std::endl;

			showBatteryStatus(sec, std::cout);
			break;

		case 2:
			while (true)
			{
				struct stat info;
				if (stat("src/BatteryReports/", &info) != 0)
					CreateDirectory("src/BatteryReports/", NULL);

				std::cout << "What should the file be named: ";
				std::string B;
				std::cin >> B;
				std::string temp(".txt");
				if (B.size() < 5 || B.substr(B.size() - 4) != ".txt")
					B += ".txt";

				std::fstream file;
				file.open("src/BatteryReports/" + B, std::ios::out);

				if (file.is_open())
				{
					std::cout << "\nWhat intervals (in seconds) should be\nbetween each check of the status of the battery: ";
					double sec;
					std::cin >> sec;

					system("cls");
					std::cout << "All data is being saved to file " << B << " in BatteryReports dictionary \n";
					std::cout << "You can stop collecting data and exit the program at all times\nby clicking X in top-right corner\n";
					std::cout << "or by clicking Ctrl+C on your keyboard\n" << std::endl;
					showBatteryStatus(sec, file);

					file.close();
					break;
				}
				std::cout << "There was an error! Try once more" << std::endl;
			}
			break;

		case 3:
			program = false;
			break;
		default:
			std::cout << "ERROR";
			program = false;
			break;
		}
	}
	return 0;
}

void showBatteryStatus(double i, std::ostream& str)
{
	i *= 1000.0;

	SYSTEM_POWER_STATUS spsPwr;

	str << "Start date: ";
	currentTime(str, 4);
	str << "\n" << std::endl;

	while (GetSystemPowerStatus(&spsPwr))
	{
		str << "Time: ";
		currentTime(str);

		int AC = static_cast<int>(spsPwr.ACLineStatus);
		str << "   " << "Charging: ";
		if (AC == 0) str << "NO";
		else if (AC == 1) str << "YES";
		else str << "UNKNOWN";

		int BLP = static_cast<int>(spsPwr.BatteryLifePercent);
		str << "   " << "Battery Life: ";
		if (BLP != 255) str << BLP << "%" << std::endl;
		else str << "UNKNOWN" << std::endl;

		str.flush();
		Sleep(i);
	}
}

void currentTime(std::ostream& str, int i)
{
	time_t my_time = time(NULL);
	char tempBuffer[26];
	ctime_s(tempBuffer, 26, &my_time);

	for (; i < 19; i++)
	{
		str << tempBuffer[i];

	}
}

int option(const unsigned opt, std::ostream& str, std::istream& istr)
{
	std::string a;
	str << "Pick your option: ";

	while (true)
	{
		istr >> a;
		for (unsigned i = 1; i <= opt; i++)
		{
			if (a == std::to_string(i)) return i;
		}
		str << "There is no such an option. Try once more: ";
	}
}