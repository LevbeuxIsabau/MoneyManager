#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <conio.h>
#include <windows.h>
#include <windows.graphics.h>
#include <typeinfo>
#include <iomanip>

bool FileIsEmpty(std::string Filepath)
{
	struct stat buffer;
	stat(Filepath.c_str(), &buffer);
	return (buffer.st_size == 0);
}

int FileSize(std::string filepath)
{
	std::fstream file;
	std::string line;
	int filesize = 0;
	file.open(filepath);
	if (file.is_open()) {
		while (std::getline(file, line)) {
			filesize++;
		}
		file.close();
	}
	
	return filesize;
}

void LoadBase(std::string filepath)
{
/*
	agregar opcion de llenar la base al momento
	de crear el archivo
*/
	std::fstream file;
	file.open(filepath, std::ios::app);
	if (file.is_open()) {
		file << "Saldo: 0" << std::endl << "--/---/---- @ --:-- --" << std::endl
			<< "Concepto: " << std::endl << "Monto: +0" << std::endl << "Banco : " << std::endl
			<< "Diferencia : 0" << std::endl << "--------------------" << std::endl;
		file.close();
	}
}

double CaptureLine(std::string filepath, int fileline, int linesize)
{
	std::fstream file;
	std::string n_str, num_n;
	int current_line = 0, n_size, filesize = FileSize(filepath);
	char ch_n;
	double n_float = 0.0;
	int n_line = filesize - fileline;
	file.open("registros.txt");
	if (file.is_open()) {
		while (!file.eof()) {
			current_line++;
			std::getline(file, n_str);
			if (current_line == n_line) break;
		}
		if (current_line < n_line) {
			std::cout << "Error" << std::endl;
			return 1;
		}
		file.close();
	}

	n_size = n_str.length();
	for (int i = linesize; i < n_size; i++) {
		ch_n = n_str.at(i);
		num_n.push_back(ch_n);
	}
	
	n_float = std::stof(num_n);
	

	return n_float;
}


//Menu
std::string CreateUser() {
	std::string username;

	std::cout << "Ingresa el nombre de usuario: ";
	std::getline(std::cin, username);
	username += ".txt";

	std::fstream file;

	file.open(username);
	if (file.is_open()) {
		//Usuario ya existe
		std::cout << "Usuario ya regitrado\n";
		file.close();
	}
	else {
		//Crear Archivo
		std::cout << "Creando Archivo\n";
		file.close();
		file.open(username, std::ios::out);
		if (!file.is_open()) {
			std::cout << "Error\n";
		}
		else {
			file << "";
			std::cout << "Archivo creado exitosamente\n";
			file.close();
		}
	}
	file.close();
	return username;

}

void CreatePass(std::string filepath) {
	std::string pass;
	std::fstream file;

	std::cout << "Ingresa una contraseña: ";
	std::getline(std::cin, pass);

	file.open(filepath, std::ios::app);
	if (!file.is_open()) {
		std::cout << "Error, Archivo no se puede abrir\n";
	}
	else {
		file << pass << "\n";
		std::cout << "Contraseña guardada exitosamente\n";
		file.close();
	}
	file.close();
}

void CreateProfile() {
	std::string userpath;
	userpath = CreateUser();
	CreatePass(userpath);
}

void LogIn() {
	bool usuario = false, password = false;
	std::string user, pass, filepath, filepass;
	std::fstream file;
	do {
		std::cout << "Ingresa tu nombre de usuario: ";
		std::getline(std::cin, user);
		filepath = user + ".txt";
		file.open(filepath);
		if (!file.is_open()) {
			//usuario incorrecto
			std::cout << "Usuario incorrecto\n";
			file.close();
		}
		else {
			usuario = true;
			std::cout << "Usuario Correcto\n";
			file.close();
		}
	} while (false == usuario);

	do {
		//usuario correcto
		file.open(filepath, std::ios::in);
		if (file.is_open()) {
			//contraseña
			std::getline(file, filepass);
			std::cout << "Ingresa la contraseña: ";
			std::getline(std::cin, pass);
			if (pass != filepass) {
				//contraseña incorrecta
				std::cout << "Contraseña incorrecta\n";
			}
			else {
				password = true;
				//contraseña correcta
				std::cout << "Contraseña correcta\nPresiona cualquier tecla para continuar...";
				_getch();
				system("cls");
				std::cout << "Bienvenido " << user << "!\n";
			}
		}

	} while (false == password);
}

