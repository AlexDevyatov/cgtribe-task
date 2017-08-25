#ifndef GROUP_H
#define GROUP_H

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

#include <memory>
#include <thread>

#include "CustomCallback.h"
#include "CustomFigure.h"

class CustomGroup : public osg::Group
{
private:
	osg::ref_ptr<CustomFigure> figure;
	std::unique_ptr<std::thread> thr;
public:



	CustomGroup();

	CustomGroup(osg::ref_ptr<osg::Vec3Array> vertices, osg::ref_ptr<osg::Vec3Array> normals, std::mutex * mtx, double l, double r, double step);

	void StartThread();

	void JoinThread(std::thread & thr);

	osg::ref_ptr<CustomFigure> GetFigure();

	void JoinCalcThread();
};
#endif