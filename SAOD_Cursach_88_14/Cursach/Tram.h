#pragma once
#include <iostream>
#include <string>

using namespace std;

class Tram
{
private:
    struct TramData {
        string model;
        int year;
    } theTram;

public:
    Tram() {
        theTram.model = "<ÍÅÒ ÄÀÍÍÛÕ>";
        theTram.year = 0;
    }

    void Set(string _model, int _year) {
        theTram.model = _model;
        theTram.year = _year;
    }

    string GetModel() { return theTram.model; }
    int GetYear() { return theTram.year; }

    void Clear() {
        theTram.model = "<ÍÅÒ ÄÀÍÍÛÕ>";
        theTram.year = 0;
    };

    ~Tram() { };
};