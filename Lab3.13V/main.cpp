#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

struct Point {
    double x;
    double y;
    double z;
};

struct Vector {
    double x;
    double y;
    double z;
};

struct FieldEntry {
    Point coordinates;
    Vector field;
    bool operator> (const FieldEntry& right) {
        return coordinates.z > right.coordinates.z;
    }
    bool operator< (const FieldEntry& right) {
        return coordinates.z < right.coordinates.z;
    }
};

double module3D(double x, double y, double z);
double module2D(double x, double y);

int main(int argc, char** argv) {
    ifstream in;
    ofstream out;
    vector<FieldEntry> magneticField;
    vector<double> gradient;
    vector<double> flatGradient;
    vector<double> fieldModule;
    vector<double> flatField;
    FieldEntry fieldBuffer;
    char buffer;
    in.open(argv[1], ios_base::in);
    out.open(argv[2], ios_base::out);
    in >> buffer;
    while (buffer == '%') {
        in.ignore(256, '\n');
        in >> buffer;
    }

    in.seekg(-1, ios_base::cur);
    while (!in.eof()) {
        in >> fieldBuffer.coordinates.x >> fieldBuffer.coordinates.y >>
              fieldBuffer.coordinates.z >> fieldBuffer.field.x >> fieldBuffer.field.y >>
              fieldBuffer.field.z;
        magneticField.push_back(fieldBuffer);
    }

    sort(magneticField.begin(), magneticField.end());
    for (int i = 0; i < magneticField.size(); i++) {
        flatField.push_back(module2D(magneticField[i].field.x, magneticField[i].field.y));
        fieldModule.push_back(module3D(magneticField[i].field.x, magneticField[i].field.y, magneticField[i].field.z));
        if (i < 1) {
            flatGradient.push_back(1);
            gradient.push_back(1);
        } else {
            flatGradient.push_back(*(flatField.end() - 2) / *(flatField.end() - 1));
            gradient.push_back(magneticField[i - 1].field.z / magneticField[i].field.z);
        }
    }

    out << "Field values:\n";
    out << "Z\t\t\t|B|\t\t\tGradBz\t\tGradBxy\n";
    out.precision(8);
    out << fixed;
    for (int i = 0; i < magneticField.size(); i++) {
        out << magneticField[i].coordinates.z << '\t' <<
               fieldModule[i] << '\t' <<
               gradient[i]<<'\t' <<
               flatGradient[i] << '\n';
    }
}

double module3D(double x, double y, double z) {
    double sum = pow(x, 2) + pow(y, 2) + pow(z, 2);
    return sqrt(sum);
}

double module2D(double x, double y) {
    double sum = pow(x, 2) + pow(y, 2);
    return sqrt(sum);
}