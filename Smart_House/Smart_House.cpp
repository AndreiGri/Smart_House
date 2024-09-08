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

int main()
{
	system("chcp 1251>nul");
	system("color 70");

	system("pause>nul");
	return 0;
}