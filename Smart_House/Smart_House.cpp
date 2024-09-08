﻿#include <iostream>
#include <sstream>
#include <cstdlib>
#include <string>

using namespace std;

string fire_control;                                            // Переменная для записи положения датчика пожара
string flood_control;                                           // Переменная для записи положения датчика затопления дома
string motion_sensor;                                           // Переменная для записи положения датчика движения
string l_inside;                                                // Переменная для записи положения выключателя света в доме 
int t_inside;                                                   // Переменная для записи положения датчика температуры в доме
int t_outside;                                                  // Переменная для записи положения датчика температуры на улице
int illumination = 5000;                                        // Переменная для записи освещённости помещения
int switches_state = 0;                                         // Переменная для записи состояния датчиков

string name_switch[6]{                                          // Массив названий датчиков                 
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

// Метод для управления питанием всего дома датчиком пожара.
void switch_fire_control(string text)                           // Передаём методу положение датчика пожара (ON или OFF)
{
	if ((text == "ON") &&                                       // если передано включение датчика пожара (ON) и
		!(switches_state & POWER_SUPPLY))                       // электропитание дома не выключено
	{
		cout << " Пожар! Дом обесточен!" << endl;               // выводим сообщение об отключении электропитания дома и о пожаре
		switches_state |= POWER_SUPPLY;                         // и в переменную switches_state добавляем флаг POWER_SUPPLY
	}
	else                                                        // иначе
	{
		if ((text == "OFF") &&                                  // если передано вЫключение датчика пожара (OFF) и
			(switches_state & POWER_SUPPLY))                    // электропитание дома выключено
		{
			cout << " Питание дома включено!" << endl;          // выводим сообщение о включении электропитания дома
			switches_state &= ~POWER_SUPPLY;                    // и из переменной switches_state удаляем флаг POWER_SUPPLY
		}
	}
}

// Метод для управления питанием розеточной группы дома датчиком затопления.
void switch_flood_control(string text)                          // Передаём методу положение датчика затопления (ON или OFF)
{
	if ((text == "ON") &&                                       // если передано включение датчика затопления (ON) и
		!(switches_state & POWER_SUPPLY) &&                     // электропитание дома не выключено и
		!(switches_state & ELECTRICAL_SOCKETS))                 // датчик затопления не был включен
	{
		cout << " Затопление помещения!";                       // выводим сообщение об отключении розеточной группы
		cout << " Розетки дома отключены!" << endl;
		switches_state |= ELECTRICAL_SOCKETS;                   // и в переменную switches_state добавляем флаг ELECTRICAL_SOCKETS
	}
	else                                                        // иначе
	{
		if ((text == "OFF") &&                                  // если передано вЫключение датчика затопления (OFF) и
			!(switches_state & POWER_SUPPLY) &&                 // электропитание дома не выключено и
			(switches_state & ELECTRICAL_SOCKETS))              // датчик затопления включен
		{
			cout << " Питание розеток включено!" << endl;       // выводим сообщение о включении розеточной группы
			switches_state &= ~ELECTRICAL_SOCKETS;              // и из переменной switches_state удаляем флаг ELECTRICAL_SOCKETS
		}
	}
}

// Метод для управления освещением во дворе дома датчиком движения по времени.
void switch_motion_sensor(string text, int hour)                // Передаём методу положение датчика движения (ON или OFF)
{                                                               // и врямя суток
	if ((text == "ON") &&                                       // если передано включение датчика движения (ON) и
		!(switches_state & POWER_SUPPLY) &&                     // электропитание дома не выключено и
		(hour > 16 || hour < 5) &&                              // время болше 16:00 или меньше 05:00 и
		!(switches_state & LIGHTS_OUTSIDE))                     // наружное освещение не было включено
	{
		cout << " Датчик движения сработал!";                   // выводим сообщение о включении наружнего освещения
		cout << " Наружное освещение включено!" << endl;
		switches_state |= LIGHTS_OUTSIDE;                       // и в переменную switches_state добавляем флаг LIGHTS_OUTSIDE
	}
	else                                                        // иначе
	{
		if (((text == "OFF") &&                                 // если переданно вЫключение датчика движения (OFF) и
			!(switches_state & POWER_SUPPLY) &&                 // электропитание дома не выключено и
			!(hour <= 16 && hour >= 5) &&                       // переданное время не меньше или равно 16:00 и не больше или равно 05:00 и
			(switches_state & LIGHTS_OUTSIDE)) ||               // наружное освещение включено или
			((hour <= 16 && hour >= 5) &&                   // переданное время меньше или равно 16:00 и больше или равно 05:00 и
				!(switches_state & POWER_SUPPLY) &&             // электропитание дома не выключено и
				(switches_state & LIGHTS_OUTSIDE)))             // наружное освещение включено
		{
			cout << " Наружное освещение вЫключено!" << endl;   // выводим сообщение о включении наружнего освещения
			switches_state &= ~LIGHTS_OUTSIDE;                  // и из переменной switches_state удаляем флаг LIGHTS_OUTSIDE
		}
	}
}



int main()
{
	system("chcp 1251>nul");
	system("color 70");

	system("pause>nul");
	return 0;
}