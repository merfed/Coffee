/*
 * ResizeDialog.h
 *
 *  Created on: 09.06.2009
 *      Author: Bastian
 */

#ifndef RESIZEDIALOG_H_
#define RESIZEDIALOG_H_

 #include <QDialog>

 class QLineEdit;
 class QPushButton;

 class ResizeDialog : public QDialog
 {
     Q_OBJECT

 public:
     ResizeDialog(QWidget *parent = 0);
     QString getFactor();

 public slots:
     void resizeClicked();

 private:
     QPushButton *resizeButton;
     QLineEdit *lineEdit;
     QString factorText;

 };
#endif /* RESIZEDIALOG_H_ */
