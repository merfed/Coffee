/*
 * ResizeDialog.h
 *
 *  Created on: 09.06.2009
 *      Author: Bastian
 */

#ifndef TEXTUREDIALOG_H_
#define TEXTUREDIALOG_H_

 #include <QDialog>
#include "../Tools/modelheaders.h"

 class QLineEdit;
 class QPushButton;
 class QComboBox;
 class TextureDialog : public QDialog
 {
     Q_OBJECT

 public:
     TextureDialog(QWidget *parent = 0);
     QString getTexture();
     ModelTextureDef getTexDef();

 public slots:
     void addClicked();

 private:
     QPushButton *addButton;
     QComboBox *type;
     QComboBox *flag;
     QLineEdit *lineEdit;
     QString Texture;
     ModelTextureDef modeltex;

 };
#endif /* TEXTUREDIALOG_H_ */
