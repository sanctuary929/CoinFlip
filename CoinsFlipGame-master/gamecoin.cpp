#include "gamecoin.h"

GameCoin::GameCoin(QString img, QWidget *parent) : QPushButton(parent), isfront(true), timer(new QTimer(this))
{
    QPixmap pix(img);

    setFixedSize(pix.width(), pix.height());
    //设置按钮的尺寸
    setStyleSheet("QPushButton{border:0px;}");
    //设置边框样式
    setIcon(pix);
    //设置按钮图标
    setIconSize(QSize(width(), height()));
    //设置图标大小

    connect(timer, &QTimer::timeout, this, &GameCoin::BeingFlip);
}

void GameCoin::CoinInit(bool flag){

    isfront = flag;
    if (flag){
        QPixmap pix(":/pictures/res/Coin0001.png");
        setIcon(pix);
    }
    else{
        QPixmap pix(":/pictures/res/Coin0008.png");
        setIcon(pix);
    }
//通过“1”和“2”判断金币正反面
}

void GameCoin::BeingFlip(){
    if (isfront){
        QPixmap pix(QString(":/pictures/res/Coin000%1.png").arg(img_min++));

        setFixedSize(pix.width(), pix.height());
        //设置按钮的尺寸
        setStyleSheet("QPushButton{border:0px;}");
        //设置边框样式
        setIcon(pix);
        //设置按钮图标
        setIconSize(QSize(width(), height()));
        //设置图标大小

        if (img_max < img_min){
            img_min =1;
            isfront = false;
            timer -> stop();
        }
    }

    //转向正方向
    else{
        QPixmap pix(QString(":/pictures/res/Coin000%1.png").arg(img_max--));

        setFixedSize(pix.width(), pix.height());
        //设置按钮的尺寸
        setStyleSheet("QPushButton{border:0px;}");
        //设置边框样式
        setIcon(pix);
        //设置按钮图标
        setIconSize(QSize(width(), height()));
        //设置图标大小

        if (img_max < img_min){
            img_max =8;
            isfront = true;
            timer -> stop();
        }
    }

    //转向反方向
}

void GameCoin::CoinFlip(){
    if (!isforbidden)

        timer -> start(30);
}
