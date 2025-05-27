#pragma once
#include "TramPark.h"
#include <iostream>
#include <vector>

using namespace std;

const int MAX_PARKS = 2; 

class CityTransport {
private:
    struct ParkNode {
        int parkNumber;
        TramPark* park;
        int next; 
        bool used; 
    };

    ParkNode parksArray[MAX_PARKS];
    int head;
    int freeList; 

    void InitFreeList() {
        for (int i = 0; i < MAX_PARKS - 1; i++) {
            parksArray[i].next = i + 1;
            parksArray[i].used = false;
        }
        parksArray[MAX_PARKS - 1].next = -1;
        parksArray[MAX_PARKS - 1].used = false;
        freeList = 0;
    }

    int AllocateNode() {
        if (freeList == -1) return -1; 
        int index = freeList;
        freeList = parksArray[freeList].next;
        parksArray[index].used = true;
        parksArray[index].next = -1;
        return index;
    }

    void FreeNode(int index) {
        parksArray[index].next = freeList;
        parksArray[index].used = false;
        freeList = index;
    }

public:
    CityTransport() : head(-1) {
        InitFreeList();
    }

    bool AddPark(int parkNumber) {
        int current = head;
        while (current != -1) {
            if (parksArray[current].parkNumber == parkNumber) {
                cout << "Ошибка: парк с номером " << parkNumber << " уже существует!\n";
                return false;
            }
            current = parksArray[current].next;
        }

        if (freeList == -1) {
            cout << "Ошибка: достигнуто максимальное количество парков (" << MAX_PARKS << ")!\n";
            return false;
        }

        int newNodeIndex = AllocateNode();
        if (newNodeIndex == -1) {
            cout << "Ошибка: невозможно добавить парк (внутренняя ошибка)!\n";
            return false;
        }

        parksArray[newNodeIndex].parkNumber = parkNumber;
        parksArray[newNodeIndex].park = new TramPark(parkNumber);

        if (head == -1 || parksArray[head].parkNumber > parkNumber) {
            parksArray[newNodeIndex].next = head;
            head = newNodeIndex;
            cout << "Трамвайный парк №" << parkNumber << " успешно добавлен.\n";
            return true;
        }

        current = head;
        int prev = -1;
        while (current != -1 && parksArray[current].parkNumber < parkNumber) {
            prev = current;
            current = parksArray[current].next;
        }

        parksArray[newNodeIndex].next = current;
        if (prev != -1) {
            parksArray[prev].next = newNodeIndex;
        }

        cout << "Трамвайный парк №" << parkNumber << " успешно добавлен.\n";
        return true;
    }

    bool RemovePark(int parkNumber) {
        int current = head;
        int prev = -1;

        while (current != -1 && parksArray[current].parkNumber != parkNumber) {
            prev = current;
            current = parksArray[current].next;
        }

        if (current == -1) return false; 

        if (prev == -1) {
            head = parksArray[current].next;
        }
        else {
            parksArray[prev].next = parksArray[current].next;
        }

        delete parksArray[current].park;
        FreeNode(current);
        return true;
    }

    TramPark* FindPark(int parkNumber) {
        int current = head;
        while (current != -1) {
            if (parksArray[current].parkNumber == parkNumber) {
                return parksArray[current].park;
            }
            current = parksArray[current].next;
        }
        return nullptr;
    }

    void ShowParks() {
        int current = head;
        while (current != -1) {
            cout << "Трамвайный парк №" << parksArray[current].parkNumber << ":" << endl;
            parksArray[current].park->ShowTrams();
            cout << endl;
            current = parksArray[current].next;
        }
    }

    string GetString() {
        string result;
        int current = head;
        while (current != -1) {
            result += to_string(parksArray[current].parkNumber) + " " +
                parksArray[current].park->GetString() + "\n";
            current = parksArray[current].next;
        }
        return result;
    }

    void FindTram(const string& model) {
        bool foundAny = false;
        int current = head;

        while (current != -1) {
            TramPark* park = parksArray[current].park;
            TramPark::Node* node = park->GetHead()->next;

            vector<Tram*> matchedTrams;

            while (node != nullptr) {
                if (node->data->GetModel() == model) {
                    matchedTrams.push_back(node->data);
                }
                node = node->next;
            }

            if (!matchedTrams.empty()) {
                if (!foundAny) {
                    cout << "Найденные трамваи марки \"" << model << "\":" << endl;
                    foundAny = true;
                }

                cout << "Парк №" << parksArray[current].parkNumber << ":\n";
                for (const auto& tram : matchedTrams) {
                    cout << "  Марка: " << tram->GetModel()
                        << ", Год выпуска: " << tram->GetYear() << endl;
                }
            }

            current = parksArray[current].next;
        }

        if (!foundAny) {
            cout << "Трамваи марки \"" << model << "\" не найдены.\n";
        }
    }

    ~CityTransport() {
        int current = head;
        while (current != -1) {
            delete parksArray[current].park;
            int next = parksArray[current].next;
            parksArray[current].used = false;
            current = next;
        }
    }
};