#include "tulip/AnimatedGraphicsObject.h"

AnimatedGraphicsObject::AnimatedGraphicsObject(QGraphicsItem *parent): QGraphicsObject(parent), _currentPositionAnimation(0), _currentSizeAnimation(0), _animated(true), _behavior(StopPreviousAnimation) {}

AnimatedGraphicsObject::~AnimatedGraphicsObject() {
  if (_currentPositionAnimation)
    _currentPositionAnimation->stop();
  if (_currentSizeAnimation)
    _currentSizeAnimation->stop();
}

void AnimatedGraphicsObject::moveItem(const QPointF &from, const QPointF &to, int msec, const QEasingCurve &easing) {
  if (from == to)
    return;

  if (_currentPositionAnimation) {
    if (_behavior == ContinuePreviousAnimation)
      return;
    _currentPositionAnimation->stop();
  }

  if (_animated) {
    _currentPositionAnimation = new QPropertyAnimation(this, "pos");
    _currentPositionAnimation->setStartValue(from);
    _currentPositionAnimation->setEndValue(to);
    _currentPositionAnimation->setDuration(msec);
    _currentPositionAnimation->setEasingCurve(easing);
    _currentPositionAnimation->start(QAbstractAnimation::KeepWhenStopped);
    connect(_currentPositionAnimation, SIGNAL(finished()), this, SLOT(animationFinished()), Qt::DirectConnection);
  }

  else
    setPos(to);
}

void AnimatedGraphicsObject::moveItem(const QPointF &to, int msec, const QEasingCurve &easing) {
  moveItem(pos(), to, msec, easing);
}

void AnimatedGraphicsObject::resizeItem(const QSizeF &from, const QSizeF &to, int msec, const QEasingCurve &easing) {
  if (from == to)
    return;

  if (_currentSizeAnimation) {
    if (_behavior == ContinuePreviousAnimation)
      return;
    _currentSizeAnimation->stop();
  }

  if (_animated) {
    _currentSizeAnimation = new QPropertyAnimation(this, "itemSize");
    _currentSizeAnimation->setStartValue(from);
    _currentSizeAnimation->setEndValue(to);
    _currentSizeAnimation->setDuration(msec);
    _currentSizeAnimation->setEasingCurve(easing);
    _currentSizeAnimation->start(QAbstractAnimation::KeepWhenStopped);
    connect(_currentSizeAnimation, SIGNAL(finished()), this, SLOT(animationFinished()), Qt::DirectConnection);
  }

  else
    setItemSize(to);
}

void AnimatedGraphicsObject::resizeItem(const QSizeF &to, int msec, const QEasingCurve &easing) {
  resizeItem(itemSize(),to,msec,easing);
}

void AnimatedGraphicsObject::animationFinished() {
  if (sender() == _currentPositionAnimation) {
    _currentPositionAnimation->deleteLater();
    _currentPositionAnimation = 0;
  }
  else if (sender() == _currentSizeAnimation) {
    _currentSizeAnimation->deleteLater();
    _currentSizeAnimation = 0;
  }
}
