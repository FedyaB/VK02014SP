#include "controls.h"
#include <QStyle>
#include <QHBoxLayout>

Controls::Controls(QWidget *parent) :
    QWidget(parent)
{
   playbutton = new QToolButton(this);
   playbutton -> setIcon(style() -> standardIcon(QStyle::SP_MediaPlay));
   connect(playbutton, SIGNAL(clicked()), SLOT(playclicked()));
   playing = false;

   nextbutton = new QToolButton(this);
   nextbutton -> setIcon(style() -> standardIcon(QStyle::SP_MediaSkipForward));
   connect(nextbutton, SIGNAL(clicked()), SIGNAL(next()));

   previousbutton = new QToolButton(this);
   previousbutton -> setIcon(style() -> standardIcon(QStyle::SP_MediaSkipBackward));
   connect(previousbutton, SIGNAL(clicked()), SIGNAL(previous()));

   slider = new QSlider(Qt::Horizontal, this);
   slider -> setRange(0, 100);
   QHBoxLayout* layout = new QHBoxLayout;
   layout -> addWidget(slider);
   layout -> addWidget(previousbutton);
   layout -> addWidget(playbutton);
   layout -> addWidget(nextbutton);
   setLayout(layout);
}

void Controls::setprogress(float rate)
{
    slider -> setValue(rate);
}

void Controls::playclicked()
{
    if (playing == false)
    {
        playbutton -> setIcon(style() -> standardIcon(QStyle::SP_MediaPause));
        playing = true;
        emit play();
    }
    else
    {
        playbutton -> setIcon(style() -> standardIcon(QStyle::SP_MediaPlay));
        playing = false;
        emit pause();
    }
}
