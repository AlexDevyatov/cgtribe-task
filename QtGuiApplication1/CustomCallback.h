#ifndef CALLBACK_H
#define CALLBACK_H

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

#include <mutex>

class CustomCallBack : public osg::NodeCallback
{
private:

	std::mutex * mtx;

public:

	CustomCallBack(std::mutex * mtx);
	~CustomCallBack();

	virtual void operator() (osg::Node* node, osg::NodeVisitor* nv);
};
#endif