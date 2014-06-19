#include "controlbar.h"

#include <QStyle>
#include <QtWidgets/QSlider>
#include <QHBoxLayout>

ControlBar::ControlBar(QWidget *parent) :
    QWidget(parent), isAudioPlaying(false)
{
    this -> playButton = new QToolButton(this);
    this -> playButton -> setIcon( this -> style() -> standardIcon(QStyle::SP_MediaPlay));
    connect(this -> playButton, SIGNAL( clicked() ), this, SLOT(playButtonClicked()));

    this -> nextButton = new QToolButton(this);
    this -> nextButton -> setIcon( this -> style() -> standardIcon(QStyle::SP_MediaSkipForward));
    connect(this -> nextButton, SIGNAL( clicked() ), this, SIGNAL( nextAudio() ) );

    this -> previousButton = new QToolButton(this);
    this -> previousButton -> setIcon( this -> style() -> standardIcon(QStyle::SP_MediaSkipBackward));
    connect(this -> previousButton, SIGNAL( clicked() ), this, SIGNAL( previousAudio() ) );

    this -> progressBar = new QSlider(Qt::Horizontal, this);
    progressBar -> setRange(0,100);

    QHBoxLayout* buttonBoxLayout = new QHBoxLayout;
    buttonBoxLayout -> addWidget(progressBar);
    buttonBoxLayout -> addWidget(playButton);
    buttonBoxLayout -> addWidget(previousButton);
    buttonBoxLayout -> addWidget(nextButton);

    this -> setLayout(buttonBoxLayout);
}

void ControlBar::playButtonClicked()
{
   if (isAudioPlaying)
   {
       this -> playButton -> setIcon( this -> style() -> standardIcon(QStyle::SP_MediaPlay));
       emit pauseAudio();
   }
   else
   {
       this -> playButton -> setIcon( this -> style() -> standardIcon(QStyle::SP_MediaPause));
       emit playAudio();
   }

   isAudioPlaying = !isAudioPlaying;

}

void ControlBar::updateProgressBar(float rate)
{
    progressBar -> setValue(rate);
}
