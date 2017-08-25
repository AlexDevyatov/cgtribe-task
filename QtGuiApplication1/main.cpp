#include "QtGuiApplication1.h"
#include <QtWidgets/QApplication>
#include <QtWidgets>

#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Material>
#include <osg/Vec3>
#include <osg/MatrixTransform>
#include <osg/Texture2D>
#include <osg/PolygonStipple>
#include <osg/TemplatePrimitiveFunctor>
#include <osg/ShapeDrawable>
#include <osgText/Text>
#include <osg/io_utils>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgGA/TrackballManipulator>

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>

#include <osg/Math>
#include <osg/Timer>

#include <iostream>
#include <windows.h>
#include <math.h>
#include <cassert>
#include <thread>
#include <mutex>

#include "CustomGroup.h"
#include "CustomViewer.h"
#include "CustomSlider.h"
#include "gui.h"

void render(osg::ref_ptr<CustomGroup> group, osg::ref_ptr<CustomGroup> group2)
{
	osg::ref_ptr<osg::Group> ROOOOOT = new osg::Group;
	ROOOOOT->addChild(group);
	auto transform = new osg::MatrixTransform(osg::Matrix::translate(
		10.0f, 10.0f, 0.0f));
	ROOOOOT->addChild(transform);
	transform->addChild(group2);

	osgViewer::Viewer viewer;
	viewer.setSceneData(ROOOOOT.get());
	viewer.setUpViewInWindow(100, 100, 1280, 720);
	viewer.addEventHandler(new osgViewer::StatsHandler);
	viewer.setThreadingModel(osgViewer::Viewer::SingleThreaded);
	viewer.run();

	group->JoinCalcThread();
	group2->JoinCalcThread();
}

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	double time = 1.0;
	std::mutex mtx;
	osg::ref_ptr<CustomGroup> group = new CustomGroup(new osg::Vec3Array, new osg::Vec3Array, &mtx, -10, 10, 0.5);
	osg::ref_ptr<CustomGroup> group2 = new CustomGroup(new osg::Vec3Array, new osg::Vec3Array, &mtx, -10, 10, 0.5);

	std::thread renderThread(render, group, group2);

	auto figure = group->GetFigure();
	Gui * gui = new Gui(argc, argv, figure);
	auto figure2 = group2->GetFigure();
	Gui * gui2 = new Gui(argc, argv, figure2);

	a.exec();

	delete gui;
	delete gui2;
	renderThread.join();
}
