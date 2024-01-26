#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scale = new ScaleSettings();
    makeToolTipStrings();
    qDebug() << "inicializando";
    openingFile = false;
    //QFont font("Courier");
    //font.setPointSize(10);
    //ui->textBrowser->setFont(font);

    // -------- Defining First Tab - NameTab --------

    ui->objective->setToolTip(toolTipStrings[OBJ_TXT]);

    Form *form = new Form(ui->checkBox,ui->criterion1);
    ui->checkBox->setToolTip(toolTipStrings[CRI_CB]);
    ui->criterion1->setToolTip(toolTipStrings[CRI_TXT]);
    listCriteria.push_back(form);

    form = new Form(ui->checkBox_2, ui->criterion2);
    ui->checkBox_2->setToolTip(toolTipStrings[CRI_CB]);
    ui->criterion2->setToolTip(toolTipStrings[CRI_TXT]);
    listCriteria.push_back(form);

    form = new Form(ui->checkBox_3, ui->alternative1);
    ui->checkBox_3->setToolTip(toolTipStrings[ALT_CB]);
    ui->alternative1->setToolTip(toolTipStrings[ALT_TXT]);
    listAlternative.push_back(form);

    form = new Form(ui->checkBox_4, ui->alternative2);
    ui->checkBox_4->setToolTip(toolTipStrings[ALT_CB]);
    ui->alternative2->setToolTip(toolTipStrings[ALT_TXT]);
    listAlternative.push_back(form);
    changed = false;
    addedFiled = false;
    decidePressed = false;
    on_actionNew_triggered();

    ui->btnAddCriterion->setToolTip(toolTipStrings[CRI_BTN]);
    ui->btnAddAlternative->setToolTip(toolTipStrings[ALT_BTN]);
    ui->btnDelete->setToolTip(toolTipStrings[DEL_BTN]);
    ui->btnUpdate->setToolTip(toolTipStrings[UPD_BTN]);
    //init2();



}

MainWindow::~MainWindow()
{
    delete ui;
    delete scale;
    delete[] toolTipStrings;
}

void MainWindow::addTab(QString name){
    QWidget *tab = new QWidget();
    stringstream ss;
    ss << "Tab_" << ui->tabWidget->count();
    tab->setObjectName(QString::fromStdString(ss.str()));
    ui->tabWidget->addTab(tab,name);

    QVBoxLayout *layoutTab = new QVBoxLayout(tab);
    layoutTab->setObjectName("horizontalLayoutTab");
    tab->setLayout(layoutTab);

    QFrame *frame = new QFrame(tab);

    QHBoxLayout *layoutFrame = new QHBoxLayout(frame);
    layoutFrame->setContentsMargins(0, 0, 0, 0);

    QTableWidget* table = new QTableWidget(tab);
    stringstream s2;
    s2 << "Table_" << listTable.size();
    table->setObjectName(QString::fromStdString(s2.str()));
    layoutFrame->addWidget(table);
    listTable.push_back(table);
    table = new QTableWidget(tab);
    s2.str("");
    s2 << "Table_Weight_" << listWVector.size();
    table->setObjectName(QString::fromStdString(s2.str()));
    layoutFrame->addWidget(table);
    listWVector.push_back(table);

    layoutTab->addWidget(frame);
}

