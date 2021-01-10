#ifndef BIRD_H
#define BIRD_H

#include <QWidget>
#include <QObject>
#include <QMovie>
#include <QLabel>
#include <QPoint>
#include <QRandomGenerator>
#include <QSoundEffect>

#define BIRDL_IMAGE "/home/pi/Desktop/game/resources/images/birds/bird_left.gif"
#define BIRDR_IMAGE "/home/pi/Desktop/game/resources/images/birds/bird_right.gif"
#define BIRDL_DIE_IMAGE "/home/pi/Desktop/game/resources/images/birds/bird_left_die.gif"
#define BIRDR_DIE_IMAGE "/home/pi/Desktop/game/resources/images/birds/bird_right_die.gif"
#define BIRD_DIE_SOUND "/home/pi/Desktop/game/resources/sounds/bird_die.wav"

class bird
{
public:
    enum class fly_direction
    {
        left = 0,
        right = 1
    };
    bird(QWidget *parent, fly_direction direction); 
    ~bird();
    void setLocation(int x, int y);
    void setXIncrement(int _increment);
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

private:
    QWidget *parentWidget;
    QMovie *bird_movie;
    QMovie *bird_die_movie;
    QLabel *bird_label;
    QSoundEffect bird_die_effect;
    int movie_width = 0;
    int movie_height = 0;
    int x_increment = 0;
    bool isShot;
};

#endif // BIRD_H
