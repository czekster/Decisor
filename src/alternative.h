#ifndef ALTERNATIVE_H
#define ALTERNATIVE_H

#include <string>

using namespace std;

class Alternative
{
public:
    Alternative();
    Alternative(string nome, double weight);
    ~Alternative();
    enum BOCR {B = 0,O,C,R,BOCR_TOTAL};
    bool operator <(const Alternative &b);
    //static bool sort(Alternative a, Alternative b);
    static bool sort(Alternative *a, Alternative *b);
    double getWeight() const;
    void setWeight(double value);

    string getNome() const;
    void setNome(const string &value);

    double *getBocrValues() const;
    void setBocrValues(double *value);
    void setBocrValue(double value, int pos);

private:
    string nome;
    double weight;
    double result;
    double* bocrValues;
};

#endif // ALTERNATIVE_H
