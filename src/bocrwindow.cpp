#include "bocrwindow.h"
#include "ui_bocrwindow.h"

using namespace std;

BocrWindow::BocrWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BocrWindow)
{
    ui->setupUi(this);
    reader = 0;
    writer = 0;
}

BocrWindow::~BocrWindow()
{
    delete ui;
    if(!listResults.empty()){
        while(!listResults.empty()){
            Alternative *a = listResults.front();
            delete a;
            listResults.pop_front();
        }
        listResults.clear();
    }
    if(!alternatives.empty()){
        while(!alternatives.empty()){
            Alternative *a = alternatives.front();
            delete a;
            alternatives.pop_front();
        }
        alternatives.clear();
    }
    if(reader){
        delete reader;
    }
    if(writer){
        delete writer;
    }
}

void BocrWindow::setAlternatives(const list<Alternative*> &value)
{
    alternatives = value;
    double *weight = (double*)malloc(sizeof(double) * alternatives.size());
    //alternatives.sort(Alternative::sort);
    ui->tableWidget->setRowCount(alternatives.size());
    ui->textBrowser->setMaximumHeight(20*(alternatives.size()+1));
    int c = 0;
    for(list<Alternative*>::iterator it = alternatives.begin(); it != alternatives.end(); it++, c++){
        Alternative* a = *it;
        weight[c] = a->getWeight();
        QTableWidgetItem *item = new QTableWidgetItem();
        item->setText(a->getNome().c_str());
        ui->tableWidget->setVerticalHeaderItem(c,item);
    }
    int row = ui->tableWidget->rowCount();
    int column = ui->tableWidget->columnCount();
    for (int x = 0; x < row; x++) {
        for (int y = 0; y < column; y++) {
            QDoubleSpinBox *item = new QDoubleSpinBox();
            item->setAlignment(Qt::AlignCenter);
            item->setDecimals(4);
            item->setMaximum(numeric_limits<double>::max());
            if(y==0){
                item->setReadOnly(true);
                item->setEnabled(false);
                item->setValue(weight[x]);
            }else{
                item->setValue(0);
            }
            //connect(item,SIGNAL(valueChanged(double)),this,SLOT(ondoubleSpinBox_valueChanged(double)));
            ui->tableWidget->setCellWidget(x,y,item);
        }
    }
    ui->tableWidget_2->setRowCount(row);
    QStringList labels;
    for (int x = 0; x < row; ++x) {
        labels.push_back("");
        QLabel* label = new QLabel();
        label->setText("0");
        label->setAlignment(Qt::AlignCenter);
        stringstream ss;
        ss << "Label_" << x;
        label->setObjectName(QString::fromStdString(ss.str()));
        ui->tableWidget_2->setCellWidget(x,0,label);
    }
    ui->tableWidget_2->setVerticalHeaderLabels(labels);
    /*for (int x = 0; x < ui->tableWidget->rowCount(); ++x) {
        QWidget* w = ui->tableWidget->cellWidget(x,column);
        w->setEnabled(false);
    }*/
    free(weight);
}

void BocrWindow::on_pushButton_clicked()
{
    int row = ui->tableWidget_2->rowCount();
    list<double*> normalized = normalizeTablesWithBOCR();
    double* results = new double[row];
    double* aux = new double[row];
    for (int var = 0; var < row; ++var) {
        if(ui->comboBox->currentIndex() == MULT){
            results[var] = 1;
            aux[var] = 1;
        }else{
            results[var] = 0;
            aux[var] = 0;
        }
    }
    int c = 0;
    copyAlternatives();

    for(list<double*>::iterator it = normalized.begin(); it != normalized.end(); it++){
        double* values = *it;
        for (int var = 0; var < row; ++var) {
            switch (ui->comboBox->currentIndex()) {
            case MULT:
                if(c<2){
                    results[var] *= values[var];
                }else{
                    aux[var] *= values[var];
                }
                break;
            case REC:
                results[var] += values[var];
                break;
            case NEG:
                if(c<2){
                    results[var] += values[var];
                }else{
                    results[var] -= values[var];
                }
                break;
            default:
                break;
            }
        }
        delete[] values;
        c++;
    }
    if(ui->comboBox->currentIndex() == MULT){
        for (int x = 0; x < row; ++x) {
            results[x] /= aux[x];
        }
    }
    list<Alternative*>::iterator it2;
    it2 = listResults.begin();
    for (int x = 0; x < row; ++x) {
        QWidget* widget = ui->tableWidget_2->cellWidget(x,0);
        QLabel* label = qobject_cast<QLabel*>(widget);
        if(label){
            label->setText(QString::number(results[x], 'f', 4));
            Alternative* a = *it2;
            a->setWeight(results[x]);
        }
        it2++;
    }

    showResults();
}

void BocrWindow::showResults()
{
    listResults.sort(Alternative::sort);
    list<Alternative*>::iterator it;
    stringstream ss;
    ss << "Results:" << endl;
    for(it = listResults.begin();it != listResults.end(); it++){
        Alternative* a = *it;
        ss << a->getNome() << ":\t" << setprecision(4) << a->getWeight() << endl;
    }
    ui->textBrowser->setText(ss.str().c_str());
}

