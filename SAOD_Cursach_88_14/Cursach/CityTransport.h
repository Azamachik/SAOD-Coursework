#pragma once
#include "TramPark.h"
#include <iostream>

using namespace std;

class CityTransport
{
private:
    struct ParkNode {
        int parkNumber;
        TramPark* park;
        ParkNode* next;
        ParkNode* prev;
    };

    ParkNode* head;

public:
    CityTransport() {
        head = new ParkNode();
        head->parkNumber = -1;
        head->park = new TramPark(-1);
        head->next = head;
        head->prev = head;
    }

    bool AddPark(int parkNumber) {
        ParkNode* current = head->next;
        while (current != head) {
            if (current->parkNumber == parkNumber) {
                return false; 
            }
            current = current->next;
        }

        ParkNode* newNode = new ParkNode();
        newNode->parkNumber = parkNumber;
        newNode->park = new TramPark(parkNumber);

        current = head->next;
        while (current != head && current->parkNumber < parkNumber) {
            current = current->next;
        }

        newNode->next = current;
        newNode->prev = current->prev;
        current->prev->next = newNode;
        current->prev = newNode;

        return true;
    }
    bool RemovePark(int parkNumber) {
        ParkNode* current = head->next;
        while (current != head && current->parkNumber != parkNumber) {
            current = current->next;
        }

        if (current == head) return false;

        current->prev->next = current->next;
        current->next->prev = current->prev;
        delete current->park;
        delete current;
        return true;
    }

    ParkNode* GetFirstPark() const {
        return head->next;
    }

    TramPark* FindPark(int parkNumber) {
        ParkNode* current = head->next;
        while (current != head && current->parkNumber != parkNumber) {
            current = current->next;
        }
        return (current != head) ? current->park : NULL;
    }

    void ShowParks() {
        ParkNode* current = head->next;
        while (current != head) {
            cout << "���������� ���� �" << current->parkNumber << ":" << endl;
            current->park->ShowTrams();
            cout << endl;
            current = current->next;
        }
    }

    string GetString() {
        string result;
        ParkNode* current = head->next;
        while (current != head) {
            result += to_string(current->parkNumber) + " " + current->park->GetString() + "\n";
            current = current->next;
        }
        return result;
    }

    void FindTram(const string& model) {
        bool found = false;
        ParkNode* currentPark = head->next;

        while (currentPark != head) {
            Tram* tram = currentPark->park->FindTram(model);

            if (tram != nullptr) {
                if (!found) {
                    cout << "��������� ������� ����� \"" << model << "\":" << endl;
                    found = true;
                }
                cout << "���� �" << currentPark->parkNumber << ": "
                    << "�����: " << tram->GetModel()
                    << ", ��� �������: " << tram->GetYear() << endl;
            }
            currentPark = currentPark->next;
        }

        if (!found) {
            cout << "������� ����� \"" << model << "\" �� �������." << endl;
        }
    }

    ~CityTransport() {
        ParkNode* current = head->next;
        while (current != head) {
            ParkNode* temp = current;
            current = current->next;
            delete temp->park;
            delete temp;
        }
        delete head->park;
        delete head;
    }
};