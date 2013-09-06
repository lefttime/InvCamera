#include "InvExtrusionSceneNode.hpp"

#include "Util.hpp"
#include "AbstractTextureImage.hpp"

#include <QtDebug>
#include <Qt3D/qvector2darray.h>

#include <Inventor/nodes/SoTexture2.h>
#include <Inventor/nodes/SoExtrusion.h>
#include <Inventor/nodes/SoTextureCoordinate2.h>

#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>

#include <Inventor/elements/SoViewVolumeElement.h>
#include <Inventor/elements/SoModelMatrixElement.h>

class InvExtrusionSceneNode::InvExtrusionSceneNodePrivate
{
public:

  InvExtrusionSceneNodePrivate( InvExtrusionSceneNode* me,
                                const QVector3DArray& trajectory,
                                const QVector<qreal>& scales )
    : m_self( me ),
    m_trajectory( trajectory ),
    m_scales( scales ),
    m_textureImage( 0 ),
    m_resolution( 0 ) {
    float totalLength = 0;
    m_textureValues = QVector<qreal>( trajectory.count(), 0 );
    for( int idx = 1; idx < trajectory.count(); ++idx ) {
      totalLength += (trajectory.at( idx-1 ) - trajectory.at( idx )).length();
      m_textureValues[idx] = totalLength;
    }
    for( int idx = 1; idx < m_textureValues.count(); ++idx ) {
      m_textureValues[idx] /= totalLength;
    }
  }

  void init() {
    m_textureCoordsNode = new SoTextureCoordinate2;
    m_self->addChild( m_textureCoordsNode );

    m_textureNode = new SoTexture2;
    m_self->addChild( m_textureNode );

    m_shapeNode = new SoExtrusion;
    m_shapeNode->endCap = false;
    m_shapeNode->beginCap = false;
    m_self->addChild( m_shapeNode );

    rebuild();
  }

  void rebuild( float resolution=1.0f ) {
    if( !m_trajectory.isEmpty() ) {
      if( resolution != m_resolution ) {
        int interval = qMax( 1.0f, m_trajectory.count() / (m_trajectory.count() * resolution) );
        QVector<int> indexes = generateIndexes( interval );
        rebuildSpine( indexes );
        rebuildScale( indexes );
        rebuildCrossSection();
        if( m_textureImage ) {
          rebuildTexture();
          rebuildTextureCoords( indexes );
        }
        m_resolution = resolution;
      }
    } else {
      clear();
    }
  }

  void clear() {
    m_shapeNode->spine.deleteValues( 0 );
    m_shapeNode->scale.deleteValues( 0 );
    m_shapeNode->crossSection.deleteValues( 0 );

    m_textureCoordsNode->point.deleteValues( 0 );
  }

  QVector<int> generateIndexes( int interval ) {
    QVector<int> result;

    int ptCount = m_trajectory.count() / interval;
    for( int idx = 0; idx < ptCount; ++idx ) {
      result << idx * interval;
    }
    if( !result.contains( m_trajectory.count()-1 ) ) {
      result << m_trajectory.count()-1;
    }

    return result;
  }

  void rebuildSpine( const QVector<int>& indexes ) {
    m_shapeNode->spine.setNum( indexes.count() );
    SbVec3f* spinePtr = m_shapeNode->spine.startEditing();
    for( int idx = 0; idx < indexes.count(); ++idx ) {
      *spinePtr++ = Util::toSbVec3f( m_trajectory.at( indexes.at( idx ) ) );
    }
    m_shapeNode->spine.finishEditing();
  }

  void rebuildScale( const QVector<int>& indexes ) {
    m_shapeNode->scale.setNum( indexes.count() );
    SbVec2f* scalePtr = m_shapeNode->scale.startEditing();
    for( int idx = 0; idx < indexes.count(); ++idx ) {
      float scale = m_scales.at( indexes.at( idx ) );
      *scalePtr++ = SbVec2f(scale, scale);
    }
    m_shapeNode->scale.finishEditing();
  }

  void rebuildCrossSection( int numSides=12 ) {
    numSides = qMax( 2, numSides );
    m_shapeNode->crossSection.setNum( numSides+1 );

    float dTheta = (float)(2.0 * M_PI / (double)numSides);
    const float eps = 1e-6f;
    for( int side = 0; side < numSides; ++side ) {
      float theta  = dTheta * side;
      float x = 0.5f * sin( theta );
      float z = 0.5f * cos( theta );
      if(fabs(x) < eps) x = 0;
      if(fabs(z) < eps) z = 0;
      m_shapeNode->crossSection.set1Value( side, SbVec2f(x, z) );
    }
    m_shapeNode->crossSection.set1Value( numSides, SbVec2f(0, 0.5f) ); // close loop
  }

