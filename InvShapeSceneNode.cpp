#include "InvShapeSceneNode.hpp"

#include <Inventor/nodes/SoCallback.h>

class InvShapeSceneNode::InvShapeSceneNodePrivate
{
public:

  InvShapeSceneNodePrivate( InvShapeSceneNode* me ) : m_self( me ) {
  }

  void init() {
    SoCallback* callbackNode = new SoCallback();
    callbackNode->setCallback( callbackCB, m_self );
    m_self->addChild( callbackNode );

  }

  static void callbackCB( void* userData, SoAction* action ) {
    InvShapeSceneNode* selfObject = static_cast<InvShapeSceneNode*>( userData );
    if( selfObject ) {
      selfObject->processCallback( action );
    }
    //if( selfObject && action->isOfType(SoGLRenderAction::getClassTypeId()) ) {
    //  SoState* currentState = action->getState();
    //  selfObject->processCallback( currentState );
    //}
  }

  InvShapeSceneNode*         m_self;
};

InvShapeSceneNode::InvShapeSceneNode()
  : SoSeparator(), _pd( new InvShapeSceneNodePrivate( this ) )
{
  _pd->init();
}

InvShapeSceneNode::~InvShapeSceneNode()
{
}

void InvShapeSceneNode::processCallback( SoAction* )
{
}
