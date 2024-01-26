#include "Decisor.hpp"

Decisor::Decisor() {
}

Decisor::~Decisor() {
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
    sz = wvectors.size();
    for (int i = 0; i < sz; i++)
        delete wvectors[i];

    for (int i = 0; i < alt_mat_order; i++)
        if(csmatrix[i]){
            delete csmatrix[i];
        }
    delete csmatrix;
    delete results;
    delete cr;
}

void Decisor::process(string filename) {
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

        while ( getline (myfile, line) ) {
            if (line.size()>= 2 && line.substr(0,2) == COMMENT) // bypasses comments in the file (see #define)
                continue;
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
        myfile.close();
        // TODO: validate input file matrix orders and elements (alphabetical and other errors)
    }
    else //cout << "Error: Unable to open file" << endl;

        cri_mat_order = orders[0]; // criteria matrix order (first matrix)
    alt_mat_order = orders[1]; // alternatives matrix order (second matrix and on)

    if (alternatstr.size() == 0) { // if the user haven't entered any alternative name in the input file
        for (int p = 1; p <= alt_mat_order; p++) {
            stringstream ss;
            ss << p;
            alternatstr.push_back(ss.str());
        }
    }

    // after reading the matrices, one must invert all positive values into their respective places
    //cout << "inverting weights..." << endl;
    invert_weights();
    // uniformize the matrices
    //cout << "uniformizing..." << endl;
    uniformize();
    // computes the weight vector
    //cout << "computing wvectors..." << endl;
    compute_wvector();
    // performs synthesis and composition
    //cout << "performing synthesis/composition..." << endl;
    synthesis();
    // compute consistency rate (CR) and consistency index (CI)
    consistency_rate();
}

void Decisor::processFile(string filename)
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

        while ( getline (myfile, line) ) {
            if (line.size()>= 2 && line.substr(0,2) == COMMENT) // bypasses comments in the file (see #define)
                continue;
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
        myfile.close();
        // TODO: validate input file matrix orders and elements (alphabetical and other errors)
    }//cout << "Error: Unable to open file" << endl;

    cri_mat_order = orders[0]; // criteria matrix order (first matrix)
    alt_mat_order = orders[1]; // alternatives matrix order (second matrix and on)

    if (alternatstr.size() == 0) { // if the user haven't entered any alternative name in the input file
        for (int p = 1; p <= alt_mat_order; p++) {
            stringstream ss;
            ss << p;
            alternatstr.push_back(ss.str());
        }
    }
}

void Decisor::process()
{
    cri_mat_order = orders[0]; // criteria matrix order (first matrix)
    alt_mat_order = orders[1]; // alternatives matrix order (second matrix and on)

    // uniformize the matrices
    //cout << "uniformizing..." << endl;
    uniformize();
    // computes the weight vector
    //cout << "computing wvectors..." << endl;
    int sz = wvectors.size();
    for (int i = 0; i < sz; i++)
        delete wvectors[i];
    wvectors.clear();
    compute_wvector();
    // performs synthesis and composition
    //cout << "performing synthesis/composition..." << endl;
    synthesis();
    // compute consistency rate (CR) and consistency index (CI)
    consistency_rate();
}

/**
 * This method will take any matrix and, for every non-zero position [i,j],
 *    invert on its symmetrical oposite place [j,i]
*/
void Decisor::invert_weights() {
    int sz = matrices.size();
    for (int i = 0; i < sz; i++) {
        double** m = matrices[i];
        double** um = umatrices[i];
        int o = orders[i];
        for (int j = 0; j < o; j++)
            for (int k = 0; k < o; k++) {
                if (j == k) { // all diagonal values consists of '1'
                    um[k][j] = 1;
                    m[k][j] = 1;
                } else {
                    um[k][j] = (um[j][k] > 0) ? 1/um[j][k] : um[k][j];
                    m[k][j] = um[k][j];
                }
            }
    }
}

