#include "widgetbutton.h"

WidgetButton::WidgetButton(QString normal_img, QString focus_img, QWidget *parent)
    : QPushButton(parent), normal_img(normal_img), focus_img(focus_img)
{
    QPixmap pix(normal_img);

    //set button size
    setFixedSize(pix.width(), pix.height());
    //set border style
    setStyleSheet("QPushButton{border:0px;}");
    //set button icon
    setIcon(pix);
    //set icon size
    setIconSize(QSize(width(), height()));
}

void WidgetButton::ZoomUp(){
    //创建动画对象
    QPropertyAnimation *animation = new QPropertyAnimation(this, "geometry", this);
    //设置时间间隔，单位毫秒
    animation ->setDuration(200);
    //创建起始位置
    animation->setStartValue(QRect(this->x(),this->y(),this->width(),this->height()));
    //创建结束位置
    animation->setEndValue(QRect(this->x(),this->y() +10,this->width(),this->height()));
    //设置缓和曲线，QEasingCurve::OutBounce 为弹跳效果
    animation->setEasingCurve(QEasingCurve::OutBounce);
    //开始执行动画
    animation->start();
}

void WidgetButton::ZoomDown(){
    QPropertyAnimation *animation = new QPropertyAnimation(this, "geometry", this);
    animation ->setDuration(200);
    animation->setStartValue(QRect(this->x(),this->y() +10,this->width(),this->height()));
    animation->setEndValue(QRect(this->x(),this->y(),this->width(),this->height()));

    animation->setEasingCurve(QEasingCurve::InBounce);

    animation->start();
}

//鼠标事件
void WidgetButton::mousePressEvent(QMouseEvent *e){
    if (focus_img != "")//选中路径不为空，显示选中图片
    {
        QPixmap pix(focus_img);
        setFixedSize(pix.width(), pix.height());
        setStyleSheet("QPushButton{border:0px;}");
        setIcon(pix);
        setIconSize(QSize(width(), height()));
    }
    //交给父类执行按下事件
    return QPushButton::mousePressEvent(e);
}

void WidgetButton::mouseReleaseEvent(QMouseEvent *e){
    if (focus_img != ""){
        QPixmap pix(normal_img);
        setFixedSize(pix.width(), pix.height());
        setStyleSheet("QPushButton{border:0px;}");
        setIcon(pix);
        setIconSize(QSize(width(), height()));
    }

    return QPushButton::mouseReleaseEvent(e);
}

void WidgetButton::ButtonBounce(){
    ZoomUp();
    ZoomDown();
}

