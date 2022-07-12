#include <QtWidgets>
#include <QtGlobal>
#include "encrypterwindow.h"
#include "collection.h"
#define setCurrentFile(file) if(textEdit==inputText){inputFile=file;}else{resultFile=file;resultText->document()->setModified(false);setWindowModified(false);}
using namespace Encrypt;
EncrypterWindow::EncrypterWindow(QWidget *parent)
    : QMainWindow(parent)
    ,inputText(new QTextEdit),resultText(new QTextEdit)
{


//    QLayout *playout =layout();
   mainLayout = new QHBoxLayout;
   QWidget * wid= new QWidget(this);
    setCentralWidget(wid);
   wid->setLayout(mainLayout);
   mainLayout->setContentsMargins(2,8,2,2);
   mainLayout->setSpacing(5);
   mainLayout->addStrut(300);
   mainLayout->setGeometry(QRect(50,50,250,300));
     mainLayout->addWidget(inputText,1);
     mainLayout->addWidget(resultText,1);


//     playout->addItem(mainLayout);
    collection = Collection::getCollection();
    collection->init();
//    collection->
     createActions();
     createStatusBar();
     createDockWindows();
     readSettings();
     loadHistory();
}

EncrypterWindow::~EncrypterWindow()
{
}

void EncrypterWindow::encrypt()
{
    QString preset = presetList->currentItem()->text();
    Collection::Presets *set = collection->getPresets();
    if(preset.isEmpty() || !set || set->find(preset)==set->end())
        return;

    const QTextDocument *doc = inputText->document();
    QTextDocument *docres = resultText->document();
    docres->clear();
    QString src=(doc->toRawText());
    QString res;
    //res.reserve(doc->toRawText().size());
    set->operator[](preset)->operator()(src,res);
//    for(auto it:src )
//    {
//        if(it.isLetter())
//            res+=QChar(it.unicode()+3);
//        else res+=it;
//    }
   docres->setPlainText(res);
   saveHistory();
    //resultText->setDocument(docres);
}

void EncrypterWindow::decrypt()
{
    const QTextDocument *doc = resultText->document();
    QTextDocument *docres = inputText->document();
    docres->clear();
    QString res;
    res.reserve(doc->toRawText().size());
    for(auto it:doc->toRawText() )
    {
        if(it.isLetter())
            res+=QChar(it.unicode()-3);
        else res+=it;
    }
   docres->setPlainText(res);
    //inputText->setDocument(docres);
}

