#include <iostream>
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
	}
	else                                                        // иначе
	{
		if ((switches_state & LIGHTS_INSIDE) &&                 // если выключатель внутреннего освещения включен и
			(hour == 0) &&                                      // переданное время равно 00:00 и
			!(switches_state & POWER_SUPPLY))                   // электропитание дома не выключено
		{
			illumination = 5000;                                // сбрасываем освещённость на максимум
		}
	}
	                                                            // При этом:
	if ((switches_state & LIGHTS_INSIDE) &&                     // если выключатель внутреннего освещения включен и
		!(switches_state & POWER_SUPPLY) &&                     // электропитание дома не выключено и
		hour >= 20)                                             // переданное время больше или равно 20:00
	{
		cout << " Освещённость помещения составляет: "          // выводим последнее вычесленное значение освещённости в 20:00
			<< illumination << "K." << endl;
	}
	else
	{
		if ((switches_state & LIGHTS_INSIDE) &&                 // если выключатель внутреннего освещения включен и
			!(switches_state & POWER_SUPPLY))                   // электропитание дома не выключено и
		{
			cout << " Освещённость помещения составляет: "      // выводим максимальное или вычесленное с 16:00 до 19:00 значение освещённости
				<< illumination << "K." << endl;
		}
	}
}

void temperature_control_inside(int t)
{

}

void temperature_control_outside(int t)
{

}

void output_time(int c)
{
	string time;
	if (c < 10)
	{
		cout << " Время: 0" << c << ":00." << endl;
	}
	else
	{
		if (c > 9)
		{
			cout << " Время: " << c << ":00." << endl;
		}
		
	}
	for (int j = 0; j < 6; j++)                                          // Цикл for выводит названия датчиков из массива
	{
		cout << ' ' << name_switch[j];
	}
	cout << endl << " ";
}

void input_check()
{
	bool chek = true;
	while (chek)                                                             // Цикл продолжается до тех пор, пока пользователь 
	{                                                                        // не введёт корректное значение
		cin >> fire_control                                                  // Записываем данные датчиков и переключателей
			>> flood_control
			>> motion_sensor
			>> l_inside
			>> t_inside
			>> t_outside;

		if ((cin.fail() || cin.peek() != '\n') ||
			(fire_control != "ON" && fire_control != "OFF" ||                 // Если предыдущее извлечение оказалось неудачным,
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

	for (int k = 0; k < 2; k++)                                              // Цикл for отсчитывает двое суток
	{
		for (int i = 0; i < 24; ++i)                                         // Вложенный for считает часы от 0 до 23 
		{
			output_time(i);
			input_check();
			switch_fire_control(fire_control);
			switch_flood_control(flood_control);
			switch_motion_sensor(motion_sensor, i);
			switch_light_inside(l_inside, i);

			cout << endl;
		}
	}
	system("pause>nul");
	return 0;
}