/**
* Uniformizes the matrix by dividing each column element by the sum of each column
* Uses umatrices vector (the list of uniformized matrices),
*     leaving the matrices list with original values
*/
void Decisor::uniformize() {
    int sz = umatrices.size();
    for (int i = 0; i < sz; i++) {
        double** um = umatrices[i];
        int o = orders[i];
        double* s = new double[o]; // vector of sums for each column
        // start the vector
        for (int j = 0; j < o; j++)
            s[j] = 0;
        // sums columns
        for (int j = 0; j < o; j++)
            for (int k = 0; k < o; k++)
                s[j] += um[k][j];
        // divides
        for (int j = 0; j < o; j++)
            for (int k = 0; k < o; k++)
                um[k][j] = um[k][j]/s[j];
        delete s;
    }
}

void Decisor::compute_wvector() {
    int sz = matrices.size();
    for (int i = 0; i < sz; i++) {
        double** um = umatrices[i];
        int o = orders[i];
        double* w = new double[o];
        // initialize
        for (int j = 0; j < o; j++)
            w[j] = 0;
        // sums lines
        for (int j = 0; j < o; j++)
            for (int k = 0; k < o; k++)
                w[j] += um[j][k];
        // computes average for each line
        for (int j = 0; j < o; j++)
            w[j] /= o;
        wvectors.push_back(w);
    }
}

void Decisor::synthesis() {
    csmatrix = new double*[alt_mat_order];
    for (int i = 0; i < alt_mat_order; i++)
        csmatrix[i] = new double[cri_mat_order];

    // computation

    double* wvector_cri = wvectors[0]; // wvector for the criteria
    for (int i = 1; i < cri_mat_order + 1; i++) {
        double* wvector_alt = wvectors[i];
        for (int j = 0; j < alt_mat_order; j++) {
            csmatrix[j][i-1] = wvector_cri[i-1] * wvector_alt[j];
        }
    }
    // line average
    results = new double[alt_mat_order];
    double aux;
    for (int i = 0; i < alt_mat_order; i++) {
        aux = 0;
        for (int j = 0; j < cri_mat_order; j++) {
            aux += csmatrix[i][j];
        }
        results[i] = aux;
    }
}

void Decisor::consistency_rate() {
    double* v = new double[matrices.size()];
    cr = new double[matrices.size()];
    int sz = matrices.size();
    for (int i = 0; i < sz; i++) {
        double** m = matrices[i];
        int o = orders[i];
        double* wvector = wvectors[i];
        double aux;
        for (int j = 0; j < o; j++) {
            aux = 0;
            // 1. multiplies original alternative matrix by wvector
            for (int k = 0; k < o; k++) {
                aux += m[j][k] * wvector[k];
            }
            // 2. divides each position by wvector
            v[j] = aux/wvector[j];
        }
        // 3. computes average value (lambda_max)
        lambda_max = 0;
        int orderaux = (i == 0 ? cri_mat_order : alt_mat_order);
        for (int j = 0; j < orderaux; j++)
            lambda_max += v[j];

        lambda_max /= orderaux;

        // 4. computes consistency index (CI)
        double ci = (lambda_max - orderaux) / (orderaux - 1);

        // 5. computes consistency rate (CR)
        double RI[] = { 0, 0, 0.52, 0.90, 1.12, 1.24, 1.32, 1.41, 1.45, 1.51 };
        if (orderaux - 1 > 1){
            cr[i] = ci / RI[orderaux-1]; // RI comes from a table of constants
        }else{
            cr[i] = 0.0;
        }
    }
    //delete v;
}

