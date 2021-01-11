#include "person.h"

person::person(QWidget *parent, int person, QImage image)
{
    parentWidget = parent;
    person_die_effect.setSource(QUrl::fromLocalFile(PERSON_DIE_SOUND));
    person_die_effect.setVolume(1.0f);

    switch (person)
    {
    case 0:
        person_image = new QImage(PERSON_IMAGE1);       
        break;
    case 1:
        person_image = new QImage(PERSON_IMAGE2);        
        break;
    case 2:
        person_image = new QImage(PERSON_IMAGE3);        
        break;
    case 3:
        person_image = new QImage(PERSON_IMAGE4);        
        break;
    default:
        person_image = new QImage(PERSON_IMAGE1);        
        break;
    }
    
    QPainter painter(person_image);
    switch (person)
    {
    case 0:
        image = image.scaled(tv_rect1.width(), tv_rect1.height(), Qt::IgnoreAspectRatio);
        painter.drawImage(tv_rect1.topLeft(), image);
        break;
    case 1:
        image = image.scaled(tv_rect2.width(), tv_rect2.height(), Qt::IgnoreAspectRatio);
        painter.drawImage(tv_rect2.topLeft(), image);
        break;
    case 2:
        image = image.scaled(tv_rect3.width(), tv_rect3.height(), Qt::IgnoreAspectRatio);
        painter.drawImage(tv_rect3.topLeft(), image);
        break;
    case 3:
        image = image.scaled(tv_rect4.width(), tv_rect4.height(), Qt::IgnoreAspectRatio);
        painter.drawImage(tv_rect4.topLeft(), image);
        break;
    default:
        image = image.scaled(tv_rect1.width(), tv_rect1.height(), Qt::IgnoreAspectRatio);
        painter.drawImage(tv_rect1.topLeft(), image);
        break;
    }
    painter.end();

    person_width = person_image->width();
    person_height = person_image->height();
    person_die_movie = new QMovie(PERSON_DIE_IMAGE);

    person_label = new QLabel(parent);
    person_label->setPixmap(QPixmap::fromImage(*person_image));
    person_label->setGeometry(0, 0, person_image->width(), person_image->height());
    isShot = false;
    this->hide();
}

person::~person()
{
    delete person_label;
    delete person_image;
    delete person_die_movie;
}

QPoint person::pos()
{
    QPoint pt(person_label->pos().x(), person_label->pos().y());
    return pt;
}

int person::width()
{
    return person_label->width();
}

QColor person::getCurrentPixelColor(int x, int y)
{
    QColor color(person_image->pixelColor(QPoint(x, y)));
    return color;
}

bool person::contains(int x, int y)
{

    return person_label->rect().contains(x, y);
}

bool person::checkShot(QPoint pos)
{
    if (isShot)
    {
        return false;
    }
    if (person_label->isVisible() == false)
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

void person::setLocation(int x, int y)
{
    person_label->setGeometry(x - (person_width / 2), y - (person_height / 2), person_width, person_height);
}

void person::setXIncrement(int _increment)
{
    x_increment = _increment;
}

void person::setEasyShot(bool isEasy)
{
    isEasyShot = isEasy;
}

void person::move()
{
    person_label->setGeometry(person_label->x() + x_increment, person_label->y(), person_width, person_height);
}

void person::show()
{
    person_label->setPixmap(QPixmap::fromImage(*person_image));
    person_label->setVisible(true);
    isShot = false;
}

void person::hide()
{
    //person_label->setMovie(new QMovie(BIRDR_DIE_IMAGE));
    person_label->setVisible(false);
}

void person::die()
{
    person_die_effect.play();
    person_label->setMovie(person_die_movie);
    person_die_movie->start();
}

bool person::isDead()
{
    if (isShot && person_die_movie->state() == QMovie::NotRunning)
    {
        return true;
    }
    if (isShot == false)
    {
        if (x_increment > 0)
        {
            if (person_label->x() > parentWidget->width())
            {
                return true;
            }
        }
        else
        {
            if (person_label->x() + person_width < 0)
            {
                return true;
            }
        }
    }

    return false;
}