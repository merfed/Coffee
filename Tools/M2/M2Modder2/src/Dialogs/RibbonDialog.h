/*
 * RibbonDialog.h
 *
 *  Created on: 11.06.2009
 *      Author: Bastian
 */

#ifndef RIBBONDIALOG_H_
#define RIBBONDIALOG_H_
#include <QDialog>
#include "../Tools/modelhelpstructs.h"
#include "../Tools/modelheaders.h"

 class QLineEdit;
 class QPushButton;
 class QSlider;
 class RibbonDialog : public QDialog
 {
     Q_OBJECT

 public:
	 RibbonDialog(QWidget *parent = 0);

 public slots:
     void addClicked();
     RibbonHelper getRibbonHelper();
     ModelRibbonEmitterDef getRibbon();

 private:
	 RibbonHelper help;
	 ModelRibbonEmitterDef ribbon;
     QPushButton *addButton;
     QLineEdit *texEdit,*renderEdit,*boneEdit,*xEdit,*yEdit,*zEdit,*emiEdit;
     QSlider *rEdit,*gEdit,*bEdit,*opacitySlider, *aboveSlider,*belowSlider,*resSlider,*lenSlider;


 };

#endif /* RIBBONDIALOG_H_ */
