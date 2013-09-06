#include "InvBoudingAxis.hpp"

#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoAnnoText3Property.h>

#include <Inventor/nodekits/SoShapeKit.h>

#include <Inventor/nodes/SoFont.h>
#include <Inventor/nodes/SoText2.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoAnnoText3.h>
#include <Inventor/nodes/SoAnnotation.h>
#include <Inventor/nodes/SoTranslation.h>

#include <Inventor/actions/SoSearchAction.h>

#include <MeshViz/graph/PoLinearAxis.h>
#include <MeshViz/graph/PoGroup6Axis3.h>

class InvBoudingAxis::InvBoudingAxisPrivate
{
public:

  InvBoudingAxisPrivate( InvBoudingAxis* me ) : m_self( me ) {
    PoMeshViz::init();
    m_values = m_extent = SbBox3f(SbVec3f(0, 0, 0), SbVec3f(1, 1, -1));
  }

  void init() {
    m_bboxNode = new SoShapeKit;
    m_bboxNode->set( "drawStyle { style LINES }" );
    m_bboxNode->set( "pickStyle { style UNPICKABLE }" );
    m_bboxNode->set( "lightModel { model BASE_COLOR }" );
    m_self->addChild( m_bboxNode );

    SoAnnoText3Property* annoText3Prop = new SoAnnoText3Property;
    annoText3Prop->renderPrintType = SoAnnoText3Property::RENDER2D_PRINT_RASTER;
    annoText3Prop->setMaxViewingFontSize( 20 );
    m_self->addChild( annoText3Prop );

    m_transNode = new SoTransform;
    m_self->addChild( m_transNode );

    m_axisNode = new PoGroup6Axis3;
    m_axisNode->addPostRebuildCallback( __postRebuildCB, m_self );
    m_self->addChild( m_axisNode );

    update();
  }

  void update() {
    SbVec3f extSize = m_extent.getSize();
    SoCube* cubeNode = SO_GET_PART( m_bboxNode, "shape", SoCube );
    cubeNode->width = extSize[0];
    cubeNode->height= extSize[1];
    cubeNode->depth = extSize[2];
    SoTransform* transNode = SO_GET_PART( m_bboxNode, "transform", SoTransform );
    transNode->translation = m_extent.getCenter();

    m_axisNode->start = m_values.getMin();
    m_axisNode->end   = m_values.getMax();

    m_transNode->setMatrix( getTransform() );

    SbVec3f scale = m_transNode->scaleFactor.getValue();
    scale[0] = qAbs( scale[0] );
    scale[1] = qAbs( scale[1] );
    scale[2] = qAbs( scale[2] );
    m_tickLength[0] = qMin( qAbs(extSize[0]),
                            qMin( qAbs(extSize[1]), qAbs(extSize[2]) ) ) * 0.02;
    m_tickLength[0] /= scale[0];
    scale /= scale[0];

    m_tickLength[1] = m_tickLength[0] * scale[1];
    m_tickLength[2] = m_tickLength[0] * scale[2];
  }

  SbMatrix getTransform() {
    SbMatrix result = SbMatrix::identity();
    {
      SbMatrix temp;
      
      temp.setTranslate( -m_values.getMin() );
      result.multRight( temp );

      SbVec3f scaleFactor( m_extent.getSize()[0]/m_values.getSize()[0],
                           m_extent.getSize()[1]/m_values.getSize()[1],
                           m_extent.getSize()[2]/m_values.getSize()[2] );
      temp.setScale( scaleFactor );
      result.multRight( temp );

      temp.setTranslate( m_extent.getMin() );
      result.multRight( temp );
    }
    return result;
  }

private:

  void updateAxisInfo( PoLinearAxis* axisNode ) {
    axisNode->tickNumOrPeriod = 9;
    axisNode->tickMainLength = m_tickLength[0];
    axisNode->tickSubLength  = m_tickLength[0]*0.5;
    axisNode->tickSubDef = PoLinearAxis::NUM_SUB_TICK;
    axisNode->setFormat( new PbNumericDisplayFormat( PbNumericDisplayFormat::INTEGER_FORMAT ) );
    axisNode->addPostRebuildCallback( __postAxisRebuildCB, m_self );
  }