void EncrypterWindow::createActions()
{
        QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
        QToolBar *fileToolBar = addToolBar(tr("File"));

        const QIcon newIcon = QIcon();//::fromTheme("document-new", QIcon(":/images/new.png"));
        //const QIcon newIcon = QIcon::fromTheme("document-new", QIcon(":/images/new.png"));
        QAction *newAct = new QAction(newIcon, tr("&New"), this);
        newAct->setShortcuts(QKeySequence::New);
        newAct->setStatusTip(tr("Create a new encrypt"));
        connect(newAct, &QAction::triggered, this, &EncrypterWindow::newFile);
        fileMenu->addAction(newAct);
        fileToolBar->addAction(newAct);

    //! [19]
        const QIcon openIcon = QIcon::fromTheme("document-open", QIcon(":/images/open.png"));
        QAction *openAct = new QAction(openIcon, tr("&Open..."), this);
        openAct->setShortcuts(QKeySequence::Open);
        openAct->setStatusTip(tr("Open an existing file"));
        connect(openAct, &QAction::triggered, this, &EncrypterWindow::open);
        fileMenu->addAction(openAct);
        fileToolBar->addAction(openAct);
//--
        const QIcon openCryptIcon = QIcon::fromTheme("document-open-crypted", QIcon(":/images/open_crypted.png"));
        openAct = new QAction(openCryptIcon, tr("&Open crypted..."), this);
                openAct->setShortcuts(QKeySequence::Open);
                openAct->setStatusTip(tr("Open an crypted file"));
                connect(openAct, &QAction::triggered, this, &EncrypterWindow::openCrypted);
                fileMenu->addAction(openAct);
                fileToolBar->addAction(openAct);
    //! [18] //! [19]

        const QIcon saveIcon = QIcon();//::fromTheme("document-save", QIcon(":/images/save.png"));
        QAction *saveAct = new QAction(saveIcon, tr("&Save Result"), this);
        saveAct->setShortcuts(QKeySequence::Save);
        saveAct->setStatusTip(tr("Save the encrypted result to disk"));
        connect(saveAct, &QAction::triggered, this, &EncrypterWindow::save);
        fileMenu->addAction(saveAct);
        fileToolBar->addAction(saveAct);

        const QIcon saveResAsIcon = QIcon::fromTheme("document-save-res-as");
        QAction *saveResAsAct = fileMenu->addAction(saveResAsIcon, tr("Save &Result As..."), this, &EncrypterWindow::saveResAs);
        //connect(saveResAsAct, &QAction::triggered, this, &EncrypterWindow::saveResAs);
        //saveResAsAct->setShortcuts(QKeySequence::SaveAs);
        saveResAsAct->setStatusTip(tr("Save the encrypted result under a new name"));

        const QIcon saveAsIcon = QIcon::fromTheme("document-save-as");
        QAction *saveAsAct = fileMenu->addAction(saveAsIcon, tr("Save Original &As..."), this, &EncrypterWindow::saveAs);
        //connect(saveAsAct, &QAction::triggered, this, &EncrypterWindow::saveAs);
        //saveAsAct->setShortcuts(QKeySequence::SaveAs);
        saveAsAct->setStatusTip(tr("Save the document under a new name"));

        const QIcon printIcon = QIcon();//::fromTheme("document-print", QIcon(":/images/print.png"));
        QAction *printAct = new QAction(printIcon, tr("&Print..."), this);
        printAct->setShortcuts(QKeySequence::Print);
        printAct->setStatusTip(tr("Print the current form letter"));
        connect(printAct, &QAction::triggered, this, &EncrypterWindow::print);
        fileMenu->addAction(printAct);
        fileToolBar->addAction(printAct);

        fileMenu->addSeparator();

        QAction *quitAct = fileMenu->addAction(tr("&Quit"), this, &QWidget::close);
        quitAct->setShortcuts(QKeySequence::Quit);
        quitAct->setStatusTip(tr("Quit the application"));

        QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));
        QToolBar *editToolBar = addToolBar(tr("Edit"));

        const QIcon undoIcon = QIcon();//::fromTheme("edit-undo", QIcon(":/images/undo.png"));
        QAction *undoAct = new QAction(undoIcon, tr("&Undo"), this);
        undoAct->setShortcuts(QKeySequence::Undo);
        undoAct->setStatusTip(tr("Undo the last editing action"));
        connect(undoAct, &QAction::triggered, this, &EncrypterWindow::undo);
        editMenu->addAction(undoAct);
        editToolBar->addAction(undoAct);

        const QIcon encryptIcon = QIcon();//::fromTheme("edit-encrypt", QIcon(":/images/encrypt.png"));
        QAction *encryptAct = new QAction(encryptIcon, tr("&Encrypt"), this);
//        encryptAct->setShortcuts(QKeySequence::Undo);
        encryptAct->setStatusTip(tr("Encrypt text"));
        connect(encryptAct, &QAction::triggered, this, &EncrypterWindow::encrypt);
        editMenu->addAction(encryptAct);
        editToolBar->addAction(encryptAct);

        const QIcon decryptIcon = QIcon::fromTheme("edit-decrypt");//, QIcon(":/images/decrypt.png"));
        QAction *decryptAct = new QAction(decryptIcon, tr("&Decrypt"), this);
//        decryptAct->setShortcuts(QKeySequence::Undo);
        decryptAct->setStatusTip(tr("Encrypt text"));
        connect(decryptAct, &QAction::triggered, this, &EncrypterWindow::decrypt);
        editMenu->addAction(decryptAct);
        editToolBar->addAction(decryptAct);
        editMenu->addSeparator();

        const QIcon copyIcon = QIcon::fromTheme("edit-copy-to");//, QIcon(":/images/copyto.png"));
        QAction *copyAct = new QAction(copyIcon, tr("&Copy to clipboard"), this);
//        decryptAct->setShortcuts(QKeySequence::Undo);
        copyAct->setStatusTip(tr("Copy encrypted result to clipboard"));
        //connect(decryptAct, &QAction::triggered, this, &EncrypterWindow::copyTo);
        editMenu->addAction(copyAct);
        editToolBar->addAction(copyAct);

        const QIcon historyIcon = QIcon::fromTheme("edit-from-history");//, QIcon(":/images/copyto.png"));
        QAction *historyAct = new QAction(historyIcon, tr("From &History"), this);
