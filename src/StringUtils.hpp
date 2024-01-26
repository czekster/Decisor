#ifndef STRINGUTILS_H
#define STRINGUTILS_H

#include <vector>
#include <sstream>
#include <string>

using namespace std;

class StringUtils {
   public:
      StringUtils();
      virtual ~StringUtils();
      static vector<string> split(const string &s, char delim);
   protected:
   private:
      static vector<string> &split(const string &s, char delim, vector<string> &elems);
};

#endif // STRINGUTILS_H
