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
#include <QFontDatabase>
#include <QCursor>
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
    void timer_mousetracking_timeout();
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void resizeEvent(QResizeEvent *evt);
    void paintEvent(QPaintEvent *);

private:
    enum GameState
    {
        None,
        Menu,
        Start,
        GameOver
    };

    Ui::MainWindow *ui;
    GameState gameState = Menu;
    GameState gameState_prev = None;
    QSoundEffect shoot_soundeffect;    
    QSoundEffect bgm_soundeffect;
    QSoundEffect pick_soundeffect;
    QImage *bullethole_canvas;
    QImage *bullethole_image;
    QImage *menu_image;
    QPixmap *background_img;
    QTimer *timer_bird;
    QTimer *timer_mousetracking;
    QLabel *scoreboard;
    const QRect start_btn = QRect(475, 236, 336, 85);
    const QRect settings_btn = QRect(475, 359, 336, 85);
    const QRect exit_btn = QRect(475, 490, 336, 85);
    char menu_status = 0x00;
    char menu_status_prev = 0x00;

    bird *bd1;
    bird *bd2;
    bird *bd3;
    //bird *birds
    int xpos = 0;
    int score = 0;

    void updateBackgroundImage();
    void gameStart();
};

#endif // MAINWINDOW_H

