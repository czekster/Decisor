#include "form.h"

Form::Form()
{

}

Form::Form(QCheckBox* checkBox, QLineEdit* lineEdit){
    this->checkBox = checkBox;
    this->lineEdit = lineEdit;
}

QLineEdit* Form::getLineEdit(){
    return lineEdit;
}

QCheckBox* Form::getCheckBox(){
    return checkBox;
}

