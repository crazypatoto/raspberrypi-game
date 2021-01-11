#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define BULLETHOLES_MAX (20)

#include "bird.h"
#include <unistd.h>
#include <QMainWindow>
#include <QPainter>
#include <QPixmap>
#include <QPalette>
#include <QMovie>
#include <QLabel>
#include <QTimer>
#include <QMouseEvent>
#include <QRect>
#include <QSoundEffect>
#include <QFontDatabase>
#include <QCursor>
#include <QGraphicsBlurEffect>
#include <QList>
#include <QQueue>
#include <QRandomGenerator>
#include <opencv2/opencv.hpp>

using namespace cv;

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
    void timer_mousetracking_timeout();
    void timer_targets_timeout();
    void timer_game_timeout();
    void timer_scoreboard_animation_timeout();
    void timer_camera_timeout();
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
        Settings,
        ModeSelect,
        Start,
        Over
    };

    Ui::MainWindow *ui;
    GameState gameState = Menu;
    GameState gameState_prev = None;
    QSoundEffect shoot_soundeffect;
    QSoundEffect bgm_menu_soundeffect;
    QSoundEffect bgm_game_soundeffect;
    QSoundEffect gameover_soundeffect;
    QSoundEffect pick_soundeffect;
    QSoundEffect beep_soundeffect;
    QGraphicsBlurEffect *scoreboard_blureffect;
    QGraphicsBlurEffect *time_label_blureffect;
    QImage *bullethole_image;
    QImage *menu_image;
    QImage *gamemode_menu_image;
    QImage *scoreboard_image;
    QImage *camera_image;
    QImage *picture_frame_image;
    QImage *wood_button_image;
    QPixmap *background_img;
    QTimer *timer_mousetracking;
    QTimer *timer_game;
    QTimer *timer_targets;
    QTimer *timer_scoreboard_animation;
    QTimer *timer_camera;
    QLabel *scoreboard;
    QLabel *time_label;
    const QRect start_btn = QRect(475, 236, 336, 85);
    const QRect settings_btn = QRect(475, 359, 336, 85);
    const QRect exit_btn = QRect(475, 480, 336, 85);
    const QRect easy_btn = start_btn;
    const QRect hard_btn = settings_btn;
    const QRect return_btn = exit_btn;
    const QRect continue_btn = QRect(508, 512, 267, 92);
    const QRect capture_btn = QRect(489, 646, 294, 89);
    char menu_status = 0x00;
    char menu_status_prev = 0x00;

    QList<bird *> birds;
    QQueue<QPoint> bulletholes;

    VideoCapture video_capture;

    int score = 0;
    const int gameTime = 40;
    const int gameStartDelayTime = 2;
    int gameTimeCount = 0;
    int scoreboard_ypos = 0;
    int bird_shots = 0;
    int total_shots = 0;
    float accuarcy = 0.0f;
    bool isEasyMode;

    void updateBackgroundImage();
    void updateTime();
    void gameModeSelect();
    void gameSettings();
    void gameStart();
    void gameOver();
    void gameMenu();
    void spawnBirds(int num);
    void captureFace();
};

#endif // MAINWINDOW_H
