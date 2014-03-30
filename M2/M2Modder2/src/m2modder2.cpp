#include "m2modder2.h"

M2Modder2::M2Modder2(QWidget *parent)
    : QMainWindow(parent)
{
	ui.setupUi(this);


	rDialog=new ResizeDialog;
	tDialog=new TextureDialog;
	riDialog=new RibbonDialog;
    createActions();
    createMenus();
    createToolBars();
    createStatusBar();

    readSettings();

    setCurrentFile("");
    setUnifiedTitleAndToolBarOnMac(true);



}

M2Modder2::~M2Modder2()
{

}


void M2Modder2::closeEvent(QCloseEvent *event)
{
    if (maybeSave()) {
        writeSettings();
        event->accept();
    } else {
        event->ignore();
    }
}

void M2Modder2::open()
{
    if (maybeSave()) {
        QString fileName = QFileDialog::getOpenFileName(NULL,tr("Speichern unter"),
			    QDir::homePath(),"*.m2",NULL);
        if (!fileName.isEmpty())
            loadFile(fileName);
    }
}

bool M2Modder2::save()
{

    return saveFile(curFile);

}

void M2Modder2::resizeModel(){
	if(!curFile.isEmpty()){
	rDialog->show();
	if(rDialog->exec() == QDialog::Accepted){
		QString temp=rDialog->getFactor();
		editor->ResizeModel(temp.toFloat());
	}
	}
}

void M2Modder2::addTexture(){
	if(!curFile.isEmpty()){
	tDialog->show();
	if(tDialog->exec() == QDialog::Accepted){
		editor->addTexture(tDialog->getTexDef(),(const char *)tDialog->getTexture().toAscii());
	}
	}
}

void M2Modder2::addRibbon(){
	if(!curFile.isEmpty()){
	riDialog->show();
	if(riDialog->exec() == QDialog::Accepted){
		editor->addRibbon(riDialog->getRibbon(),riDialog->getRibbonHelper());
	}
	}
}

/*void M2Modder2::setTrans(){
	if(!curFile.isEmpty()){
	trDialog->show();
	if(trDialog->exec() == QDialog::Accepted){
	}
	}
}*/

void M2Modder2::createActions()
 {

     openAct = new QAction(tr("&Open..."), this);
     openAct->setShortcuts(QKeySequence::Open);
     openAct->setStatusTip(tr("Open an existing file"));
     connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

     saveAct = new QAction(tr("&Save..."), this);
     saveAct->setShortcuts(QKeySequence::Save);
     saveAct->setStatusTip(tr("Save the File"));
     connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

     exitAct = new QAction(tr("&Exit..."), this);
     exitAct->setShortcuts(QKeySequence::Close);
     exitAct->setStatusTip(tr("Exit Programm"));
     connect(exitAct, SIGNAL(triggered()), this, SLOT(closeEvent()));

     resizeAct = new QAction(tr("&Resize Model"),this);
     connect(resizeAct, SIGNAL(triggered()), this, SLOT(resizeModel()));

     textureAct = new QAction(tr("&Add Texture"),this);
     connect(textureAct, SIGNAL(triggered()), this, SLOT(addTexture()));

     ribbonAct = new QAction(tr("&Add Ribbon"),this);
     connect(ribbonAct, SIGNAL(triggered()), this, SLOT(addRibbon()));

 }

/*void M2Modder2::createLaterDialogs(){
	trDialog=new TransparencyDialog(0,editor);
    transparencyAct = new QAction(tr("&Set Transparency"),this);
    connect(transparencyAct, SIGNAL(triggered()), this, SLOT(setTrans()));

    editMenu->addAction(transparencyAct);
}*/

void M2Modder2::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    editMenu = menuBar()->addMenu((tr("&Tools")));
    editMenu->addAction(resizeAct);
    editMenu->addAction(textureAct);
    editMenu->addAction(ribbonAct);



}

void M2Modder2::createToolBars()
{

}

void M2Modder2::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}



void M2Modder2::readSettings()
{
    QSettings settings("Modcraft", "M2Modder");
    QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("size", QSize(400, 400)).toSize();
    resize(size);
    move(pos);
}

void M2Modder2::writeSettings()
{
    QSettings settings("Modcraft", "M2Modder");
    settings.setValue("pos", pos());
    settings.setValue("size", size());
}

bool M2Modder2::maybeSave()
{
    /*if (textEdit->document()->isModified()) {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("Application"),
                     tr("The document has been modified.\n"
                        "Do you want to save your changes?"),
                     QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (ret == QMessageBox::Save)
            return save();
        else if (ret == QMessageBox::Cancel)
            return false;
    }*/
    return true;
}

void M2Modder2::loadFile(const QString &fileName)
{

	QFile file(fileName);
    if (!file.open(QFile::ReadOnly)) {
        QMessageBox::warning(this, tr("M2Modder"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }

	FileSize=file.size();
	mfile=new char[FileSize];
    QApplication::setOverrideCursor(Qt::WaitCursor);
	QDataStream in(&file);
    QApplication::restoreOverrideCursor();
	in.setByteOrder(in.LittleEndian);
	in.readRawData(mfile,FileSize);



    editor=new ModelEditor(mfile,FileSize);
    //createLaterDialogs();
    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File loaded"), 2000);
}

bool M2Modder2::saveFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly)) {
        QMessageBox::warning(this, tr("M2Modder"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }

    QDataStream out(&file);
    out.setByteOrder(out.LittleEndian);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    out.writeRawData(editor->getFile(),editor->GetFileSize());
    QApplication::restoreOverrideCursor();

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File saved"), 2000);
    return true;
}

void M2Modder2::setCurrentFile(const QString &fileName)
{
    curFile = fileName;
    setWindowModified(false);

    QString shownName;
    if (curFile.isEmpty())
        shownName = "No File loaded";
    else
        shownName = strippedName(curFile);

    setWindowTitle(tr("%1[*] - %2").arg(shownName).arg(tr("M2Modder")));
}

QString M2Modder2::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}
