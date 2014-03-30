/*
 * ResizeDialog.cpp
 *
 *  Created on: 09.06.2009
 *      Author: Bastian
 */

#include <QtGui>
 #include "TransparencyDialog.h"

 TransparencyDialog::TransparencyDialog(QWidget *parent,ModelEditor *editor)
     : QDialog(parent)
 {

     addButton = new QPushButton(tr("&Add"));
     int nTrans=0;
     short * tvalues=editor->getTransparency(&nTrans);
     //wrong array creation->need new one
     sliders=(QSlider **)(new QSlider[nTrans]);
     /*for(int i=0;i<nTrans;i++){
    	 sliders[i]=new QSlider(Qt::Horizontal);
    	 sliders[i]->setRange(0,0x7FFF);
    	 sliders[i]->setSingleStep(0x7F);
         sliders[i]->setPageStep(0x7F0);
         sliders[i]->setSliderPosition(tvalues[i]);
         sliders[i]->setTickInterval(0x7F0);
         sliders[i]->setTickPosition(QSlider::TicksRight);
     }*/
     QHBoxLayout *layout = new QHBoxLayout;
     layout->addWidget(addButton);
     /*for(int i=0;i<nTrans;i++){
    	 layout->addWidget(sliders[i]);
     }*/


     setLayout(layout);
     setWindowTitle(tr("Set Transparency"));
     connect(addButton, SIGNAL(clicked()), this, SLOT(addClicked()));
     connect(addButton, SIGNAL(clicked()), this, SLOT(accept()));
 }
 void TransparencyDialog::addClicked()
 {
    hide();
 }
