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
namespace Encrypt{
//class ParamMap;
class Collection;
}

class EncrypterWindow : public QMainWindow
{
    Q_OBJECT

public:
    EncrypterWindow(QWidget *parent = nullptr);
    ~EncrypterWindow();
    void loadFile(const QString &fileName, QTextEdit *textEdit);
protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void encrypt();
    void decrypt();
    void open();
    void openCrypted();
    void newFile();
    bool save();
    //bool saveRes();
    bool saveAs();
    bool saveResAs();
    void saveToLog(){};
    void print(){};
    void undo(){};
    void about(){};
    void settings(){};
    void insertCustomer(const QString &customer){};
    void addParagraph(const QString &paragraph){};
    void clearHistory();
    void showSettings(const QString &preset);
    void changeSettings(QTableWidgetItem *item);

private:
    void createActions();
    void createStatusBar();
    void createDockWindows();
    bool maybeSave();
    bool saveFile(const QString &fileName, QTextEdit* edit);
    bool saveDialog(QTextEdit *inputText);
    //void setCurrentFile(const QString &fileName){resultFile=fileName;};
    void readSettings();
    void writeSettings();
    void loadHistory();
    void saveHistory();
    void fromHistory();
   // Encrypt::ParamMap* currentParam();
    QMap<QString,QVariant>*currentParam();

    QTextEdit *inputText;
    QTextEdit *resultText;
    QListWidget *presetList;
    QListWidget *historyList;
    QTableWidget *settingsTable;

    QMenu *viewMenu;
    QHBoxLayout *mainLayout;
    Encrypt::Collection *collection;
    QString inputFile;
    QString resultFile;
};
#endif // ENCRYPTERWINDOW_H

