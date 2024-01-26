#ifndef FILEREADER_H
#define FILEREADER_H

#include "Decisor.hpp"
#include <string>
#include <fstream>
#include <vector>

using namespace std;

class FileReader
{
public:
    FileReader(string filename);
    virtual ~FileReader();    
    vector<string> getAlternatstr() const;
    vector<string> getCriteriastr() const;
    vector<double **> getMatrices() const;
    vector<double **> getUmatrices() const;
    vector<vector<double> > getMatrixBOCR() const;
    string getObjective() const;

private:
    string objective;
    vector<string> alternatstr;
    vector<string> criteriastr;
    vector<double**> matrices;
    vector<double**> umatrices;
    vector<vector<double>> matrixBOCR;
    vector<int> orders;
};

#endif // FILEREADER_H
