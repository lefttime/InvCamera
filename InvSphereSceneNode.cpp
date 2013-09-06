#include "InvSphereSceneNode.hpp"

#include <Inventor/SbViewVolume.h>

#include <Inventor/nodes/SoSphere.h>

#include <Inventor/nodes/SoCallback.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoComplexity.h>

#include <Inventor/actions/SoGLRenderAction.h>

#include <Inventor/elements/SoViewVolumeElement.h>
#include <Inventor/elements/SoModelMatrixElement.h>

class InvSphereSceneNode::InvSphereSceneNodePrivate
{
public:

  InvSphereSceneNodePrivate( InvSphereSceneNode* me ) : m_self( me ) {
  }

  void init() {
    m_materialNode = new SoMaterial;
    m_self->addChild( m_materialNode );

    m_drawStyleNode = new SoDrawStyle;
    m_drawStyleNode->style = SoDrawStyle::LINES;
    m_self->addChild( m_drawStyleNode );

    m_complexityNode = new SoComplexity;
    m_self->addChild( m_complexityNode );

    m_shapeNode = new SoSphere;
    m_self->addChild( m_shapeNode );
  }

  void processGLRenderAction( SoAction* action ) {
    SoState* currentState = action->getState();
    const SbViewVolume& viewvol = SoViewVolumeElement::get( currentState );
    SbMatrix csMatrix = viewvol.getCameraSpaceMatrix().inverse();
    SbVec3f cameraPosition(csMatrix[3][0], csMatrix[3][1], csMatrix[3][2]);

    SbVec3f currentPosition;
    const SbMatrix& modelMtx = SoModelMatrixElement::get( currentState );
    modelMtx.multVecMatrix( SbVec3f(0,0,0), currentPosition );

    float shapeLength = m_shapeNode->radius.getValue() * 2;
    float viewDistance= (currentPosition-cameraPosition).length();
    if( viewDistance < shapeLength * 2 ) {
      m_complexityNode->value = 1.0f;
    } else if( viewDistance < shapeLength * 4 ) {
      m_complexityNode->value = 0.5f;
    } else {
      m_complexityNode->value = 0.1f;
    }

  }

  InvSphereSceneNode*         m_self;

  SoSphere*                   m_shapeNode;
  SoMaterial*                 m_materialNode;
  SoDrawStyle*                m_drawStyleNode;
  SoComplexity*               m_complexityNode;
};

InvSphereSceneNode::InvSphereSceneNode()
  : InvShapeSceneNode(), _pd( new InvSphereSceneNodePrivate( this ) )
{
  _pd->init();
}

InvSphereSceneNode::~InvSphereSceneNode()
{
}

void InvSphereSceneNode::processCallback( SoAction* action )
{
  if( action->isOfType(SoGLRenderAction::getClassTypeId()) ) {
    _pd->processGLRenderAction( action );
  }
}
