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
    QMenu* mExitMenu; //�Ҽ��˳��Ĳ˵�
    QAction* mExitAct; //�˳�����Ϊ
    QNetworkAccessManager* mNetAccessManager; //�������ڻ�ȡhttpͨ�ŵĶ���
    QMap<QString, QString>* mCityMap; //�������б���
    QMap<QString, QString> mTypeMap;//��������ͼ��
protected:
    QVector<QString> HighTemp; //��������
    QVector<QString> LowTemp;  //��������
    void contextMenuEvent(QContextMenuEvent* event);//�Ҽ�����¼�
    void getWeatherInfo(QString CityCode);//��ȡ��������
    void getTodayTime(QString WeekDay);//��ȡ��������
    void PantStraightLine(QVector<QString>& High, QVector<QString>& Low);//�������ƺ���
    void PaintHighEvent();//���������¼�
    void PaintLowEvent();//���������¼�
    int h[7] = { 0 };
    int l[7] = { 0 };
    bool eventFilter(QObject* watched, QEvent* event);
};
