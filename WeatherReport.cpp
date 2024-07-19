#include "WeatherReport.h"

WeatherReport::WeatherReport(QWidget *parent)
    : QWidget(parent)
{
    //���ô����ޱ߿�
    setWindowFlag(Qt::FramelessWindowHint);
    //�̶����ڴ�С
    /*setFixedSize(width(), height());*/
    mNetAccessManager = new  QNetworkAccessManager();
    mExitMenu = new QMenu(this);
    mExitAct = new QAction();
    mExitAct->setText("�˳�");
    mExitAct->setIcon(QIcon(":/WeatherReport/res/close.png"));
    ui.setupUi(this);


    mTypeMap.insert("��ѩ", ":/WeatherReport/res/BaoXue.png");
    mTypeMap.insert("����", ":/WeatherReport/res/BaoYu.png");
    mTypeMap.insert("���굽����", ":/WeatherReport/res/BaoYuDaoDaBaoYu.png");
    mTypeMap.insert("����", ":/WeatherReport/res/DaBaoYu.png");
    mTypeMap.insert("���굽�ش���", ":/WeatherReport/res/DaBaoYuDaoTeDaBaoYu.png");
    mTypeMap.insert("�󵽱�ѩ", ":/WeatherReport/res/DaDaoBaoXue.png");
    mTypeMap.insert("��ѩ", ":/WeatherReport/res/DaXue.png");
    mTypeMap.insert("����", ":/WeatherReport/res/DaYu.png");
    mTypeMap.insert("����", ":/WeatherReport/res/DongYu.png");
    mTypeMap.insert("����", ":/WeatherReport/res/DuoYun.png");
    mTypeMap.insert("����", ":/WeatherReport/res/FuChen.png");
    mTypeMap.insert("������", ":/WeatherReport/res/LeiZhenYu.png");
    mTypeMap.insert("��������б���", ":/WeatherReport/res/LeiZhenYuBanYouBingBao.png");
    mTypeMap.insert("��", ":/WeatherReport/res/Mai.png");
    mTypeMap.insert("ǿɳ����", ":/WeatherReport/res/QiangShaChenBao.png");
    mTypeMap.insert("��", ":/WeatherReport/res/Qing.png");
    mTypeMap.insert("ɳ����", ":/WeatherReport/res/ShaChenBao.png");
    mTypeMap.insert("�ش���", ":/WeatherReport/res/TeDaBaoYu.png");
    mTypeMap.insert("undefined", ":/WeatherReport/res/undefined.png");
    mTypeMap.insert("��", ":/WeatherReport/res/Wu.png");
    mTypeMap.insert("С����ѩ", ":/WeatherReport/res/XiaoDaoZhongXue.png");
    mTypeMap.insert("С������", ":/WeatherReport/res/XiaoDaoZhongYu.png");
    mTypeMap.insert("Сѩ", ":/WeatherReport/res/XiaoXue.png");
    mTypeMap.insert("С��", ":/WeatherReport/res/XiaoYu.png");
    mTypeMap.insert("ѩ", ":/WeatherReport/res/Xue.png");
    mTypeMap.insert("��ɳ", ":/WeatherReport/res/YangSha.png");
    mTypeMap.insert("��", ":/WeatherReport/res/Yin.png");
    mTypeMap.insert("��", ":/WeatherReport/res/Yu.png");
    mTypeMap.insert("���ѩ", ":/WeatherReport/res/YuJiaXue.png");
    mTypeMap.insert("��ѩ", ":/WeatherReport/res/ZhenXue.png");
    mTypeMap.insert("����", ":/WeatherReport/res/ZhenYu.png");
    mTypeMap.insert("�е���ѩ", ":/WeatherReport/res/ZhongDaoDaXue.png");
    mTypeMap.insert("�е�����", ":/WeatherReport/res/ZhongDaoDaYu.png");
    mTypeMap.insert("��ѩ", ":/WeatherReport/res/ZhongXue.png");
    mTypeMap.insert("����", ":/WeatherReport/res/ZhongYu.png"); \
    ui.HighTempView->installEventFilter(this);
    ui.LowTempView->installEventFilter(this);
    //����CSS
    ui.TodayTime->setStyleSheet("font-family: 'Microsoft YaHei'; font-size: 20px;");

    ui.Week0->setAlignment(Qt::AlignCenter);
    ui.fig0->setAlignment(Qt::AlignCenter);
    ui.Week2->setAlignment(Qt::AlignCenter);
    ui.fig1->setAlignment(Qt::AlignCenter);
    ui.Week3->setAlignment(Qt::AlignCenter);
    ui.fig3->setAlignment(Qt::AlignCenter);
    ui.Week4->setAlignment(Qt::AlignCenter);
    ui.fig4->setAlignment(Qt::AlignCenter);
    ui.Week5->setAlignment(Qt::AlignCenter);
    ui.fig5->setAlignment(Qt::AlignCenter);
    ui.Week6->setAlignment(Qt::AlignCenter);
    ui.fig6->setAlignment(Qt::AlignCenter);

    // ���ð�ť��ͼƬ
    QPixmap CitySearchPixmap(":/WeatherReport/res/search.png");
    CitySearchPixmap = CitySearchPixmap.scaled(140, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui.SearchCity->setIcon(CitySearchPixmap);
    ui.SearchCity->setIconSize(QSize(30, 30));  // ȷ��ͼ���С��ȷ
    ui.SearchCity->setStyleSheet("QPushButton { text-align: center;background-color: rgb(255, 255, 191); }");  // ������ʾ�ı�


    //���÷�����Ƭ
    QPixmap FlSpeedPixmap(":/WeatherReport/res/wind.png");
    ui.FlSpeedIcon->setPixmap(FlSpeedPixmap);

    //���ÿ�������ָ����Ƭ
    QPixmap AqiPixmap(":/WeatherReport/res/pm25.png");
    AqiPixmap = AqiPixmap.scaled(25, 25, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui.AqiIcon->setPixmap(AqiPixmap);
 

    
    mExitMenu->addAction(mExitAct);
    connect(mExitAct, &QAction::triggered, this, [=] {
        qApp->exit(0);
        });

    // ��ȡ����json·��
    QMap<QString, QString>* mCityMap = new QMap<QString, QString>;
    QString Path = "E:/CodeOfProject/C++/Weather/WeatherReport/res/citycode-2019-08-23.json";
    QFile file(Path);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QByteArray jsonData = file.readAll();
        file.close();

        // ����JSON����
        QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
        if (jsonDoc.isArray()) {
            QJsonArray jsonArray = jsonDoc.array();

            // ����JSON����
            for (const QJsonValue& value : jsonArray) {
                if (value.isObject()) {
                    QJsonObject jsonObject = value.toObject();
                    QString city = jsonObject["city_name"].toString();
                    QString cityCode = jsonObject["city_code"].toString();
                    //qDebug() << city << ' ' << cityCode << '\n';
                    // �����кʹ���д��mCityMap
                    mCityMap->insert(city, cityCode);
                }
            }
        }
        else {
            qDebug() << "JSON is not an array";
        }
    }
    else {
        qDebug() << "Cannot open file";
    }

    //// ��ӡ���д�������֤
    //QString cityCode = mCityMap->value("����");
    //qDebug() << "City Code for Beijing:" << cityCode;

    getWeatherInfo(mCityMap->value("����"));
    ui.CityName->setText("����");
    //�����������к���
    connect(ui.SearchCity, &QPushButton::clicked, ui.CityIn, [=] {
        ui.CityName->setText(ui.CityIn->text());
        getWeatherInfo(mCityMap->value(ui.CityIn->text()));
        });
    //���巭ҳ����
    connect(ui.NextPage, &QPushButton::clicked, [=] {
        ui.stackedWidget->setCurrentIndex(1);
        });
    connect(ui.LastPage, &QPushButton::clicked, [=] {
        ui.stackedWidget->setCurrentIndex(0);
        });
}

WeatherReport::~WeatherReport()
{}

void WeatherReport::contextMenuEvent(QContextMenuEvent * event)
{
    //�����Ҽ��˵�
    mExitMenu->exec(QCursor::pos());
    event->accept();

}

void WeatherReport::getWeatherInfo(QString CityCode)
{
    HighTemp.clear();
    LowTemp.clear();
    QUrl url("http://t.weather.itboy.net/api/weather/city/" + CityCode);
    QNetworkReply* reply = mNetAccessManager->get(QNetworkRequest(url));
    connect(reply, &QNetworkReply::finished, [=]() 
   {
        if (reply->error() == QNetworkReply::NoError) 
        {
            // ��ȡ����
            QByteArray data = reply->readAll();
            QString jsonData = data;

            // ���� JSON ����
            QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
            QJsonObject jsonObj = jsonDoc.object();
            qDebug() << jsonObj << '\n';
            // ��ȡ data ����
            QJsonObject Wdata = jsonObj["data"].toObject();
            // ��ȡ forecast ����
            QJsonArray forecastArray = Wdata["forecast"].toArray();
            
            //��ȡ������е�������Ϣ
            QJsonObject firstForecast = forecastArray.at(0).toObject();
            int aqiValue = firstForecast["aqi"].toInt();
            QString aqiText = QString::number(aqiValue);

            QString firstHigh = firstForecast["high"].toString();
            QString firstLow = firstForecast["low"].toString();
            HighTemp.push_back(firstHigh);
            LowTemp.push_back(firstLow);
            QString WindFlow = firstForecast["fx"].toString();
            QString WindPower = firstForecast["fl"].toString();
            QString ZeroDate = firstForecast["week"].toString();
            QString Type = firstForecast["type"].toString();
            QString Notice = firstForecast["notice"].toString();
            ui.HightestTemp->setText(firstHigh);
            ui.LowestTemp->setText(firstLow);
            ui.Aqi->setText("��������ָ��:" + aqiText);
            ui.WindFlow->setText("����:" + WindFlow);
            ui.FlSpeed->setText("����:" + WindPower);
            ui.Notice->setText("��ðָ��:" + Notice);
            getTodayTime(ZeroDate);

            ui.Situation->setText(Type);
            ui.Situation->setStyleSheet("QLabel { color: #ffffff; font-size: 25px; }");
            ui.Situation->setAlignment(Qt::AlignCenter);
          
			QPixmap pixmap(mTypeMap[Type]);
			pixmap = pixmap.scaled(140, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation);
			ui.BigFigure->setAlignment(Qt::AlignCenter);
			ui.BigFigure->setPixmap(pixmap);
            
            
            
            

            //��ȡ�ڶ����������Ϣ
            QJsonObject SecondForecast = forecastArray.at(1).toObject();
            QString TwoDate = SecondForecast["week"].toString();
            QString WeaterWeek2 = SecondForecast["type"].toString();
            QString Type1= SecondForecast["type"].toString();
            QString SecondLow = SecondForecast["low"].toString();
            QString SecondHigh = SecondForecast["high"].toString();
            HighTemp.push_back(SecondHigh);
            LowTemp.push_back(SecondLow);
            ui.fig0->setText(WeaterWeek2);
            ui.Week0->setText(TwoDate);
            ui.label->setPixmap(mTypeMap[Type1]);

            //��ȡ�������������Ϣ
            QJsonObject ThirdForecast = forecastArray.at(2).toObject();
            QString ThreeDate = ThirdForecast["week"].toString();
            QString WeaterWeek3 = ThirdForecast["type"].toString();
            QString Type2 = ThirdForecast["type"].toString();
            QString ThirdLow = ThirdForecast["low"].toString();
            QString ThirdHigh = ThirdForecast["high"].toString();
            HighTemp.push_back(ThirdHigh);
            LowTemp.push_back(ThirdLow);
            ui.fig1->setText(WeaterWeek3);
            ui.Week2->setText(ThreeDate);
            ui.label_2->setPixmap(mTypeMap[Type2]);


            //��ȡ�������������Ϣ
            QJsonObject FourForecast = forecastArray.at(3).toObject();
            QString FourDate = FourForecast["week"].toString();
            QString WeaterWeek4 = FourForecast["type"].toString();
            QString Type3 = FourForecast["type"].toString();
            QString FourLow = FourForecast["low"].toString();
            QString FourHigh = FourForecast["high"].toString();
            HighTemp.push_back(FourHigh);
            LowTemp.push_back(FourLow);
            ui.fig3->setText(WeaterWeek4);
            ui.Week3->setText(FourDate);
            ui.label_3->setPixmap(mTypeMap[Type3]);

            //��ȡ�������������Ϣ
            QJsonObject FiveForecast = forecastArray.at(4).toObject();
            QString FiveDate = FiveForecast["week"].toString();
            QString WeaterWeek5 = FiveForecast["type"].toString();
            QString Type4 = FiveForecast["type"].toString();
            QString FiveLow = FiveForecast["low"].toString();
            QString FiveHigh = FiveForecast["high"].toString();
            HighTemp.push_back(FiveHigh);
            LowTemp.push_back(FiveLow);
            ui.fig4->setText(WeaterWeek5);
            ui.Week4->setText(FiveDate);
            ui.label_4->setPixmap(mTypeMap[Type4]);
            //��ȡ�������������Ϣ
            QJsonObject SixForecast = forecastArray.at(5).toObject();
            QString SixDate = SixForecast["week"].toString();
            QString WeaterWeek6 = SixForecast["type"].toString();
            QString Type5 = SixForecast["type"].toString();
            QString SixLow = SixForecast["low"].toString();
            QString SixHigh = SixForecast["high"].toString();
            HighTemp.push_back(SixHigh);
            LowTemp.push_back(SixLow);
            ui.fig5->setText(WeaterWeek6);
            ui.Week5->setText(SixDate);
            ui.label_5->setPixmap(mTypeMap[Type5]);


            //��ȡ�������������Ϣ
            QJsonObject SevenForecast = forecastArray.at(6).toObject();
            QString SevenDate = SevenForecast["week"].toString();
            QString WeaterWeek7 = SevenForecast["type"].toString();
            QString Type6 = SevenForecast["type"].toString();
            QString SevenLow = SevenForecast["low"].toString();
            QString SevenHigh = SevenForecast["high"].toString();
            HighTemp.push_back(SevenHigh);
            LowTemp.push_back(SevenLow);

       
            ui.fig6->setText(WeaterWeek7);
            ui.Week6->setText(SevenDate);
            ui.label_6->setPixmap(mTypeMap[Type6]);
            
            PantStraightLine(HighTemp, LowTemp);
            ui.Temp1->setText(QString::number(h[1]) + "��C\n" + QString::number(l[1]) + "��C");
            ui.Temp2->setText(QString::number(h[2]) + "��C\n" + QString::number(l[2]) + "��C");
            ui.Temp3->setText(QString::number(h[3]) + "��C\n" + QString::number(l[3]) + "��C");
            ui.Temp4->setText(QString::number(h[4]) + "��C\n" + QString::number(l[4]) + "��C");
            ui.Temp5->setText(QString::number(h[5]) + "��C\n" + QString::number(l[5]) + "��C");
            ui.Temp6->setText(QString::number(h[6]) + "��C\n" + QString::number(l[6]) + "��C");
        }
        reply->deleteLater();
   });
}

void WeatherReport::getTodayTime(QString WeekDay)
{
    QDate today = QDate::currentDate();
    QString date = today.toString("yyyy-MM-dd");
    ui.TodayTime->setText(date + "\n" + WeekDay);
    ui.TodayTime->setStyleSheet("QLabel {"
        "color: white;"  // ������ɫΪ��ɫ
        "background-color: transparent;"  // ������ɫ͸��
        "}");
}

void WeatherReport::PantStraightLine(QVector<QString>& High, QVector<QString>& Low)
{
   /* int h[7] = { 0 };
    int l[7] = { 0 };*/
    //���ַ�����Ϊ����
    for (int i = 0; i < 7; i++)
    {
        h[i] = HighTemp[i][4].toLatin1() - '0' + (HighTemp[i][3].toLatin1() - '0') * 10;
        l[i] = LowTemp[i][4].toLatin1() - '0' + (LowTemp[i][3].toLatin1() - '0') * 10;
    }
    ui.LowTempView->update();
    ui.HighTempView->update();
}

void WeatherReport::PaintHighEvent()
{
    QPainter painter(ui.HighTempView);
    painter.setRenderHint(QPainter::Antialiasing, true);
    //X����
    int X_Location = 50;
    int dx = ui.HighTempView->width() / 7;
    //Y����
    int SumHighTemp = 0;
    for (int i = 0; i < 7; i++)
    {
        SumHighTemp += h[i];
    }
    int AvgHighTemp = SumHighTemp / 7;
    int Y_Location = ui.HighTempView->height() / 2;
    //����
    QPen pen = painter.pen();
    pen.setWidth(1);                    //���û��ʿ��Ϊ1
    pen.setColor(QColor(255, 170, 0));  //������ɫ
    painter.save();
    painter.setPen(pen);
    painter.setBrush(QColor(255, 170, 0));  //���û�ˢ��ɫ,���ͼ��
    int pointx[7] = { 0 }, pointy[7] = { 0 };
    for (int i = 0; i < 7; i++,X_Location+=dx)
    {
        pointx[i] = X_Location;
        pointy[i] = Y_Location + 3 * (h[i] - AvgHighTemp);
        painter.drawEllipse(QPoint(X_Location, Y_Location + 3 * (h[i] - AvgHighTemp)), 3, 3);
        //�����ı�
        QString Temp = QString::number(h[i]) + "��C";
        painter.drawText(QPoint(pointx[i] - 3, pointy[i] - 5),  Temp);
    }
	for (int i = 0; i < 6; i++, X_Location += dx)
	{
		painter.drawLine(pointx[i], pointy[i],
			pointx[i + 1], pointy[i + 1]);
	}



    painter.restore();
}

void WeatherReport::PaintLowEvent()
{
    QPainter painter(ui.LowTempView);
    painter.setRenderHint(QPainter::Antialiasing, true);
    ui.LowTempView->setStyleSheet("background-color: rgba(255, 255, 255, 128);");
    //X����
    int X_Location = 50;
    int dx = ui.LowTempView->width() / 7;
    //Y����
    int SumLowTemp = 0;
    for (int i = 0; i < 7; i++)
    {
        SumLowTemp += l[i];
    }
    int AvgLowTemp = SumLowTemp / 7;
    int Y_Location = ui.LowTempView->height() / 2;
    //����
    QPen pen = painter.pen();
    pen.setWidth(1);                    //���û��ʿ��Ϊ1
    pen.setColor(QColor(0, 127, 255));  //������ɫ
    painter.save();
    painter.setPen(pen);
    painter.setBrush(QColor(0, 127, 255));  //���û�ˢ��ɫ
    //����
    int pointx[7] = { 0 }, pointy[7] = { 0 };
    painter.drawLine(0, 120, 800, 120);
    painter.drawText(QPoint(300, 110), "δ��һ���ڵ��¶�����ͼ");
    for (int i = 0; i < 7; i++, X_Location += dx)
    {
        pointx[i] = X_Location;
        pointy[i] = Y_Location + 3 * (l[i] - AvgLowTemp);
        painter.drawEllipse(QPoint(X_Location, Y_Location + 3 * (l[i] - AvgLowTemp)), 3, 3);
        QString Temp = QString::number(l[i]) + "��C";
        painter.drawText(QPoint(pointx[i] - 3, pointy[i] - 5), Temp);
    }
    //����
    X_Location = 50;
    for (int i = 0; i < 6; i++, X_Location += dx)
    {
        painter.drawLine(pointx[i], pointy[i],
            pointx[i + 1], pointy[i + 1]);
    }
    
    painter.restore();
    
}



bool WeatherReport::eventFilter(QObject* watched, QEvent* event)
{
    if (watched == ui.HighTempView && event->type() == QEvent::Paint)
    {
        PaintHighEvent();
    }
     if (watched == ui.LowTempView && event->type() == QEvent::Paint) 
    {
        PaintLowEvent();
    }
    return QWidget::eventFilter(watched, event);
    
}
