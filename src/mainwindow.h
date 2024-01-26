#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#define ALTERNATIVE 1
#define CRITERION 2

#include <QDebug>

#include <string>
#include <sstream>
#include <list>
#include <vector>
#include <iostream>
#include <fstream>

#include <QTextBrowser>
#include <QMainWindow>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QFormLayout>
#include <QCheckBox>
#include <QLine>
#include <QTableWidget>
#include <QDoubleSpinBox>
#include <QFileDialog>
#include <QCloseEvent>
#include <QtGlobal>

#include "Decisor.hpp"
#include "scalesettings.h"
#include "filereader.h"
#include "form.h"
#include "bocrwindow.h"
#include "alternative.h"
#include "filewriter.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_btnAddCriterion_clicked();
    void on_btnAddAlternative_clicked();
    void on_btnUpdate_clicked();
    void on_btnDelete_clicked();
    void ondoubleSpinBox_valueChanged(double arg1);
    void onLineEdit_textEdited(const QString & text);
    void on_actionExit_triggered();
    void on_actionOpen_triggered();
    void on_actionSaveModelAs_triggered();
    void on_actionSaveAll_triggered();
    void on_actionAbout_triggered();
    void on_actionNew_triggered();
    void on_actionNewBOCR_triggered();
    void on_actionLoadBOCR_triggered();
    void on_actionSet_Scale_triggered();
    void on_actionToggle_Scale_toggled(bool checked);
    void on_actionOpen_Several_triggered();
    void on_actionSaveModel_triggered();
    void on_actionCalculate_triggered();
    void on_actionAutomatic_Calculation_toggled(bool arg1);

private:
    enum toolTipEnum {OBJ_TXT =0, CRI_TXT, CRI_CB, ALT_TXT, ALT_CB, CRI_BTN, ALT_BTN, DEL_BTN, UPD_BTN, RESULT, SIZE};
    QString *toolTipStrings;
    Ui::MainWindow *ui;
    std::list<Form*> listCriteria;
    std::list<Form*> listAlternative;
    std::list<QTableWidget*> listTable;
    std::list<QTableWidget*> listWVector;
    std::vector<std::vector<double>> storedBOCR;
    std::vector<std::vector<std::vector<double>>> storedTables;
    Decisor *decisor;
    double lambda_max;
    std::vector<double> cr;
    std::vector<double> results;
    bool changed;
    bool addedFiled;
    bool decidePressed;
    BocrWindow* bocrWindow;
    QString saveAdress;
    ScaleSettings* scale;
    bool openingFile;

    void makeToolTipStrings();
    void closeEvent(QCloseEvent *event);
    void init();
    void init2();
    void decide();
    void exportTables();
    void importWVector();
    void populateWeightTable(int index, double* values);
    void saveTables(QString name);
    void saveAll(QString name);
    int deleteCheckedFields();
    void deleteCriteria();
    void deleteAlternative();
    void updateSpinBoxNames();
    void updateLineEditNames();
    void updateWVector();
    void updateTables();
    void deleteAll();
    void updateCriteriaTables();
    void updateAlternativeTables();
    void updateCriterion(QLineEdit *line);
    void updateAlternative(QLineEdit *name);
    void importAlternatives(std::vector<std::string> alternatives);
    void importCriteria(std::vector<std::string> criteria);
    void importTables(std::vector<double**> matrices);
    void makeTabOrder();
    bool duplicatedCriterion();
    bool dublicatedAlternative();
    void addLine(int list);
    void makeButton(QPushButton* btn,int type);
    void makeTableCriterion();
    void makeTableAlternative();
    void makeTableWeight(int size);
    void makeTableWeight(int size, int index);
    void addTab(QString name);
    int unsavedChangesWarning();
    double getScaleValue(double value);
    FileWriter* makeWriter();
};

#endif // MAINWINDOW_H
