#include "filewriter.h"

FileWriter::FileWriter()
{
    tableBOCR = 0;
}

FileWriter::~FileWriter()
{
    list<double**>::iterator it;
    for(it = listTable.begin(); it!=listTable.end(); it++){
        double** matrix = *it;
        int size;
        if(it == listTable.begin()){
            size = listCriteria.size();
        }else{
            size = listAlternative.size();
        }
        for (int x = 0; x < size; ++x) {
            delete[] matrix[x];
        }
        delete[] matrix;
    }
    if(tableBOCR){
        for (int x = 0; x < listAlternative.size(); ++x) {
            delete[] tableBOCR[x];
        }
        delete[] tableBOCR;
    }
}

void FileWriter::save(QString fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        return;
    }
    QTextStream ss(&file);
    ss << OBJECTIVE << objective << "\n";
    ss << SEPARATOR << "Criteria Matrix\n";
    list<double**>::iterator it;
    int c = 0;
    for(it = listTable.begin(); it != listTable.end(); it++){
        double** table = *it;
        int size;
        if(c==0){
            size = listCriteria.size();
        }else{
            size = listAlternative.size();
        }
        for (int x = 0; x < size; x++) {
            for (int y = 0; y < size; y++) {
                double value = table[x][y];
                if(value <1){
                    value = 0;
                }
                if(y != size-1){
                    ss << value << DELIMITER;
                }else{
                    ss << value << "\n";
                }
            }
        }
        if(*it != listTable.back()){
            ss << SEPARATOR << listCriteria.at(c);
            if(it == listTable.begin()){
                ss << ";";
                QStringList::iterator itA;
                for(itA = listAlternative.begin(); itA != listAlternative.end();itA++){
                    QString alternative = *itA;
                    ss << alternative;
                    if(*itA != listAlternative.back()){
                        ss << "-";

                    }
                }
            }
            ss << "\n";
        }
        c++;
    }
    ss << SEPARATOR << "\n";
    ss << SEPARATOR << SEPARATOR << "End of model description" << "\n";
    ss << SEPARATOR << "BOCR Matrix" << "\n";
    for (int x = 0; x < listAlternative.size(); x++) {
        for (int y = 0; y < BOCR_SIZE; y++) {
            if(y != BOCR_SIZE-1){
                ss << tableBOCR[x][y] << DELIMITER;
            }else{
                ss << tableBOCR[x][y] << "\n";
            }
        }
    }
    ss << SEPARATOR;
    file.close();
}

void FileWriter::setTableBOCR(double **value)
{
    if(tableBOCR){
        for (int x = 0; x < listAlternative.size(); ++x) {
            delete[] tableBOCR[x];
        }
        delete[] tableBOCR;
    }
    tableBOCR = value;
}

void FileWriter::setListTable(const list<double **> &value)
{
    listTable = value;
}

void FileWriter::setListCriteria(const QStringList &value)
{
    listCriteria = value;
}

void FileWriter::setListAlternative(const QStringList &value)
{
    listAlternative = value;
}

void FileWriter::setObjective(const QString &value)
{
    objective = value;
}
