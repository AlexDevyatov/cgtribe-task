#ifndef EMITTER_H
#define EMITTER_H

#include <QSlider>

class CustomSlider : public QSlider
{
	Q_OBJECT;
public:

	CustomSlider();

public slots:

	void SetDefault();
};
#endif