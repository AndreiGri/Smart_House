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
int illum;                                                      // Переменная для записи минимального значения освещённости
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
		(hour >= 16 || hour < 5) &&                             // время болше 16:00 или меньше 05:00 и
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

// Метод для управления включения освещения внутри дома выключателем и управления освещённости по времени. 
void switch_light_inside(string text, int hour)                 // Передаём методу положение выключателя (ON или OFF) 
{                                                               // и врямя суток
	if ((text == "ON") &&                                       // если передано включение (ON) и
		!(switches_state & POWER_SUPPLY) &&                     // электропитание дома не выключено и 
		!(switches_state & LIGHTS_INSIDE))                      // выключатель внутреннего освещения до этого не был включен
	{
		cout << " Внутреннее освещение включено!" << endl;      // выводим сообщение о включении внутреннего освещения
		switches_state |= LIGHTS_INSIDE;                        // и в переменную switches_state добавляем флаг LIGHTS_INSIDE
	}
	else                                                        // иначе
	{
		if ((text == "OFF") &&                                  // если переданно вЫключение (OFF) и 
			!(switches_state & POWER_SUPPLY) &&                 // электропитание дома не выключено и
			(switches_state & LIGHTS_INSIDE))                   // выключатель внутреннего освещения включен
		{
			cout << " Внутреннее освещение вЫключено!" << endl; // выводим сообщение о вЫключении внутреннего освещения
			switches_state &= ~LIGHTS_INSIDE;                   // и из переменной switches_state удаляем флаг LIGHTS_INSIDE
		}
	}
	                                                            // При этом:
	if ((switches_state & LIGHTS_INSIDE) &&                     // если выключатель внутреннего освещения включен и 
		(hour >= 16 && hour <= 20) &&                           // переданное время больше или равно 16:00 или меньше или равно 20:00 и
		!(switches_state & POWER_SUPPLY))                       // электропитание дома не выключено
	{
		illumination -= ((hour - 15) * 460);                    // из переменной illumination вычитаем долю освещённости на данный час
		illum = illumination;
		cout << " Освещённость помещения составляет: "          // выводим освещённость
			 << illumination << "K." << endl;
		illumination = 5000;                                    // сбрасываем освещённость на максимум
	}
	else                                                        // иначе
	{
		if ((switches_state & LIGHTS_INSIDE) &&                 // если выключатель внутреннего освещения включен и
			(hour >= 0 && hour < 16) &&                         // переданное время равно 00:00 и
			!(switches_state & POWER_SUPPLY))                   // электропитание дома не выключено
		{
			cout << " Освещённость помещения составляет: "      // выводим освещённость
				 << illumination << "K." << endl;               
		}
		else
		{
			if ((switches_state & LIGHTS_INSIDE) &&             // если выключатель внутреннего освещения включен и
				!(switches_state & POWER_SUPPLY) &&             // электропитание дома не выключено и
				hour > 20)                                      // переданное время больше или равно 20:00
			{
				cout << " Освещённость помещения составляет: " 
					 << illum << "К." << endl;                  // выводим последнее вычесленное значение освещённости в 20:00
			}
			
		}
	}
}

// Метод по показаниям датчика температуры внутри дома управляет отоплением и кондиционером
void temperature_control_inside(int t)                          // Передаём методу температуру в доме
{
	if (t < 22 &&                                               // Если температура меньше 22 градусов и
		!(switches_state & HEATERS) &&                          // не установлен флаг включенного отопления и
		!(switches_state & POWER_SUPPLY) &&                     // не установлен флаг отключения электропитания дома и
		!(switches_state & ELECTRICAL_SOCKETS))                 // не установлен флаг отключения розеточной группы
	{
		cout << " Отопление в доме включено!" << endl;          // выводим сообщение о включении отопления
		switches_state |= HEATERS;                              // устанавливаем флаг включения отопления
	}

	if (t >= 25 &&                                              // если температура более или равно 25 градусов и
		(switches_state & HEATERS) &&                           // установлен флаг включенного отопления и
		!(switches_state & POWER_SUPPLY) &&                     // не установлен флаг отключения электропитания дома и
		!(switches_state & ELECTRICAL_SOCKETS))                 // не установлен флаг отключения розеточной группы
	{
		cout << " Отопление в доме отключено!" << endl;         // выводим сообщение о вЫключении отопления
		switches_state &= ~HEATERS;                             // снимаем флаг включения отопления
	}

	if (t >= 30 &&                                              // если температура более или равно 30 градусов и
		!(switches_state & CONDITIONER) &&                      // не установлен флаг включения кондиционера и
		!(switches_state & POWER_SUPPLY) &&                     // не установлен флаг отключения электропитания дома и
		!(switches_state & ELECTRICAL_SOCKETS))                 // не установлен флаг отключения розеточной группы
	{
		cout << " Кондиционер включен!" << endl;                // выводим сообщение о включении кондиционера
		switches_state |= CONDITIONER;                          // устанавливаем флаг включения кондиционера
	}

	if (t <= 25 &&                                              // Если температура меньше или равно 22 градусов и
		(switches_state & CONDITIONER) &&                       // установлен флаг включения кондиционера и
		!(switches_state & POWER_SUPPLY) &&                     // не установлен флаг отключения электропитания дома и
		!(switches_state & ELECTRICAL_SOCKETS))                 // не установлен флаг отключения розеточной группы
	{
		cout << " Кондиционер вЫключен!" << endl;               // выводим сообщение о вЫключении кондиционера
		switches_state &= ~CONDITIONER;                         // снимаем флаг включения кондиционера
	}
}
// Метод по показаниям датчика наружной температуры управляет электрообогревом трубопровода
void temperature_control_outside(int t)                         // Передаём методу температуру на улице
{
	if ((t < 0) &&                                              // Если температура меньше 0 градусов и
		!(switches_state & WATER_PIPE_HEATING) &&               // не установлен флаг включения обогрева труб
		!(switches_state & POWER_SUPPLY) &&                     // не установлен флаг отключения электропитания дома и
		!(switches_state & ELECTRICAL_SOCKETS))                 // не установлен флаг отключения розеточной группы
	{
		cout << " Обогрев трубопровода включен!" << endl;       // выводим сообщение о включении обогрева труб
		switches_state |= WATER_PIPE_HEATING;                   // устанавливаем флаг включения обогрева труб
	}

	if ((t > 5) &&                                              // Если температура больше 5 градусов и
		(switches_state & WATER_PIPE_HEATING) &&                // установлен флаг включения обогрева труб
		!(switches_state & POWER_SUPPLY) &&                     // не установлен флаг отключения электропитания дома и
		!(switches_state & ELECTRICAL_SOCKETS))                 // не установлен флаг отключения розеточной группы
	{
		cout << " Обогрев трубопровода отключен!" << endl;      // выводим сообщение о вЫключении обогрева труб
		switches_state &= ~WATER_PIPE_HEATING;                  // снимаем флаг включения обогрева труб
	}
}

