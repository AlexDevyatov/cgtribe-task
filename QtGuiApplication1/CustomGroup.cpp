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

#include <thread>

#include "CustomFigure.h"
#include "CustomCallback.h"
#include "CustomGroup.h"

CustomGroup::CustomGroup()
{

}

CustomGroup::CustomGroup(osg::ref_ptr<osg::Vec3Array> vertices, osg::ref_ptr<osg::Vec3Array> normals, std::mutex * mtx, double l, double r, double step)
{
	figure = new CustomFigure(new osg::Vec3Array, new osg::Vec3Array, mtx, l, r, step);
	setUpdateCallback(new CustomCallBack(mtx));

	auto transform = new osg::MatrixTransform(osg::Matrix::scale(1.0f, 1.0f, -1.0f));
	transform->addChild(figure);
	
	thr = std::make_unique<std::thread>(&CustomFigure::calc, figure);
	addChild(figure);
	addChild(transform);
}

void CustomGroup::JoinCalcThread()
{
	figure->StopExecute();
	thr->join();
}

osg::ref_ptr<CustomFigure> CustomGroup::GetFigure()
{
	return figure;
}