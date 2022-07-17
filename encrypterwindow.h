//+------------------------------------------------------------------+
//+------------------------------------------------------------------+
#ifndef ENCRYPTERWINDOW_H
#define ENCRYPTERWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QAction;
class QListWidget;
class QMenu;
class QTextEdit;
class QTableWidget;
class QTableWidgetItem;
class QTableView;
class QHBoxLayout;

QT_END_NAMESPACE
namespace Encrypt
{
class Collection;
}
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
class EncrypterWindow : public QMainWindow
  {
   Q_OBJECT

public:
                     EncrypterWindow(QWidget *parent = nullptr);
                    ~EncrypterWindow();
   void              loadFile(const QString &fileName, QTextEdit *textEdit);
protected:
   void              closeEvent(QCloseEvent *event) override;

private slots:
   void              encrypt();
   void              decrypt();
   void              open();
   void              openCrypted();
   void              newFile();
   bool              save();
   bool              saveAs();
   bool              saveResAs();
   void              copy();
   void              showSettings(const QString &preset);
   void              changeSettings(QTableWidgetItem *item);
   void              about();

private:
   void              createActions();
   void              createStatusBar();
   void              createDockWindows();
   bool              maybeSave();
   bool              saveFile(const QString &fileName, QTextEdit* edit);
   bool              saveDialog(QTextEdit *inputText);
   void              readSettings();
   void              writeSettings();
   void              loadHistory();
   void              saveHistory();
   void              fromHistory();

   QMap<QString,QVariant>* currentParam();

   QTextEdit         *inputText;
   QTextEdit         *resultText;
   QListWidget       *presetList;
   QListWidget       *historyList;
   QTableWidget      *settingsTable;

   QMenu             *viewMenu;
   QHBoxLayout       *mainLayout;
   Encrypt::Collection *collection;
   QString           inputFile;
   QString           resultFile;
  };
#endif // ENCRYPTERWINDOW_H

//+------------------------------------------------------------------+
