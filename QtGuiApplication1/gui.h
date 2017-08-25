#ifndef GUI_H
#define GUI_H

#include <QtWidgets/QApplication>
#include <QtWidgets>
#include <thread>
#include <memory>
#include "CustomFigure.h"
#include "CustomSlider.h"

class Gui
{
private:
	std::tr1::shared_ptr<QWidget> _window;
	CustomFigure * _figure;
	std::unique_ptr<std::thread> upd;
	bool execute;
	QLabel * labelAVal;
	QLabel * labelBVal;
	static void updateData(Gui * gui, osg::ref_ptr<CustomFigure> figure);
public:

	Gui(int argc, char *argv[], osg::ref_ptr<CustomFigure> figure);
	~Gui();

	void Display(int argc, char *argv[], osg::ref_ptr<CustomFigure> figure);
};

#endif