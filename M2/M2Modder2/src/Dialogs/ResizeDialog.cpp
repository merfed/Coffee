/*
 * ResizeDialog.cpp
 *
 *  Created on: 09.06.2009
 *      Author: Bastian
 */

#include <QtGui>
 #include "ResizeDialog.h"

 ResizeDialog::ResizeDialog(QWidget *parent)
     : QDialog(parent)
 {
     QLabel *resizeLabel = new QLabel(tr("Enter the Resize-Factor:"));
     lineEdit = new QLineEdit;

     resizeButton = new QPushButton(tr("&Resize"));
     factorText = "1.0";

     QHBoxLayout *layout = new QHBoxLayout;
     layout->addWidget(resizeLabel);
     layout->addWidget(lineEdit);
     layout->addWidget(resizeButton);

     setLayout(layout);
     setWindowTitle(tr("Resize your Model"));
     connect(resizeButton, SIGNAL(clicked()), this, SLOT(resizeClicked()));
     connect(resizeButton, SIGNAL(clicked()), this, SLOT(accept()));
 }
 void ResizeDialog::resizeClicked()
 {
     QString text = lineEdit->text();

     if (text.isEmpty()) {
         QMessageBox::information(this, tr("Empty Field"),
             tr("Please the Factor"));
         return;
     } else {
         factorText = text;
         lineEdit->clear();
         hide();
     }
 }
 QString ResizeDialog::getFactor()
 {
     return factorText;
 }