void BocrWindow::copyAlternatives()
{

    if(!listResults.empty()){
        while(!listResults.empty()){
            Alternative *a = listResults.front();
            delete a;
            listResults.pop_front();
        }
    }
    list<Alternative*>::iterator it;
    for(it = alternatives.begin(); it != alternatives.end(); it++){
        Alternative *a = *it;
        Alternative *b  = new Alternative(a->getNome(),a->getWeight());
        listResults.push_back(b);
    }
}

double *BocrWindow::getValues(int row)
{
    double* values = new double[Alternative::BOCR_TOTAL];
    for (int x = 1; x <= Alternative::BOCR_TOTAL; ++x) {
        QWidget *item = ui->tableWidget->cellWidget(row,x);
        QDoubleSpinBox *spin = qobject_cast<QDoubleSpinBox*>(item);
        if(spin){
            values[x-1] = spin->value();
        }
    }
    return values;
}

void BocrWindow::normalizeTables()
{
    int row = ui->tableWidget->rowCount();
    int column = ui->tableWidget->columnCount();
    double *values = (double*)malloc(sizeof(double) * row);
    for (int var = 1; var < column; ++var) {
        double sum = 0;
        for (int x = 0; x < row; ++x) {
            QWidget *item = ui->tableWidget->cellWidget(x,var);
            QDoubleSpinBox *spinBox = qobject_cast<QDoubleSpinBox*>(item);
            if(spinBox){
                values[x] = spinBox->value();
                sum += spinBox->value();
            }
        }
        for (int x = 0; x < row; ++x) {
            if(sum != 0){
                values[x] /= sum;
            }
        }

        for (int x = 0; x < row; ++x) {
            QWidget *item = ui->tableWidget->cellWidget(x,var);
            QDoubleSpinBox *spinBox = qobject_cast<QDoubleSpinBox*>(item);
            if(spinBox){
                spinBox->setValue(values[x]);
                spinBox->setEnabled(false);
            }
        }
    }
    free(values);
}


list<double*> BocrWindow::normalizeTablesWithBOCR()
{
    int row = ui->tableWidget->rowCount();
    int column = ui->tableWidget->columnCount();
    list<double*> normalized;
    double* values;
    for (int var = 1; var < column; ++var) {
        double sum = 0;
        values = new double[row];
        for (int x = 0; x < row; ++x) {
            QWidget *item = ui->tableWidget->cellWidget(x,var);
            QDoubleSpinBox *spinBox = qobject_cast<QDoubleSpinBox*>(item);
            if(spinBox){
                values[x] = spinBox->value();
                sum += spinBox->value();
            }
        }
        list<Alternative*>::iterator it;
        it = alternatives.begin();
        for (int x = 0; x < row; ++x) {
            Alternative* a = *it;
            if(sum != 0){
                values[x] /= sum;
            }

            switch (ui->comboBox->currentIndex()) {
            case MULT:
                values[x] = pow(values[x],a->getWeight());
                break;
            case REC:
                if(var<3 || a->getWeight() == 0){
                    values[x] *= a->getWeight();
                }else{
                    if(values[x] != 0){
                        values[x] = a->getWeight()/values[x];
                    }
                }
                break;
            case NEG:
                if(values[x] != 0){
                    values[x] *= a->getWeight();
                }
                break;
            default:
                break;
            }


            it++;
        }
        normalized.push_back(values);
    }
    return normalized;
}

void BocrWindow::saveTable(QString fileName)
{
    ofstream file;
    file.open(fileName.toStdString(), ofstream::out|ofstream::app);
    stringstream ss;
    ss << SEPARATOR << "BOCR Matrix" << endl;
    for (int x = 0; x < ui->tableWidget->rowCount(); x++) {
        for (int y = 1; y < ui->tableWidget->columnCount(); y++) {
            QWidget *item = ui->tableWidget->cellWidget(x,y);
            QDoubleSpinBox *spin = static_cast<QDoubleSpinBox*>(item);
            if(spin){
                double value = spin->value();
                if(y != ui->tableWidget->columnCount()-1){
                    ss << value << DELIMITER;
                }else{
                    ss << value << endl;
                }
            }else{
                //ERROR
            }
        }
    }
    file << ss.str();
    file << SEPARATOR;
    file.close();
}

void BocrWindow::readTable(QString fileName)
{
    bool found = false;
    QFile myfile(fileName);
    if (!myfile.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&myfile);
    while (!in.atEnd()) {
        QString line = in.readLine();
        stringstream ss;
        ss << SEPARATOR << "BOCR Matrix";
        if(line.compare(QString(ss.str().c_str()))==0){
            found = true;
            break;
        }
    }
    if(found){
        int c = 0;
        while(!in.atEnd()){
            QString line = in.readLine();
            QStringList list = line.split(QRegExp("\\t"), QString::SkipEmptyParts);
            for (int x = 0; x < list.size(); ++x) {
                QString value = list.at(x);
                QWidget *item = ui->tableWidget->cellWidget(c,x+1);
                QDoubleSpinBox *spin = static_cast<QDoubleSpinBox*>(item);
                if(spin){
                    spin->setValue(value.toDouble());
                }
            }
        c++;
        }
    }


}

