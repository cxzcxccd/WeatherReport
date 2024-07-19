#pragma once
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

#include <QtWidgets/QWidget>
#include "ui_WeatherReport.h"
#include <QMenu>
#include <qevent.h>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <qstring.h>
#include <qdebug.h>
#include <QJsonDocument>
#include <QJsonObject>
#include <qjsonarray.h>
#include <QtCharts>
#include <qpainter.h>
#include <Qevent>


class WeatherReport : public QWidget
{
    Q_OBJECT

public:
    WeatherReport(QWidget *parent = nullptr);
    ~WeatherReport();


private:
    Ui::WeatherReportClass ui;
    QMenu* mExitMenu; //右键退出的菜单
    QAction* mExitAct; //退出的行为
    QNetworkAccessManager* mNetAccessManager; //声明用于获取http通信的对象
    QMap<QString, QString>* mCityMap; //声明城市编码
    QMap<QString, QString> mTypeMap;//声明天气图标
protected:
    QVector<QString> HighTemp; //声明高温
    QVector<QString> LowTemp;  //声明低温
    void contextMenuEvent(QContextMenuEvent* event);//右键点击事件
    void getWeatherInfo(QString CityCode);//获取天气数据
    void getTodayTime(QString WeekDay);//获取今天日期
    void PantStraightLine(QVector<QString>& High, QVector<QString>& Low);//声明绘制函数
    void PaintHighEvent();//声明绘制事件
    void PaintLowEvent();//声明绘制事件
    int h[7] = { 0 };
    int l[7] = { 0 };
    bool eventFilter(QObject* watched, QEvent* event);
};
