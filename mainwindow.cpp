#include <QDebug>
#include <QTime>
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "netreceiver.h"


pthread_mutex_t mutex_here = PTHREAD_MUTEX_INITIALIZER;

int abc = 5;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);        
    
    QTableWidgetItem *item_t;
    for (int i = 0; i < ui->tableWidget->rowCount(); i++) {
        for (int j = 0; j < ui->tableWidget->columnCount(); j++) {
            item_t = new QTableWidgetItem (i);
            item_t->setBackgroundColor(Qt::gray);
            ui->tableWidget->setItem(i,j, item_t);
        }
    }
    for (int i = 0; i < ui->tableWidget_2->rowCount(); i++) {
        for (int j = 0; j < ui->tableWidget_2->columnCount(); j++) {
            item_t = new QTableWidgetItem (i);
            //item_t->setBackgroundColor(Qt::gray);
            ui->tableWidget_2->setItem(i,j, item_t);
        }
    }
    NetReceiver *n_r = new NetReceiver();
    n_r->moveToThread(&net_rec_thr);
    connect (n_r, &NetReceiver::SendStringToMain, this, &MainWindow::RecChannelName);
    
    connect (&net_rec_thr, &QThread::started, n_r, &NetReceiver::ReceivePacket);        
    net_rec_thr.start();
    
    connect (ui->tableWidget, &QTableWidget::itemClicked, this, &MainWindow::TestSlot);
}
void MainWindow::RecChannelName (unsigned char *_packet_ts, int _size) 
{

  PacketTS *packet_ts =  (PacketTS*)(_packet_ts + 42);
  QString q_str =  packet_ts->name;  
  QTime q_time = QTime::currentTime();
          
  //qDebug () << _q_str << q_time.toString("hh:mm:ss.zzzzzz") << "main win";
  
  static int cell_num = 0;
  if (chann_to_num.find (q_str.toStdString()) == chann_to_num.end())
       chann_to_num[q_str.toStdString()] = cell_num++;
  
 /*
  for (int i = 0; i < ui->tableWidget->rowCount()-1; i++) {
        QTableWidgetItem *item1 = ui->tableWidget->item(i, 0);
        QTableWidgetItem *item2 = ui->tableWidget->item(i+1, 0);                        
        item1->setText(item2->text());
  }
 */
 
  if (chann_to_num[q_str.toStdString()] < ui->tableWidget->rowCount()) {
      QTableWidgetItem *item = ui->tableWidget->item(chann_to_num[q_str.toStdString()], 0);
      //item->setData(Qt::EditRole, _q_str);
      item->setText(q_str);
      //item = ui->tableWidget->item(msg_counter%ui->tableWidget->rowCount(), 1);
      //item->setData(Qt::EditRole, msg_counter+1);  
  }
  else {
      qDebug () << "table overload";
      ui->tableWidget->setRowCount(ui->tableWidget->rowCount() + 1);          
      //ui->tableWidget->insertRow(ui->tableWidget->rowCount());          
      QTableWidgetItem *item_t = new QTableWidgetItem (7);
      item_t->setBackgroundColor(Qt::gray);
      item_t->setText(q_str);
      ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,0, item_t);
      
      item_t = new QTableWidgetItem (7);
      item_t->setBackgroundColor(Qt::gray);
      ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,1, item_t);
  }
  
  
   
  //int num  = item->text().toInt();
  //msg_counter++;  
  pthread_mutex_unlock(&mutex_here);        
  if (looking_channel == q_str)  {
       qDebug () << "Channel name is here:"  << q_str << "size: " << QString("%1").arg((ushort)*(packet_ts->name +strlen(packet_ts->name) + 1),0,10) ;
       FillTzkTable(_packet_ts, _size);
       
  }
  ui->tableWidget->viewport()->update();
  
}

void MainWindow::TestSlot(QTableWidgetItem *item) 
{
    qDebug () << "Item clicked" << " " << item->text();
    looking_channel = item->text();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::FillTzkTable( unsigned char *_buff, int _size)
{
    PacketTS *packet_ts = (PacketTS*)(_buff + 42);
    ushort * group = (ushort *) (packet_ts->name + (strlen(packet_ts->name)) + 1 + 2 + 2);
    for (int i = 0; i < ui->tableWidget_2->rowCount(); i++) {
        ushort mask = 0x0001;
        for (int j = 0; j < ui->tableWidget_2->columnCount(); j++) {  
             mask=mask<<j;
             if (mask && *group)
                  ui->tableWidget_2->item (i, j)->setText ("1");
             else
                  ui->tableWidget_2->item (i, j)->setText ("0");
        }
        group++;
    }
    ui->tableWidget_2->viewport()->update();
}


