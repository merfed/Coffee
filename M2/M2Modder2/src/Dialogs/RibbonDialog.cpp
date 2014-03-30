/*
 * RibbonDialog.cpp
 *
 *  Created on: 11.06.2009
 *      Author: Bastian
 */

#include <QtGui>
#include "RibbonDialog.h"

RibbonDialog::RibbonDialog(QWidget *parent)
	:QDialog(parent)
{
	QLabel *bonelabel=new QLabel(tr("Bone"));
	boneEdit=new QLineEdit;
	QLabel *poslabel=new QLabel(tr("Position"));
	QLabel *xlabel=new QLabel(tr("X:"));
	xEdit=new QLineEdit;
	QLabel *ylabel=new QLabel(tr("Y:"));
	yEdit=new QLineEdit;
	QLabel *zlabel=new QLabel(tr("Z:"));
	zEdit=new QLineEdit;
	QLabel *texlabel=new QLabel(tr("Texture-Number:"));
	texEdit=new QLineEdit;
	QLabel *renderlabel=new QLabel(tr("Renderflag-Number:"));
	renderEdit=new QLineEdit;
	QLabel *colorlabel=new QLabel(tr("Color"));
	QLabel *redlabel=new QLabel(tr("Red:"));
	rEdit=new QSlider(Qt::Horizontal);
	rEdit->setRange(0,100);
	rEdit->setSingleStep(1);
	rEdit->setPageStep(10);
	rEdit->setSliderPosition(100);
	rEdit->setTickInterval(10);
	rEdit->setTickPosition(QSlider::TicksRight);
	QLabel *greenlabel=new QLabel(tr("Green:"));
	gEdit=new QSlider(Qt::Horizontal);
	gEdit->setRange(0,100);
	gEdit->setSingleStep(1);
	gEdit->setPageStep(10);
	gEdit->setSliderPosition(100);
	gEdit->setTickInterval(10);
	gEdit->setTickPosition(QSlider::TicksRight);
	QLabel *bluelabel=new QLabel(tr("Blue:"));
	bEdit=new QSlider(Qt::Horizontal);
	bEdit->setRange(0,100);
	bEdit->setSingleStep(1);
	bEdit->setPageStep(10);
	bEdit->setSliderPosition(100);
	bEdit->setTickInterval(10);
	bEdit->setTickPosition(QSlider::TicksRight);
	QLabel *opacitylabel=new QLabel(tr("Opacity:"));
	opacitySlider=new QSlider(Qt::Horizontal);
	opacitySlider->setRange(0,0x7FFF);
	opacitySlider->setSingleStep(0x7F);
	opacitySlider->setPageStep(0x7F0);
	opacitySlider->setSliderPosition(0x7FFF);
	opacitySlider->setTickInterval(0x7F0);
	opacitySlider->setTickPosition(QSlider::TicksRight);
	QLabel *abovelabel=new QLabel(tr("Above:"));
	//sliders vs. floats <.< so we divide later by 10
	aboveSlider=new QSlider(Qt::Horizontal);
	aboveSlider->setRange(0,20);
	aboveSlider->setSingleStep(1);
	aboveSlider->setPageStep(5);
	aboveSlider->setSliderPosition(2);
	aboveSlider->setTickInterval(5);
	aboveSlider->setTickPosition(QSlider::TicksRight);
	QLabel *belowlabel=new QLabel(tr("Below:"));
	belowSlider=new QSlider(Qt::Horizontal);
	belowSlider->setRange(0,20);
	belowSlider->setSingleStep(1);
	belowSlider->setPageStep(5);
	belowSlider->setSliderPosition(2);
	belowSlider->setTickInterval(5);
	belowSlider->setTickPosition(QSlider::TicksRight);
	QLabel *reslabel=new QLabel(tr("Resolution:"));
	resSlider=new QSlider(Qt::Horizontal);
	resSlider->setRange(0,100);
	resSlider->setSingleStep(1);
	resSlider->setPageStep(10);
	resSlider->setSliderPosition(30);
	resSlider->setTickInterval(10);
	resSlider->setTickPosition(QSlider::TicksRight);
	QLabel *lenlabel=new QLabel(tr("Length:"));
	lenSlider=new QSlider(Qt::Horizontal);
	lenSlider->setRange(0,10);
	lenSlider->setSingleStep(1);
	lenSlider->setPageStep(5);
	lenSlider->setSliderPosition(2);
	lenSlider->setTickInterval(5);
	lenSlider->setTickPosition(QSlider::TicksRight);
	QLabel *emilabel=new QLabel(tr("Emissionangle:"));
	emiEdit=new QLineEdit;
	addButton=new QPushButton(tr("&Add"));
    connect(addButton, SIGNAL(clicked()), this, SLOT(addClicked()));
    connect(addButton, SIGNAL(clicked()), this, SLOT(accept()));



	QVBoxLayout *mainlayout = new QVBoxLayout;
	mainlayout->addWidget(bonelabel);
	mainlayout->addWidget(boneEdit);
	mainlayout->addWidget(poslabel);
	mainlayout->addWidget(xlabel);
	mainlayout->addWidget(xEdit);
	mainlayout->addWidget(ylabel);
	mainlayout->addWidget(yEdit);
	mainlayout->addWidget(zlabel);
	mainlayout->addWidget(zEdit);
	mainlayout->addStretch();

	QVBoxLayout *vallayout = new QVBoxLayout;
	vallayout->addWidget(texlabel,Qt::AlignTop);
	vallayout->addWidget(texEdit);
	vallayout->addWidget(renderlabel);
	vallayout->addWidget(renderEdit);
	vallayout->addWidget(colorlabel);
	vallayout->addWidget(redlabel);
	vallayout->addWidget(rEdit);
	vallayout->addWidget(greenlabel);
	vallayout->addWidget(gEdit);
	vallayout->addWidget(bluelabel);
	vallayout->addWidget(bEdit);
	vallayout->addWidget(opacitylabel);
	vallayout->addWidget(opacitySlider);
	vallayout->addWidget(abovelabel);
	vallayout->addWidget(aboveSlider);
	vallayout->addWidget(belowlabel);
	vallayout->addWidget(belowSlider);
	vallayout->addWidget(reslabel);
	vallayout->addWidget(resSlider);
	vallayout->addWidget(lenlabel);
	vallayout->addWidget(lenSlider);
	vallayout->addWidget(emilabel);
	vallayout->addWidget(emiEdit);
	vallayout->addStretch();

	QGridLayout *layout = new QGridLayout;
	layout->addLayout(mainlayout,0,0);
	layout->addLayout(vallayout,0,1);
	layout->addWidget(addButton,1,1);

	setLayout(layout);
	setWindowTitle(tr("Add Ribbon"));

}

