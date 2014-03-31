/*
 * ResizeDialog.h
 *
 *  Created on: 09.06.2009
 *      Author: Bastian
 */

#ifndef TRANSPARENCYDIALOG_H_
#define TRANSPARENCYDIALOG_H_

 #include <QDialog>
#include "../Tools/modelheaders.h"
#include "../Tools/modeleditor.h"

 class QPushButton;
 class QSlider;
 class TransparencyDialog : public QDialog
 {
     Q_OBJECT

 public:
	 TransparencyDialog(QWidget *parent = 0,ModelEditor *editor = NULL);

 public slots:
     void addClicked();

 private:
	 QSlider **sliders;
	 QPushButton *addButton;

 };
#endif /* TRANSPARENCYDIALOG_H_ */
