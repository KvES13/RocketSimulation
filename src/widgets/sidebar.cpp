#include "sidebar.h"

#include <QPaintEvent>
#include <QPainter>
#include <QDebug>
#include <QEvent>

#define action_height 84

SideBar::SideBar(QWidget *parent)
    : QWidget(parent), mCheckedAction(nullptr), mOverAction(nullptr) {
  setMouseTracking(true);
}

void SideBar::paintEvent(QPaintEvent *event) {
    QPainter p(this);

    QFont fontText(p.font());
    fontText.setFamily("Helvetica Neue");
    fontText.setPointSize(10);
    p.setFont(fontText);

    int action_y = 0;
    p.fillRect(rect(),QColor(0, 20, 30) ); //QColor(255, 255, 255)
    for (const auto action : qAsConst(mActions)) {

    QRect actionRect(0, action_y, event->rect().width(), action_height);

    if (action->isChecked()) {
      p.fillRect(actionRect, QColor(25, 35, 45));
    }

    if (action == mOverAction) {
      p.fillRect(actionRect, QColor(47, 53, 57));
    }
    p.setPen(QColor(255, 255, 255));
    QSize size = p.fontMetrics().size(Qt::TextSingleLine, action->text());
    QRect actionTextRect(QPoint(actionRect.width() / 2 - size.width() / 2,
                                actionRect.bottom() - size.height() - 5),
                         size);
    p.drawText(actionTextRect, Qt::AlignCenter, action->text());

    QRect actionIconRect(0, action_y + 10, actionRect.width(),
                         actionRect.height() - 2 * actionTextRect.height() -
                             10);
    QIcon actionIcon(action->icon());
    actionIcon.paint(&p, actionIconRect);

    action_y += actionRect.height();
    }
}

QSize SideBar::minimumSizeHint() const {
    return action_height * QSize(1, mActions.size());
}

void SideBar::activateFirstAction()
{
    if(mActions.isEmpty())
        return;;
    auto action = mActions.at(0);
    action->setChecked(true);
    mCheckedAction = action;
    update();
}

void SideBar::addAction(QAction *action) {
    mActions.push_back(action);
    action->setCheckable(true);
    update();
}

QAction *SideBar::addAction(const QString &text, const QIcon &icon) {
    QAction *action = new QAction(icon, text, this);
    action->setCheckable(true);
  //  action->
    mActions.push_back(action);
    update();
    return action;
}

void SideBar::mousePressEvent(QMouseEvent *event) {
    QAction *tempAction = actionAt(event->pos());
    if (tempAction == nullptr || tempAction->isChecked())
    return;

    if (mCheckedAction)
    mCheckedAction->setChecked(false);
    if (mOverAction == tempAction)
    mOverAction = nullptr;
    mCheckedAction = tempAction;
    tempAction->setChecked(true);
    update();


    QWidget::mousePressEvent(event);
}

void SideBar::mouseMoveEvent(QMouseEvent *event) {
    QAction *tempAction = actionAt(event->pos());
    if (tempAction == nullptr) {
        mOverAction = nullptr;
        update();
        return;
    }
    if (tempAction->isChecked() || mOverAction == tempAction)
        return;
    mOverAction = tempAction;
    update();
    QWidget::mouseMoveEvent(event);
}

void SideBar::leaveEvent(QEvent *event) {
    mOverAction = nullptr;
    update();
    QWidget::leaveEvent(event);
}

QAction *SideBar::actionAt(const QPoint &at) {
  int action_y = 0;
  for (auto action : qAsConst(mActions)) {
    QRect actionRect(0, action_y, rect().width(), action_height);
    if (actionRect.contains(at))
      return action;
    action_y += actionRect.height();
  }
  return nullptr;
}


#undef action_height