  void replaceTextNode( SoGroup* groupNode, SoText2::Justification justification ) {
    SoSearchAction sa;
    sa.setType( SoAnnoText3::getClassTypeId() );
    sa.setInterest( SoSearchAction::ALL );   // Find ALL instances
    sa.setSearchingAll( TRUE );
    SoBaseKit::setSearchingChildren( TRUE );
    sa.apply( groupNode );
    SoPathList &pathList = sa.getPaths();
    int numPaths = pathList.getLength();
    for( int idx = 0; idx < numPaths; ++idx ) {
      SoFullPath* pPath = (SoFullPath*)pathList[idx];
      SoAnnoText3* targetNode = (SoAnnoText3*)pPath->getTail();
      SoText2* textNode = new SoText2;
      textNode->justification = justification;
      textNode->string.setValue( targetNode->string[0] );
      pPath->pop();
      SoGroup* parentNode = (SoGroup*)pPath->getTail();
      parentNode->replaceChild( targetNode, textNode );
    }
  }

  static void __postRebuildCB( void* userData, PoBase* base ) {
    InvBoudingAxis* selfObject = static_cast<InvBoudingAxis*>( userData );
    if( selfObject ) {
      SoSearchAction sa;
      sa.setType( PoLinearAxis::getClassTypeId() );
      sa.setInterest( SoSearchAction::ALL );
      sa.setSearchingAll( TRUE );
      SoBaseKit::setSearchingChildren( TRUE );
      sa.apply( base );
      SoPathList &pathList = sa.getPaths();
      int numPaths = pathList.getLength();
      for( int idx = 0; idx < numPaths; ++idx ) {
        SoFullPath* pPath = (SoFullPath*)pathList[idx];
        PoLinearAxis* targetNode = (PoLinearAxis*)pPath->getTail();
        selfObject->_pd->updateAxisInfo( targetNode );
      }
    }
  }

  static void __postAxisRebuildCB( void* userData, PoBase* base ) {
    InvBoudingAxis* selfObject = static_cast<InvBoudingAxis*>( userData );
    if( selfObject ) {
      SoAppearanceKit* appearanceKit = SO_GET_PART( base, "mainGradTextApp", SoAppearanceKit );
      appearanceKit->set( "material { diffuseColor 1 1 1 }" );
      SoFont* fontNode = SO_GET_PART( appearanceKit, "font", SoFont );
      fontNode->name = "Arial:Bold";
      fontNode->size = 12;
      fontNode->renderStyle = SoFont::TEXTURE;

      SoGroup* groupNode = SO_GET_PART( base, "mainGradText", SoGroup  );
      selfObject->_pd->replaceTextNode( groupNode, SoText2::CENTER );

      appearanceKit = SO_GET_PART( base, "titleApp", SoAppearanceKit );
      appearanceKit->set( "material { diffuseColor 1 1 1 }" );
      fontNode = SO_GET_PART( appearanceKit, "font", SoFont );
      fontNode->name = "Arial:Bold";
      fontNode->size = 16;
      fontNode->renderStyle = SoFont::TEXTURE;

      groupNode = SO_GET_PART( base, "title", SoGroup  );
      selfObject->_pd->replaceTextNode( groupNode, SoText2::LEFT );
    }
  }

public:

  InvBoudingAxis*         m_self;
  SoShapeKit*             m_bboxNode;
  PoGroup6Axis3*          m_axisNode;
  SoTransform*            m_transNode;

  SbBox3f                 m_values;
  SbBox3f                 m_extent;

  SbVec3f                 m_tickLength;
};

InvBoudingAxis::InvBoudingAxis()
  : InvShapeSceneNode(), _pd( new InvBoudingAxisPrivate( this ) )
{
  _pd->init();
}

InvBoudingAxis::~InvBoudingAxis()
{
}

void InvBoudingAxis::setValues( const SbBox3f& values )
{
  if( _pd->m_values != values ) {
    _pd->m_values = values;
    _pd->update();
  }
}

void InvBoudingAxis::setExtent( const SbBox3f& extent )
{
  if( _pd->m_extent != extent ) {
    _pd->m_extent = extent;
    _pd->update();
  }
}
