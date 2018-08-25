
//> Link Library, pragma comment
#pragma once
#pragma comment(lib, "osgViewerd.lib")
#pragma comment(lib, "osgGAd.lib")
#pragma comment(lib, "osgd.lib")
#pragma comment(lib, "osgDBd.lib")
#pragma comment(lib, "osgUtild.lib")
#pragma comment(lib, "osgShadowd.lib")

//> System Header
#include <stdio.h>
#include <iostream>
#include <vector>

// Include specific osg(OpenSceneGraph)
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

//> Include used defined lib or class
#include "dgSkyDome.h"
#include "dgTestObject.h"

//> Namespace
using namespace::std;
using std::vector;
