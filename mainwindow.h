#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QPixmap>
#include <QPalette>
#include <QMovie>
#include <QLabel>
#include <QTimer>
#include "bird.h"
#include <unistd.h>
#include <QMouseEvent>
#include <QRect>
#include <QSoundEffect>

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void timer_bird_timeout();
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void resizeEvent(QResizeEvent *evt);
    void paintEvent(QPaintEvent *);

private:
    Ui::MainWindow *ui;    
    QImage *bullethole_canvas;
    QImage *bullethole;
    QTimer *timer_bird;

    bird *bd1;
    bird *bd2;
    bird *bd3;
    //bird *birds
    int xpos = 0;

    QSoundEffect effect;
};

#endif // MAINWINDOW_H
