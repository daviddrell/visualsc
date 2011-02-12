#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "smproject.h"
#include <QString>
#include "sceditcontroller.h"

class QSettings;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    SMProject *project;
    QSettings *settings;
    static QString _keyLastFilePath;

private slots:
    void handleFileOpenClick();
    void handleFileSaveClick();
    void handleNewClick();
};

#endif // MAINWINDOW_H