void Menu_registro() {
	bool salir = false;
	int opcion;
	do {
		std::cout << "Ingrese una opcion: \n"
			<< "1. Registrarse\n"
			<< "2. Iniciar Sesion\n"
			<< "3. Salir\n"
			<< ">> ";
		std::cin >> opcion;
		std::cin.ignore();
		if (1 == opcion) {
			//Registro
			system("cls");
			CreateProfile();
		}
		else if (2 == opcion) {
			//Inicio de sesion
			system("cls");
			LogIn();
			salir = true;
		}
		else if (3 == opcion) {
			//salir
			std::cout << "Saliendo...";
			salir = true;
		}
	} while (false == salir);

}
//



int main() {

	//Variables necesarias
	std::fstream file;
	std::string line, filepath = "registros.txt";
	std::vector <std::string> Data(4);
		//Data: [0]Fecha, [1]Hora, [2]Concepto, [3]Banco
	int filesize = 0, s_fileline = 6, d_fileline = 1, m_fileline = 3,
		s_linesize = 7, d_linesize = 13, m_linesize = 7;
	double dif = 0.0, mon = 0.0, sal;
	bool WrongData = true;
	//Colores
	HANDLE hconsole;
	int red = 12, green = 10, def = 7;
	hconsole = GetStdHandle(STD_OUTPUT_HANDLE);

	




	while (FileIsEmpty(filepath)) {
		file.open(filepath, std::ios::out);
		if (file.is_open()) {
			file << "";
			LoadBase(filepath);
			file.close();
		}
		std::cout << "File created successfully!" << std::endl;
	}

	std::cout << "Saldo actual: " << std::setprecision(8) << CaptureLine(filepath, d_fileline, d_linesize) << std::endl;
	
	std::cout << "Fecha [DD/MMM/AAAA ej. 10/FEB/2016]: ";
	std::getline(std::cin, Data[0]);

	std::cout << "Hora [HH:MM AM/PM ej. 02:45 PM]: ";
	std::getline(std::cin, Data[1]);

	std::cout << "Concepto: ";
	std::getline(std::cin, Data[2]);

	std::cout << "Banco: ";
	std::getline(std::cin, Data[3]);
	
	do{
		std::cout << "+: Abono\t-: Cargo" << std::endl;
		std::cout << "Monto [+/-###.## ej. +150.50]: ";
		std::cin >> mon;
		
		if (!mon) {
			std::cout << "Error: Ingresa datos correctos." << std::endl;
			std::cin.clear();
			std::cin.ignore(10000000, '\n');
		}
		else {
			break;
		}
	}
	while (!mon);
	std::cout << "Data saved successfuly" << std::endl;
	

	dif = (CaptureLine(filepath, d_fileline, d_linesize)) + mon;
	sal = CaptureLine(filepath, d_fileline, d_linesize);

	system("cls");
	file.open(filepath, std::ios::app);
	if (file.is_open()) {
		file << "Saldo: " << std::fixed << std::setprecision(2) << sal << std::endl;
		file << Data[0] << " @ " << Data[1] << std::endl
			<< "Concepto: " << Data[2] << std::endl;
		std::cout << "Saldo: " << std::fixed << std::setprecision(2) << sal << std::endl;
		std::cout << Data[0] << " @ " << Data[1] << std::endl
			<< "Concepto: " << Data[2] << std::endl;

		if (mon > 0) {
			SetConsoleTextAttribute(hconsole, green);
			file << "Monto: +" << mon << std::endl;
			std::cout << "Monto: +" << mon << std::endl;
			SetConsoleTextAttribute(hconsole, def);
		}
		if (mon < 0) {
			SetConsoleTextAttribute(hconsole, red);
			file << "Monto: " << mon << std::endl;
			std::cout << "Monto: " << mon << std::endl;
			SetConsoleTextAttribute(hconsole, def);
		}
		if (mon == 0){
			SetConsoleTextAttribute(hconsole, def);
			file << "Monto: " << mon << std::endl;
			std::cout << "Monto: " << mon << std::endl;
		}
		
		
		file << "Banco : " << Data[3] << std::endl
			<< std::fixed << std::setprecision(2) << "Diferencia : " << dif << std::endl << "--------------------" << std::endl;
		std::cout << "Banco : " << Data[3] << std::endl
			<< std::fixed<< std::setprecision(2) << "Diferencia : " << dif << std::endl << "--------------------" << std::endl;
		file.close();
	}
	

	


	return 0;
}