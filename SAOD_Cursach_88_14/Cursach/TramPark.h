#pragma once
#include "Tram.h"
#include <iostream>

using namespace std;

class TramPark
{
public:
    struct Node {
        Tram* data;
        Node* next;
    };

    Node* head;
    Node* rear;
    int parkNumber;

    TramPark(int number) {
        head = new Node();
        head->data = new Tram();
        head->data->Set("<HEAD>", 0);
        head->next = NULL;
        rear = head;
        parkNumber = number;
    }

    int GetParkNumber() { return parkNumber; }

    bool Enqueue(Tram* tram) {
        Node* temp = new Node();
        temp->data = tram;
        temp->next = NULL;

        rear->next = temp;
        rear = temp;
        return true;
    }

    Tram* Dequeue() {
        if (head->next == NULL) return NULL;

        Node* temp = head->next;
        Tram* tram = temp->data;

        head->next = temp->next;
        if (temp == rear) rear = head;

        delete temp;
        return tram;
    }

    Tram* FindTram(const string& model) const {
        Node* current = head->next;
        while (current != nullptr) {
            if (current->data->GetModel() == model) {
                return current->data;
            }
            current = current->next;
        }
        return nullptr;
    }

    bool RemoveTram(string model) {
        Node* prev = head;
        Node* current = head->next;

        while (current != NULL) {
            if (current->data->GetModel() == model) {
                prev->next = current->next;
                if (current == rear) rear = prev;
                delete current->data;
                delete current;
                return true;
            }
            prev = current;
            current = current->next;
        }
        return false;
    }
    Node* GetHead() const {
        return head;
    }
    void ShowTrams() {
        Node* current = head->next;
        while (current != NULL) {
            cout << "   Марка: " << current->data->GetModel()
                << ", Год выпуска: " << current->data->GetYear() << endl;
            current = current->next;
        }
    }

    string GetString() {
        string result;
        Node* current = head->next;
        while (current != NULL) {
            result += current->data->GetModel() + " " + to_string(current->data->GetYear()) + " ";
            current = current->next;
        }
        return result;
    }

    ~TramPark() {
        while (head->next != NULL) {
            Node* temp = head->next;
            head->next = temp->next;
            delete temp->data;
            delete temp;
        }
        delete head->data;
        delete head;
    }
};