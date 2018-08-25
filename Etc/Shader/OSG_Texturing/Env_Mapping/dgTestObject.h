// include standard lib.
#include <stdio.h>
#include <iostream>
#include <vector>
#include <assert.h>

// include specific osg lib.
#include <osgViewer/Viewer>
#include <osgDB/ReadFile>
#include <osgGA/TrackballManipulator>
#include <osg/MatrixTransform>
#include <osg/ShapeDrawable>
#include <osg/ref_ptr>
#include <osg/Vec3>
#include <osg/Vec4>
#include <osg/Group>
#include <osg/Geode>


// include specific osg lib.
#include <osg/Texture2D>
#include <osg/LightSource>
#include <osg/MatrixTransform>

#include <osgDB/ReadFile>

/**
	@ brief
		Make a test object
*/
extern osg::ref_ptr<osg::Group> makeTestObject();

/**
	@ brief
		Class - Test Object
*/
class dgTestObj
{
public:
	dgTestObj();
	virtual ~dgTestObj();

	osg::ref_ptr<osg::Group> getScene() const;

private:
	// private member functions
	void initShader();
	void makeSphere();
	void applyCubeMap();

	void loadShaderFromFile(osg::ref_ptr<osg::Shader>, std::string);

private:
	// member vairiables
	osg::ref_ptr<osg::MatrixTransform> mSphereTransform;
	osg::ref_ptr<osg::Geode>           mSphereGeode;
	osg::ref_ptr<osg::ShapeDrawable>   mSphereShape;

	osg::ref_ptr<osg::Program>	mShaderProgram;
	osg::ref_ptr<osg::Shader>	mVertexShader;
	osg::ref_ptr<osg::Shader>	mFragmentShader;
	osg::ref_ptr<osg::Group>	mRootScene;
};