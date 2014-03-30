/*
 * ResizeDialog.cpp
 *
 *  Created on: 09.06.2009
 *      Author: Bastian
 */

#include <QtGui>
 #include "TextureDialog.h"

 TextureDialog::TextureDialog(QWidget *parent)
     : QDialog(parent)
 {
     QLabel *resizeLabel = new QLabel(tr("Enter the TexName:"));
     lineEdit = new QLineEdit;

     addButton = new QPushButton(tr("&Add"));
     Texture = "";
     flag=new QComboBox();
     flag->addItem(tr("0 None"));
     flag->addItem(tr("1 - X-Wrap"));
     flag->addItem(tr("2 - Y-Wrap"));
     flag->addItem(tr("3 - XY-Wrap"));
     type=new QComboBox();
     type->addItem(tr("0 Hardcoded"));
     type->addItem(tr("1 Body&Clothes"));
     type->addItem(tr("2 Cape"));
     type->addItem(tr("6 Hair&Beard"));
     type->addItem(tr("8 Tauren Fur"));
     type->addItem(tr("11 CreatureSkin1"));
     type->addItem(tr("12 CreatureSkin2"));
     type->addItem(tr("13 CreatureSkin3"));
     QHBoxLayout *layout = new QHBoxLayout;
     layout->addWidget(flag);
     layout->addWidget(type);
     layout->addWidget(resizeLabel);
     layout->addWidget(lineEdit);
     layout->addWidget(addButton);


     setLayout(layout);
     setWindowTitle(tr("Add Texture"));
     connect(addButton, SIGNAL(clicked()), this, SLOT(addClicked()));
     connect(addButton, SIGNAL(clicked()), this, SLOT(accept()));
 }
 void TextureDialog::addClicked()
 {
    QString text = lineEdit->text();
    modeltex.nameLen=text.length()+1;
    modeltex.flags=flag->currentIndex();
    switch(type->currentIndex()){
    case 3:modeltex.type=6;break;
    case 4:modeltex.type=8;break;
    case 5:modeltex.type=11;break;
    case 6:modeltex.type=12;break;
    case 7:modeltex.type=13;break;
    default:modeltex.type=type->currentIndex();break;
    }
    Texture = text;
    lineEdit->clear();
    hide();
 }
 QString TextureDialog::getTexture()
 {
     return Texture;
 }

 ModelTextureDef TextureDialog::getTexDef(){
	 return modeltex;
 }
