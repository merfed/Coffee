#ifndef M2MODDER2_H
#define M2MODDER2_H

#include <QtGui>
#include <stdio.h>
#include "ui_m2modder2.h"
#include "Tools/modeleditor.h"
#include "Dialogs/ResizeDialog.h"
#include "Dialogs/TextureDialog.h"
#include "Dialogs/RibbonDialog.h"
//#include "Dialogs/TransparencyDialog.h"
class M2Modder2 : public QMainWindow
{
    Q_OBJECT

public:
    M2Modder2(QWidget *parent = 0);
    ~M2Modder2();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void open();
    bool save();
    void resizeModel();
    void addTexture();
    void addRibbon();
    //void setTrans();



private:
    Ui::M2Modder2Class ui;
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    //void createLaterDialogs();
    void readSettings();
    void writeSettings();
    bool maybeSave();
    void loadFile(const QString &fileName);
    bool saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);
    QString curFile;

    QMenu *fileMenu;
    QToolBar *fileToolBar;
    QMenu *editMenu;
    QAction *openAct;
    QAction *saveAct;
    QAction *exitAct;
    QAction *resizeAct;
    QAction *textureAct;
    QAction *ribbonAct;
    //QAction *transparencyAct;

    ResizeDialog *rDialog;
    TextureDialog *tDialog;
    RibbonDialog *riDialog;
    //TransparencyDialog *trDialog;

    ModelEditor *editor;


    char *mfile;
    qint64 FileSize;

};

#endif // M2MODDER2_H
