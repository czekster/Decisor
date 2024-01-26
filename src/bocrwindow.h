#ifndef BOCRWINDOW_H
#define BOCRWINDOW_H


#include <QMainWindow>
#include <QTableWidget>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QFileDialog>
#include <QFile>
#include <sstream>
#include <list>
#include <vector>
#include <iostream>
#include <cmath>
#include <fstream>
#include <QTextStream>


#include "alternative.h"
#include "Decisor.hpp"
#include "filereader.h"
#include "filewriter.h"


using namespace std;

namespace Ui {
class BocrWindow;
}

class BocrWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit BocrWindow(QWidget *parent = 0);
    ~BocrWindow();
    enum SELECTION{MULT = 0,REC,NEG};
    void setAlternatives(const list<Alternative *> &value);
    void setFileAdress(const QString &value);
    void clearTable();
    void clear();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_toggled(bool checked);
    void on_actionSave_triggered();
    void on_actionNew_triggered();

private:
    Ui::BocrWindow *ui;
    list<Alternative*> alternatives;
    list<Alternative*> listResults;
    QString fileAdress;
    FileReader* reader;
    FileWriter* writer;

    void showResults();
    void copyAlternatives();
    double *getValues(int row);
    void populateTables();
    void normalizeTables();
    list<double*> normalizeTablesWithBOCR();
    void saveTable(QString fileName);
    void readTable(QString fileName);

};

#endif // BOCRWINDOW_H
