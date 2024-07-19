#include "WeatherReport.h"

WeatherReport::WeatherReport(QWidget *parent)
    : QWidget(parent)
{
    //设置窗口无边框
    setWindowFlag(Qt::FramelessWindowHint);
    //固定窗口大小
    /*setFixedSize(width(), height());*/
    mNetAccessManager = new  QNetworkAccessManager();
    mExitMenu = new QMenu(this);
    mExitAct = new QAction();
    mExitAct->setText("退出");
    mExitAct->setIcon(QIcon(":/WeatherReport/res/close.png"));
    ui.setupUi(this);


    mTypeMap.insert("暴雪", ":/WeatherReport/res/BaoXue.png");
    mTypeMap.insert("暴雨", ":/WeatherReport/res/BaoYu.png");
    mTypeMap.insert("暴雨到大暴雨", ":/WeatherReport/res/BaoYuDaoDaBaoYu.png");
    mTypeMap.insert("大暴雨", ":/WeatherReport/res/DaBaoYu.png");
    mTypeMap.insert("大暴雨到特大暴雨", ":/WeatherReport/res/DaBaoYuDaoTeDaBaoYu.png");
    mTypeMap.insert("大到暴雪", ":/WeatherReport/res/DaDaoBaoXue.png");
    mTypeMap.insert("大雪", ":/WeatherReport/res/DaXue.png");
    mTypeMap.insert("大雨", ":/WeatherReport/res/DaYu.png");
    mTypeMap.insert("冻雨", ":/WeatherReport/res/DongYu.png");
    mTypeMap.insert("多云", ":/WeatherReport/res/DuoYun.png");
    mTypeMap.insert("浮沉", ":/WeatherReport/res/FuChen.png");
    mTypeMap.insert("雷阵雨", ":/WeatherReport/res/LeiZhenYu.png");
    mTypeMap.insert("雷阵雨伴有冰雹", ":/WeatherReport/res/LeiZhenYuBanYouBingBao.png");
    mTypeMap.insert("霾", ":/WeatherReport/res/Mai.png");
    mTypeMap.insert("强沙尘暴", ":/WeatherReport/res/QiangShaChenBao.png");
    mTypeMap.insert("晴", ":/WeatherReport/res/Qing.png");
    mTypeMap.insert("沙尘暴", ":/WeatherReport/res/ShaChenBao.png");
    mTypeMap.insert("特大暴雨", ":/WeatherReport/res/TeDaBaoYu.png");
    mTypeMap.insert("undefined", ":/WeatherReport/res/undefined.png");
    mTypeMap.insert("雾", ":/WeatherReport/res/Wu.png");
    mTypeMap.insert("小到中雪", ":/WeatherReport/res/XiaoDaoZhongXue.png");
    mTypeMap.insert("小到中雨", ":/WeatherReport/res/XiaoDaoZhongYu.png");
    mTypeMap.insert("小雪", ":/WeatherReport/res/XiaoXue.png");
    mTypeMap.insert("小雨", ":/WeatherReport/res/XiaoYu.png");
    mTypeMap.insert("雪", ":/WeatherReport/res/Xue.png");
    mTypeMap.insert("扬沙", ":/WeatherReport/res/YangSha.png");
    mTypeMap.insert("阴", ":/WeatherReport/res/Yin.png");
    mTypeMap.insert("雨", ":/WeatherReport/res/Yu.png");
    mTypeMap.insert("雨夹雪", ":/WeatherReport/res/YuJiaXue.png");
    mTypeMap.insert("阵雪", ":/WeatherReport/res/ZhenXue.png");
    mTypeMap.insert("阵雨", ":/WeatherReport/res/ZhenYu.png");
    mTypeMap.insert("中到大雪", ":/WeatherReport/res/ZhongDaoDaXue.png");
    mTypeMap.insert("中到大雨", ":/WeatherReport/res/ZhongDaoDaYu.png");
    mTypeMap.insert("中雪", ":/WeatherReport/res/ZhongXue.png");
    mTypeMap.insert("中雨", ":/WeatherReport/res/ZhongYu.png"); \
    ui.HighTempView->installEventFilter(this);
    ui.LowTempView->installEventFilter(this);
    //各种CSS
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

    // 设置按钮的图片
    QPixmap CitySearchPixmap(":/WeatherReport/res/search.png");
    CitySearchPixmap = CitySearchPixmap.scaled(140, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui.SearchCity->setIcon(CitySearchPixmap);
    ui.SearchCity->setIconSize(QSize(30, 30));  // 确保图标大小正确
    ui.SearchCity->setStyleSheet("QPushButton { text-align: center;background-color: rgb(255, 255, 191); }");  // 居中显示文本


    //设置风速照片
    QPixmap FlSpeedPixmap(":/WeatherReport/res/wind.png");
    ui.FlSpeedIcon->setPixmap(FlSpeedPixmap);

    //设置空气质量指数照片
    QPixmap AqiPixmap(":/WeatherReport/res/pm25.png");
    AqiPixmap = AqiPixmap.scaled(25, 25, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui.AqiIcon->setPixmap(AqiPixmap);
 

    
    mExitMenu->addAction(mExitAct);
    connect(mExitAct, &QAction::triggered, this, [=] {
        qApp->exit(0);
        });

    // 读取城市json路径
    QMap<QString, QString>* mCityMap = new QMap<QString, QString>;
    QString Path = "E:/CodeOfProject/C++/Weather/WeatherReport/res/citycode-2019-08-23.json";
    QFile file(Path);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QByteArray jsonData = file.readAll();
        file.close();

        // 解析JSON数据
        QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
        if (jsonDoc.isArray()) {
            QJsonArray jsonArray = jsonDoc.array();

            // 遍历JSON数组
            for (const QJsonValue& value : jsonArray) {
                if (value.isObject()) {
                    QJsonObject jsonObject = value.toObject();
                    QString city = jsonObject["city_name"].toString();
                    QString cityCode = jsonObject["city_code"].toString();
                    //qDebug() << city << ' ' << cityCode << '\n';
                    // 将城市和代码写入mCityMap
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

    //// 打印城市代码以验证
    //QString cityCode = mCityMap->value("北京");
    //qDebug() << "City Code for Beijing:" << cityCode;

    getWeatherInfo(mCityMap->value("北京"));
    ui.CityName->setText("北京");
    //定义搜索城市函数
    connect(ui.SearchCity, &QPushButton::clicked, ui.CityIn, [=] {
        ui.CityName->setText(ui.CityIn->text());
        getWeatherInfo(mCityMap->value(ui.CityIn->text()));
        });
    //定义翻页函数
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
    //弹出右键菜单
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
            // 读取数据
            QByteArray data = reply->readAll();
            QString jsonData = data;

            // 解析 JSON 数据
            QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
            QJsonObject jsonObj = jsonDoc.object();
            qDebug() << jsonObj << '\n';
            // 获取 data 对象
            QJsonObject Wdata = jsonObj["data"].toObject();
            // 获取 forecast 数组
            QJsonArray forecastArray = Wdata["forecast"].toArray();
            
            //获取今天城市的天气信息
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
            ui.Aqi->setText("空气质量指数:" + aqiText);
            ui.WindFlow->setText("风向:" + WindFlow);
            ui.FlSpeed->setText("风速:" + WindPower);
            ui.Notice->setText("感冒指数:" + Notice);
            getTodayTime(ZeroDate);

            ui.Situation->setText(Type);
            ui.Situation->setStyleSheet("QLabel { color: #ffffff; font-size: 25px; }");
            ui.Situation->setAlignment(Qt::AlignCenter);
          
			QPixmap pixmap(mTypeMap[Type]);
			pixmap = pixmap.scaled(140, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation);
			ui.BigFigure->setAlignment(Qt::AlignCenter);
			ui.BigFigure->setPixmap(pixmap);
            
            
            
            

            //获取第二天的天气信息
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

            //获取第三天的天气信息
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


            //获取第四天的天气信息
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

            //获取第五天的天气信息
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
            //获取第六天的天气信息
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


            //获取第七天的天气信息
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
            ui.Temp1->setText(QString::number(h[1]) + "°C\n" + QString::number(l[1]) + "°C");
            ui.Temp2->setText(QString::number(h[2]) + "°C\n" + QString::number(l[2]) + "°C");
            ui.Temp3->setText(QString::number(h[3]) + "°C\n" + QString::number(l[3]) + "°C");
            ui.Temp4->setText(QString::number(h[4]) + "°C\n" + QString::number(l[4]) + "°C");
            ui.Temp5->setText(QString::number(h[5]) + "°C\n" + QString::number(l[5]) + "°C");
            ui.Temp6->setText(QString::number(h[6]) + "°C\n" + QString::number(l[6]) + "°C");
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
        "color: white;"  // 字体颜色为白色
        "background-color: transparent;"  // 背景颜色透明
        "}");
}

void WeatherReport::PantStraightLine(QVector<QString>& High, QVector<QString>& Low)
{
   /* int h[7] = { 0 };
    int l[7] = { 0 };*/
    //将字符串变为整数
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
    //X坐标
    int X_Location = 50;
    int dx = ui.HighTempView->width() / 7;
    //Y坐标
    int SumHighTemp = 0;
    for (int i = 0; i < 7; i++)
    {
        SumHighTemp += h[i];
    }
    int AvgHighTemp = SumHighTemp / 7;
    int Y_Location = ui.HighTempView->height() / 2;
    //绘制
    QPen pen = painter.pen();
    pen.setWidth(1);                    //设置画笔宽度为1
    pen.setColor(QColor(255, 170, 0));  //设置颜色
    painter.save();
    painter.setPen(pen);
    painter.setBrush(QColor(255, 170, 0));  //设置画刷颜色,填充图形
    int pointx[7] = { 0 }, pointy[7] = { 0 };
    for (int i = 0; i < 7; i++,X_Location+=dx)
    {
        pointx[i] = X_Location;
        pointy[i] = Y_Location + 3 * (h[i] - AvgHighTemp);
        painter.drawEllipse(QPoint(X_Location, Y_Location + 3 * (h[i] - AvgHighTemp)), 3, 3);
        //绘制文本
        QString Temp = QString::number(h[i]) + "°C";
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
    //X坐标
    int X_Location = 50;
    int dx = ui.LowTempView->width() / 7;
    //Y坐标
    int SumLowTemp = 0;
    for (int i = 0; i < 7; i++)
    {
        SumLowTemp += l[i];
    }
    int AvgLowTemp = SumLowTemp / 7;
    int Y_Location = ui.LowTempView->height() / 2;
    //绘制
    QPen pen = painter.pen();
    pen.setWidth(1);                    //设置画笔宽度为1
    pen.setColor(QColor(0, 127, 255));  //设置颜色
    painter.save();
    painter.setPen(pen);
    painter.setBrush(QColor(0, 127, 255));  //设置画刷颜色
    //画点
    int pointx[7] = { 0 }, pointy[7] = { 0 };
    painter.drawLine(0, 120, 800, 120);
    painter.drawText(QPoint(300, 110), "未来一周内的温度折线图");
    for (int i = 0; i < 7; i++, X_Location += dx)
    {
        pointx[i] = X_Location;
        pointy[i] = Y_Location + 3 * (l[i] - AvgLowTemp);
        painter.drawEllipse(QPoint(X_Location, Y_Location + 3 * (l[i] - AvgLowTemp)), 3, 3);
        QString Temp = QString::number(l[i]) + "°C";
        painter.drawText(QPoint(pointx[i] - 3, pointy[i] - 5), Temp);
    }
    //画线
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
