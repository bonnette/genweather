/*  This program interigates the openweathermap.org website to get JSON info from it.
 *  The response looks something like this:
 *  {
 * "lat":30.22,
 * "lon":-95.36,
 * "timezone":"America/Chicago",
 * "timezone_offset":-18000,
 * "current":{"dt":1602692411,"sunrise":1602678180,"sunset":1602719476,"temp":77.56,"feels_like":82.8,"pressure":1017,"humidity":71,"dew_point":67.41,
 *            "uvi":6.84,"clouds":0,"visibility":10000,"wind_speed":1.99,"wind_deg":185,"wind_gust":4,"weather":[{"id":800,"main":"Clear",
 *            "description":"clear sky","icon":"01d"}]
 * }
 * }
 *
 * This version looks a little different because it interigates the website a little differently
 * in order to get wind speed , wind gust and more.
 * From this response I extract weather data and display it in a window on a raspberry pi
 * Written by Larry Bonnette October 2020
 */

#include "math.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

// Raspberry Pi version of the get weather app for PC

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_downloadButton_clicked()
{
    QEventLoop eventLoop;
    QNetworkAccessManager mgr;
    QObject::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

    // the HTTP request to openweathermap.com
    QNetworkRequest req( QUrl( QString("http://api.openweathermap.org/data/2.5/onecall?lat=30.22&lon=-95.36&exclude=hourly,minutely,alerts,daily&units=imperial&appid=c3f3c20cf253315f8646755291334ab0") ) );
    QNetworkReply *reply = mgr.get(req);
    eventLoop.exec(); // blocks stack until "finished()" has been called

    if (reply->error() == QNetworkReply::NoError) {

        QString strReply = (QString)reply->readAll(); // strReply holds the full data stream from openweathermap.org

        //qDebug() << strReply;

        ui->incomingtxtlbl->setText(strReply); // Places all of the weather (JSON) text into a large label.

        QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8());  //Convert JSON from website to JSON Document

        QJsonObject root_obj = jsonResponse.object(); // Convert JSON Doc to Object
        QVariantMap root_map = root_obj.toVariantMap(); // Create a map
        QVariantMap current_map = root_map["current"].toMap(); // key with temperature data in it is called "current" convert it to a map

/*        qDebug() << "Current Temperature " << current_map["temp"].toString() << endl; // extract temperature from "current" map and display it
 *        qDebug() << "Feels Like Temperature " << current_map["feels_like"].toString() << endl;
 *        qDebug() << "Humidity " << current_map["humidity"].toString() << endl;
 *        qDebug() << "Pressure in milibars " << current_map["pressure"].toString() << endl;
 *        qDebug() << "Wind Speed in MPH " << current_map["wind_speed"].toString() << endl;
 */

        ui->idoortemplbl->setText(current_map["temp"].toString()); // Extract Temperature and place in label

        ui->humidlbl->setText(current_map["humidity"].toString()); // Extrac humidity and place in label

        ui->itemplbl->setText(current_map["feels_like"].toString()); // Write Minimum Temperature to label

        ui->presslbl->setText(current_map["pressure"].toString()); // Write Pressure in milibars to label

        ui->speedlbl->setText(current_map["wind_speed"].toString());

        ui->gustlbl->setText(current_map["wind_gust"].toString());


        QString dir = current_map["wind_deg"].toString(); // Current wind direction

        //qDebug() << dir;

        double bdir = dir.toDouble();
        // bdir = 68;
        //qDebug() << bdir;

        if (bdir >= 0 && bdir < 22){
            ui->dirlbl->setText("North");
        }
        else if (bdir >= 22 && bdir < 45){
            ui->dirlbl->setText("North NE");
        }
        else if (bdir >= 45 && bdir < 67){
            ui->dirlbl->setText("North East");
        }
        else if (bdir >= 67 && bdir < 90){
            ui->dirlbl->setText("East NE");
        }
        else if (bdir >= 90 && bdir < 112){
            ui->dirlbl->setText("East");
        }
        else if (bdir >= 112 && bdir < 135){
            ui->dirlbl->setText("East SE");
        }
        else if (bdir >= 135 && bdir < 157){
            ui->dirlbl->setText("South East");
        }
        else if (bdir >= 157 && bdir < 180){
            ui->dirlbl->setText("South SE");
        }
        else if (bdir >= 180 && bdir < 202){
            ui->dirlbl->setText("South");
        }
        else if (bdir >= 202 && bdir < 225){
            ui->dirlbl->setText("South SW");
        }
        else if (bdir >= 225 && bdir < 247){
            ui->dirlbl->setText("South West");
        }
        else if (bdir >= 247 && bdir < 270){
            ui->dirlbl->setText("West SW");
        }
        else if (bdir >= 270 && bdir < 292){
            ui->dirlbl->setText("West");
        }
        else if (bdir >= 292 && bdir < 315){
            ui->dirlbl->setText("West NW");
        }
        else if (bdir >= 315 && bdir < 337){
            ui->dirlbl->setText("North West");
        }
        else if (bdir >= 337 && bdir < 359){
            ui->dirlbl->setText("North NW");
        }


     delete reply;

}
}

void MainWindow::on_quitButton_clicked()
{
    this -> close();
}
