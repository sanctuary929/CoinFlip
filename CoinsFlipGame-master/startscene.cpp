#include "startscene.h"
#include "ui_startscene.h"

StartScene::StartScene(QWidget *parent) :
    Scene(parent), ui(new Ui::StartScene)
{
    ui->setupUi(this);

    SceneInit();
}

StartScene::~StartScene()
{
    delete ui;
}

void StartScene::AttributeSet(){
    setFixedSize(400, 600);
    setWindowTitle("Coins Flip Game");

    //Only make sence for window
    setWindowIcon(QIcon(":/res/Coin0001.png"));
}

void StartScene::MenuBarBuild(){
    //add menubar
    QMenuBar *menu_bar = menuBar();
    setMenuBar(menu_bar);

    //add menus
    QMenu *control_menu = new QMenu("control", menu_bar);
    QMenu *options_menu = new QMenu("options", menu_bar);
    menu_bar -> addMenu(control_menu);
    menu_bar -> addMenu(options_menu);

    //add actions in each menu
    QAction *restart_action = new QAction("restart");
    QAction *close_action = new QAction("close");
    QAction *expend_action = new QAction("expend");
    control_menu -> addAction(restart_action);
    control_menu -> addAction(close_action);
    options_menu -> addAction(expend_action);

    connect(close_action, &QAction::triggered, this, &QMainWindow::close);
}

void StartScene::WidgetsBuild(){
    //开始按钮
    WidgetButton *start_button = new WidgetButton(":/pictures/res/MenuSceneStartButton.png", "", this);
    start_button -> move(width() * 0.5 - start_button->width() * 0.5, height() * 0.7);

    connect(start_button, &WidgetButton::clicked, [=](){
        sound -> play();
        start_button -> ButtonBounce();
        QTimer::singleShot(500, this, [=]{
            //隐藏当前界面
            this -> hide();
            //显示选关界面
            //keep the scene position the same
            select_scene ->setGeometry(this -> geometry());
            select_scene -> show();
        });
    });

    //new select scene
    select_scene = new SelectScene(this);
    connect(select_scene, &SelectScene::BacktoStartScene, [=]{
       select_scene -> hide();
       //keep the position the same
       this -> setGeometry(select_scene -> geometry());
       this -> show();
    });
}

void StartScene::SoundsSet(){
    sound = new QSound(":/sounds/res/TapButtonSound.wav", this);
}

void StartScene::paintEvent(QPaintEvent *ev){
    //background scene
    //创建画家，指定绘图设备
    QPainter painter(this);
    //创建QPixmap对象
    QPixmap pix;
    //加载图片
    pix.load(":/pictures/res/PlayLevelSceneBg.png");
    //绘制背景图
    painter.drawPixmap(0, 0, width(), height(), pix);

    //add title
   //加载标题
    pix.load(":/pictures/res/Title.png");
    //绘制标题
    painter.drawPixmap(0, 0, pix.width(), pix.height(), pix);
}