void RibbonDialog::addClicked(){
	ribbon.id=-1;
	ribbon.bone=boneEdit->text().toInt();
	ribbon.pos[0]=xEdit->text().toFloat();
	ribbon.pos[1]=yEdit->text().toFloat();
	ribbon.pos[2]=zEdit->text().toFloat();
	ribbon.length=lenSlider->value();
	ribbon.res=resSlider->value();
	ribbon.s1=1;
	ribbon.s2=1;
	ribbon.unk=emiEdit->text().toFloat();
	ribbon.unknown=0;

	help.above=aboveSlider->value();
	help.below=belowSlider->value();
	help.blend=renderEdit->text().toInt();
	help.one=1;
	help.zero=0;
	help.opacity=opacitySlider->value();
	help.texture=texEdit->text().toInt();
	help.color[0]=rEdit->value();
	help.color[1]=gEdit->value();
	help.color[2]=bEdit->value();
	//well we need this, as direct division doesn't work oO
	help.above/=10;
	help.below/=10;
	help.color[0]/=100;
	help.color[1]/=100;
	help.color[2]/=100;
}

RibbonHelper RibbonDialog::getRibbonHelper(){
	return help;
}

ModelRibbonEmitterDef RibbonDialog::getRibbon(){
	return ribbon;
}
