#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    QFontDatabase fontDB;
    fontDB.addApplicationFont("/home/pi/Desktop/qt-game/game/resources/VTKS_ANIMAL_2.ttf");

    ui->setupUi(this);
    this->setWindowState(Qt::WindowFullScreen);
    this->setMouseTracking(true);

    // bd1 = new bird(this, bird::fly_direction::right);
    // bd2 = new bird(this, bird::fly_direction::left);
    // bd3 = new bird(this, bird::fly_direction::right);
    timer_mousetracking = new QTimer(this);
    timer_targets = new QTimer(this);
    timer_game = new QTimer(this);
    timer_scoreboard_animation = new QTimer(this);
    timer_camera = new QTimer(this);

    connect(timer_mousetracking, SIGNAL(timeout()), this, SLOT(timer_mousetracking_timeout()));
    connect(timer_targets, SIGNAL(timeout()), this, SLOT(timer_targets_timeout()));
    connect(timer_game, SIGNAL(timeout()), this, SLOT(timer_game_timeout()));
    connect(timer_scoreboard_animation, SIGNAL(timeout()), this, SLOT(timer_scoreboard_animation_timeout()));
    connect(timer_camera, SIGNAL(timeout()), this, SLOT(timer_camera_timeout()));

    shoot_soundeffect.setSource(QUrl::fromLocalFile("/home/pi/Desktop/qt-game/game/resources/sounds/gun_shot.wav"));
    shoot_soundeffect.setVolume(1.0);

    bgm_menu_soundeffect.setSource(QUrl::fromLocalFile("/home/pi/Desktop/qt-game/game/resources/sounds/bgm_menu.wav"));
    bgm_menu_soundeffect.setVolume(0.7);
    bgm_menu_soundeffect.setLoopCount(QSoundEffect::Infinite);

    bgm_game_soundeffect.setSource(QUrl::fromLocalFile("/home/pi/Desktop/qt-game/game/resources/sounds/bgm_game.wav"));
    bgm_game_soundeffect.setVolume(0.7);
    bgm_game_soundeffect.setLoopCount(QSoundEffect::Infinite);

    gameover_soundeffect.setSource(QUrl::fromLocalFile("/home/pi/Desktop/qt-game/game/resources/sounds/gameover.wav"));
    gameover_soundeffect.setVolume(1.0);

    pick_soundeffect.setSource(QUrl::fromLocalFile("/home/pi/Desktop/qt-game/game/resources/sounds/pick.wav"));
    pick_soundeffect.setVolume(1.0);

    beep_soundeffect.setSource(QUrl::fromLocalFile("/home/pi/Desktop/qt-game/game/resources/sounds/beep.wav"));
    beep_soundeffect.setVolume(1.0);

    shutter_soundeffect.setSource(QUrl::fromLocalFile("/home/pi/Desktop/qt-game/game/resources/sounds/shutter.wav"));
    shutter_soundeffect.setVolume(1.0);

    camera_image = new QImage();

    bullethole_image = new QImage("/home/pi/Desktop/qt-game/game/resources/images/effects/bullethole.png");
    *bullethole_image = bullethole_image->scaled(QSize(16, 16), Qt::IgnoreAspectRatio);

    menu_image = new QImage("/home/pi/Desktop/qt-game/game/resources/images/menu.png");
    *menu_image = menu_image->scaled(QSize(menu_image->width() / 3, menu_image->height() / 3), Qt::KeepAspectRatio);

    gamemode_menu_image = new QImage("/home/pi/Desktop/qt-game/game/resources/images/gamemode_menu.png");
    *gamemode_menu_image = gamemode_menu_image->scaled(QSize(gamemode_menu_image->width() / 3, gamemode_menu_image->height() / 3), Qt::KeepAspectRatio);

    scoreboard_image = new QImage("/home/pi/Desktop/qt-game/game/resources/images/scoreboard.png");
    *scoreboard_image = scoreboard_image->scaled(QSize(scoreboard_image->width() * 0.4, scoreboard_image->height() * 0.4), Qt::KeepAspectRatio);

    picture_frame_image = new QImage("/home/pi/Desktop/qt-game/game/resources/images/picture_frame.png");
    *picture_frame_image = picture_frame_image->scaled(QSize(picture_frame_image->width() * 1.5, picture_frame_image->height() * 1.5), Qt::KeepAspectRatio);

    wood_button_image = new QImage("/home/pi/Desktop/qt-game/game/resources/images/wood_button.png");
    *wood_button_image = wood_button_image->scaled(QSize(wood_button_image->width() * 0.4, wood_button_image->height() * 0.4), Qt::KeepAspectRatio);

    teletubbies_image = new QImage("/home/pi/Desktop/qt-game/game/resources/images/teletubbies/all.png");
    *teletubbies_image = teletubbies_image->scaled(QSize(teletubbies_image->width() * 2.0, teletubbies_image->height() * 2.0), Qt::KeepAspectRatio);

    teletubbies_mask1_image = new QImage("/home/pi/Desktop/qt-game/game/resources/images/teletubbies/mask_1.png");
    *teletubbies_mask1_image = teletubbies_mask1_image->scaled(QSize(teletubbies_mask1_image->width() * 2.0, teletubbies_mask1_image->height() * 2.0), Qt::KeepAspectRatio);

    teletubbies_mask2_image = new QImage("/home/pi/Desktop/qt-game/game/resources/images/teletubbies/mask_2.png");
    *teletubbies_mask2_image = teletubbies_mask2_image->scaled(QSize(teletubbies_mask2_image->width() * 2.0, teletubbies_mask2_image->height() * 2.0), Qt::KeepAspectRatio);

    teletubbies_mask3_image = new QImage("/home/pi/Desktop/qt-game/game/resources/images/teletubbies/mask_3.png");
    *teletubbies_mask3_image = teletubbies_mask3_image->scaled(QSize(teletubbies_mask3_image->width() * 2.0, teletubbies_mask3_image->height() * 2.0), Qt::KeepAspectRatio);

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

