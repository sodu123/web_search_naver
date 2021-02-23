#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateEdit>
#include <QPixmap>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QUrl>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonArray>
#include <QMessageBox>
#include <QJsonObject>




void MainWindow::set_calendar()
{
    ui->dateEdit->setCalendarPopup(true);
    ui->dateEdit_2->setCalendarPopup(true);
    ui->dateEdit_2->setDate(QDate::currentDate());
    ui->dateEdit->setDate(QDate::currentDate());
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    set_calendar();
    QPixmap pix(":/20201108_190700.jpg");
    int w = ui->label_pic->width();
    int h = ui->label_pic->height();
    ui->label_pic->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);


}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::handleFinished(QNetworkReply *reply){
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject jsonobj = document.object();
    QJsonArray arr = jsonobj.value("items").toArray();
    if(arr.isEmpty())
    {

    QMessageBox::information(this, "info", "0 news searched");
    }
    else
    {
    for (int i = 0 ; i<arr.size();++i)
        {
           QJsonObject json = arr[i].toObject();
           QString d = json.value("pubDate").toString();
           QString t = json.value("title").toString().toLocal8Bit();
           QString l = json.value("link").toString();
           ui->tableWidget->insertRow(ui->tableWidget->rowCount());
           ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,0,new QTableWidgetItem(d)); //when passed in a constructor, does it delete automatically?
           ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,1,new QTableWidgetItem(t));
           ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,2,new QTableWidgetItem(l));
        }
    }


    reply->deleteLater();
}

void MainWindow::on_pushButton_clicked()
{


   QNetworkAccessManager *mana = new QNetworkAccessManager(this);

    QString url("https://openapi.naver.com/v1/search/news.json?query=");


    QString keyword = ui->lineEdit->text();

    url += keyword;


    QNetworkRequest req;
    req.setUrl (QUrl(url));
    req.setRawHeader("X-Naver-Client-Id", "Ow2ShRpR717a486G4zdh");
    req.setRawHeader("X-Naver-Client-Secret", "RNoG_2Qr2r");
    QNetworkReply *reply = mana->get(req);
    connect(mana, &QNetworkAccessManager::finished, this, &MainWindow::handleFinished); // async signal and slot!! study


    //form url?query=text&parameter=blah
}



