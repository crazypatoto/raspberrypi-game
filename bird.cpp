#include "bird.h"

bird::bird(QWidget *parent, fly_direction direction)
{
    parentWidget = parent;
    bird_die_effect.setSource(QUrl::fromLocalFile(BIRD_DIE_SOUND));
    bird_die_effect.setVolume(1.0f);
    if (direction == fly_direction::right)
    {
        bird_movie = new QMovie(BIRDR_IMAGE);
        bird_die_movie = new QMovie(BIRDR_DIE_IMAGE);
    }
    else if (direction == fly_direction::left)
    {
        bird_movie = new QMovie(BIRDL_IMAGE);
        bird_die_movie = new QMovie(BIRDL_DIE_IMAGE);
    }
    bird_movie->start();

    bird_movie->setScaledSize(QSize(200, 200));
    bird_label = new QLabel(parent);
    bird_label->setMovie(bird_movie);
    for (size_t i = 0; i < QRandomGenerator::global()->generate() % 0xFF; i++)
    {
        bird_movie->jumpToNextFrame();
    }
    movie_width = bird_movie->scaledSize().width();
    movie_height = bird_movie->scaledSize().height();
    bird_label->setGeometry(0, 0, movie_width, movie_height);
    isShot = false;
    this->hide();
}

bird::~bird()
{
    delete bird_label;
    delete bird_movie;
    delete bird_die_movie;
}

QPoint bird::pos()
{
    QPoint pt(bird_label->pos().x(), bird_label->pos().y());
    return pt;
}

int bird::width()
{
    return bird_label->width();
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

bool bird::checkShot(QPoint pos)
{
    if (isShot)
    {
        return false;
    }
    if (bird_label->isVisible() == false)
    {
        return false;
    }
    if (this->contains(pos.x() - this->pos().x(), pos.y() - this->pos().y()))
    {
        if (isEasyShot)
        {
            isShot = true;
            this->die();
            return true;
        }
        QColor bd_color = this->getCurrentPixelColor(pos.x() - this->pos().x(), pos.y() - this->pos().y());
        // printf("Alpha: %d\n", bd_color.alpha());
        if (bd_color.alpha() == 255)
        {
            isShot = true;
            this->die();
            return true;
        }
    }
    return false;
}

void bird::setLocation(int x, int y)
{
    bird_label->setGeometry(x - (movie_width / 2), y - (movie_height / 2), movie_width, movie_height);
}

void bird::setXIncrement(int _increment)
{
    x_increment = _increment;
}

void bird::setEasyShot(bool isEasy)
{
    isEasyShot = isEasy;
}

void bird::move()
{
    bird_label->setGeometry(bird_label->x() + x_increment, bird_label->y(), movie_width, movie_height);
}

void bird::show()
{
    bird_label->setMovie(bird_movie);
    bird_label->setVisible(true);
    isShot = false;
}

void bird::hide()
{
    //bird_label->setMovie(new QMovie(BIRDR_DIE_IMAGE));
    bird_label->setVisible(false);
}

void bird::die()
{
    bird_die_effect.play();
    bird_label->setMovie(bird_die_movie);
    bird_die_movie->start();
}

bool bird::isDead()
{
    if (isShot && bird_die_movie->state() == QMovie::NotRunning)
    {
        return true;
    }
    if (isShot == false)
    {
        if (x_increment > 0)
        {
            if (bird_label->x() > parentWidget->width())
            {
                return true;
            }
        }
        else
        {
            if (bird_label->x() + movie_width < 0)
            {
                return true;
            }
        }
    }

    return false;
}