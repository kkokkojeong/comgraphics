
#include "dgEvnMapping.h"
#include <assert.h>

/**
	@ brief
		Main
*/
int main(int argc, char *argv[])
{
	// allocate a root scene data
	osg::ref_ptr<osg::Group> sceneData = new osg::Group();
	assert(sceneData != NULL);

	// make objects
	dgSkyDome *skyDome = new dgSkyDome();
	/*
  	skyDome->makeSkyDome("../Cube/box_03_right.bmp", "../Cube/box_06_bottom.bmp", "../Cube/box_02_front.bmp",
  		"../Cube/box_01_left.bmp", "../Cube/box_05_Top.bmp", "../Cube/box_04_back.bmp");
	*/
  	skyDome->makeSkyDome("../Cube/posx.bmp", "../Cube/posy.bmp", "../Cube/posz.bmp",
  		"../Cube/negx.bmp", "../Cube/negy.bmp", "../Cube/negz.bmp");

	// insert sky dome
	dgTestObj *testObj = new dgTestObj();
//	osg::ref_ptr<osg::Group> obj = makeTestObject();
	sceneData->addChild(skyDome->getSkyDome());
	sceneData->addChild(testObj->getScene());

	// allocate a view
	osgViewer::Viewer viewer;
	viewer.setSceneData(sceneData.get());
	viewer.setUpViewInWindow(300, 100, 1280, 720);		// position, resolution

	return viewer.run();
}