int MainWindow::unsavedChangesWarning()
{
    if(changed){
        QMessageBox msgBox;
        msgBox.setWindowTitle("Warning!");
        msgBox.setText("You have unsaved changes to the model\nAre you sure you want to continue?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);
        return msgBox.exec();
    }
    return QMessageBox::Yes;
}

double MainWindow::getScaleValue(double value)
{
    double newValue = value;
    double aValue = scale->getValue();
    switch (scale->getSelected()) {
    case ScaleSettings::BTN_LINEAR:
        newValue = newValue*aValue;
        break;
    case ScaleSettings::BTN_POWER:
        newValue = pow(newValue, aValue);
        break;
    case ScaleSettings::BTN_GEOMETRIC:
        newValue = pow(aValue,newValue-1);
        break;
    case ScaleSettings::BTN_LOGARITHMIC:
        newValue = log(newValue+(aValue-1)) / log(aValue);
        break;
    case ScaleSettings::BTN_ROOT:
        newValue = pow(newValue,1.0/aValue);
        break;
    case ScaleSettings::BTN_ASYMPTOTICAL:
        newValue = 1/(tanh((sqrt(3)*(newValue-1))/14));
        break;
    case ScaleSettings::BTN_INVERSE:
        newValue = 9/(10-newValue);
        break;
    case ScaleSettings::BTN_BALANCED:
        newValue = newValue/(1-newValue);
        break;
    default:
        break;
    }

    return newValue;
}

FileWriter *MainWindow::makeWriter()
{
    FileWriter* writer = new FileWriter();
    double** tableBOCR;
    tableBOCR = new double*[listAlternative.size()];
    int sz = listAlternative.size();
    for (int x = 0; x < sz; ++x) {
        tableBOCR[x] = new double[FileWriter::BOCR_SIZE]();
    }

    int size = 0;
    if(listAlternative.size()>storedBOCR.size()){
        size = storedBOCR.size();
    }else{
        size = listAlternative.size();
    }

    for (int x = 0; x < size; ++x) {
        int sz = storedBOCR[x].size();
        for (int y = 0; y < sz; ++y) {
            tableBOCR[x][y] = storedBOCR[x][y];
        }
    }
    writer->setTableBOCR(tableBOCR);

    list<double**> listTables;
    for (list<QTableWidget*>::iterator it = listTable.begin(); it != listTable.end(); it++) {
        QTableWidget* table = *it;
        double** matrix = new double*[table->rowCount()];
        for (int x = 0; x < table->rowCount(); x++) {
            matrix[x] = new double[table->colorCount()];
            for (int y = 0; y < table->columnCount(); y++) {
                QWidget *item = table->cellWidget(x,y);
                QDoubleSpinBox *spin = static_cast<QDoubleSpinBox*>(item);
                if(spin){
                    matrix[x][y] = spin->value();
                }
            }
        }
        listTables.push_back(matrix);
    }
    writer->setListTable(listTables);

    QStringList listC;
    for (list<Form*>::iterator it = listCriteria.begin(); it != listCriteria.end(); it++) {
        Form* form = *it;
        QString criterion = form->getLineEdit()->text();
        listC.push_back(criterion);
    }
    writer->setListCriteria(listC);

    QStringList listA;
    for (list<Form*>::iterator it = listAlternative.begin(); it != listAlternative.end(); it++) {
        Form* form = *it;
        QString alternative = form->getLineEdit()->text();
        listA.push_back(alternative);
    }
    writer->setListAlternative(listA);
    writer->setObjective(ui->objective->text());

    return writer;
}

void MainWindow::init2()
{
    on_btnAddAlternative_clicked();
    on_btnAddAlternative_clicked();
    on_btnAddCriterion_clicked();
    on_btnAddCriterion_clicked();
    int c = 0;
    list<Form*>::iterator x;
    for(x = listCriteria.begin(); x != listCriteria.end(); x++){
        Form* form = *x;
        stringstream ss;
        ss << "criteria " << c++;
        form->getLineEdit()->setText(QString::fromStdString(ss.str()));
    }

    c = 0;
    for(x = listAlternative.begin(); x != listAlternative.end(); x++){
        Form* form = *x;
        stringstream ss;
        ss << "alternative " << c++;
        form->getLineEdit()->setText(QString::fromStdString(ss.str()));
    }
}

void MainWindow::exportTables()
{
    vector<double**> matrices;
    vector<int> orders;
    vector<string> alternatives;
    vector<string> criteria;
    list<QTableWidget*>::iterator it;
    int c =0;
    for(it = listTable.begin(); it != listTable.end(); it++){
        QTableWidget* table = *it;
        double** matrix = new double*[table->rowCount()];

        for (int i = 0; i < table->rowCount(); i++) {
           matrix[i] = new double[table->rowCount()];
        }
        //std::cout << "Table: " << table->objectName().toStdString() << std::endl;
        for (int x = 0; x < table->rowCount(); x++) {
            for (int y = 0; y < table->columnCount(); y++) {
                QWidget *item = table->cellWidget(x,y);
                QDoubleSpinBox *spin = static_cast<QDoubleSpinBox*>(item);
                matrix[x][y] = getScaleValue(spin->value());
             //   std::cout << matrix[x][y] << "\t";
            }
           // std::cout << std::endl;
        }
        //std::cout << std::endl<< std::endl;
        matrices.push_back(matrix);
        orders.push_back(table->rowCount());
        c++;
    }
    list<Form*>::iterator itForm;
    for(itForm = listCriteria.begin(); itForm != listCriteria.end(); itForm++){
        Form* form = *itForm;
        QLineEdit* line = form->getLineEdit();
        string nome = line->text().toStdString();
        criteria.push_back(nome);
    }
    for(itForm = listAlternative.begin(); itForm != listAlternative.end(); itForm++){
        Form* form = *itForm;
        QLineEdit* line = form->getLineEdit();
        string nome = line->text().toStdString();
        alternatives.push_back(nome);
    }

    decisor->setCriteriastr(criteria);
    decisor->setAlternatstr(alternatives);
    decisor->setMatrices(matrices);
    vector<double**> ummatrices;
    int sz = matrices.size();
    for(int z = 0; z < sz; z++){
        double** matrix = matrices[z];

        double** ummatrix = new double*[orders[z]];
        int sz2 = orders[z];
        for (int i = 0; i < sz2; i++) {
           ummatrix[i] = new double[orders[z]];
        }
        for(int x = 0; x < sz2; x++){
            for(int y=0; y < sz2; y++){
                ummatrix[x][y] = matrix[x][y];
            }
        }
        ummatrices.push_back(ummatrix);
    }
    decisor->setUmatrices(ummatrices);
    decisor->setOrders(orders);
}

void MainWindow::importWVector()
{
    vector<double*> wvector = decisor->getWvectors();
    int sz = wvector.size();
    for(int x = 0; x < sz; x++){
        double* weights = wvector[x];
        populateWeightTable(x,weights);
    }
}

void MainWindow::populateWeightTable(int index, double *values)
{
    list<QTableWidget*>::iterator it;
    it = listWVector.begin();
    while(index>0 && it!=listWVector.end()){
        index--;
        it++;
    }
    QTableWidget* table = *it;
    for(int x = 0; x<table->rowCount(); x++){
        QWidget* widget = table->cellWidget(x,0);
        QLabel* label = static_cast<QLabel*>(widget);
        if(label){
            label->setText(QString::number(values[x]));
        }
    }
}

void MainWindow::saveTables(QString name)
{
    ofstream file;
    file.open(name.toStdString());
    stringstream ss;
    ss << COMMENT << ui->objective->text().toStdString() << endl;
    ss << SEPARATOR << "Criteria Matrix" << endl;
    list<QTableWidget*>::iterator it;
    list<Form*>::iterator itC;
    itC = listCriteria.begin();
    for(it = listTable.begin(); it != listTable.end(); it++){
        Form *form = *itC;
        QLineEdit *criteria = form->getLineEdit();
        QTableWidget* table = *it;
        for (int x = 0; x < table->rowCount(); x++) {
            for (int y = 0; y < table->columnCount(); y++) {
                QWidget *item = table->cellWidget(x,y);
                QDoubleSpinBox *spin = static_cast<QDoubleSpinBox*>(item);
                if(spin){
                    double value = spin->value();
                    if(value <1){
                        value = 0;
                    }
                    if(y != table->columnCount()-1){
                        ss << value << DELIMITER;
                    }else{
                        ss << value << endl;
                    }
                }else{
                    //ERROR
                }
            }
        }
        if(*it != listTable.back()){
            ss << SEPARATOR << criteria->text().toStdString();
            if(it == listTable.begin()){
                ss << ";";
                list<Form*>::iterator itA;
                for(itA = listAlternative.begin(); itA != listAlternative.end();itA++){
                    QLineEdit *alternative = (*itA)->getLineEdit();
                    ss << alternative->text().toStdString();
                    if(*itA != listAlternative.back()){
                        ss << "-";

                    }
                }
            }
            ss << endl;
        }
        itC++;
    }
    file << ss.str();
    file << SEPARATOR << endl;
    file << SEPARATOR << SEPARATOR << "End of model description" << endl;;
    file.close();
    changed = false;

}

void MainWindow::saveAll(QString name)
{
    ofstream file;
    file.open(name.toStdString());
    stringstream ss;
    stringstream strCriteria;
    strCriteria << ";";
    stringstream strAlternative;
    strAlternative << ";";
    ss << "Objective:;" << ui->objective->text().toStdString() << endl;
    ss << "Matrizes:" << endl;
    list<QTableWidget*>::iterator it;
    list<QTableWidget*>::iterator itW;
    list<Form*>::iterator itC;
    for(itC = listCriteria.begin(); itC != listCriteria.end(); itC++){
        Form* form = *itC;
        strCriteria << form->getLineEdit()->text().toStdString() << ";";
    }
    strCriteria << ";";
    for(itC = listAlternative.begin(); itC != listAlternative.end(); itC++){
        Form* form = *itC;
        strAlternative << form->getLineEdit()->text().toStdString() << ";";
    }
    strAlternative << ";";
    strCriteria << "WVector;;CR" << endl;
    strAlternative << "WVector;;CR" << endl;
    ss << strCriteria.str();
    itW = listWVector.begin();
    itC = listCriteria.begin();
    int c = 0;
    for(it = listTable.begin(); it != listTable.end(); it++){
        Form *form = *itC;
        QLineEdit *criteria = form->getLineEdit();
        QTableWidget* table = *it;
        QTableWidget* weight = *itW;
        if(it != listTable.begin()){
            ss << criteria->text().toStdString() << strAlternative.str();
            itC++;
        }
        for (int x = 0; x < table->rowCount(); x++) {
            ss << table->verticalHeaderItem(x)->text().toStdString() << ";";
            for (int y = 0; y < table->columnCount(); y++) {
                QWidget *item = table->cellWidget(x,y);
                QDoubleSpinBox *spin = static_cast<QDoubleSpinBox*>(item);
                if(spin){
                    double value = spin->value();
                    ss << value << ";";
                }else{
                    //ERROR
                }
            }            
            QWidget *item = weight->cellWidget(x,0);
            QLabel *label = static_cast<QLabel*>(item);
            if(label){
                ss << ";" << label->text().toStdString() << ";;";
            }
            if(x == 0){
                ss << cr[c];
            }
            ss << endl;
        }
        ss << endl << endl;
        c++;
    }
    ss << "Results:" << endl;
    list<Form*>::iterator itA;
    itA = listAlternative.begin();
    c = 0;
    while(itA != listAlternative.end()){
        Form *form = *itA;
        ss << form->getLineEdit()->text().toStdString() << ";" << results[c] << endl;
        c++;
        itA++;
    }
    ss << endl << "Lambda Max;" << lambda_max << endl;
    string s = ss.str();
    if(ui->actionDecimal_Comma->isChecked()){
        replace(s.begin(), s.end(), '.',',');
    }

    file << s;
    file.close();
}

int MainWindow::deleteCheckedFields()
{
    int c = 0;
    list<Form*>::iterator x;
    for(x = listCriteria.begin(); x!= listCriteria.end();){
        Form* form = *x;
        if(form->getCheckBox()->isChecked()){
            form->getCheckBox()->deleteLater();
            form->getLineEdit()->deleteLater();
            if (!listCriteria.empty()) {
                x = listCriteria.erase(x);
                c++;
            }
        } else {
            x++;
        }
    }
    for(x = listAlternative.begin(); x!= listAlternative.end();x=x){
        Form* form = *x;
        if(form->getCheckBox()->isChecked()){
            form->getCheckBox()->deleteLater();
            form->getLineEdit()->deleteLater();
            if (!listAlternative.empty()) {
                x = listAlternative.erase(x);
                c++;
            }
        }else{
            x++;
        }
    }
    return c;
}

void MainWindow::deleteCriteria()
{
    list<QTableWidget*>::iterator it;
    it = listTable.begin();
    QTableWidget* table = *it;
    for(int x = 0;x <table->rowCount();x++){
        bool found = false;
        QString header = table->horizontalHeaderItem(x)->text();
        list<Form*>::iterator it2;
        for(it2 = listCriteria.begin(); it2!=listCriteria.end(); it2++){
            Form *form = *it2;
            QLineEdit *line = form->getLineEdit();
            if(header == line->text()){
                found = true;
                break;
            }
        }

        if(found == false){
            table->removeColumn(x);
            table->removeRow(x);

            list<QTableWidget*>::iterator itTables;
            list<QTableWidget*>::iterator weight;
            weight = listWVector.begin();
            itTables = listTable.begin();
            for(int d =0; d<x+1;d++){
                itTables++;
                weight++;
            }
            listTable.erase(itTables);
            listWVector.erase(weight);
            ui->tabWidget->removeTab(x+2);
            x--;
        }
    }
}

void MainWindow::deleteAlternative()
{
    list<QTableWidget*>::iterator it;
    it = listTable.begin();
    it++;
    QTableWidget* table = *it;
    for(int x = 0;x <table->rowCount();x++){
        bool found = false;
        QString header = table->horizontalHeaderItem(x)->text();
        //std::cout << header.toStdString() << std::endl;
        list<Form*>::iterator it2;
        for(it2 = listAlternative.begin(); it2!=listAlternative.end(); it2++){
            Form *form = *it2;
            QLineEdit *line = form->getLineEdit();
            if(header == line->text()){
                found = true;
                break;
            }
        }

        if(found == false){
            list<QTableWidget*>::iterator itTables;
            itTables = listTable.begin();
            itTables++;
            while(itTables != listTable.end()){
                QTableWidget* t = *itTables;
                t->removeColumn(x);
                t->removeRow(x);
                itTables++;
            }
            x--;
        }
    }

}

void MainWindow::updateSpinBoxNames()
{
    list<QTableWidget*>::iterator it;
    int tableCount = 0;
    for(it = listTable.begin();it != listTable.end();it++){
        QTableWidget* table = *it;
        int tam = table->rowCount();
        for(int x = 0;x <tam; x++){
            for (int y = 0; y < tam; y++) {
                QWidget *spin = table->cellWidget(x,y);
                stringstream ss;
                ss << "spinBox_" << x << "_" << y << "_" << tableCount;
                spin->setObjectName(QString::fromStdString(ss.str()));
            }
        }
        tableCount++;
    }
}

void MainWindow::updateLineEditNames()
{
    list<Form*>::iterator it;
    int c= 0;
    for(it = listAlternative.begin();it != listAlternative.end();it++){
        Form* form = *it;
        stringstream name;
        name << "checkBoxAlternative" << c;
        form->getCheckBox()->setObjectName(name.str().c_str());
        name.str("");
        name << "alternative" << c;
        form->getLineEdit()->setObjectName(name.str().c_str());
        //form->getLineEdit()->setText(form->getLineEdit()->text());
        c++;
    }
    c=0;
    for(it = listCriteria.begin();it != listCriteria.end();it++){
        Form* form = *it;
        stringstream name;
        name << "checkBoxCriterion" << c;
        form->getCheckBox()->setObjectName(name.str().c_str());
        name.str("");
        name << "criterion" << c;
        form->getLineEdit()->setObjectName(name.str().c_str());
        //form->getLineEdit()->setText(form->getLineEdit()->text());
        c++;
    }
}

void MainWindow::updateWVector()
{
    exportTables();
    decisor->process();
    importWVector();
}

void MainWindow::updateTables()
{
    if(ui->tabWidget->count()<2 && listAlternative.size()>0 && listCriteria.size()>0){
        addTab("Criteria");
        makeTableCriterion();
        list<Form*>::iterator x;
        for (x = listCriteria.begin(); x != listCriteria.end(); x++) {
            Form* form = *x;
            addTab(form->getLineEdit()->text());
            makeTableAlternative();
        }
    }else if(listAlternative.size()>0 && listCriteria.size()>0){

        deleteCriteria();
        deleteAlternative();

        updateAlternativeTables();
        updateCriteriaTables();

    }
    if(listAlternative.size()>0 && listCriteria.size()>0){
        makeTableWeight(listTable.front()->rowCount(),0);
        int sz = listWVector.size();
        for(int x = 1; x < sz; x++){
            makeTableWeight(listTable.back()->rowCount(),x);
        }
        updateSpinBoxNames();
        if(ui->actionAutomatic_Calculation->isChecked()){
            decide();
        }
    }
    if(listAlternative.size() == 0 && listCriteria.size() == 0){
        deleteAll();
    }
}

void MainWindow::deleteAll()
{
    list<Form*>::iterator it;
    for(it = listCriteria.begin(); it != listCriteria.end(); it++){
       Form* form = *it;
       form->getCheckBox()->setChecked(true);
    }
    for(it = listAlternative.begin(); it != listAlternative.end(); it++){
       Form* form = *it;
       form->getCheckBox()->setChecked(true);
    }
    deleteCheckedFields();
    while(ui->tabWidget->count()>1){
        ui->tabWidget->removeTab(1);
    }
    listTable.clear();
    listWVector.clear();
    ui->textBrowser->setText("");
    ui->objective->setText("");
}

void MainWindow::updateCriteriaTables()
{
    list<Form*>::iterator itForm;
    for(itForm = listCriteria.begin(); itForm != listCriteria.end(); itForm++){
        bool found = false;
        Form* form = *itForm;
        QLineEdit* line = form->getLineEdit();
        list<QTableWidget*>::iterator it;
        it = listTable.begin();
        QTableWidget* table = *it;
        for(int x = 0; x<table->rowCount();x++){
            QString header = table->horizontalHeaderItem(x)->text();
            if(header == line->text()){
                found = true;
                break;
            }
        }
        if(found == false){
            QTableWidget *firstTable = listTable.front();
            int tam = firstTable->rowCount();
            firstTable->setColumnCount(tam+1);
            firstTable->setRowCount(tam+1);
            QTableWidgetItem* item;
            item= new QTableWidgetItem();
            item->setText(line->text());
            firstTable->setHorizontalHeaderItem(tam,item);
            item = new QTableWidgetItem();
            item->setText(line->text());
            firstTable->setVerticalHeaderItem(tam,item);


            for (int x = 0; x < tam+1; ++x) {
                for (int y = 0; y < tam+1; ++y) {
                    if(x==tam || (y==tam && x<tam)){
                        if(x==y){
                            QSpinBox *item1 = new QSpinBox();
                            item1->setValue(1);
                            item1->setReadOnly(true);
                            item1->setAlignment(Qt::AlignCenter);
                            item1->setEnabled(false);
                            firstTable->setCellWidget(x,y,item1);
                        }else{
                            QDoubleSpinBox* spinBox = new QDoubleSpinBox();
                            spinBox->setDecimals(4);
                            spinBox->setMaximum(100);
                            spinBox->setValue(1);
                            firstTable->setCellWidget(x,y,spinBox);
                            connect(spinBox,SIGNAL(valueChanged(double)),this,SLOT(ondoubleSpinBox_valueChanged(double)));
                        }
                    }
                }
            }
            addTab(line->text());
            makeTableAlternative();
        }
    }
}

void MainWindow::updateAlternativeTables()
{
    list<Form*>::iterator itForm;
    for(itForm = listAlternative.begin(); itForm != listAlternative.end(); itForm++){
        bool found = false;
        Form* form = *itForm;
        QLineEdit* line = form->getLineEdit();
        list<QTableWidget*>::iterator it;
        it = listTable.begin();
        it++;
        QTableWidget* table = *it;
        for(int x = 0; x<table->rowCount();x++){
            QString header = table->horizontalHeaderItem(x)->text();
            if(header == line->text()){
                found = true;
                break;
            }
        }
        if(found == false){
            list<QTableWidget*>::iterator itTables;
            itTables = listTable.begin();
            itTables++;
            while(itTables!=listTable.end()){
                QTableWidget* altTable = *itTables;
                int tam = altTable->rowCount();
                altTable->setRowCount(tam+1);
                altTable->setColumnCount(tam+1);
                QTableWidgetItem* item;
                item= new QTableWidgetItem();
                item->setText(line->text());
                altTable->setHorizontalHeaderItem(tam,item);
                item = new QTableWidgetItem();
                item->setText(line->text());
                altTable->setVerticalHeaderItem(tam,item);

                for (int x = 0; x < tam+1; ++x) {
                    for (int y = 0; y < tam+1; ++y) {
                        if(x==tam || (y==tam && x<tam)){
                            if(x==y){
                                QSpinBox *item1 = new QSpinBox();
                                item1->setValue(1);
                                item1->setReadOnly(true);
                                item1->setAlignment(Qt::AlignCenter);
                                item1->setEnabled(false);
                                altTable->setCellWidget(x,y,item1);
                            }else{
                                QDoubleSpinBox* spinBox = new QDoubleSpinBox();
                                spinBox->setDecimals(4);
                                spinBox->setMaximum(100);
                                spinBox->setValue(1);
                                altTable->setCellWidget(x,y,spinBox);
                                connect(spinBox,SIGNAL(valueChanged(double)),this,SLOT(ondoubleSpinBox_valueChanged(double)));
                            }
                        }
                    }
                }
                itTables++;
            }
        }
    }
}

void MainWindow::updateCriterion(QLineEdit *line)
{
    QString indexSTR = line->objectName();
    indexSTR = indexSTR[indexSTR.size()-1];
    int index = indexSTR.toInt();
    QTableWidget* table = listTable.front();
    QTableWidgetItem* item = table->horizontalHeaderItem(index);
    if(item){
        item->setText(line->text());
        item = table->verticalHeaderItem(index);
        item->setText(line->text());
        ui->tabWidget->setTabText(index+2,line->text());
    }
}

void MainWindow::updateAlternative(QLineEdit *line)
{
    QString indexSTR = line->objectName();
    indexSTR = indexSTR[indexSTR.size()-1];
    int index = indexSTR.toInt();
    list<QTableWidget*>::iterator it;
    it = listTable.begin();
    it++;
    while(it != listTable.end()){
        QTableWidget* table = *it;
        QTableWidgetItem* item = table->horizontalHeaderItem(index);
        if(item){
            item->setText(line->text());
            item = table->verticalHeaderItem(index);
            item->setText(line->text());
        }
        it++;
    }

}

void MainWindow::importAlternatives(vector<string> alternatives)
{
    while(alternatives.size()>listAlternative.size()){
        on_btnAddAlternative_clicked();
    }
    vector<string>::iterator it;
    list<Form*>::iterator itForm;
    itForm = listAlternative.begin();
    for(it = alternatives.begin(); it!=alternatives.end(); it++){
        string line = *it;
        Form* form = *itForm;
        form->getLineEdit()->setText(QString::fromStdString(line));
        itForm++;
    }
}

void MainWindow::importCriteria(vector<string> criteria)
{
    while(criteria.size()>listCriteria.size()){
        on_btnAddCriterion_clicked();
    }
    vector<string>::iterator it;
    list<Form*>::iterator itForm;
    itForm = listCriteria.begin();
    for(it = criteria.begin(); it!=criteria.end(); it++){
        string line = *it;
        Form* form = *itForm;
        form->getLineEdit()->setText(QString::fromStdString(line));
        itForm++;
    }
}

void MainWindow::importTables(vector<double **> matrices)
{
    vector<double**>::iterator it;
    list<QTableWidget*>::iterator itTables;
    itTables = listTable.begin();
    for(it= matrices.begin(); it<matrices.end();it++){
        double** matrix = *it;
        QTableWidget* table = *itTables;
        if(it == matrices.begin()){
            int sz = listCriteria.size();
            for(int x=0; x < sz; x++){
                for(int y=0; y < sz; y++){
                    if(matrix[x][y] != 0 && x!=y){
                        QWidget *item = table->cellWidget(x,y);
                        QDoubleSpinBox *spinBox = qobject_cast<QDoubleSpinBox*>(item);
                        if(spinBox){
                            spinBox->setValue(matrix[x][y]);
                        }
                    }
                }
            }
        } else {
            int sz = listAlternative.size();
            for(int x=0; x < sz; x++){
                for(int y=0; y < sz; y++){
                    if(matrix[x][y] != 0 && x!=y){
                        QWidget *item = table->cellWidget(x,y);
                        QDoubleSpinBox *spinBox = qobject_cast<QDoubleSpinBox*>(item);
                        if(spinBox){
                            double value = matrix[x][y];
                            spinBox->setValue(value);
                        }
                    }
                }
            }
        }
        itTables++;
    }
}

void MainWindow::makeTabOrder()
{
    list<Form*>::iterator it;
    it = listCriteria.begin();
    Form* form = *it;
    QWidget *tab = ui->tabWidget->widget(0);
    tab->setTabOrder(ui->objective,form->getCheckBox());
    tab->setTabOrder(form->getCheckBox(),form->getLineEdit());
    it++;
    while(it != listCriteria.end()){
        Form* form2 = *it;
        tab->setTabOrder(form->getLineEdit(), form2->getCheckBox());
        tab->setTabOrder(form2->getCheckBox(), form2->getLineEdit());
        it++;
        form = *it;
    }
    it--;
    form = *it;
    tab->setTabOrder(form->getLineEdit(), ui->btnAddCriterion);
    it = listAlternative.begin();
    form = *it;
    tab->setTabOrder(ui->btnAddCriterion, form->getCheckBox());
    tab->setTabOrder(form->getCheckBox(), form->getLineEdit());
    while(it!= listAlternative.end()){
        Form* form2 = *it;
        tab->setTabOrder(form->getLineEdit(), form2->getCheckBox());
        tab->setTabOrder(form2->getCheckBox(), form2->getLineEdit());
        it++;
        form = *it;
    }
    it--;
    form = *it;
    tab->setTabOrder(form->getLineEdit(), ui->btnAddAlternative);
    tab->setTabOrder(ui->btnAddAlternative, ui->btnDelete);
    tab->setTabOrder(ui->btnDelete, ui->btnUpdate);
}

bool MainWindow::duplicatedCriterion()
{
    bool duplicated = false;
    list<Form*>::iterator it;
    list<Form*>::iterator back;
    back = listCriteria.end();
    back--;
    for(it = listCriteria.begin(); it != back && !duplicated; it++){
        list<Form*>::iterator it2;
        Form* form1 = *it;
        it2 = it;
        it2++;
        while(it2 != listCriteria.end() && !duplicated){
            Form* form2 = *it2;
            if(form1->getLineEdit()->text() == form2->getLineEdit()->text()){
                duplicated = true;
            }
            it2++;
        }
    }
    return duplicated;
}

bool MainWindow::dublicatedAlternative()
{
    bool duplicated = false;
    list<Form*>::iterator it;
    list<Form*>::iterator back;
    back = listAlternative.end();
    back--;
    for(it = listAlternative.begin(); it != back && !duplicated; it++){
        list<Form*>::iterator it2;
        Form* form1 = *it;
        it2 = it;
        it2++;
        while(it2 != listAlternative.end() && !duplicated){
            Form* form2 = *it2;
            if(form1->getLineEdit()->text() == form2->getLineEdit()->text()){
                duplicated = true;
            }
            it2++;
        }
    }
    return duplicated;
}

void MainWindow::addLine(int list){
    QWidget* tab1 = ui->tabWidget->widget(0);
    if(list == CRITERION){
        QCheckBox* checkBox = new QCheckBox(tab1);
        stringstream name;
        name << "checkBoxCriterion" << listCriteria.size();
        checkBox->setObjectName(name.str().c_str());
        checkBox->setToolTip(toolTipStrings[CRI_CB]);
        QLineEdit* lineEdit = new QLineEdit(tab1);
        name.str("");
        name << "criterion" << listCriteria.size();
        lineEdit->setObjectName(name.str().c_str());
        lineEdit->setToolTip(toolTipStrings[CRI_TXT]);
        connect(lineEdit,SIGNAL(textEdited(QString)),this,SLOT(onLineEdit_textEdited(QString)));
        ui->horizontalLayoutCriteria->addWidget(checkBox);
        ui->horizontalLayoutCriteria->addWidget(lineEdit);
        listCriteria.push_back(new Form(checkBox,lineEdit));

    }else if (list == ALTERNATIVE){
        QCheckBox* checkBox = new QCheckBox(tab1);
        stringstream name;
        name << "checkBoxAlternative" << listAlternative.size();
        checkBox->setToolTip(toolTipStrings[ALT_CB]);
        checkBox->setObjectName(name.str().c_str());
        QLineEdit* lineEdit = new QLineEdit(tab1);
        name.str("");
        name << "alternative" << listAlternative.size();
        lineEdit->setObjectName(name.str().c_str());
        lineEdit->setToolTip(toolTipStrings[ALT_TXT]);
        connect(lineEdit,SIGNAL(textEdited(QString)),this,SLOT(onLineEdit_textEdited(QString)));
        ui->horizontalLayoutAlternative->addWidget(checkBox);
        ui->horizontalLayoutAlternative->addWidget(lineEdit);
        listAlternative.push_back(new Form(checkBox,lineEdit));
    }
}

void MainWindow::makeButton(QPushButton* btn, int type){
    if(type == ALTERNATIVE){
        btn->setObjectName("btnAddAlternative");
        btn->setToolTip(toolTipStrings[ALT_BTN]);
        ui->horizontalLayoutAlternative->addWidget(btn);
        connect(btn,SIGNAL(clicked(bool)),this,SLOT(on_btnAddAlternative_clicked()));
    }else if(type == CRITERION){
        btn->setObjectName("btnAddCriteria");
        btn->setToolTip(toolTipStrings[CRI_BTN]);
        ui->horizontalLayoutCriteria->addWidget(btn);
        connect(btn,SIGNAL(clicked(bool)),this,SLOT(on_btnAddCriterion_clicked()));
    }
    btn->setText("ADD");
}


void MainWindow::makeTableCriterion(){
    int nC = listCriteria.size();
    QTableWidget* table = listTable.back();

    table->setColumnCount(nC);
    table->setRowCount(nC);

    list<Form*>::iterator it;
    it = listCriteria.begin();
    QTableWidgetItem *item;
    for (int x = 0; x < nC; ++x) {
        Form *criteria = *it;
        item = new QTableWidgetItem();
        item->setText(criteria->getLineEdit()->text());
        table->setHorizontalHeaderItem(x,item);
        item = new QTableWidgetItem();
        item->setText(criteria->getLineEdit()->text());
        table->setVerticalHeaderItem(x,item);
        it++;
    }
    int tamTable = table->columnCount();
    for (int x = 0; x < tamTable; x++) {
        for (int y = 0; y < tamTable; y++) {
            if(x==y){
                QSpinBox *item1 = new QSpinBox();
                item1->setValue(1);
                item1->setReadOnly(true);
                item1->setAlignment(Qt::AlignCenter);
                item1->setEnabled(false);
                table->setCellWidget(x,y,item1);
            }else{
                QDoubleSpinBox* spinBox = new QDoubleSpinBox();
                spinBox->setDecimals(4);
                spinBox->setMaximum(100);
                spinBox->setValue(1);
                table->setCellWidget(x,y,spinBox);
                connect(spinBox,SIGNAL(valueChanged(double)),this,SLOT(ondoubleSpinBox_valueChanged(double)));
            }
        }
    }
    makeTableWeight(tamTable);

}

void MainWindow::makeTableAlternative()
{
    int nA = listAlternative.size();
    QTableWidget* table = listTable.back();

    table->setColumnCount(nA);
    table->setRowCount(nA);

    list<Form*>::iterator it;
    it = listAlternative.begin();
    QTableWidgetItem *item;
    for (int x = 0; x < nA;x++) {
        Form *alternative = *it;
        item = new QTableWidgetItem();
        item->setText(alternative->getLineEdit()->text());
        table->setHorizontalHeaderItem(x,item);
        item = new QTableWidgetItem();
        item->setText(alternative->getLineEdit()->text());
        table->setVerticalHeaderItem(x,item);
        it++;
    }
    int tamTable = table->columnCount();
    for (int x = 0; x < tamTable; x++) {
        for (int y = 0; y < tamTable; y++) {
            if(x==y){
                QSpinBox *item1 = new QSpinBox();
                item1->setValue(1);
                item1->setReadOnly(true);
                item1->setAlignment(Qt::AlignCenter);
                item1->setEnabled(false);
                table->setCellWidget(x,y,item1);
            }else{
                QDoubleSpinBox* spinBox = new QDoubleSpinBox();
                spinBox->setDecimals(4);
                spinBox->setMaximum(100);
                spinBox->setValue(1);
                table->setCellWidget(x,y,spinBox);
                connect(spinBox,SIGNAL(valueChanged(double)),this,SLOT(ondoubleSpinBox_valueChanged(double)));
            }
        }
    }
    makeTableWeight(tamTable);

}

void MainWindow::makeTableWeight(int size)
{
    makeTableWeight(size,listWVector.size()-1);
}

void MainWindow::makeTableWeight(int size, int index)
{
    list<QTableWidget*>::iterator it;
    it = listWVector.begin();
    while(index>0 && it!=listWVector.end()){
        index--;
        it++;
    }
    QTableWidget* table = *it;

    table->setColumnCount(1);
    table->setRowCount(size);
    table->setMaximumWidth(115);

    QTableWidgetItem *item;
    item = new QTableWidgetItem();
    item->setText("Weight Vector");
    table->setHorizontalHeaderItem(0,item);
    QStringList labels;
    for (int x = 0; x < size; ++x) {
        labels.push_back("");
        QLabel* label = new QLabel();
        label->setText("0");
        label->setAlignment(Qt::AlignCenter);
        stringstream ss;
        ss << "Label_" << x;
        label->setObjectName(QString::fromStdString(ss.str()));
        table->setCellWidget(x,0,label);
    }
    table->setVerticalHeaderLabels(labels);
}

void MainWindow::on_btnDelete_clicked(){
    QMessageBox msgBox;
    msgBox.setWindowTitle("Warning!");
    msgBox.setText("The selected fields are going to be deleted.\nAre you sure you want to continue?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    int res = msgBox.exec();

    if(res == QMessageBox::Yes){
        int c = deleteCheckedFields();
        if(c>0){
            changed = true;
            addedFiled = true;
            updateLineEditNames();
            updateTables();
        }
    }

}



void MainWindow::decide()
{
    decisor = new Decisor();
    updateWVector();
    string res = decisor->print();
    double* consistency = decisor->getCr();
    cr.clear();
    int sz = listTable.size();
    for(int x = 0; x < sz; x++){
       cr.push_back(consistency[x]);
    }
    double* r = decisor->getResults();
    results.clear();
    sz = listAlternative.size();
    for(int x = 0; x < sz; x++){
        results.push_back(r[x]);
    }
    ui->textBrowser->setText(QString::fromStdString(res));
    lambda_max = decisor->getLambda_max();
    delete decisor;
    decidePressed = true;
}

void MainWindow::ondoubleSpinBox_valueChanged(double arg1)
{
    QString s = sender()->objectName();
    QStringList nome = s.split("_");
    int x = nome.at(1).toInt();
    int y = nome.at(2).toInt();
    int c = nome.at(3).toInt();
    list<QTableWidget*>::iterator it;
    it = listTable.begin();
    while(c>0){
        c--;
        it++;
    }
    QTableWidget* table = *it;
    QWidget *item = table->cellWidget(x,y);
    QDoubleSpinBox *spinBox = qobject_cast<QDoubleSpinBox*>(item);
    if(spinBox){
        item = table->cellWidget(y,x);
        spinBox = qobject_cast<QDoubleSpinBox*>(item);
        if(spinBox){
            spinBox->blockSignals(true);
            spinBox->setValue(1/arg1);
            spinBox->blockSignals(false);
        }
    }
    changed = true;
    if(storedTables.empty() && ui->actionAutomatic_Calculation->isChecked()){
        decide();
    }
}

void MainWindow::onLineEdit_textEdited(const QString & text)
{
    qDebug() << "line being edited ";
    if(ui->tabWidget->count()>1){
        QLineEdit* line = static_cast<QLineEdit*>(sender());
        qDebug() << line->objectName();
        if(line->objectName().contains("criterion")){
            updateCriterion(line);
        }else{
            updateAlternative(line);
        }
    }
    changed = true;
}


void MainWindow::on_btnAddCriterion_clicked()
{
    //if(listCriteria.size()<10){
        QWidget* tab1 = ui->tabWidget->widget(0);

        ui->btnAddCriterion->deleteLater();
        addLine(CRITERION);

        ui->btnAddCriterion = new QPushButton(tab1);
        makeButton(ui->btnAddCriterion,CRITERION);
        //makeTabOrder();
        changed = true;
        addedFiled = true;
        if(!openingFile){
            updateTables();
        }
    //}

}

void MainWindow::on_btnAddAlternative_clicked(){

    //if(listAlternative.size()<10){
        QWidget* tab1 = ui->tabWidget->widget(0);
        ui->btnAddAlternative->deleteLater();
        addLine(ALTERNATIVE);

        ui->btnAddAlternative = new QPushButton(tab1);
        makeButton(ui->btnAddAlternative, ALTERNATIVE);
        //makeTabOrder();
        changed = true;
        addedFiled = true;
        if(!openingFile){
            updateTables();
        }
    //}
}


void MainWindow::on_btnUpdate_clicked(){
    int nC = listCriteria.size();
    int nA = listAlternative.size();
    bool foundBlankField = false;
    list<Form*>::iterator x;
    for(x = listCriteria.begin(); x != listCriteria.end(); x++){
        Form* form = *x;
        string content = form->getLineEdit()->text().toStdString();
        if(content == ""){
            foundBlankField = true;
            break;
        }
    }
    for(x = listAlternative.begin(); x != listAlternative.end(); x++){
        Form* form = *x;
        string content = form->getLineEdit()->text().toStdString();
        if(content == ""){
            foundBlankField = true;
            break;
        }
    }
    if(foundBlankField){
        QMessageBox msgBox;
        msgBox.setWindowTitle("Warning!");
        msgBox.setText("Please fill out or delete blank text fields.");
        //msgBox.setInformativeText("Please fill out or delete blank text fields.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
    }/*else if(nC<2 || nA<2){
        QMessageBox msgBox;
        msgBox.setWindowTitle("Warning!");
        msgBox.setText("You need to fill out at least two Criteria and two Alternatives.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
    }*/else if(duplicatedCriterion()){
        QMessageBox msgBox;
        msgBox.setWindowTitle("Warning!");
        msgBox.setText("There are duplicated Criteria.\nPlease rename or delete them.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
    }else if(dublicatedAlternative()){
        QMessageBox msgBox;
        msgBox.setWindowTitle("Warning!");
        msgBox.setText("There are duplicated Alternative.\nPlease rename or delete them.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
    }else{
        int nForms;
        nForms = (nC*(nA*(nA-1))/2) + (nC*(nC-1)/2);

        stringstream warning;
        warning << "It will be necessary ";
        warning << nForms << " pairwise comparisons." << endl;
        warning << "Do you want to continue?";

        QMessageBox msgBox;
        msgBox.setWindowTitle("Warning!");
        msgBox.setText(QString::fromStdString(warning.str()));
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);
        int res = msgBox.exec();

        if(res == QMessageBox::Yes){
            ui->actionToggle_Scale->setChecked(false);
            updateTables();

        }
        if(res == QMessageBox::No){
        }
    }
}


void MainWindow::on_actionExit_triggered()
{
    int res = unsavedChangesWarning();
    if( res == QMessageBox::Yes) {
        close();
    }
}

void MainWindow::on_actionOpen_triggered()
{
    if(unsavedChangesWarning() == QMessageBox::Yes){
        QString fileName = QFileDialog::getOpenFileName(ui->centralWidget,tr("Open File"),NULL, tr("Text (*.txt);;All Files (*.*)"));
        if(fileName != ""){
            saveAdress = fileName;
            deleteAll();
            openingFile = true;
            FileReader *reader = new FileReader(fileName.toStdString());
            string obj = reader->getObjective();
            vector<string> alternatives = reader->getAlternatstr();
            vector<string> criteria = reader->getCriteriastr();
            if(criteria[criteria.size()-1]==""){
                criteria.pop_back();
            }
            vector<double**> matrices = reader->getMatrices();
            storedBOCR = reader->getMatrixBOCR();
            ui->objective->setText(QString(obj.c_str()));
            importAlternatives(alternatives);
            importCriteria(criteria);
            updateTables();
            importTables(matrices);
            delete reader;
            decide();
            changed = false;
            addedFiled = false;
            openingFile = false;
        }
    }
}

void MainWindow::on_actionSaveModelAs_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(ui->centralWidget,tr("Save File"),NULL, tr("Text (*.txt)"));
    if(fileName != ""){
        on_btnUpdate_clicked();
        FileWriter* writer = makeWriter();
        writer->save(fileName);
        delete writer;

        changed = false;
        addedFiled = false;
    }
    saveAdress = fileName;
}

void MainWindow::on_actionSaveAll_triggered()
{    
    QString fileName = QFileDialog::getSaveFileName(ui->centralWidget,tr("Save File"),NULL, tr("Text (*.txt)"));
    if(fileName != ""){
        on_btnUpdate_clicked();
        updateTables();
        saveAll(fileName);
    }
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("About");
    msgBox.setTextFormat(Qt::RichText);
    stringstream ss;
    ss << "<font face=\"Arial\" size=\"+1\">Decisor<br>";
    ss << "More information on:<br></font>";
    ss << "<a href=\"https://github.com/czekster/Decisor/wiki\">https://github.com/czekster/Decisor/wiki</a>";
    msgBox.setText(ss.str().c_str());
    msgBox.setStandardButtons(QMessageBox::Close);
    msgBox.setDefaultButton(QMessageBox::Close);
    msgBox.exec();
    //QMessageBox::about(ui->centralWidget,"About","<a href=\"https://github.com/unisc/DecisorGui\">https://github.com/unisc/DecisorGui</a>");
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    int res = unsavedChangesWarning();
    if( res == QMessageBox::Yes) {
        close();
    } else {
       event->ignore();
    }
}

void MainWindow::init()
{
    on_btnAddCriterion_clicked();
    on_btnAddCriterion_clicked();
    on_btnAddAlternative_clicked();
    on_btnAddAlternative_clicked();
    /*int c=1;
    list<Form*>::iterator it;
    for(it = listAlternative.begin();it!= listAlternative.end();it++){
        Form* form= *it;
        form->getLineEdit()->setText(QString("Alternative").append(c));
        c++;
    }
    c = 1;
    for(it = listCriteria.begin();it!= listCriteria.end();it++){
        Form* form= *it;
        form->getLineEdit()->setText(QString("Criterion").append(c));
        c++;
    }*/
}

void MainWindow::on_actionNew_triggered()
{
    if(unsavedChangesWarning() == QMessageBox::Yes){
        deleteAll();
        init();
        changed = false;
        addedFiled = false;
        decidePressed = false;
        saveAdress;
        ui->actionToggle_Scale->setChecked(false);
    }
}

void MainWindow::makeToolTipStrings()
{
    toolTipStrings = new QString[SIZE];
    toolTipStrings[OBJ_TXT] = QString("Type your objective here");
    toolTipStrings[CRI_TXT] = QString("Type a criterion here");
    toolTipStrings[CRI_CB] = QString("Mark this criterion for deletion");
    toolTipStrings[ALT_TXT] = QString("Type an alternative here");
    toolTipStrings[ALT_CB] = QString("Mark this alternative for deletion");
    toolTipStrings[CRI_BTN] = QString("Add another criterion field");
    toolTipStrings[ALT_BTN] = QString("Add another alternative field");
    toolTipStrings[DEL_BTN] = QString("Delete all selected fields");
    toolTipStrings[UPD_BTN] = QString("Update tables");
    toolTipStrings[RESULT] = QString("The results will be displayed here");


}


void MainWindow::on_actionNewBOCR_triggered()
{
    if(decidePressed){
        on_actionLoadBOCR_triggered();
        bocrWindow->clearTable();
    }
    else{

    }
}

void MainWindow::on_actionLoadBOCR_triggered()
{
    if(!decidePressed){
        return;
    }
    int answer = QMessageBox::Yes;
    if(changed){
        QMessageBox msgBox;
        msgBox.setWindowTitle("Warning!");
        msgBox.setText("You must save your changes to the model before continuing.\n Do you want to continue?\n(this will prompt you to save your work)");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::Yes);
        answer = msgBox.exec();
        if(answer == QMessageBox::Yes){
            on_actionSaveModelAs_triggered();
            if(saveAdress == ""){
                return;
            }
        }
    }
    if(answer == QMessageBox::Yes){
        list<Alternative*> alternatives;
        int x=0;
        for(list<Form*>::iterator it = listAlternative.begin(); it != listAlternative.end(); it++, x++){
            Form *form = *it;
            string nome = form->getLineEdit()->text().toStdString();
            double weight = results[x];
            Alternative* alternative= new Alternative(nome,weight);
            alternatives.push_back(alternative);
        }
        bocrWindow = new BocrWindow();
        bocrWindow->setAlternatives(alternatives);        
        bocrWindow->setFileAdress(this->saveAdress);
        bocrWindow->setWindowModality(Qt::ApplicationModal);
        bocrWindow->setAttribute(Qt::WA_DeleteOnClose);
        bocrWindow->show();        
    }
}

void MainWindow::on_actionSet_Scale_triggered()
{
    scale->exec();
    if(decidePressed){
        ui->actionToggle_Scale->setChecked(false);
        decide();
    }
}

void MainWindow::on_actionToggle_Scale_toggled(bool checked)
{
    if(decidePressed){
        if(checked){
            storedTables.resize(listTable.size());
        }
        int c = 0;
        for(list<QTableWidget*>::iterator it = listTable.begin(); it != listTable.end(); it++){
            QTableWidget* table = *it;
            if(checked){
                storedTables[c].resize(table->rowCount());
            }
            for (int x = 0; x < table->rowCount(); ++x) {
                if(checked){
                    storedTables[c][x].resize(table->columnCount());
                }
                for (int y = 0; y < table->columnCount(); ++y) {
                    QWidget *item = table->cellWidget(x,y);
                    QDoubleSpinBox *spin = static_cast<QDoubleSpinBox*>(item);
                    if(checked){
                        double value = spin->value();
                        if(value>1){
                            storedTables[c][x][y] = value;
                            spin->setValue(getScaleValue(value));
                        }else{
                            storedTables[c][x][y] = 0.0;
                        }
                        spin->setDisabled(true);
                    }else{
                        double value = storedTables[c][x][y];
                        if(value>=1){
                            spin->setValue(value);
                        }
                        spin->setEnabled(true);
                    }
                }
            }
            c++;
        }
        if(!checked){
            storedTables.clear();
        }
    }
}

void MainWindow::on_actionOpen_Several_triggered()
{
    if(unsavedChangesWarning() == QMessageBox::Yes){
        QStringList fileNames = QFileDialog::getOpenFileNames(ui->centralWidget,tr("Open File"),NULL, tr("Text (*.txt);;All Files (*.*)"));
        if(fileNames.size()>1){
            FileReader **readers = new FileReader*[fileNames.size()];
            vector<string> alternatives;
            vector<string> criteria;
            int altSize = 0;
            int criSize = 0;
            vector<vector<double**>> allMatrices;
            for (int x = 0; x < fileNames.size(); ++x) {
                readers[x] = new FileReader(fileNames.at(x).toStdString());
                if(x == 0){
                    alternatives = readers[x]->getAlternatstr();
                    criteria = readers[x]->getCriteriastr();
                    altSize = alternatives.size();
                    criSize = criteria.size();
                }else{
                    if(altSize != readers[x]->getAlternatstr().size() ||
                            criSize != readers[x]->getCriteriastr().size()){
                        QMessageBox msgBox;
                        msgBox.setWindowTitle("Warning!");
                        msgBox.setText("You have selected files that do not have\nthe same number of criteria and/or alternatives.");
                        msgBox.setStandardButtons(QMessageBox::Ok);
                        msgBox.setDefaultButton(QMessageBox::Ok);
                        msgBox.exec();
                        for(int y = 0; y <= x; y++){
                            delete readers[y];
                        }
                        delete[] readers;
                        return;
                    }
                }
                allMatrices.push_back(readers[x]->getMatrices());
            }
            if(criteria[criteria.size()-1]==""){
                criteria.pop_back();
                criSize--;
            }
            vector<double**> newMatrices;
            for (int x = 0; x < criSize+1; ++x){
                int maxValue = 0;
                if(x == 0){
                    maxValue = criSize;
                }else{
                    maxValue = altSize;
                }
                double** mult = new double*[maxValue];
                for (int i = 0; i < maxValue; ++i) {
                    mult[i] = new double[maxValue]();
                }
                newMatrices.push_back(mult);
            }
            int sz = allMatrices.size();
            for (int x = 0; x < sz; ++x) {
                vector<double**> matrices = allMatrices[x];
                int sz2 = matrices.size();
                for (int y = 0; y < sz2; ++y) {
                    double** matrix = matrices[y];
                    double** mult = newMatrices[y];
                    int maxValue = 0;
                    if(y == 0){
                        maxValue = criSize;
                    }else{
                        maxValue = altSize;
                    }
                    for (int i = 0; i < maxValue; ++i) {
                        for (int j = 0; j < maxValue; ++j) {
                            if(x == 0){
                                mult[i][j] = matrix[i][j];
                            }else{
                                mult[i][j] *= matrix[i][j];
                            }
                        }
                    }
                }
            }
            sz = newMatrices.size();
            for (int x = 0; x < sz; ++x) {
              double** v = newMatrices[x];
              int maxValue = 0;
              if(x == 0){
                  maxValue = criSize;
              }else{
                  maxValue = altSize;
              }
              for (int i = 0; i < maxValue; ++i) {
                  for (int j = 0; j < maxValue; ++j) {
                      if(v[i][j] > 1){
                          double one = 1;
                          double power = one/fileNames.size();
                          v[i][j] = pow(v[i][j], power);
                      }
                  }
              }
            }
            deleteAll();
            storedBOCR = readers[0]->getMatrixBOCR();
            importAlternatives(alternatives);
            importCriteria(criteria);
            updateTables();
            importTables(newMatrices);
            delete[] readers;
            decide();
            changed = true;
            addedFiled = false;
        }
    }
}

void MainWindow::on_actionSaveModel_triggered()
{
    if(saveAdress != ""){
            on_btnUpdate_clicked();
            FileWriter* writer = makeWriter();
            writer->save(saveAdress);
            delete writer;

            changed = false;
            addedFiled = false;
    }else{
        on_actionSaveModelAs_triggered();
    }
}

void MainWindow::on_actionCalculate_triggered()
{
    if(ui->tabWidget->count()>1){
            decide();
    }
}

void MainWindow::on_actionAutomatic_Calculation_toggled(bool arg1)
{
    if(arg1 == false)
        ui->textBrowser->setText("YOU HAVE SELECTED NOT TO PERFORM AUTOMATIC CALCULATIONS.\n PLEASE CLICK ON 'DECIDE' (CTRL-D) TO COMPUTE THE RESULTS WHEN YOU ARE FINISHED ALTERING THE WEIGHTS");
    else
        on_actionCalculate_triggered();
}