//        decryptAct->setShortcuts(QKeySequence::Undo);
        historyAct->setStatusTip(tr("Load text data from history list"));
        connect(historyAct, &QAction::triggered, this, &EncrypterWindow::fromHistory);
        editMenu->addAction(historyAct);
        editToolBar->addAction(historyAct);

        viewMenu = menuBar()->addMenu(tr("&View"));

        menuBar()->addSeparator();

        QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));

        QAction *aboutAct = helpMenu->addAction(tr("&About"), this, &EncrypterWindow::about);
        aboutAct->setStatusTip(tr("Show the application's About box"));

        QAction *aboutQtAct = helpMenu->addAction(tr("About &Qt"), qApp, &QApplication::aboutQt);
        aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
}
void EncrypterWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}
void EncrypterWindow::createDockWindows()
{
    QDockWidget *dock = new QDockWidget(tr("preset list"), this); //resultText
        dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
        presetList = new QListWidget(dock);
        for(auto &item:*(collection->getPresets()))
            presetList->addItem(item.first);
        presetList->setCurrentRow(1);
        dock->setWidget(presetList);
        addDockWidget(Qt::LeftDockWidgetArea, dock);
        viewMenu->addAction(dock->toggleViewAction());

        dock = new QDockWidget(tr("history of encrypt/decrypt"), this);
        historyList = new QListWidget(dock);

        dock->setWidget(historyList);
        addDockWidget(Qt::RightDockWidgetArea, dock);
        viewMenu->addAction(dock->toggleViewAction());

        dock = new QDockWidget(tr("Cipher setings"), this);
        settingsTable = new QTableWidget(dock);

        dock->setWidget(settingsTable);
        addDockWidget(Qt::LeftDockWidgetArea, dock);
        viewMenu->addAction(dock->toggleViewAction());

//        dock = new QDockWidget(tr("result text"), this);
//        resultText = new QTextEdit(dock);
//        dock->setWidget(resultText);
//        addDockWidget(Qt::RightDockWidgetArea, dock);
//        viewMenu->addAction(dock->toggleViewAction());

        connect(presetList, &QListWidget::currentTextChanged,
                this, &EncrypterWindow::showSettings);
        connect(historyList, &QListWidget::currentTextChanged,
                this, &EncrypterWindow::fromHistory);

}
void EncrypterWindow::readSettings()
//! [34] //! [36]
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    const QByteArray geometry = settings.value("geometry", QByteArray()).toByteArray();
    if (geometry.isEmpty()) {
        const QRect availableGeometry = screen()->availableGeometry();
        resize(availableGeometry.width() / 3, availableGeometry.height() / 2);
        move((availableGeometry.width() - width()) / 2,
             (availableGeometry.height() - height()) / 2);
    } else {
        restoreGeometry(geometry);
    }
  QByteArray doc = settings.value("inputText",  QByteArray()).toByteArray();
  if(!doc.isEmpty())
  {
      inputText->document()->setPlainText(QString(doc));
              //setDocument(doc.to)
  }
  doc = settings.value("resultText",  QByteArray()).toByteArray();
  if(!doc.isEmpty())
  {
      resultText->document()->setPlainText(QString(doc));
              //setDocument(doc.to)
  }
 QString file = settings.value("inputFile",  QString()).toString();
 if(!file.isEmpty())
     inputFile=file;
 file = settings.value("resultFile",  QString()).toString();
 if(!file.isEmpty())
     resultFile=file;
}

//! [35] //! [36]

//! [37] //! [38]
void EncrypterWindow::writeSettings()
//! [37] //! [39]
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    settings.setValue("geometry", saveGeometry());
    settings.setValue("inputText",inputText->document()->toPlainText());
    settings.setValue("resultText",resultText->document()->toPlainText());
    settings.setValue("inputFile",inputFile);
    settings.setValue("resultFile",resultFile);
}
#define SETFORMAT QSettings::IniFormat
void EncrypterWindow::loadHistory()
{
    QSettings settings(SETFORMAT, QSettings::UserScope,QCoreApplication::organizationName(), QCoreApplication::applicationName());
    settings.beginGroup("history");
    QStringList dates = settings.childGroups();
    historyList->addItems(dates);
    for(auto date:dates)
    {
        ;
    }
    //settings.endGroup();
settings.endGroup();
}
void EncrypterWindow::saveHistory()
{
    QSettings settings(SETFORMAT, QSettings::UserScope,QCoreApplication::organizationName(), QCoreApplication::applicationName());
    QDateTime dt(QDateTime::currentDateTime());
    QString date=dt.toString(Qt::DateFormat::ISODateWithMs );
    settings.beginGroup("history");
        settings.beginGroup(date);
        settings.setValue("input",inputText->document()->toRawText());
        settings.setValue("result",resultText->document()->toRawText());
        settings.endGroup();
    settings.endGroup();
    historyList->addItem(date);
}
//--
void EncrypterWindow::fromHistory()
{
    QListWidgetItem * item =historyList->currentItem();
    QString text=item->text();
    if(text.isEmpty())
            return;
    QSettings settings(SETFORMAT, QSettings::UserScope,QCoreApplication::organizationName(), QCoreApplication::applicationName());
    settings.beginGroup("history");
    settings.beginGroup(text);
    inputText->document()->setPlainText( settings.value("input").toByteArray());
    resultText->document()->setPlainText(settings.value("result").toByteArray());
    settings.endGroup();
    settings.endGroup();
}
//--
void EncrypterWindow::closeEvent(QCloseEvent *event)
//! [3] //! [4]
{
    //if (maybeSave()) {
        writeSettings();
        event->accept();
//    } else {
//        event->ignore();
//    }
}
//
void EncrypterWindow::newFile()
//! [5] //! [6]
{
    if (maybeSave()) {
        resultText->clear();
        resultFile=QString();
    }
}
//! [6]

