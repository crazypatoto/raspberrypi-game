#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    QFontDatabase fontDB;
    fontDB.addApplicationFont("/home/pi/Desktop/game/resources/VTKS_ANIMAL_2.ttf");

    ui->setupUi(this);
    this->setWindowState(Qt::WindowFullScreen);
    this->setMouseTracking(true);

    // bd1 = new bird(this, bird::fly_direction::right);
    // bd2 = new bird(this, bird::fly_direction::left);
    // bd3 = new bird(this, bird::fly_direction::right);
    timer_mousetracking = new QTimer(this);
    timer_game = new QTimer(this);
    timer_scoreboard_animation = new QTimer(this);

    connect(timer_mousetracking, SIGNAL(timeout()), this, SLOT(timer_mousetracking_timeout()));
    connect(timer_game, SIGNAL(timeout()), this, SLOT(timer_game_timeout()));
    connect(timer_scoreboard_animation, SIGNAL(timeout()), this, SLOT(timer_scoreboard_animation_timeout()));

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

    scoreboard_blureffect = new QGraphicsBlurEffect(this);
    time_label_blureffect = new QGraphicsBlurEffect(this);

    scoreboard_blureffect->setBlurRadius(0);
    time_label_blureffect->setBlurRadius(0);

    scoreboard->setGraphicsEffect(scoreboard_blureffect);
    time_label->setGraphicsEffect(time_label_blureffect);

    timer_mousetracking->start(16);
    bgm_menu_soundeffect.play();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    // bd1->show();
    // bd2->show();
    // bd3->show();
    // xpos = -100;
}

void MainWindow::timer_mousetracking_timeout()
{
    QPoint mousePos = QCursor::pos();
    //    printf("X: %d Y: %d\n", mousePos.x(), mousePos.y());
    menu_status = 0x00;
    switch (gameState)
    {
    case None:
        break;
    case Menu:
        if (start_btn.contains(mousePos.x(), mousePos.y()))
        {
            menu_status |= 0x01;
        }
        else if (settings_btn.contains(mousePos.x(), mousePos.y()))
        {
            menu_status |= 0x02;
        }
        else if (exit_btn.contains(mousePos.x(), mousePos.y()))
        {
            menu_status |= 0x04;
        }
        break;
    case Start:
        break;
    case Over:
        if (continue_btn.contains(mousePos.x(), mousePos.y()))
        {
            menu_status |= 0x08;
        }
        break;
    }
    if (menu_status != menu_status_prev)
    {
        if (menu_status)
        {
            pick_soundeffect.play();
        }
        repaint();
        menu_status_prev = menu_status;
    }
}

void MainWindow::timer_game_timeout()
{
    gameTimeCount_10ms++;
    if (gameTimeCount_10ms == 300)
    {
        for (int i = 0; i < birds.size(); i++)
        {
            birds.at(i)->show();
        }
        // bd1->show();
        // bd2->show();
        // bd3->show();
        xpos = -100;
    }
    birds.at(0)->setLocation(xpos+=10, 100);
    birds.at(1)->setLocation(this->width() - xpos, 200);
    birds.at(2)->setLocation(xpos + 50, 300);
    if(xpos > this->width()){
        xpos = -100;
    }

    if (gameTimeCount_10ms % 100 == 0)
    {
        QString str;
        str.sprintf("TIME %d:%02d", ((gameTime_10ms - gameTimeCount_10ms) / 100) / 60, ((gameTime_10ms - gameTimeCount_10ms) / 100) % 60);
        if ((60000 - gameTimeCount_10ms) <= 10)
        {
            time_label->setStyleSheet("QLabel { color : red; }");
        }
        else
        {
            time_label->setStyleSheet("QLabel { color : white; }");
        }
        time_label->setText(str);
    }
    if (gameTimeCount_10ms == 6000)
    {
        gameOver();
    }
}

