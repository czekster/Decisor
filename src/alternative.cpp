#include "alternative.h"

Alternative::Alternative()// : Alternative("", 0.0)
{
   Alternative("", 0.0);
}

Alternative::Alternative(string nome, double weight)
{
    this->nome = nome;
    this->weight = weight;
    bocrValues = new double[BOCR_TOTAL];
    for (int var = 0; var < BOCR_TOTAL; ++var) {
        bocrValues[var] = 0;
    }
}

Alternative::~Alternative()
{
    delete[] bocrValues;
}

bool Alternative::operator <(const Alternative &b)
{
    return (this->getWeight()<b.getWeight());
}
/*
bool Alternative::sort(Alternative a, Alternative b)
{
    return (a.getWeight()<b.getWeight());
}*/

bool Alternative::sort(Alternative *a, Alternative *b)
{
    return (a->getWeight()>b->getWeight());
}

double Alternative::getWeight() const
{
    return weight;
}

void Alternative::setWeight(double value)
{
    weight = value;
}

string Alternative::getNome() const
{
    return nome;
}

void Alternative::setNome(const string &value)
{
    nome = value;
}

double *Alternative::getBocrValues() const
{
    return bocrValues;
}

void Alternative::setBocrValues(double *value)
{
    delete[] bocrValues;
    bocrValues = value;
}

void Alternative::setBocrValue(double value, int pos)
{
    bocrValues[pos] = value;
}


