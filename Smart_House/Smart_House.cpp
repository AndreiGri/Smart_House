#include <iostream>
#include <sstream>
#include <cstdlib>
#include <string>

using namespace std;

string fire_control;                             // Переменная для записи положения датчика пожара
string flood_control;                            // Переменная для записи положения датчика затопления дома
string motion_sensor;                            // Переменная для записи положения датчика движения
string l_inside;                                 // Переменная для записи положения выключателя света в доме 
int t_inside;                                    // Переменная для записи положения датчика температуры в доме
int t_outside;                                   // Переменная для записи положения датчика температуры на улице
int illumination = 5000;                         // Переменная для записи освещённости помещения
int switches_state = 0;                          // Переменная для записи состояния датчиков

string name_switch[6]{                           // Массив названий датчиков                 
	" Датчик пожара,",
	" Датчик затопления,",
	" Датчик движения,",
	" Выключатель в доме,",
	" Температура внутренняя,",
	" Температура наружняя: "
};

enum switches                                                   // Переечисление флагов состояния оборудования
{
	POWER_SUPPLY = 1,                                           // Электропитание дома
	ELECTRICAL_SOCKETS = 2,                                     // Электропитание розеточной группы
	LIGHTS_INSIDE = 4,                                          // Освещение внутри дома
	LIGHTS_OUTSIDE = 8,                                         // Освещение снаружи
	HEATERS = 16,                                               // Отопление дома
	WATER_PIPE_HEATING = 32,                                    // Электрообогрев труб водоснабжения
	CONDITIONER = 64                                            // Кондиционер
};

int main()
{
	system("chcp 1251>nul");
	system("color 70");

	system("pause>nul");
	return 0;
}