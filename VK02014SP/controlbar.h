#ifndef CONTROLBAR_H
#define CONTROLBAR_H

#include <QWidget>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QAbstractSlider>

class ControlBar : public QWidget
{
    Q_OBJECT
public:
    explicit ControlBar(QWidget *parent = 0);

private:
    QToolButton* playButton, *nextButton, *previousButton;
    QAbstractSlider* progressBar;
    bool isAudioPlaying;


signals:
    void playAudio();
    void pauseAudio();
    void nextAudio();
    void previousAudio();

public slots:
    void updateProgressBar(float);

private slots:
    void playButtonClicked();

};

#endif // CONTROLBAR_H