  void rebuildTextureCoords( const QVector<int>& indexes ) {
    int sTotal = indexes.count();
    int tTotal = m_shapeNode->crossSection.getNum();
    m_textureCoordsNode->point.setNum( sTotal * tTotal );

    QVector<qreal> tTextureValues( tTotal );
    for( int tIdx = 0; tIdx < tTotal; ++tIdx ) {
      tTextureValues[tIdx] = float(tIdx) / (tTotal-1);
    }

    SbVec3f* spinePtr = m_shapeNode->spine.startEditing();
    SbVec2f* pointPtr = m_textureCoordsNode->point.startEditing();
    for( int sIdx = 0; sIdx < sTotal; ++sIdx ) {
      float sVal = m_textureValues.at( indexes.at( sIdx ) );
      for( int tIdx = 0; tIdx < tTotal; ++tIdx ) {
        int offset = sIdx * tTotal + tIdx;
        float tVal = tTextureValues.at( tIdx );
        pointPtr[offset] = SbVec2f(sVal, tVal);
      }
    }
    m_textureCoordsNode->point.finishEditing();
    m_shapeNode->spine.finishEditing();
  }

  void rebuildTexture() {
    QImage image = m_textureImage->generateTextureImage(QVector2D(0, 1), QSize(0, 1));
    Util::toSoSFImage( image, m_textureNode->image );
  }

  void processGLRenderAction( SoGLRenderAction* action ) {
    const SbMatrix& modelMtx = SoModelMatrixElement::get( action->getState() );

    SoGetBoundingBoxAction gbAction( action->getViewportRegion() );
    gbAction.apply( m_shapeNode );
    SbBox3f bbox = gbAction.getBoundingBox();

    SbVec3f boxMin = bbox.getMin();
    SbVec3f boxMax = bbox.getMax();
    modelMtx.multVecMatrix( boxMin, boxMin );
    modelMtx.multVecMatrix( boxMax, boxMax );
    bbox.setBounds( boxMin, boxMax );

    SbVec3f objectPosition = bbox.getCenter();
    SbVec3f bSize = bbox.getSize();
    float shapeSize = qMax( bSize[0], qMax( bSize[1], bSize[2] ) );

    const SbViewVolume& viewvol = SoViewVolumeElement::get( action->getState() );
    SbMatrix csMatrix = viewvol.getCameraSpaceMatrix().inverse();
    SbVec3f cameraPosition(csMatrix[3][0], csMatrix[3][1], csMatrix[3][2]);

    float viewDistance= (objectPosition-cameraPosition).length();
    //qDebug() << "-------->InvExtrusionSceneNode::InvExtrusionSceneNodePrivate::processGLRenderAction";
    //qDebug() << "shapeSize: " << shapeSize;
    //qDebug() << "viewDistance: " << viewDistance;
    //qDebug() << "-----------------------------------------------------------------------------------\n";
    if( viewDistance < shapeSize * 0.5f ) {
      rebuild( 0.8f );
    } else if( viewDistance < shapeSize ) {
      rebuild( 0.1f );
    } else {
      rebuild( 0.01f );
    }
  }

  InvExtrusionSceneNode*         m_self;
  QVector3DArray                 m_trajectory;
  QVector<qreal>                 m_scales;
  QVector<qreal>                 m_textureValues;

  SoExtrusion*                   m_shapeNode;
  SoTexture2*                    m_textureNode;
  AbstractTextureImage*          m_textureImage;
  SoTextureCoordinate2*          m_textureCoordsNode;

  float                          m_resolution;
};

InvExtrusionSceneNode::InvExtrusionSceneNode( const QVector3DArray& trajectory,
  const QVector<qreal>& scales )
  : InvShapeSceneNode(),
  _pd( new InvExtrusionSceneNodePrivate( this, trajectory, scales ) )
{
  _pd->init();
}

InvExtrusionSceneNode::~InvExtrusionSceneNode()
{

}

void InvExtrusionSceneNode::processCallback( SoAction* action )
{
  if( action->isOfType(SoGLRenderAction::getClassTypeId()) ) {
    _pd->processGLRenderAction( (SoGLRenderAction*)action );
  }
}

void InvExtrusionSceneNode::setTextureImage( AbstractTextureImage* textureImage )
{
  _pd->m_textureImage = textureImage;
}
