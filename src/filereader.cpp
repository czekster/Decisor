#include "filereader.h"
using namespace std;

FileReader::FileReader(string filename)
{
    string line;
    ifstream myfile (filename.c_str());

    bool created = false;
    int sz;
    int l,c;

    vector<string> v;       // auxiliary vector for spliting strings
    double** matrix;        // auxiliary matrix
    double** umatrix;       // auxiliary uniformized matrix
    int cont = 0;

    if (myfile.is_open()) {
        stringstream ss;
        ss << SEPARATOR << SEPARATOR << "End of model description";
        while ( getline (myfile, line)  ) {
            if(line.find(ss.str()) != string::npos){
                break;
            }
            if (line.size()>= 2 && line.substr(0,2) == COMMENT) // bypasses comments in the file (see #define)
                continue;
            if (line.substr(0,1) == OBJECTIVE){
                objective = line.substr(1,string::npos);
                cout << objective << endl;
                continue;
            }
            if (line.substr(0,1) == SEPARATOR) {
                if (cont++ > 0) {
                    vector<string> altstraux = StringUtils::split(line, ';');
                    if (altstraux.size() > 1) {
                        vector<string> v2 = StringUtils::split(altstraux[1], '-');
                        int sz = v2.size();
                        for (int p = 0; p < sz; p++) {
                            alternatstr.push_back(v2[p]);
                        }
                    }
                    string linestr = line.substr(1, line.find(';') - 1);
                    criteriastr.push_back(linestr);
                }
                if (created == true) {
                    matrices.push_back(matrix); // add matrix to vector
                    umatrices.push_back(umatrix);
                    orders.push_back(sz);
                }
                l = 0;
                created = false;
                continue;
            }
            if (!created) {
                v = StringUtils::split(line, DELIMITER);
                sz = v.size();
                matrix = new double*[sz];
                umatrix = new double*[sz];

                for (int i = 0; i < sz; i++) {
                    matrix[i] = new double[sz];
                    umatrix[i] = new double[sz];
                }
                for (int i = 0; i < sz; i++)
                    for (int j = 0; j < sz; j++) {
                        matrix[i][j] = 0;
                        umatrix[i][j] = 0;
                    }
                created = true;
            }
            c = 0;
            if (c < sz) {
                v = StringUtils::split(line, DELIMITER);
                for (int i = 0; i < sz; i++) {
                    matrix[l][c] = atof(v[i].c_str());
                    umatrix[l][c] = atof(v[i].c_str());
                    c++;
                }
                l++;
            }
        }
        while(getline (myfile, line)){
            if(line.find(SEPARATOR) !=  string::npos){
                continue;
            }
            vector<string> list = StringUtils::split(line, DELIMITER);
            vector<double> linha;
            int sz = list.size();
            for (int i = 0; i < sz; i++) {
                linha.push_back(atof(list[i].c_str()));
            }
            matrixBOCR.push_back(linha);
        }

        myfile.close();
        // TODO: validate input file matrix orders and elements (alphabetical and other errors)
    }
}

FileReader::~FileReader()
{
    // delete structures
    int sz = matrices.size();
    for (int i = 0; i < sz; i++) {
        double** m = matrices[i];
        int o = orders[i];
        for (int j = 0; j < o; j++)
            delete[] m[j];
        delete[] m;
    }
    sz = umatrices.size();
    for (int i = 0; i < sz; i++) {
        double** um = umatrices[i];
        int o = orders[i];
        for (int j = 0; j < o; j++)
            delete[] um[j];
        delete[] um;
    }
}

vector<double **> FileReader::getUmatrices() const
{
    return umatrices;
}

vector<vector<double> > FileReader::getMatrixBOCR() const
{
    return matrixBOCR;
}

string FileReader::getObjective() const
{
    return objective;
}

vector<double **> FileReader::getMatrices() const
{
    return matrices;
}

vector<string> FileReader::getCriteriastr() const
{
    return criteriastr;
}

vector<string> FileReader::getAlternatstr() const
{
    return alternatstr;
}
