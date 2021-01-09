#ifndef BIRD_H
#define BIRD_H

#include <QWidget>
#include <QObject>
#include <QMovie>
#include <QLabel>
#include <QPoint>
#include <QRandomGenerator>
#include <QSoundEffect>

#define BIRDL_LOCATION "/home/pi/Desktop/game/resources/images/birds/bird_left.gif"
#define BIRDR_LOCATION "/home/pi/Desktop/game/resources/images/birds/bird_right.gif"
#define BIRDL_DIE_LOCATION "/home/pi/Desktop/game/resources/images/birds/bird_left_die.gif"
#define BIRDR_DIE_LOCATION "/home/pi/Desktop/game/resources/images/birds/bird_right_die.gif"
#define BIRD_DIE_SOUND_LOCATION "/home/pi/Desktop/game/resources/sounds/bird_die.wav"

class bird
{
public:
    enum class fly_direction
    {
        left,
        right
    };
    bird(QWidget *parent, fly_direction direction);
    void setLocation(int x, int y);
    void show();
    void hide();
    void die();
    QPoint pos();
    QColor getCurrentPixelColor(int x, int y);
    bool contains(int x, int y);

private:
    QMovie *bird_movie;
    QMovie *bird_die_movie;
    QLabel *bird_label;
    QSoundEffect bird_die_effect;
    int movie_width = 0;
    int movie_height = 0;
};

#endif // BIRD_H
