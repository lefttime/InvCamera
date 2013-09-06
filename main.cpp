#include "GenericTextureImage.hpp"

#include "InvBoudingAxis.hpp"
#include "InvSphereSceneNode.hpp"
#include "InvExtrusionSceneNode.hpp"

#include <time.h>
#include <QtDebug>
#include <Qt3D/qvector3darray.h>

#include <Inventor/Qt/viewers/SoQtExaminerViewer.h>

#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoMultipleCopy.h>

#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoShapeHints.h>

#include <Inventor/nodekits/SoShapeKit.h>

#include <MeshViz/nodes/PoDomain.h>
#include <MeshViz/graph/PoAutoCubeAxis.h>

SoNode* generateAxisSample()
{
  InvBoudingAxis* sampleNode = new InvBoudingAxis();
  sampleNode->setValues( SbBox3f(SbVec3f(290, 240, 0), SbVec3f(470, 370, 2000)) );
  sampleNode->setExtent( SbBox3f(SbVec3f(0, 0, 0), SbVec3f(3599.39, 2599.97, 2001)) );
  return sampleNode;
}

SoNode* generateSphereSample()
{
  return new InvSphereSceneNode;
}

SoNode* generateExtrusionSample()
{
  QVector<qreal> scales;
  QVector3DArray trajectory;
  for( int idx = 0; idx < 1024; ++idx ) {
    scales << 50;
    trajectory << QVector3D(0, 0, -idx);
  }
  InvExtrusionSceneNode* shapeNode = new InvExtrusionSceneNode( trajectory, scales );

  //QImage image( "E:/Tulips.jpg" );
  //GenericTextureImage* textureImage = new GenericTextureImage( image );
  //shapeNode->setTextureImage( textureImage );

  return shapeNode;
}

void buildScene( SoSeparator* sceneRoot )
{
  sceneRoot->addChild( generateAxisSample() );
  return;

  SoShapeHints* shapeHintsNode = new SoShapeHints;
  shapeHintsNode->creaseAngle = 30;
  shapeHintsNode->vertexOrdering = SoShapeHints::COUNTERCLOCKWISE;
  sceneRoot->addChild( shapeHintsNode );

  SoMultipleCopy* multipleCopyNode = new SoMultipleCopy;
  sceneRoot->addChild( multipleCopyNode );
  multipleCopyNode->addChild( generateSphereSample() );

  int objectCount = 30;
  multipleCopyNode->matrix.setNum( objectCount );
  SbMatrix* mtxPtr = multipleCopyNode->matrix.startEditing();
  for( int idx = 0; idx < objectCount; ++idx ) {
    float xpos = qrand() % (objectCount*100);
    float ypos = qrand() % (objectCount*100);
    float zpos = qrand() % (objectCount*100);

    SbMatrix mtx;
    mtx.setTranslate( SbVec3f(xpos, ypos, zpos) );
    mtxPtr[idx] = mtx;
  }
  multipleCopyNode->matrix.finishEditing();
}

int main(int argc, char *argv[])
{
  qsrand( (unsigned int)time(0) );

  Widget hWindow = SoQt::init( argc, argv[0] );
  if( !hWindow ) {
    exit( -1 );
  }
  hWindow->setWindowTitle( "Camera sample." );

  SoQtExaminerViewer* hViewer = new SoQtExaminerViewer( hWindow );
  hViewer->setDecoration( false );
  hViewer->setSeekTime( 1 );

  SoSeparator* hRoot = new SoSeparator;
  buildScene( hRoot );
  hViewer->setSceneGraph( hRoot );

  hViewer->show();
  SoQt::show( hWindow );

  SoQt::mainLoop();
  SoQt::finish();

  return 0;
}
