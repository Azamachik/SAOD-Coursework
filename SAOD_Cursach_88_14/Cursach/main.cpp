#include <iostream>
#include "CityTransport.h"
#include "Storage.h"
#include <io.h>
#include <fcntl.h>
#include <Windows.h>
#ifdef max
#undef max
#endif

using namespace std;

CityTransport* transport = NULL;
Storage file("transport_data.txt");

int failure() {
    int a;
    while (!(cin >> a) || (cin.peek() != '\n')) {
        cin.clear();
        while (cin.get() != '\n');
        cout << "\nОшибка ввода. Повторите ввод: ";
    }
    return a;
}
void AddPark() {
    int parkNumber;
    bool validInput = false;
    while (!validInput) {
        cout << "Введите номер трамвайного парка: ";
        parkNumber = failure();

        if (parkNumber <= 0) {
            cout << "Ошибка: номер парка должен быть положительным числом! (должен быть > 0)\n";
        }
        else {
            validInput = true;
        }
    }
    transport->AddPark(parkNumber);
}
void RemovePark() {
    int parkNumber;
    cout << "Введите номер трамвайного парка для удаления: ";
    parkNumber = failure();

    if (transport->RemovePark(parkNumber)) {
        cout << "Трамвайный парк №" << parkNumber << " удален.\n";
    }
    else {
        cout << "Трамвайный парк не найден.\n";
    }
}

void FindPark() {
    int parkNumber;
    cout << "Введите номер трамвайного парка для поиска: ";
    parkNumber = failure();

    TramPark* park = transport->FindPark(parkNumber);
    if (park != NULL) {
        cout << "Трамвайный парк №" << parkNumber << " найден:\n";
        park->ShowTrams();
    }
    else {
        cout << "Трамвайный парк не найден.\n";
    }
}

void AddTram() {
    int parkNumber;
    cout << "Введите номер трамвайного парка: ";
    parkNumber = failure();

    TramPark* park = transport->FindPark(parkNumber);
    if (park == NULL) {
        cout << "Трамвайный парк не найден.\n";
        return;
    }

    string model;
    int year = 0;

    bool validModel = false;
    while (!validModel) {
        cout << "Введите марку трамвая: ";
        cin.clear();
        cin.ignore(32767, '\n');
        getline(cin, model);

        for (char c : model) {
            if (isalpha(c)) {
                validModel = true;
                break;
            }
        }

        if (!validModel) {
            cout << "Ошибка: название модели должно содержать хотя бы одну букву!\n";
        }
    }

    while (year <= 0) {
        cout << "Введите год выпуска: ";
        year = failure();

        if (year <= 0) {
            cout << "Ошибка: год выпуска должен быть положительным числом!\n";
        }
    }

    Tram* tram = new Tram();
    tram->Set(model, year);

    if (park->Enqueue(tram)) {
        cout << "Трамвай добавлен в парк №" << parkNumber << ".\n";
    }
    else {
        cout << "Не удалось добавить трамвай.\n";
        delete tram;
    }
}

void RemoveTram() {
    int parkNumber;
    cout << "Введите номер трамвайного парка: ";
    parkNumber = failure();

    TramPark* park = transport->FindPark(parkNumber);
    if (park == NULL) {
        cout << "Трамвайный парк не найден.\n";
        return;
    }
    string model;
    cout << "Введите марку трамвая для удаления: ";
    cin.clear(); cin.ignore(32767, '\n'); getline(cin, model);

    if (park->RemoveTram(model)) {
        cout << "Трамвай удален из парка №" << parkNumber << ".\n";
    }
    else {
        cout << "Трамвай не найден.\n";
    }
}

void FindTram() {
    string model;
    cout << "Введите марку трамвая для поиска: ";

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, model);

    transport->FindTram(model);
}

int main() {
    setlocale(LC_ALL, "Russian");
    transport = new CityTransport();

    if (!file.LoadData(transport)) {
        cout << "Не удалось загрузить данные из файла. Начните с пустой базы.\n";
    }

    int choice;
    while (true) {
        cout << "\n===== Городской электротранспорт - управление трамвайными парками =====\n";
        cout << "1. Добавить трамвайный парк\n";
        cout << "2. Удалить трамвайный парк\n";
        cout << "3. Найти трамвайный парк\n";
        cout << "4. Добавить трамвай в парк\n";
        cout << "5. Удалить трамвай из парка\n";
        cout << "6. Найти трамвай\n";
        cout << "7. Показать все парки\n";
        cout << "8. Сохранить изменения в файл\n";
        cout << "0. Выход\n";
        cout << "Выберите действие: ";

        choice = failure();

        switch (choice) {
        case 1: AddPark(); break;
        case 2: RemovePark(); break;
        case 3: FindPark(); break;
        case 4: AddTram(); break;
        case 5: RemoveTram(); break;
        case 6: FindTram(); break;
        case 7: transport->ShowParks(); break;
        case 8: {
            cout << "Сохранить изменения в файл? (1 - да, 0 - нет): ";
            int save = failure();
            if (save == 1) {
                if (file.SaveData(transport)) {
                    cout << "Данные успешно сохранены.\n";
                }
                else {
                    cout << "Ошибка при сохранении данных.\n";
                }
            }
            break;
        }
        case 0: {
            cout << "Сохранить изменения в файл? (1 - да, 0 - нет): ";
            int save = failure();
            if (save == 1) {
                if (file.SaveData(transport)) {
                    cout << "Данные успешно сохранены.\n";
                }
                else {
                    cout << "Ошибка при сохранении данных.\n";
                }
            }
            delete transport;
            return 0;
        }
        default:
            cout << "Неверный выбор. Попробуйте снова.\n";
        }
    }
}