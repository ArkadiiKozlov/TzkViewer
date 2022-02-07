#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <queue>
#include <string>
#include <qtablewidget.h>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

using namespace std;

class MainWindow : public QMainWindow
{
        Q_OBJECT
    public slots:
       void RecChannelName (QString);
       void TestSlot (QTableWidgetItem *item);       
        
    public:
        MainWindow(QWidget *parent = nullptr);
        ~MainWindow();
    private:
        void ReceivePacket(void);      
    private:
        Ui::MainWindow *ui;        
        QThread net_rec_thr;
        pthread_mutex_t mutex;        
        map <string, int> chann_to_num; 
};
#endif // MAINWINDOW_H