void MainWindow::timer_mousetracking_timeout()
{
    QPoint mousePos = QCursor::pos();
    //    printf("X: %d Y: %d\n", mousePos.x(), mousePos.y());
    menu_status = 0x00;
    switch (gameState)
    {
    case None:
        break;
    case Settings:
        if (capture_btn.contains(mousePos.x(), mousePos.y()))
        {
            menu_status |= 0x10;
        }
        else if (cancel_btn.contains(mousePos.x(), mousePos.y()))
        {
            menu_status |= 0x20;
        }
        break;
    case Capture:
        if (OK_btn.contains(mousePos.x(), mousePos.y()))
        {
            menu_status |= 0x80;
        }
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
    case ModeSelect:
        if (easy_btn.contains(mousePos.x(), mousePos.y()))
        {
            menu_status |= 0x10;
        }
        else if (hard_btn.contains(mousePos.x(), mousePos.y()))
        {
            menu_status |= 0x20;
        }
        else if (return_btn.contains(mousePos.x(), mousePos.y()))
        {
            menu_status |= 0x40;
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

void MainWindow::timer_targets_timeout()
{
    for (int i = 0; i < birds.size(); i++)
    {
        birds.at(i)->move();
        if (birds.at(i)->isDead())
        {
            delete birds.at(i);
            birds.removeAt(i);
        }
    }
    if (gameTimeCount > gameStartDelayTime)
    {
        // printf("bird count = %d\n", birds.size());
        if (birds.size() <= 2)
        {
            spawnBirds(1);
        }
    }
}

void MainWindow::timer_game_timeout()
{
    gameTimeCount++;
    if (gameTimeCount == gameStartDelayTime)
    {
        spawnBirds(4);
    }
    // else if (gameTimeCount > gameStartDelayTime)
    // {
    //     printf("bird count = %d\n", birds.size());
    //     if (birds.size() <= 1)
    //     {
    //         spawnBirds(1);
    //     }
    // }

    updateTime();
    if (gameTimeCount >= gameTime)
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
    }
    else
    {
        scoreboard_ypos = 0;
        timer_scoreboard_animation->stop();
        timer_mousetracking->start(16);
    }
    repaint();
}

void MainWindow::timer_camera_timeout()
{
    Mat frame, frame_RGB, flipped;
    video_capture >> frame;
    if (frame.empty())
        return;

    cvtColor(frame, frame_RGB, CV_BGR2RGB);
    flip(frame_RGB, flipped, 1);
    *camera_image = QImage((uchar *)flipped.data, flipped.cols, flipped.rows, flipped.step, QImage::Format_RGB888).copy();
    //imshow("this is you, smile! :)", frame);

    // imshow("this is you, smile! :)", frame);

    // if (imwrite("test.jpg", frame))
    // {
    //     break;
    // }
    frame.release();
    frame_RGB.release();
    repaint();
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
    case Settings:
        if (capture_btn.contains(event->x(), event->y()))
        {
            if (captureFace())
            {
                gameCapture();
                return;
            }
        }
        else if (cancel_btn.contains(event->x(), event->y()))
        {
            gameMenu();
            return;
        }
        break;
    case Capture:
        if (OK_btn.contains(event->x(), event->y()))
        {
            gameMenu();
            video_capture.release();
            return;
        }
        break;
    case Menu:
        if (start_btn.contains(event->x(), event->y()))
        {
            gameModeSelect();
            return;
        }
        else if (settings_btn.contains(event->x(), event->y()))
        {
            gameSettings();
            return;
        }
        else if (exit_btn.contains(event->x(), event->y()))
        {
            QApplication::quit();
        }
        break;
    case ModeSelect:
        if (easy_btn.contains(event->x(), event->y()))
        {
            isEasyMode = true;
            gameStart();
            return;
        }
        else if (hard_btn.contains(event->x(), event->y()))
        {
            isEasyMode = false;
            gameStart();
            return;
        }
        else if (return_btn.contains(event->x(), event->y()))
        {
            gameMenu();
            return;
        }
        break;
    case Start:
        total_shots++;
        for (int i = 0; i < birds.size(); i++)
        {
            bird_shot_count += birds.at(i)->checkShot(event->pos());
            // if (birds.at(i)->checkShot(event->pos()))
            // {
            //     bird_shot_count++;
            // }
        }
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
        if (bulletholes.size() >= BULLETHOLES_MAX)
        {
            bulletholes.dequeue();
        }
        bulletholes.enqueue(QPoint(event->x() - bullethole_image->width() / 2, event->y() - bullethole_image->height() / 2));
        repaint();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *)
{
}

void MainWindow::resizeEvent(QResizeEvent *)
{
    updateBackgroundImage();
    bulletholes.clear();
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    switch (gameState)
    {
    case None:
        break;
    case Settings:
        painter.drawImage(QPoint(this->width() / 2 - camera_image->width() / 2, 80), *camera_image);
        painter.drawImage(QPoint(this->width() / 2 - picture_frame_image->width() / 2, 0), *picture_frame_image);
        painter.drawImage(this->width() / 2 - 250 - wood_button_image->width() / 2, 640, *wood_button_image);
        painter.drawImage(this->width() / 2 + 250 - wood_button_image->width() / 2, 640, *wood_button_image);
        painter.setFont(QFont("vtks animal 2", 42));
        painter.setPen(menu_status & 0x10 ? Qt::red : Qt::white);
        painter.drawText(capture_btn, Qt::AlignCenter, tr("Capture"));
        painter.setPen(menu_status & 0x20 ? Qt::red : Qt::white);
        painter.drawText(cancel_btn, Qt::AlignCenter, tr("Cancel"));

        break;
    case Capture:
        switch (face_images.size())
        {
        case 1:
            painter.drawImage(QPoint(this->width() / 2 - teletubbies_image->width() / 2, this->height() - teletubbies_image->height()), *teletubbies_mask1_image);
            break;
        case 2:
            painter.drawImage(QPoint(this->width() / 2 - teletubbies_image->width() / 2, this->height() - teletubbies_image->height()), *teletubbies_mask2_image);
            break;
        case 3:
            painter.drawImage(QPoint(this->width() / 2 - teletubbies_image->width() / 2, this->height() - teletubbies_image->height()), *teletubbies_mask3_image);
            break;
        case 4:
            painter.drawImage(QPoint(this->width() / 2 - teletubbies_image->width() / 2, this->height() - teletubbies_image->height()), *teletubbies_image);
            break;
        }
        for (int i = 0; i < face_images.size(); i++)
        {
            // painter.drawImage(face_draw_points[i], *face_images[i]);
            painter.drawImage(QPoint(face_draw_rects[i].x(), face_draw_rects[i].y()), face_images[i]->scaled(QSize(face_draw_rects[i].width(), face_draw_rects[i].height()), Qt::IgnoreAspectRatio));
        }
        painter.drawImage(this->width() / 2 - wood_button_image->width() / 2, 50, *wood_button_image);
        painter.setFont(QFont("vtks animal 2", 42));
        painter.setPen(menu_status & 0x80 ? Qt::red : Qt::white);
        painter.drawText(OK_btn, Qt::AlignCenter, tr("OK"));
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
    case ModeSelect:
        painter.drawImage(QPoint(this->width() / 2 - menu_image->width() / 2, this->height() / 2 - menu_image->height() / 2), *gamemode_menu_image);
        painter.setFont(QFont("vtks animal 2", 48));
        painter.setPen(menu_status & 0x10 ? Qt::red : Qt::white);
        painter.drawText(easy_btn, Qt::AlignCenter, tr("EASY"));
        painter.setPen(menu_status & 0x20 ? Qt::red : Qt::white);
        painter.drawText(hard_btn, Qt::AlignCenter, tr("HARD"));
        painter.setPen(menu_status & 0x40 ? Qt::red : Qt::white);
        painter.drawText(return_btn, Qt::AlignCenter, tr("RETURN"));
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
        printf("Game Mode changed\n");
        bulletholes.clear();
        updateBackgroundImage();
        gameState_prev = gameState;
    }
    //painter.drawImage(QPoint(0, 0), *bullethole_canvas);
    for (int i = 0; i < bulletholes.size(); i++)
    {
        painter.drawImage(bulletholes.at(i), *bullethole_image);
    }
}

void MainWindow::updateBackgroundImage()
{
    switch (gameState)
    {
    case None:
        break;
    case Settings:
        background_img = new QPixmap("/home/pi/Desktop/qt-game/game/resources/images/backgrounds/blurred_backgournd.jpg");
        break;
    case Capture:
        background_img = new QPixmap("/home/pi/Desktop/qt-game/game/resources/images/backgrounds/blurred_backgournd.jpg");
        break;
    case Menu:
        background_img = new QPixmap("/home/pi/Desktop/qt-game/game/resources/images/backgrounds/blurred_backgournd.jpg");
        break;
    case ModeSelect:
        background_img = new QPixmap("/home/pi/Desktop/qt-game/game/resources/images/backgrounds/blurred_backgournd.jpg");
        break;
    case Start:
        background_img = new QPixmap("/home/pi/Desktop/qt-game/game/resources/images/backgrounds/default_backgournd.jpg");
        break;
    case Over:
        background_img = new QPixmap("/home/pi/Desktop/qt-game/game/resources/images/backgrounds/blurred_backgournd.jpg");
        break;
    }
    QPalette mainwindow_palette;
    mainwindow_palette.setBrush(QPalette::Background, background_img->scaled(this->size(), Qt::IgnoreAspectRatio));
    this->setPalette(mainwindow_palette);
}

void MainWindow::updateTime()
{
    QString str;
    str.sprintf("TIME %d:%02d", (gameTime - gameTimeCount) / 60, (gameTime - gameTimeCount) % 60);
    if ((gameTime - gameTimeCount) <= 10)
    {
        beep_soundeffect.play();
        time_label->setStyleSheet("QLabel { color : red; }");
    }
    else
    {
        time_label->setStyleSheet("QLabel { color : white; }");
    }
    time_label->setText(str);
}

void MainWindow::gameModeSelect()
{
    gameState = ModeSelect;
    repaint();
}

void MainWindow::gameSettings()
{
    gameState = Settings;
    repaint();

    if (!video_capture.open(0))
    {
        gameMenu();
        return;
    }

    printf("Camera open success!\n");
    timer_camera->start(33);
}

void MainWindow::gameCapture()
{
    gameState = Capture;
    repaint();
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

    //birds = QList<bird *>();
    score = 0;
    bird_shots = 0;
    total_shots = 0;
    scoreboard->setText(QString::number(score));

    gameTimeCount = 0;
    updateTime();
    timer_game->start(1000);
    timer_targets->start(16);
}

void MainWindow::gameOver()
{
    gameState = Over;
    timer_game->stop();
    timer_targets->stop();
    bgm_game_soundeffect.stop();
    gameover_soundeffect.play();

    for (int i = 0; i < birds.size(); i++)
    {
        delete birds.at(i);
    }
    birds.clear();

    scoreboard_blureffect->setBlurRadius(5);
    time_label_blureffect->setBlurRadius(5);

    accuarcy = (float)bird_shots / (float)total_shots;

    scoreboard_ypos = -scoreboard_image->height();
    timer_mousetracking->stop();
    timer_scoreboard_animation->start(10);
    repaint();
}

void MainWindow::gameMenu()
{
    gameState = Menu;
    if (gameState_prev == Over)
    {
        bgm_menu_soundeffect.play();
    }

    scoreboard->setVisible(false);
    time_label->setVisible(false);

    repaint();
}

void MainWindow::spawnBirds(int num)
{
    for (int i = 0; i < num; i++)
    {
        int direction = QRandomGenerator::global()->generate() & 0x01;
        bird *bd;
        if (direction)
        {
            bd = new bird(this, bird::fly_direction::right);
            bd->setLocation(0 - bd->width() / 2, QRandomGenerator::global()->bounded(100, 500));
            bd->setXIncrement(QRandomGenerator::global()->bounded(2, 15));
        }
        else
        {
            bd = new bird(this, bird::fly_direction::left);
            bd->setLocation(this->width() + bd->width() / 2, QRandomGenerator::global()->bounded(100, 500));
            bd->setXIncrement(-QRandomGenerator::global()->bounded(2, 15));
        }
        bd->setEasyShot(isEasyMode);
        bd->show();
        birds.append(bd);
    }

    for (int i = 0; i < face_images.size(); i++)
    {
        person *p = new person(this, i, *face_images[i]);
        p->setLocation(this->width() / 2, QRandomGenerator::global()->bounded(100, 500));
        //p->setXIncrement(QRandomGenerator::global()->bounded(2, 15));
        p->show();
        people.append(p);
    }

    // birds.append(new bird(this, bird::fly_direction::right));
    // birds.append(new bird(this, bird::fly_direction::left));
    // birds.append(new bird(this, bird::fly_direction::right));

    // birds.at(0)->setLocation(0 - birds.at(0)->width() / 2, 100);
    // birds.at(1)->setLocation(this->width() + birds.at(0)->width() / 2, 200);
    // birds.at(2)->setLocation(0 - birds.at(0)->width() / 2, 300);
    // birds.at(0)->setXIncrement(1);
    // birds.at(1)->setXIncrement(-1);
    // birds.at(2)->setXIncrement(5);
    // for (int i = 0; i < birds.size(); i++)
    // {
    //     birds.at(i)->show();
    // }
}

bool MainWindow::captureFace()
{
    // Mat image;
    Mat frame, frame_RGB, flipped, original_flipped;
    video_capture >> frame;
    if (!frame.empty())
    {
        //timer_camera->stop();
        cvtColor(frame, frame_RGB, CV_BGR2RGB);
        flip(frame_RGB, flipped, 1);
        *camera_image = QImage((uchar *)flipped.data, flipped.cols, flipped.rows, flipped.step, QImage::Format_RGB888).copy();
        repaint();
        shutter_soundeffect.play();
        flip(frame, original_flipped, 1);
        imwrite("capture.jpg", original_flipped);
        system("python3 face_detection.py");
    }
    int face_count = 0;
    face_images.clear();
    QFile inputFile("./faces/count");
    if (inputFile.open(QIODevice::ReadOnly))
    {
        printf("File open success!\n");
        QTextStream in(&inputFile);
        while (!in.atEnd())
        {
            QString line = in.readLine();
            face_count = line.toInt();
        }
        inputFile.close();
    }
    printf("Face Count = %d\n", face_count);

    if (face_count > 0)
    {
        face_images.clear();
        for (int i = 0; (i < face_count) && (i < 4); i++)
        {
            QString fileName;
            fileName.sprintf("./faces/face%d.jpg", i);

            QImage *image = new QImage(fileName);
            //*image = image->scaled(QSize(80, 80), Qt::KeepAspectRatio);
            //*image = image->scaledToWidth(80);
            face_images.append(image);
        }
        // for (int i = 0; i < face_images.size(); i++)
        // {
        //     QLabel *label = new QLabel(this);
        //     label->setPixmap(QPixmap::fromImage(*face_images[i]));
        //     label->setGeometry(i * 300, 500, face_images[i]->width(), face_images[i]->height());
        // }
        return true;
    }

    return false;
}