void BocrWindow::on_pushButton_2_toggled(bool checked)
{
    if(checked){
        ui->pushButton_2->setText("Toggle Actual Values");
        list<Alternative*>::iterator it = alternatives.begin();
        int c = 0;
        while(it!=alternatives.end()){
            Alternative* a = *it;
            a->setBocrValues(this->getValues(c));
            it++;
            c++;
        }
        normalizeTables();

    }else{
        ui->pushButton_2->setText("Toggle Normalized");
        list<Alternative*>::iterator it;
        it = alternatives.begin();
        int c = 0;
        while(it!=alternatives.end()){
            Alternative* a = *it;
            double* values = a->getBocrValues();
            for (int x = 1; x <= Alternative::BOCR_TOTAL; ++x) {
                QWidget *item = ui->tableWidget->cellWidget(c,x);
                QDoubleSpinBox *spinBox = qobject_cast<QDoubleSpinBox*>(item);
                if(spinBox){
                    spinBox->setValue(values[x-1]);
                    spinBox->setEnabled(true);
                }
            }
            c++;
            it++;
        }
    }
}

void BocrWindow::on_actionSave_triggered()
{    
    if(fileAdress != ""){
        double** tableBOCR = new double*[ui->tableWidget->rowCount()];
        for (int x = 0; x < ui->tableWidget->rowCount(); x++) {
            tableBOCR[x] = new double[4];
            for (int y = 1; y < ui->tableWidget->columnCount(); y++) {
                QWidget *item = ui->tableWidget->cellWidget(x,y);
                QDoubleSpinBox *spin = static_cast<QDoubleSpinBox*>(item);
                if(spin){
                    tableBOCR[x][y-1] = spin->value();
                }else{
                    //ERROR
                }
            }
        }
        writer->setTableBOCR(tableBOCR);
        writer->save(fileAdress);
    }
}

void BocrWindow::setFileAdress(const QString &value)
{
    fileAdress = value;
    //readTable(fileAdress);
    reader = new FileReader(fileAdress.toStdString());
    writer = new FileWriter();
    vector<string> v = reader->getAlternatstr();
    QStringList l;
    int sz = v.size();
    for (int x = 0; x < sz; ++x) {
        l.push_back(v[x].c_str());
    }
    writer->setListAlternative(l);
    l.clear();
    v.clear();

    v = reader->getCriteriastr();
    if(v[v.size()-1]==""){
        v.pop_back();
    }
    sz = v.size();
    for (int x = 0; x < sz; ++x) {
        l.push_back(v[x].c_str());
    }
    writer->setListCriteria(l);

    vector<double**> tables = reader->getMatrices();
    list<double**> listTables;
    sz = tables.size();
    for (int x = 0; x < sz; ++x) {
        double** table = tables[x];
        int size;
        if(x==0){
            size = v.size();
        }else{
            size = reader->getAlternatstr().size();
        }
        double** values = new double*[size];
        for (int i = 0; i < size; ++i) {
            values[i] = new double[size];
            for (int j = 0; j < size; ++j) {
                values[i][j] = table[i][j];
            }
        }
        listTables.push_back(values);
    }
    writer->setListTable(listTables);

    vector<vector<double>> matrix = reader->getMatrixBOCR();
    sz = matrix.size();
    for (int x = 0; x < sz; ++x) {
        int sz2 = matrix[x].size();
        for (int y = 0; y < sz2; ++y) {
            QWidget *item = ui->tableWidget->cellWidget(x,y+1);
            QDoubleSpinBox *spin = static_cast<QDoubleSpinBox*>(item);
            if(spin){
                spin->setValue(matrix[x][y]);
            }
        }
    }
}

void BocrWindow::clearTable()
{
    on_actionNew_triggered();
}

void BocrWindow::clear()
{
    if(!listResults.empty()){
        while(!listResults.empty()){
            Alternative *a = listResults.front();
            delete a;
            listResults.pop_front();
        }
        listResults.clear();
    }
    if(!alternatives.empty()){
        while(!alternatives.empty()){
            Alternative *a = alternatives.front();
            delete a;
            alternatives.pop_front();
        }
        alternatives.clear();
    }
    if(reader){
        delete reader;
    }
    if(writer){
        delete writer;
    }
}

void BocrWindow::on_actionNew_triggered()
{
    for (int x = 0; x < ui->tableWidget->rowCount(); x++) {
        for (int y = 1; y < ui->tableWidget->columnCount(); y++) {
            QWidget *item = ui->tableWidget->cellWidget(x,y);
            QDoubleSpinBox *spin = static_cast<QDoubleSpinBox*>(item);
            if(spin){
                spin->setValue(0);
            }
        }
    }
}
