#ifndef FILEWRITER_H
#define FILEWRITER_H

#include <QTableWidget>
#include <list>
#include <sstream>
#include <fstream>
#include <QFile>
#include <QTextStream>
#include <QWidget>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QString>
#include <QStringList>

#include "form.h"
#include "Decisor.hpp"

class FileWriter
{
public:
    FileWriter();
    ~FileWriter();
    static const int BOCR_SIZE = 4;
    void save(QString fileName);

    void setTableBOCR(double **value);

    void setListTable(const list<double **> &value);

    void setListCriteria(const QStringList &value);

    void setListAlternative(const QStringList &value);

    void setObjective(const QString &value);

private:

    double** tableBOCR;
    list<double**> listTable;
    QStringList listCriteria;
    QStringList listAlternative;
    QString objective;

};

#endif // FILEWRITER_H
