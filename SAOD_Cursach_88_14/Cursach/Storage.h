#pragma once
#include "CityTransport.h"
#include <fstream>
#include <sstream>

class Storage
{
private:
    string filename;

public:
    Storage(string _filename) {
        filename = _filename;
    }

    bool SaveData(CityTransport* transport) {
        ofstream file(filename);
        if (!file.is_open()) return false;

        file << transport->GetString();
        file.close();
        return true;
    }

    bool LoadData(CityTransport* transport) {
        ifstream file(filename);
        if (!file.is_open()) return false;

        string line;
        while (getline(file, line)) {
            istringstream iss(line);
            int parkNumber;
            iss >> parkNumber;

            transport->AddPark(parkNumber);
            TramPark* park = transport->FindPark(parkNumber);

            string model;
            int year;
            while (iss >> model >> year) {
                Tram* tram = new Tram();
                tram->Set(model, year);
                park->Enqueue(tram);
            }
        }

        file.close();
        return true;
    }

    ~Storage() {}
};