/* old method
void Decisor::print() {
    //cout << "criteria and alternatives matrices" << endl;
    int sz = matrices.size();
    for (int i = 0; i < sz; i++) {
        double** m = matrices[i];
        int o = orders[i];
        if (i == 0){
            //cout << "criteria matrix" << endl;
        }else{ //cout << "criterion '" << criteriastr[i-1] << "'" << endl;

        }
        for (int j = 0; j < o; j++) {
            for (int k = 0; k < o; k++) {
                //cout << "" << setfill(' ') << setw(7) << setprecision(4) << m[j][k] << " ";
            }
            //cout << endl;
        }
        //cout << "w vector" << endl;
        //double* w = wvectors[i];
        //for (int j = 0; j < o; j++) {
            //cout << "[" << j << "] " << setprecision(8) << w[j] << endl;
        //}
        //cout << endl;
    }
    //cout << "synthesis/composition vector" << endl;
    for (int i = 0; i < alt_mat_order; i++) {
        for (int j = 0; j < cri_mat_order; j++) {
            //cout << "" << setfill(' ') << setw(9) << setprecision(4) << csmatrix[i][j] << " ";
        }
        //cout << endl;
    }
    //cout << endl;
    //cout << "results:" << endl;
    double best = results[0];
    int index = 0;
    for (int j = 0; j < alt_mat_order; j++) {
        //cout << "alternative " << alternatstr[j] << ": " << results[j] << endl;
        if (best < results[j]) {
            best = results[j];
            index = j;
        }
    }
    //cout << "Best value = " << best << " (alternative " << alternatstr[index] << ")" << endl;
    //cout << endl;
    //cout << "Consistency rate:" << endl;
    //cout << "'" << "Criterion Matrix" << "': " << setfill(' ') << setw(9) << setprecision(4)
    //<< cr[0] <<" ("<<(cr[0]*100)<<"%)"<< endl;
    for (int j = 1; j < cri_mat_order+1; j++) {
        //cout << "criterion '" << (criteriastr[j-1]) << "': " << setfill(' ') << setw(9) << setprecision(4)
        // << cr[j] <<" ("<<(cr[j]*100)<<"%)"<< endl;
    }
}
*/

string Decisor::print()
{
    stringstream ss;

    ss << "Synthesis/Composition vector" << endl;
    for (int i = 0; i < alt_mat_order; i++) {
        for (int j = 0; j < cri_mat_order; j++) {
            ss << "" << setfill(' ') << setw(9) << setprecision(4) << csmatrix[i][j] << " ";
        }
        ss << endl;
    }
    ss << endl;
    ss << "Results:" << endl;
    double best = results[0];
    int index = 0;
    for (int j = 0; j < alt_mat_order; j++) {
        ss << "alternative " << alternatstr[j] << ": " << results[j] << endl;
        if (best < results[j]) {
            best = results[j];
            index = j;
        }
    }
    ss << "Best value = " << best << " (alternative " << alternatstr[index] << ")" << endl;
    ss << endl;
    ss << "Consistency rate:" << endl;
    ss << "'" << "Criterion Matrix" << "': " << setprecision(4)
       << cr[0] <<" ("<<(cr[0]*100)<<"%)"<< endl;
    for (int j = 1; j < cri_mat_order+1; j++) {
        ss << "criterion '" << (criteriastr[j-1]) << "': " << setprecision(4)
                                                  << cr[j] <<" ("<<(cr[j]*100)<<"%)"<< endl;
    }
    ss << endl << "Lambda Max: " << lambda_max << endl;
    return ss.str();
}

void Decisor::setCriteriastr(const vector<string> &value)
{
    criteriastr = value;
}

void Decisor::setAlternatstr(const vector<string> &value)
{
    alternatstr = value;
}

void Decisor::setUmatrices(const vector<double **> &value)
{
    umatrices = value;
}

void Decisor::setMatrices(const vector<double **> &value)
{
    matrices = value;
}

void Decisor::setOrders(const vector<int> &value)
{
    orders = value;
}

vector<double *> Decisor::getWvectors() const
{
    return wvectors;
}

vector<double **> Decisor::getMatrices() const
{
    return matrices;
}

vector<string> Decisor::getCriteriastr() const
{
    return criteriastr;
}

vector<string> Decisor::getAlternatstr() const
{
    return alternatstr;
}

double *Decisor::getCr() const
{
    return cr;
}

double *Decisor::getResults() const
{
    return results;
}

double Decisor::getLambda_max() const
{
    return lambda_max;
}
