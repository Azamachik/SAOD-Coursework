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
        cout << "\n������ �����. ��������� ����: ";
    }
    return a;
}

void AddPark() {
    int parkNumber;
    cout << "������� ����� ����������� �����: ";
    parkNumber = failure();

    if (transport->AddPark(parkNumber)) {
        cout << "���������� ���� �" + to_string(parkNumber) + " ��������.\n";
    }
    else {
        cout << "������: ���� � ������� " + to_string(parkNumber) + " ��� ����������!\n";
    }
}

void RemovePark() {
    int parkNumber;
    cout << "������� ����� ����������� ����� ��� ��������: ";
    parkNumber = failure();

    if (transport->RemovePark(parkNumber)) {
        cout << "���������� ���� �" << parkNumber << " ������.\n";
    }
    else {
        cout << "���������� ���� �� ������.\n";
    }
}

void FindPark() {
    int parkNumber;
    cout << "������� ����� ����������� ����� ��� ������: ";
    parkNumber = failure();

    TramPark* park = transport->FindPark(parkNumber);
    if (park != NULL) {
        cout << "���������� ���� �" << parkNumber << " ������:\n";
        park->ShowTrams();
    }
    else {
        cout << "���������� ���� �� ������.\n";
    }
}

void AddTram() {
    int parkNumber;
    cout << "������� ����� ����������� �����: ";
    parkNumber = failure();

    TramPark* park = transport->FindPark(parkNumber);
    if (park == NULL) {
        cout << "���������� ���� �� ������.\n";
        return;
    }

    string model;
    int year;

    cout << "������� ����� �������: ";
    cin.clear(); cin.ignore(32767, '\n'); getline(cin, model);
    cout << "������� ��� �������: ";
    year = failure();

    Tram* tram = new Tram();
    tram->Set(model, year);

    if (park->Enqueue(tram)) {
        cout << "������� �������� � ���� �" << parkNumber << ".\n";
    }
    else {
        cout << "�� ������� �������� �������.\n";
        delete tram;
    }
}

void RemoveTram() {
    int parkNumber;
    cout << "������� ����� ����������� �����: ";
    parkNumber = failure();

    TramPark* park = transport->FindPark(parkNumber);
    if (park == NULL) {
        cout << "���������� ���� �� ������.\n";
        return;
    }

    string model;
    cout << "������� ����� ������� ��� ��������: ";
    cin.clear(); cin.ignore(32767, '\n'); getline(cin, model);

    if (park->RemoveTram(model)) {
        cout << "������� ������ �� ����� �" << parkNumber << ".\n";
    }
    else {
        cout << "������� �� ������.\n";
    }
}

void FindTram() {
    string model;
    cout << "������� ����� ������� ��� ������: ";

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, model);

    transport->FindTram(model);
}

//void setupConsole() {
//#ifdef _WIN32
//    // ��� Windows
//    _setmode(_fileno(stdout), _O_U16TEXT);
//    _setmode(_fileno(stdin), _O_U16TEXT);
//    _setmode(_fileno(stderr), _O_U16TEXT);
//    SetConsoleOutputCP(CP_UTF8);
//    SetConsoleCP(CP_UTF8);
//#else
//    // ��� Linux/Mac
//    setlocale(LC_ALL, "ru_RU.UTF-8");
//#endif
//}


int main() {
    //setupConsole();
    setlocale(LC_ALL, "Russian");
    transport = new CityTransport();

    if (!file.LoadData(transport)) {
        cout << "�� ������� ��������� ������ �� �����. ������� � ������ ����.\n";
    }

    int choice;
    while (true) {
        cout << "\n===== ��������� ���������������� - ���������� ����������� ������� =====\n";
        cout << "1. �������� ���������� ����\n";
        cout << "2. ������� ���������� ����\n";
        cout << "3. ����� ���������� ����\n";
        cout << "4. �������� ������� � ����\n";
        cout << "5. ������� ������� �� �����\n";
        cout << "6. ����� �������\n";
        cout << "7. �������� ��� �����\n";
        cout << "8. ��������� ��������� � ����\n";
        cout << "9. ��������� �������� ����������\n";
        cout << "0. �����\n";
        cout << "�������� ��������: ";

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
            cout << "��������� ��������� � ����? (1 - ��, 0 - ���): ";
            int save = failure();
            if (save == 1) {
                if (file.SaveData(transport)) {
                    cout << "������ ������� ���������.\n";
                }
                else {
                    cout << "������ ��� ���������� ������.\n";
                }
            }
            break;
        }
        case 0: {
            cout << "��������� ��������� � ����? (1 - ��, 0 - ���): ";
            int save = failure();
            if (save == 1) {
                if (file.SaveData(transport)) {
                    cout << "������ ������� ���������.\n";
                }
                else {
                    cout << "������ ��� ���������� ������.\n";
                }
            }
            delete transport;
            return 0;
        }
        default:
            cout << "�������� �����. ���������� �����.\n";
        }
    }
}