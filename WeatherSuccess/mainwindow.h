#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <weatherdata.h>

#include <QLabel>
#include <QMainWindow>
#include <QMap>
#include <QMouseEvent>
#include <QNetworkAccessManager>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    bool eventFilter(QObject* watched, QEvent* event);

protected:
    // 重写父类的方法
    void contextMenuEvent(QContextMenuEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);

    // 获取天气数据、解析JSON、更新UI
    void getWeatherInfo(QString cityCode);
    void parseJson(QByteArray& byteArray);
    void updateUI();

    // 绘制高低温曲线
    void paintHighCurve();
    void paintLowCurve();

private slots:
    void onReplied(QNetworkReply* reply);

    void on_btnSearch_clicked();

private:
    Ui::MainWindow* ui;

    // 右键退出和窗口移动
    QMenu* mExitMenu;   // 右键退出的菜单
    QAction* mExitAct;  // 退出的行为
    QPoint mOffset;     // 窗口移动时, 鼠标与窗口左上角的偏移

    // Http 请求
    QNetworkAccessManager* mNetAccessManager;

    // 今天和6天的天气
    Today mToday;
    Day mDay[6];

    // 星期和日期
    QList<QLabel*> mWeekList;
    QList<QLabel*> mDateList;

    // 天气和天气图标
    QList<QLabel*> mTypeList;
    QList<QLabel*> mTypeIconList;

    // 天气指数
    QList<QLabel*> mAqiList;

    // 风向和风力
    QList<QLabel*> mFxList;
    QList<QLabel*> mFlList;

    QMap<QString, QString> mTypeMap;
};
#endif  // MAINWINDOW_H
