#include "bird.h"
bird::bird(QWidget *parent, fly_direction direction)

{
    if (direction == fly_direction::right)
    {
        bird_movie = new QMovie(BIRDR_LOCATION);
    }
    else if (direction == fly_direction::left)
    {
        bird_movie = new QMovie(BIRDL_LOCATION);
    }

    bird_movie->setScaledSize(QSize(200, 200));
    bird_label = new QLabel(parent);
    bird_label->setMovie(bird_movie);
    bird_movie->start();
    for (size_t i = 0; i < QRandomGenerator::global()->generate() % 0xFF; i++)
    {
        bird_movie->jumpToNextFrame();
    }
    movie_width = bird_movie->scaledSize().width();
    movie_height = bird_movie->scaledSize().height();
    bird_label->setGeometry(0, 0, movie_width, movie_height);
    this->hide();
}

QPoint bird::pos()
{
    QPoint pt(bird_label->pos().x(), bird_label->pos().y());
    return pt;
}

QColor bird::getCurrentPixelColor(int x, int y)
{
    QColor color(bird_movie->currentImage().pixelColor(QPoint(x, y)));
    return color;
}

bool bird::contains(int x, int y)
{
    return bird_label->rect().contains(x, y);
}

void bird::setLocation(int x, int y)
{
    bird_label->setGeometry(x - (movie_width / 2), y - (movie_height / 2), movie_width, movie_height);
}

void bird::show()
{
    bird_label->setVisible(true);
}

void bird::hide()
{
    bird_label->setVisible(false);
}
