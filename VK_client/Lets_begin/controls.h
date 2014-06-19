#ifndef CONTROLS_H
#define CONTROLS_H

#include <QWidget>
#include <QToolButton>
#include <QSlider>

class Controls : public QWidget
{
    Q_OBJECT
public:
    explicit Controls(QWidget *parent = 0);

private:
    QToolButton* playbutton;
    QToolButton* nextbutton;
    QToolButton* previousbutton;
    QSlider* slider;
    bool playing;

signals:
    void play();
    void pause();
    void next();
    void previous();

public slots:
    void setprogress(float);
private slots:
    void playclicked();
};

#endif // CONTROLS_H