// Метод вывода имитации времени и перечисления датчиков
void output_time(int c)                                         // Передаём переменную счётчика циклов для формирования
{                                                               // имитации часов
	if (c < 10)                                                 // если переменная меньше десяти
	{
		cout << " Время: 0" << c << ":00." << endl;             // выводим строку
	}
	else                                                        // иначе
	{
		cout << " Время: " << c << ":00." << endl;              // выводим другую строку
	}
	for (int j = 0; j < 6; j++)                                 // Цикл for выводит названия датчиков из массива name_switch
	{
		cout << ' ' << name_switch[j];
	}
	cout << endl << " ";
}

// Метод для проверки корректности ввода данных
void input_check()
{
	bool chek = true;                                           // булева переменная для определения окончания проверки
	while (chek)                                                // Цикл продолжается до тех пор, пока пользователь 
	{                                                           // не введёт корректное значение
		cin >> fire_control                                     // Записываем данные датчиков и переключателей
			>> flood_control
			>> motion_sensor
			>> l_inside
			>> t_inside
			>> t_outside;

		if ((cin.fail() || cin.peek() != '\n') ||
			(fire_control != "ON" && fire_control != "OFF" ||                // Если предыдущее извлечение оказалось неудачным,
				flood_control != "ON" && flood_control != "OFF" ||
				motion_sensor != "ON" && motion_sensor != "OFF" ||
				l_inside != "ON" && l_inside != "OFF" ||
				t_inside < -35 && t_inside > 35 ||
				t_outside < -35 && t_outside > 35))
		{
			cerr << " Ошибка ввода данных! Повторите ещё раз!\n ";
			cin.clear();                                                     // то возвращаем cin в 'обычный' режим работы
			cin.ignore(numeric_limits<streamsize>::max(), '\n');             // и удаляем значения предыдущего ввода из входного 
		}                                                                    // буфера
		else
		{
			chek = false;
		}
	}
}

int main()
{
	system("chcp 1251>nul");
	system("color 70");

	cout << " Введите показания приборов в последовательности " << endl;     // Запрашиваем данные датчиков и переключателей
	cout << " выведенной в консоль, датчик пожара: ON или OFF, " << endl;
	cout << " датчик затопления: ON или OFF, датчик движения: ON или OFF," << endl;
	cout << " выключатель в доме: ON или OFF, температура внутренняя: от -35 до 35" << endl;
	cout << " температура наружняя: от -35 до 35" << endl;
	cout << endl;

	for (int k = 0; k < 2; k++)                                              // Цикл for отсчитывает двое суток
	{
		for (int i = 0; i < 24; ++i)                                         // Вложенный for считает часы от 0 до 23 
		{
			output_time(i);                                                  // последовательно вызываем методы
			input_check();
			switch_fire_control(fire_control);
			switch_flood_control(flood_control);
			switch_motion_sensor(motion_sensor, i);
			switch_light_inside(l_inside, i);
			temperature_control_inside(t_inside);
			temperature_control_outside(t_outside);

			for (int i = 0; i < 120; i++)                                   // цикл для формирования разделителя между часами
			{
				cout << '-';
			}
			cout << endl;
		}
	}
	system("pause>nul");
	return 0;
}