#ifndef DECISOR_H
#define DECISOR_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <iomanip>

#include "StringUtils.hpp"

#define DELIMITER '\t' // change here if ' ' or other char are used as delimiter
#define SEPARATOR "#"  // change here to mark each matrix separator in the input file
#define COMMENT   "--" // change here to select the comment marker used in the file
#define OBJECTIVE "%"

using namespace std;

class Decisor {
   public:
      Decisor();
      virtual ~Decisor();
      void process(string filename);
      void processFile(string filename);
      void process();
      //void print(); // old print - remove after testing - fev18
      string print();
      void setCriteriastr(const vector<string> &value);

      void setAlternatstr(const vector<string> &value);

      void setUmatrices(const vector<double **> &value);

      void setMatrices(const vector<double **> &value);

      void setOrders(const vector<int> &value);

      vector<double *> getWvectors() const;

      vector<double **> getMatrices() const;

      vector<string> getCriteriastr() const;

      vector<string> getAlternatstr() const;

      double *getCr() const;

      double *getResults() const;

      double getLambda_max() const;

protected:
private:
      vector<double**> matrices;  // stores all matrix
      vector<double**> umatrices; // stores all UNIFORMIZED matrix
      vector<int> orders;         // saves the order for each matrix
      int cri_mat_order;          // criteria matrix order
      int alt_mat_order;          // alternatives matrix order
      vector<double*> wvectors;   // this is the weight vectors
      vector<string> criteriastr; // list of criteria - string
      vector<string> alternatstr; // list of alternatives - string
      double** csmatrix;          // synthesis and composition matrix (multiplies uniform. matrix by wvector)
      double* results;
      double* cr;
      double lambda_max;
      void invert_weights();
      void uniformize();
      void compute_wvector();
      void synthesis();
      void consistency_rate();
};

#endif // DECISOR_H
