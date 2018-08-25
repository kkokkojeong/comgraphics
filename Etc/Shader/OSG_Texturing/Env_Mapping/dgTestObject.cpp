#include "dgTestObject.h"
#include <assert.h>
#include <osg/Depth>
#include <osg/CullFace>
#include <osg/TexMat>
#include <osg/TexGen>
#include <osg/TexEnv>
#include <osg/TexEnvCombine>
#include <osg/TextureCubeMap>
#include <osg/VertexProgram>
#include <osg/TextureCubeMap>


/**
	@ brief
		Make a test object
*/
osg::ref_ptr<osg::Group> makeTestObject()
{
	// allocate sub scene
	osg::ref_ptr<osg::Group> subScene = new osg::Group();
	assert(subScene != NULL);

	// allocate matrix transform for sub scene
	osg::ref_ptr<osg::MatrixTransform> sceneTransform = new osg::MatrixTransform;
	assert(sceneTransform != NULL);

	// load obj
	osg::ref_ptr<osg::Node> tmpObj = osgDB::readNodeFile("texturedknot.obj");

	subScene->addChild(sceneTransform.get());
	sceneTransform->addChild(tmpObj.get());

	// make transform matrix for sub scene
	osg::Matrix objMatrix;
	objMatrix.makeTranslate(0.0, 0.0, 15.0);
	objMatrix.preMultScale(osg::Vec3f(0.1, 0.1, 0.1));
	sceneTransform->setMatrix(objMatrix);

	return subScene;
}

/**
	@ brief
		Constructor
*/
dgTestObj::dgTestObj()
{
	// initialize
	mRootScene = new osg::Group();
	assert(mRootScene != NULL);

	// shader program
	this->initShader();

	// make a sphere
	this->makeSphere();

	// apply texture map
	this->applyCubeMap();

	// scene state setup
	mRootScene->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
}

/**
	@ brief
		Destructor
*/
dgTestObj::~dgTestObj()
{

}

/**
	@ brief
		Get a root scene
*/
osg::ref_ptr<osg::Group> dgTestObj::getScene() const
{
	return mRootScene;
}

/**
	@ brief
		Initialize a shader
*/
void dgTestObj::initShader()
{
	// create shader context
	this->mShaderProgram = new osg::Program();
	this->mShaderProgram->setName("dgEnvMap");

	// create vertex & fragment shaders
	this->mVertexShader = new osg::Shader(osg::Shader::VERTEX);
	this->mFragmentShader = new osg::Shader(osg::Shader::FRAGMENT);

	// load shaders from file
	this->loadShaderFromFile(this->mVertexShader, "dgEnvMap.vert");
	this->loadShaderFromFile(this->mFragmentShader, "dgEnvMap.frag");

	// attach shaders to context
	this->mShaderProgram->addShader(this->mVertexShader);
	this->mShaderProgram->addShader(this->mFragmentShader);
}

/**
	@ brief
		shader loading function
*/
void dgTestObj::loadShaderFromFile(osg::ref_ptr<osg::Shader> shader,std::string fileName)
{
	shader->loadShaderSourceFromFile(fileName.c_str());
}

/**
	@ brief
		Draw a sphere
*/
void dgTestObj::makeSphere()
{
	// allocate - cube object`s scene graph
	mSphereTransform = new osg::MatrixTransform();
	assert(mSphereTransform != NULL);

	mSphereGeode = new osg::Geode();
	assert(mSphereGeode != NULL);

//	mSphereShape = new osg::ShapeDrawable(new osg::Box(osg::Vec3(0.0, 0.0, 0.0), 1.0));
	mSphereShape = new osg::ShapeDrawable(new osg::Sphere(osg::Vec3(0.0, 0.0, 0.0), 5.0));
	assert(mSphereShape != NULL);

	mSphereShape->setColor(osg::Vec4(1.0, 1.0, 1.0, 1.0));

	mSphereGeode->addDrawable(mSphereShape);		
	mSphereTransform->addChild(mSphereGeode);
	mRootScene->addChild(mSphereTransform);	
}

/**
	@ brief
		Apply a cube map
*/
void dgTestObj::applyCubeMap()
{
	// allocate cubemap
	osg::ref_ptr<osg::TextureCubeMap> cubemap = new osg::TextureCubeMap;

	std::string posx = "../Cube/posx.bmp";
	std::string negx = "../Cube/negx.bmp";
	std::string posy = "../Cube/posy.bmp";
	std::string negy = "../Cube/negy.bmp";
	std::string posz = "../Cube/posz.bmp";
	std::string negz = "../Cube/negz.bmp";

	// read osgImage from file
	osg::Image *imgPosX = osgDB::readImageFile(posx);
	osg::Image *imgNegX = osgDB::readImageFile(negx);
	osg::Image *imgPosY = osgDB::readImageFile(posy);
	osg::Image *imgNegY = osgDB::readImageFile(negy);
	osg::Image *imgPosZ = osgDB::readImageFile(posz);
	osg::Image *imgNegZ = osgDB::readImageFile(negz);

	// setup the cubemap images & properties
	if (imgPosX && imgNegX && imgPosY && imgNegY && imgPosZ && imgNegZ) {
		//std::cout << "checekd";
		cubemap->setImage(osg::TextureCubeMap::POSITIVE_X, imgPosX);
		cubemap->setImage(osg::TextureCubeMap::NEGATIVE_X, imgNegX);
		cubemap->setImage(osg::TextureCubeMap::POSITIVE_Y, imgPosY);
		cubemap->setImage(osg::TextureCubeMap::NEGATIVE_Y, imgNegY);
		cubemap->setImage(osg::TextureCubeMap::POSITIVE_Z, imgPosZ);
		cubemap->setImage(osg::TextureCubeMap::NEGATIVE_Z, imgNegZ);

		cubemap->setWrap(osg::Texture::WRAP_S, osg::Texture::CLAMP_TO_EDGE);
		cubemap->setWrap(osg::Texture::WRAP_T, osg::Texture::CLAMP_TO_EDGE);
		cubemap->setWrap(osg::Texture::WRAP_R, osg::Texture::CLAMP_TO_EDGE);

		cubemap->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR_MIPMAP_LINEAR);
		cubemap->setFilter(osg::Texture::MAG_FILTER, osg::Texture::LINEAR);
	}

	// assign the material and texture to the cube object
	osg::ref_ptr<osg::StateSet> stateSet = mSphereGeode->getOrCreateStateSet();

	stateSet->addUniform(new osg::Uniform("EnvTex", 0));
	stateSet->setAttributeAndModes(mShaderProgram, osg::StateAttribute::ON);
	stateSet->setTextureAttributeAndModes(0, cubemap, osg::StateAttribute::PROTECTED | osg::StateAttribute::ON);
}