void MainWindow::timer_scoreboard_animation_timeout()
{
    if (scoreboard_ypos < 0)
    {
        // scoreboard_ypos += scoreboard_image->height() / 75;
        scoreboard_ypos += 10;
        repaint();
    }
    else
    {
        scoreboard_ypos = 0;
        repaint();
        timer_scoreboard_animation->stop();
        timer_mousetracking->start(16);
    }
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
        total_shots++;
        for (int i = 0; i < birds.size(); i++)
        {
            if (birds.at(i)->checkShot(event->pos()))
            {
                bird_shot_count++;
                birds.at(i)->die();
            }
        }
        // if (bd1->checkShot(event->pos()))
        // {
        //     bird_shot_count++;
        //     bd1->die();
        // }
        // if (bd2->checkShot(event->pos()))
        // {
        //     bird_shot_count++;
        //     bd2->die();
        // }
        // if (bd3->checkShot(event->pos()))
        // {
        //     bird_shot_count++;
        //     bd3->die();
        // }
        score += bird_shot_count * 100;
        bird_shots += bird_shot_count;
        scoreboard->setText(QString::number(score));
        break;
    case Over:
        if (continue_btn.contains(event->x(), event->y()))
        {
            gameMenu();
            return;
        }
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
        painter.drawImage(QPoint(this->width() / 2 - scoreboard_image->width() / 2, scoreboard_ypos), *scoreboard_image);

        QString score_str, shots_str, accuarcy_str;
        score_str.sprintf("Score: %d", score);
        shots_str.sprintf("%3d Birds / %3d shots", bird_shots, total_shots);
        accuarcy_str.sprintf("Accuarcy: %2.2f%%", accuarcy * 100);

        painter.setFont(QFont("vtks animal 2", 24));
        painter.setPen(Qt::white);
        painter.drawText(QRect(510, 180 + scoreboard_ypos, 269, 72), Qt::AlignCenter, score_str);

        painter.setFont(QFont("vtks animal 2", 14));
        painter.setPen(Qt::white);
        painter.drawText(QRect(510, 250 + scoreboard_ypos, 269, 44), Qt::AlignCenter, shots_str);

        painter.setFont(QFont("vtks animal 2", 16));
        painter.setPen(Qt::white);
        painter.drawText(QRect(540, 312 + scoreboard_ypos, 219, 31), Qt::AlignCenter, accuarcy_str);

        painter.setFont(QFont("vtks animal 2", 36));
        painter.setPen(menu_status & 0x08 ? Qt::red : Qt::white);
        painter.drawText(QRect(continue_btn.x(), continue_btn.y() + scoreboard_ypos, continue_btn.width(), continue_btn.height()), Qt::AlignCenter, tr("Continue"));
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
        background_img = new QPixmap("/home/pi/Desktop/game/resources/images/backgrounds/blurred_backgournd.jpg");
        break;
    }
    QPalette mainwindow_palette;
    mainwindow_palette.setBrush(QPalette::Background, background_img->scaled(this->size(), Qt::IgnoreAspectRatio));
    this->setPalette(mainwindow_palette);
}

void MainWindow::gameStart()
{
    gameState = Start;
    repaint();

    bgm_menu_soundeffect.stop();
    bgm_game_soundeffect.play();

    scoreboard_blureffect->setBlurRadius(0);
    time_label_blureffect->setBlurRadius(0);
    scoreboard->setVisible(true);
    time_label->setVisible(true);

    score = 0;
    bird_shots = 0;
    total_shots = 0;
    scoreboard->setText(QString::number(score));

    birds.clear();
    birds.append(new bird(this, bird::fly_direction::right));
    birds.append(new bird(this, bird::fly_direction::left));
    birds.append(new bird(this, bird::fly_direction::right));

    gameTimeCount_10ms = 0;
    timer_game->start(1);
}

void MainWindow::gameOver()
{
    gameState = Over;
    bgm_game_soundeffect.stop();
    gameover_soundeffect.play();

    scoreboard_blureffect->setBlurRadius(5);
    time_label_blureffect->setBlurRadius(5);
    timer_game->stop();

    accuarcy = (float)bird_shots / (float)total_shots;

    scoreboard_ypos = -scoreboard_image->height();
    timer_mousetracking->stop();
    timer_scoreboard_animation->start(16);
    repaint();
}

void MainWindow::gameMenu()
{
    gameState = Menu;
    bgm_menu_soundeffect.play();

    scoreboard->setVisible(false);
    time_label->setVisible(false);

    repaint();
}