//! [7]
void EncrypterWindow::open()
//! [7] //! [8]
{
    if (maybeSave()) {
        QString fileName = QFileDialog::getOpenFileName(this);
        if (!fileName.isEmpty())
            loadFile(fileName,inputText);
    }
}
void EncrypterWindow::openCrypted()
//! [7] //! [8]
{
    if (maybeSave()) {
        QString fileName = QFileDialog::getOpenFileName(this);
        if (!fileName.isEmpty())
            loadFile(fileName,resultText);
    }
}
//! [8]
void EncrypterWindow::loadFile(const QString &fileName, QTextEdit *textEdit)
//! [42] //! [43]
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(QDir::toNativeSeparators(fileName), file.errorString()));
        return;
    }

    QTextStream in(&file);
#ifndef QT_NO_CURSOR
    QGuiApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    textEdit->setPlainText(in.readAll());
#ifndef QT_NO_CURSOR
    QGuiApplication::restoreOverrideCursor();
#endif

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File loaded"), 2000);
}
//! [9]
bool EncrypterWindow::save()
//! [9] //! [10]
{
    if (resultFile.isEmpty()) {
        return saveResAs();
    } else {
        return saveFile(resultFile,resultText);
    }
}
//--
bool EncrypterWindow:: saveDialog(QTextEdit *inputText)
{
   QFileDialog dialog(this);
   dialog.setWindowModality(Qt::WindowModal);
   dialog.setAcceptMode(QFileDialog::AcceptSave);
   if (dialog.exec() != QDialog::Accepted)
       return false;
   return saveFile(dialog.selectedFiles().first(),inputText);
};
bool EncrypterWindow::saveAs()
{
    return saveDialog(inputText);
}
bool EncrypterWindow::saveResAs()
{
    return saveDialog(resultText);
}
//-----
bool EncrypterWindow::maybeSave()
//! [40] //! [41]
{
    if (!resultText->document()->isModified())
        return true;
    const QMessageBox::StandardButton ret
        = QMessageBox::warning(this, tr("Encrypted"),
                               tr("The  document has been encrypted.\n"
                                  "Do you want to save your result data?"),
                               QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    switch (ret) {
    case QMessageBox::Save:
        return save();
    case QMessageBox::Cancel:
        return false;
    default:
        break;
    }
    return true;
}
//--
bool EncrypterWindow::saveFile(const QString &fileName, QTextEdit* textEdit)
//! [44] //! [45]
{
    QString errorMessage;

    QGuiApplication::setOverrideCursor(Qt::WaitCursor);
    QSaveFile file(fileName);
    if (file.open(QFile::WriteOnly | QFile::Text)) {
        QTextStream out(&file);
        out << textEdit->toPlainText();
        if (!file.commit()) {
            errorMessage = tr("Cannot write file %1:\n%2.")
                           .arg(QDir::toNativeSeparators(fileName), file.errorString());
        }
    } else {
        errorMessage = tr("Cannot open file %1 for writing:\n%2.")
                       .arg(QDir::toNativeSeparators(fileName), file.errorString());
    }
    QGuiApplication::restoreOverrideCursor();

    if (!errorMessage.isEmpty()) {
        QMessageBox::warning(this, tr("Application"), errorMessage);
        return false;
    }

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File saved"), 2000);
    return true;
}
//-----
void EncrypterWindow::showSettings(const QString &preset)
{
    Collection::Presets *set = collection->getPresets();
    ParamMap params=set->operator[](preset)->getParams();
    settingsTable->clear();
    //settingsTable->setModel();
}
