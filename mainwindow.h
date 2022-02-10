#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <queue>
#include <string>
#include <qtablewidget.h>
#include <thread>
#include "./lan.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

using namespace std;

class MainWindow : public QMainWindow
{
        Q_OBJECT
    public slots:
       void RecChannelName (unsigned char *, int);
       void TestSlot (QTableWidgetItem *item);       
        
    public:
        MainWindow(QWidget *parent = nullptr);
        ~MainWindow();
    private:        
        void FillTzkTable (unsigned char*, int);                
    private:
        Ui::MainWindow *ui;        
        QThread net_rec_thr;        
        map <string, int> chann_to_num;
        QString looking_channel;
};
#endif // MAINWINDOW_H
