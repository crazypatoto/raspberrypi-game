#ifndef PERSON_H
#define PERSON_H

#include <QWidget>
#include <QObject>
#include <QImage>
#include <QPixmap>
#include <QMovie>
#include <QLabel>
#include <QPoint>
#include <QRect>
#include <QPainter>
#include <QRandomGenerator>
#include <QSoundEffect>

#define PERSON_IMAGE1 "/home/pi/Desktop/qt-game/game/resources/images/teletubbies/11.png"
#define PERSON_IMAGE2 "/home/pi/Desktop/qt-game/game/resources/images/teletubbies/22.png"
#define PERSON_IMAGE3 "/home/pi/Desktop/qt-game/game/resources/images/teletubbies/33.png"
#define PERSON_IMAGE4 "/home/pi/Desktop/qt-game/game/resources/images/teletubbies/44.png"
#define PERSON_DIE_IMAGE "/home/pi/Desktop/qt-game/game/resources/images/effects/explode.gif"
#define PERSON_DIE_SOUND "/home/pi/Desktop/qt-game/game/resources/sounds/bird_die_or.wav"

class person
{
public:
    person(QWidget *parent, int person, QImage image);
    ~person();
    void setLocation(int x, int y);
    void setXIncrement(int _increment);
    void setEasyShot(bool isEasy);
    void move();
    void show();
    void hide();
    void die();
    bool isDead();
    QPoint pos();
    int width();
    QColor getCurrentPixelColor(int x, int y);
    bool contains(int x, int y);
    bool checkShot(QPoint pos);
    const QRect tv_rect1 = QRect(45, 173, 65, 45);
    const QRect tv_rect2 = QRect(111, 166, 47, 40);
    const QRect tv_rect3 = QRect(35, 152, 58, 44);
    const QRect tv_rect4 = QRect(26, 105, 52, 53);

private:
    QWidget *parentWidget;
    QImage *person_image;
    QMovie *person_die_movie;
    QLabel *person_label;
    QSoundEffect person_die_effect;
    int x_increment = 0;
    int person_width, person_height;
    bool isShot;
    bool isEasyShot = false;
};

#endif
