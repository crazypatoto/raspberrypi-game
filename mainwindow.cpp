#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    QFontDatabase fontDB;
    fontDB.addApplicationFont("/home/pi/Desktop/game/resources/VTKS_ANIMAL_2.ttf");

    ui->setupUi(this);
    this->setWindowState(Qt::WindowFullScreen);
    this->setMouseTracking(true);

    bd1 = new bird(this, bird::fly_direction::right);
    bd2 = new bird(this, bird::fly_direction::left);
    bd3 = new bird(this, bird::fly_direction::right);
    timer_bird = new QTimer(this);
    timer_mousetracking = new QTimer(this);
    timer_game = new QTimer(this);
    timer_scoreboard = new QTimer(this);

    connect(timer_bird, SIGNAL(timeout()), this, SLOT(timer_bird_timeout()));
    connect(timer_mousetracking, SIGNAL(timeout()), this, SLOT(timer_mousetracking_timeout()));
    connect(timer_game, SIGNAL(timeout()), this, SLOT(timer_game_timeout()));
    connect(timer_scoreboard, SIGNAL(timeout()), this, SLOT(timer_scoreboard_timeout()));

    shoot_soundeffect.setSource(QUrl::fromLocalFile("/home/pi/Desktop/game/resources/sounds/gun_shot.wav"));
    shoot_soundeffect.setVolume(1.0);

    bgm_menu_soundeffect.setSource(QUrl::fromLocalFile("/home/pi/Desktop/game/resources/sounds/bgm_menu.wav"));
    bgm_menu_soundeffect.setVolume(0.7);
    bgm_menu_soundeffect.setLoopCount(QSoundEffect::Infinite);

    bgm_game_soundeffect.setSource(QUrl::fromLocalFile("/home/pi/Desktop/game/resources/sounds/bgm_game.wav"));
    bgm_game_soundeffect.setVolume(0.7);
    bgm_game_soundeffect.setLoopCount(QSoundEffect::Infinite);

    gameover_soundeffect.setSource(QUrl::fromLocalFile("/home/pi/Desktop/game/resources/sounds/gameover.wav"));
    gameover_soundeffect.setVolume(1.0);

    pick_soundeffect.setSource(QUrl::fromLocalFile("/home/pi/Desktop/game/resources/sounds/pick.wav"));
    pick_soundeffect.setVolume(1.0);

    bullethole_image = new QImage("/home/pi/Desktop/game/resources/images/effects/bullethole.png");
    *bullethole_image = bullethole_image->scaled(QSize(16, 16), Qt::IgnoreAspectRatio);

    menu_image = new QImage("/home/pi/Desktop/game/resources/images/menu.png");
    *menu_image = menu_image->scaled(QSize(menu_image->width() / 3, menu_image->height() / 3), Qt::KeepAspectRatio);

    scoreboard_image = new QImage("/home/pi/Desktop/game/resources/images/scoreboard.png");
    *scoreboard_image = scoreboard_image->scaled(QSize(scoreboard_image->width() * 0.4, scoreboard_image->height() * 0.4), Qt::KeepAspectRatio);
    //menu_image = new QImage("/home/pi/Desktop/game/resources/images/woodsign.png");
    scoreboard = new QLabel(this);
    scoreboard->setGeometry(791, 530, 160, 59);
    scoreboard->setFont(QFont("vtks animal 2", 30));
    scoreboard->setAlignment(Qt::AlignCenter);
    scoreboard->setStyleSheet("QLabel { color : white; }");
    scoreboard->setText("0");
    scoreboard->setVisible(false);

    time_label = new QLabel(this);
    time_label->setGeometry(10, 755, 300, 150);
    time_label->setFont(QFont("vtks animal 2", 30));
    time_label->setAlignment(Qt::AlignLeft);
    time_label->setStyleSheet("QLabel { color : white; }");
    time_label->setText("TIME 1:00");
    time_label->setVisible(false);
    bgm_menu_soundeffect.play();

    timer_mousetracking->start(16);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    bd1->show();
    bd2->show();
    bd3->show();
    xpos = -100;
}

void MainWindow::timer_bird_timeout()
{
    static unsigned long timeout_count = 0;
    if (timeout_count++ == 300)
    {
        bd1->show();
        bd2->show();
        bd3->show();
        xpos = -100;
    }
    bd1->setLocation(xpos += 2, 100);
    bd2->setLocation(this->width() - xpos, 200);
    bd3->setLocation(xpos + 50, 300);
}

void MainWindow::timer_mousetracking_timeout()
{
    QPoint mousePos = QCursor::pos();
    //    printf("X: %d Y: %d\n", mousePos.x(), mousePos.y());

    switch (gameState)
    {
    case None:
        break;
    case Menu:
        if (start_btn.contains(mousePos.x(), mousePos.y()))
        {
            menu_status = 0x01;
        }
        else if (settings_btn.contains(mousePos.x(), mousePos.y()))
        {
            menu_status = 0x02;
        }
        else if (exit_btn.contains(mousePos.x(), mousePos.y()))
        {
            menu_status = 0x04;
        }
        else
        {
            menu_status = 0x00;
        }
        if (menu_status != menu_status_prev)
        {
            if (menu_status > 0)
            {
                pick_soundeffect.play();
            }
            repaint();
            menu_status_prev = menu_status;
        }
        break;
    case Start:
        break;
    case Over:
        //scoreboard->setText("");
        break;
    }
}

