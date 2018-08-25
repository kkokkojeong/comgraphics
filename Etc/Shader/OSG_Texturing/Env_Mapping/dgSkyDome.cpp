
#include "dgSkyDome.h"
#include <assert.h>

/**
	@ brief
		Constructor
*/
dgSkyDome::dgSkyDome()
{
	// allocate root scene of sky dome
	mSkyDomeScene = new osg::Group();
	assert(mSkyDomeScene != NULL);
}

/**
	@ brief
		Destructor
*/
dgSkyDome::~dgSkyDome()
{

}

/**
	@ brief
		Make a sky box
*/
void dgSkyDome::makeSkyDome(std::string posx, std::string posy, std::string posz, 
	std::string negx, std::string negy, std::string negz)
{
	// clear memory
	if(mSkyDomeScene->getNumChildren() > 0) {
		for(int i =0; i < (int)mSkyDomeScene->getNumChildren(); i++) {
			mSkyDomeScene->removeChild(i);
		}
	}

	// read a cube map
	osg::ref_ptr<osg::TextureCubeMap> skyMap = 
		readCubeMap(posx, posy, posz, negx, negy, negz);

	// allocate the state set
	osg::StateSet *stateSet = new osg::StateSet();

	osg::TexEnv *te = new osg::TexEnv;			// Environment texture
	te->setMode(osg::TexEnv::REPLACE);
	stateSet->setTextureAttributeAndModes(0, te, osg::StateAttribute::ON);

	osg::TexGen *tg = new osg::TexGen;			// Normal map
	tg->setMode(osg::TexGen::NORMAL_MAP);
	stateSet->setTextureAttributeAndModes(0, tg, osg::StateAttribute::ON);

	osg::TexMat *tm = new osg::TexMat;
	stateSet->setTextureAttribute(0, tm);
	stateSet->setTextureAttributeAndModes(0, skyMap.get(), osg::StateAttribute::ON);

	// some properties are disabled
	stateSet->setMode( GL_LIGHTING,  osg::StateAttribute::OFF);
	stateSet->setMode( GL_CULL_FACE, osg::StateAttribute::OFF);

	// clear the depth to the far plane
	osg::ref_ptr<osg::Depth> depth = new osg::Depth();
	depth->setFunction(osg::Depth::LEQUAL);
	depth->setRange(1.0, 1.0);

	stateSet->setAttributeAndModes(depth, osg::StateAttribute::ON);
	stateSet->setRenderBinDetails(5, "RenderBin");

	// allocate sphere drawable
	osg::ref_ptr<osg::Drawable> drawable = new osg::ShapeDrawable(
		new osg::Sphere(osg::Vec3(0.0, 0.0, 0.0), 10.0));
	assert(drawable != NULL);

	// allocate geode
	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	assert(geode != NULL);

	geode->setCullingActive(false);				// Setting geode
	geode->setStateSet(stateSet);
	geode->addDrawable(drawable);

	// allocate transform
	osg::ref_ptr<osg::Transform> transform = new dgSkyDomeTransform;
	transform->setCullingActive(false);
	transform->addChild(geode.get());

	osg::ClearNode *clearNode = new osg::ClearNode;
	clearNode->setCullCallback(new TexMatCallback(*tm));
	clearNode->addChild(transform);

	mSkyDomeScene->addChild(clearNode);
}

/**
	@ brief
		Get a root scene of sky dome
*/
osg::ref_ptr<osg::Group> dgSkyDome::getSkyDome() const
{
	return mSkyDomeScene;
}

/**
	@ brief
		Read a cubemap texture(6 direction)
*/
osg::ref_ptr<osg::TextureCubeMap> dgSkyDome::readCubeMap(std::string posx, std::string posy, std::string posz, 
	std::string negx, std::string negy, std::string negz)
{
	// allocate cubemap
	osg::ref_ptr<osg::TextureCubeMap> cubemap = new osg::TextureCubeMap;

	// read osgImage from file
	osg::Image *imgPosX = osgDB::readImageFile(posx);
	osg::Image *imgNegX = osgDB::readImageFile(negx);
	osg::Image *imgPosY = osgDB::readImageFile(posy);
	osg::Image *imgNegY = osgDB::readImageFile(negy);
	osg::Image *imgPosZ = osgDB::readImageFile(posz);
	osg::Image *imgNegZ = osgDB::readImageFile(negz);

	// setup the cubemap images & properties
	if (imgPosX && imgNegX && imgPosY && imgNegY && imgPosZ && imgNegZ) {
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

	// return a loaded cubemap
	return cubemap;
}