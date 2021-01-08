#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    bd1 = new bird(this, bird::fly_direction::right);
    bd2 = new bird(this, bird::fly_direction::left);
    bd3 = new bird(this, bird::fly_direction::right);
    timer_bird = new QTimer(this);
    connect(timer_bird, SIGNAL(timeout()), this, SLOT(timer_bird_timeout()));

    effect.setSource(QUrl::fromLocalFile("/home/pi/Desktop/game/resources/sounds/gun_shot.wav"));
    effect.setVolume(1.0f);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    timer_bird->start(10);
    bd1->show();
    bd2->show();
    bd3->show();
    xpos = 0;
    printf("Click\n");
}

void MainWindow::timer_bird_timeout()
{
    bd1->setLocation(xpos++, 100);
    bd2->setLocation(this->width() - xpos, 200);
    bd3->setLocation(xpos + 50, 300);
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    //printf("X: %d Y: %d\n",event->x(),event->y());
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    //printf("X: %d Y: %d\n",,event->y());
    //bd1->hideBird();

    if (bd1->contains(event->x() - bd1->pos().x(), event->y() - bd1->pos().y()))
    {
        QColor bd_color = bd1->getCurrentPixelColor(event->x() - bd1->pos().x(), event->y() - bd1->pos().y());
        if (bd_color.alpha() == 255)
        {
            bd1->hide();
        }
        printf("Alpha: %d\n", bd_color.alpha());
    }

    if (bd2->contains(event->x() - bd2->pos().x(), event->y() - bd2->pos().y()))
    {
        QColor bd_color = bd2->getCurrentPixelColor(event->x() - bd2->pos().x(), event->y() - bd2->pos().y());
        if (bd_color.alpha() == 255)
        {
            bd2->hide();
        }
        printf("Alpha: %d\n", bd_color.alpha());
    }

    if (bd3->contains(event->x() - bd3->pos().x(), event->y() - bd3->pos().y()))
    {
        QColor bd_color = bd3->getCurrentPixelColor(event->x() - bd3->pos().x(), event->y() - bd3->pos().y());
        if (bd_color.alpha() == 255)
        {
            bd3->hide();
        }
        printf("Alpha: %d\n", bd_color.alpha());
    }

    //    QColor bd_color = bd1->getCurrentPixelColor(event->x()-bd1->pos().x(),event->y()-bd1->pos().y());
    //    printf("A: %d R: %d G: %d B: %d\n",bd_color.alpha(),bd_color.red(),bd_color.green(),bd_color.blue());

    //    if(bd_color.alpha()<12){
    //        bd1->hideBird();
    //    }
    effect.play();
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
}

void MainWindow::resizeEvent(QResizeEvent *evt)
{
    QPixmap background_img("/home/pi/Desktop/game/resources/images/backgrounds/default_backgournd.jpg");

    QPalette mainwindow_palette;
    mainwindow_palette.setBrush(QPalette::Background, background_img.scaled(this->size(), Qt::IgnoreAspectRatio));
    this->setPalette(mainwindow_palette);
    //QMainWindow::resizeEvent(evt); //call base implementation
}