#ifndef FORM_H
#define FORM_H


#include <QLineEdit>
#include <QCheckBox>
#include <list>

using namespace std;


class Form
{
private:
    QLineEdit *lineEdit;
    QCheckBox *checkBox;

public:
    Form();
    Form(QCheckBox* checkBox, QLineEdit* lineEdit);
    QLineEdit* getLineEdit();
    QCheckBox* getCheckBox();
};

#endif // FORM_H
