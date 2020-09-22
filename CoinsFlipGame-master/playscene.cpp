#include "playscene.h"

PlayScene::PlayScene(QWidget *parent) : Scene(parent)
{
    SceneInit();
}

void PlayScene::AttributeSet(){
    setFixedSize(400, 600);
    //设置按钮的尺寸
    setWindowTitle("Coins Flip Game");
    //设置界面的标题
    setWindowIcon(QIcon(":/res/Coin0001.png"));
}

void PlayScene::MenuBarBuild(){

    QMenuBar *menu_bar = menuBar();
    //添加菜单条
    setMenuBar(menu_bar);
    //添加菜单
    QMenu *control_menu = new QMenu("control", menu_bar);
    //添加左上角的选项“control”
    QMenu *options_menu = new QMenu("options", menu_bar);
    //添加左上角的选项“options”
    menu_bar -> addMenu(control_menu);
    menu_bar -> addMenu(options_menu);

    QAction *restart_action = new QAction("restart");
    QAction *close_action = new QAction("close");
    QAction *expend_action = new QAction("expend");
    control_menu -> addAction(restart_action);
    control_menu -> addAction(close_action);
    options_menu -> addAction(expend_action);
    //在每个菜单选项中添加动作
    //“control”菜单里加入了“restart”重新开始和“close”关闭两个选项
    //“options”菜单里加入了“expend”花费一个选项

    connect(restart_action, &QAction::triggered, [=](){LevelSet(cur_level);});
    //点击“restart”选项将清空数据重新开始当前关卡
    connect(close_action, &QAction::triggered, this, &QMainWindow::close);
    //点击“close”选项将关闭当前窗口
}

void PlayScene::WidgetsBuild(){
    WidgetButton *back_button = new WidgetButton(":/pictures/res/BackButton.png", ":/pictures/res/BackButtonSelected.png", this);
    //设置“返回”按钮
    back_button -> move(width() - back_button->width(), height() - back_button->height());
    connect(back_button, &WidgetButton::clicked, [=]{
        back_sound -> play();
        //播放音效
        QTimer::singleShot(500, this, [=]{
            emit BacktoSelectScene();
            //跳转界面
        });
    });

    QFont font;
    font.setPointSize(16);
    level_str = new QLabel(this);
    level_str -> setFont(font);
    level_str -> setGeometry(20, height() -50, 200, 50);
    //设置当前关卡标题

    win_button = new WidgetButton(":/pictures/res/LevelCompletedDialogBg.png", "", this);
    connect(win_button, &WidgetButton::clicked, [=](){
        next_sound -> play();
        win_button -> ButtonBounce();
        QTimer::singleShot(500, this, [=](){
            this -> LevelSet(cur_level +1);
        });
        //延时0.5秒进入下一关
    });
    //象征“通关”的图片掉落
    //点击“通关”的按钮即可进入下一关

    for (int i=0; i < 4; i++){
        for (int j =0; j < 4; j++){
            QLabel *coin_background = new QLabel(this);
            coin_background -> setGeometry(100 + (i %4) *50, 200 + (j %4) *50, 50, 50);
            coin_background -> setPixmap(QPixmap(":/pictures/res/BoardNode.png"));

            GameCoin *coin = new GameCoin(":/pictures/res/Coin0001.png", this);
            coin -> move(102 + (i %4) *50, 202 + (j %4) *50);
            //设置初始界面金币的背景图片

            connect(coin, &GameCoin::clicked, [=](){
                if (!coin -> isforbidden){
                    flip_sound -> play();
                    coin -> CoinFlip();
                    coins_flag[i][j] = !coins_flag[i][j];
                    //如果金币可以翻转

                    QTimer::singleShot(300, this, [=]{
                        if (i +1 < 4){
                            coins[i +1][j] -> CoinFlip();
                            coins_flag[i +1][j] = !coins_flag[i +1][j];
                        }
                        //判断下侧金币是否翻转
                        if (i -1 >= 0){
                            coins[i -1][j] -> CoinFlip();
                            coins_flag[i -1][j] = !coins_flag[i -1][j];
                        }
                        //判断上侧金币是否翻转
                        if (j +1 < 4){
                            coins[i][j +1] -> CoinFlip();
                            coins_flag[i][j +1] = !coins_flag[i][j +1];
                        }
                        //判断右侧金币是否翻转
                        if (j -1 >= 0){
                            coins[i][j -1] -> CoinFlip();
                            coins_flag[i][j -1] = !coins_flag[i][j -1];
                        }
                        //判断左侧金币是否翻转

                        bool iswin = true;
                        for (int i =0; i <4; i++){
                            for (int j =0; j <4; j++){
                                if (coins_flag[i][j] == false){
                                    iswin = false;
                                    break;
                                }
                                //在4X4的数组里判断是否十六个数据皆为true 若有一个为false则无法判断为“胜利”
                            }
                        }
                        if (iswin){
                            win_sound -> play();
                            QPropertyAnimation * animation = new QPropertyAnimation(win_button, "geometry", this);
                            animation->setDuration(1000);
                            animation->setStartValue(QRect(win_button->x(), win_button->y(), win_button->width(), win_button->height()));
                            animation->setEndValue(QRect(win_button->x(), win_button->y() + 200, win_button->width(), win_button->height()));
                            animation->setEasingCurve(QEasingCurve::OutBounce);
                            animation->start();
                            //弹出“胜利”的图片，并响起背景音乐

                            for (int i =0; i <4; i++)
                                for (int j =0; j <4; j++)
                                    coins[i][j] -> isforbidden = true;
                            //将所有金币按钮的禁止使用标志改为true，如果再次点击，直接return
                        }
                    });
                }
            });

            coins[i][j] = coin;
            //记录每个按钮的位置
        }
    }
}

void PlayScene::SoundsSet(){
    next_sound = new QSound(":/sounds/res/TapButtonSound.wav", this);
    back_sound = new QSound(":/sounds/res/BackButtonSound.wav", this);
    flip_sound = new QSound(":/sounds/res/ConFlipSound.wav", this);
    win_sound = new QSound(":/sounds/res/LevelWinSound.wav", this);
    //设置音效
}

void PlayScene::LevelSet(int level){
    cur_level = level;
    win_button -> move((width() - win_button->width()) * 0.5,  -win_button->height());
    //重置“胜利”标签
    level_str -> setText(QString("Now %1 level").arg(level));
    //关卡展示
    for (int i =0; i < 4; i++){
        for (int j =0; j < 4; j++){
            coins[i][j] -> CoinInit(config[level][i][j]);
            coins_flag[i][j] = config[level][i][j];
        }
    }

    for (int i =0; i <4; i++)
        for (int j =0; j <4; j++)
            coins[i][j] -> isforbidden = false;
    //启动投币按钮
}

void PlayScene::paintEvent(QPaintEvent *ev){
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/pictures/res/PlayLevelSceneBg.png");
    painter.drawPixmap(0, 0, width(), height(), pix);

    //设置背景图片

    pix.load(":/pictures/res/Title.png");
    painter.drawPixmap(0, 0, pix.width(), pix.height(), pix);

    //设置标题
}