void MainWindow::timer_game_timeout()
{
    if (gameState == Start)
    {
        gameTime--;
        QString str;
        str.sprintf("TIME %d:%02d", gameTime / 60, gameTime % 60);
        if (gameTime <= 10)
        {
            time_label->setStyleSheet("QLabel { color : red; }");
        }
        else
        {
            time_label->setStyleSheet("QLabel { color : white; }");
        }
        time_label->setText(str);
        if (gameTime == 0)
        {
            gameOver();
        }
    }
}

void MainWindow::timer_scoreboard_timeout()
{
    
}

void MainWindow::mouseMoveEvent(QMouseEvent *)
{
    //printf("X: %d Y: %d\n", event->x(), event->y());
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    int bird_shot_count = 0;
    shoot_soundeffect.play();
    printf("(%d,%d)\n", event->x(), event->y());

    switch (gameState)
    {
    case None:
        break;
    case Menu:
        if (start_btn.contains(event->x(), event->y()))
        {
            gameStart();
            return;
        }
        else if (settings_btn.contains(event->x(), event->y()))
        {
        }
        else if (exit_btn.contains(event->x(), event->y()))
        {
            QApplication::quit();
        }
        break;
    case Start:
        if (bd1->checkShot(event->pos()))
        {
            bird_shot_count++;
            bd1->die();
        }
        if (bd2->checkShot(event->pos()))
        {
            bird_shot_count++;
            bd2->die();
        }
        if (bd3->checkShot(event->pos()))
        {
            bird_shot_count++;
            bd3->die();
        }
        score += bird_shot_count * 100;
        scoreboard->setText(QString::number(score));
        break;
    case Over:
        //scoreboard->setText("");
        break;
    }

    //    QColor bd_color = bd1->getCurrentPixelColor(event->x()-bd1->pos().x(),event->y()-bd1->pos().y());
    //    printf("A: %d R: %d G: %d B: %d\n",bd_color.alpha(),bd_color.red(),bd_color.green(),bd_color.blue());

    //    if(bd_color.alpha()<12){
    //        bd1->hideBird();
    //    }

    if (bird_shot_count == 0)
    {
        QPainter *bullethole_painter = new QPainter(bullethole_canvas);
        bullethole_painter->drawImage(QPoint(event->x() - bullethole_image->width() / 2, event->y() - bullethole_image->height() / 2), *bullethole_image);
        bullethole_painter->end();
        repaint();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *)
{
}

void MainWindow::resizeEvent(QResizeEvent *)
{
    updateBackgroundImage();
    bullethole_canvas = new QImage(this->size(), QImage::Format_ARGB32);
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    switch (gameState)
    {
    case None:
        break;
    case Menu:
        painter.drawImage(QPoint(this->width() / 2 - menu_image->width() / 2, this->height() / 2 - menu_image->height() / 2), *menu_image);
        painter.setFont(QFont("vtks animal 2", 48));

        painter.setPen(menu_status & 0x01 ? Qt::red : Qt::white);
        painter.drawText(start_btn, Qt::AlignCenter, tr("Start"));
        painter.setPen(menu_status & 0x02 ? Qt::red : Qt::white);
        painter.drawText(settings_btn, Qt::AlignCenter, tr("Settings"));
        painter.setPen(menu_status & 0x04 ? Qt::red : Qt::white);
        painter.drawText(exit_btn, Qt::AlignCenter, tr("Exit"));
        break;
    case Start:
        break;
    case Over:
        painter.drawImage(QPoint(this->width() / 2 - scoreboard_image->width() / 2, this->height() / 2 - scoreboard_image->height() / 2), *scoreboard_image);
        break;
    }
    if (gameState != gameState_prev)
    {
        delete bullethole_canvas;
        updateBackgroundImage();
        bullethole_canvas = new QImage(this->size(), QImage::Format_ARGB32);
        gameState_prev = gameState;
    }

    painter.drawImage(QPoint(0, 0), *bullethole_canvas);
}

void MainWindow::updateBackgroundImage()
{
    switch (gameState)
    {
    case None:
        break;
    case Menu:
        background_img = new QPixmap("/home/pi/Desktop/game/resources/images/backgrounds/blurred_backgournd.jpg");
        break;
    case Start:
        background_img = new QPixmap("/home/pi/Desktop/game/resources/images/backgrounds/default_backgournd.jpg");
        break;
    case Over:
        background_img = new QPixmap("/home/pi/Desktop/game/resources/images/backgrounds/default_backgournd.jpg");
        break;
    }
    QPalette mainwindow_palette;
    mainwindow_palette.setBrush(QPalette::Background, background_img->scaled(this->size(), Qt::IgnoreAspectRatio));
    this->setPalette(mainwindow_palette);
}

void MainWindow::gameStart()
{
    gameState = Start;
    bgm_menu_soundeffect.stop();

    repaint();
    bgm_game_soundeffect.play();
    scoreboard->setVisible(true);
    time_label->setVisible(true);
    timer_bird->start(10);
    timer_game->start(100);
    gameTime = 60;
}

void MainWindow::gameOver()
{
    gameState = Over;
    bgm_game_soundeffect.stop();
    gameover_soundeffect.play();

    repaint();
    //scoreboard->setVisible(false);
    //time_label->setVisible(true);
    timer_bird->stop();
    timer_game->stop();
}