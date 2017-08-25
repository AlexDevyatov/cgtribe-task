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

#include "CustomCallback.h"
#include "CustomFigure.h"
#include "CustomGroup.h"

#include <mutex>

CustomCallBack::CustomCallBack(std::mutex * mtx)
{
	this->mtx = mtx;
}

CustomCallBack::~CustomCallBack()
{
	//delete mtx;
}

void CustomCallBack::operator() (osg::Node* node, osg::NodeVisitor* nv)
{
	this->mtx->lock();
	if (static_cast<CustomGroup*>(node)->GetFigure()->GetFlag())
	{
		static_cast<CustomGroup*>(node)->GetFigure()->SetTime(nv->getFrameStamp()->getReferenceTime());
		static_cast<CustomGroup*>(node)->GetFigure()->UpdateAll();
	}
	this->mtx->unlock();

	traverse(node, nv);
}