#ifndef	DGSKYDOME_H 
#define	DGSKYDOME_H

#include <osg/Vec3>
#include <osg/Vec4>
#include <osg/Quat>
#include <osg/Matrix>
#include <osg/ShapeDrawable>
#include <osg/Geometry>
#include <osg/Geode>
#include <osg/Transform>
#include <osg/Material>
#include <osg/NodeCallback>

#include <osg/Depth>
#include <osg/CullFace>
#include <osg/TexMat>
#include <osg/TexGen>
#include <osg/TexEnv>
#include <osg/TexEnvCombine>
#include <osg/TextureCubeMap>
#include <osg/VertexProgram>
#include <osg/TextureCubeMap>

#include <osgDB/Registry>
#include <osgDB/ReadFile>

#include <osgUtil/CullVisitor>
#include <osgUtil/SmoothingVisitor>
#include <osgUtil/Optimizer>

class dgSkyDomeTransform : public osg::Transform
{
public:

	/* Member Functions */
	// Get the transformation matrix which moves from local coords to world coords
	virtual bool computeLocalToWorldMatrix(osg::Matrix& matrix, osg::NodeVisitor* nv) const
	{
		osgUtil::CullVisitor *cv = dynamic_cast<osgUtil::CullVisitor *>(nv);
		
		if (cv) {
			osg::Vec3 eyePointLocal = cv->getEyeLocal();
			matrix.preMultTranslate(eyePointLocal);
		}
		return true;
	}

	// Get the transformation matrix which moves from world coords to local coords
	virtual bool computeWorldToLocalMatrix(osg::Matrix& matrix,osg::NodeVisitor* nv) const
	{
		osgUtil::CullVisitor *cv = dynamic_cast<osgUtil::CullVisitor *>(nv);
		if (cv) {
			osg::Vec3 eyePointLocal = cv->getEyeLocal();
			matrix.postMultTranslate(-eyePointLocal);
		}
		return true;
	}
};

/**
	@ brief
		Class - Update texture matrix for cubemap
*/
class TexMatCallback : public osg::NodeCallback
{
public:
	/* Constructor & Destructor */
	TexMatCallback(osg::TexMat &tm)
		: _texMat(tm)
	{
	}

	virtual ~TexMatCallback()
	{
	}

	// operator rotate using quaternion
	virtual void operator()(osg::Node *node, osg::NodeVisitor *nv)
	{
		osgUtil::CullVisitor *cv = dynamic_cast<osgUtil::CullVisitor *>(nv);
		if (cv) {
			const osg::Matrix &MV = *(cv->getModelViewMatrix());
			const osg::Matrix R = osg::Matrix::rotate( osg::DegreesToRadians(112.0f), 0.0f, 0.0f, 1.0f ) *
								  osg::Matrix::rotate( osg::DegreesToRadians( 90.0f), 1.0f, 0.0f, 0.0f );

			osg::Quat quat = MV.getRotate();
			const osg::Matrix C = osg::Matrix::rotate( quat.inverse() );

			_texMat.setMatrix( C * R );
		}

		traverse(node, nv);
	}
private:
	osg::TexMat& _texMat;
};

/**
	@ brief
		Class - Sky Dome
*/
class dgSkyDome
{
public:
	/* Constructor & Destructor */
	dgSkyDome();
	virtual ~dgSkyDome();

public:
	/* Member Functions */
	// make Sky dome, cube map
	void makeSkyDome(std::string posx, std::string posy, std::string posz,
						std::string negx, std::string negy, std::string negz);

	// get sky dome reference_pointer
	osg::ref_ptr<osg::Group> getSkyDome() const;

private:
	// read cube map(24bit bitmap)
	osg::ref_ptr<osg::TextureCubeMap> readCubeMap(std::string posx, std::string posy, std::string posz,
										std::string negx, std::string negy,	std::string negz);

private:
	/* Member Variables */
	osg::ref_ptr<osg::Group>	mSkyDomeScene;			//> Sky Dome Scene ptr

};

#endif	// DGSKYDOME_H