#ifndef InvSphereSceneNode_HPP
#define InvSphereSceneNode_HPP

#include "InvShapeSceneNode.hpp"

class InvSphereSceneNode : public InvShapeSceneNode
{
public:

  InvSphereSceneNode();
  ~InvSphereSceneNode();

protected:

  virtual void processCallback( SoAction* currentState );

private:

  class InvSphereSceneNodePrivate;
  QScopedPointer<InvSphereSceneNodePrivate>         _pd;

};

#endif // InvSphereSceneNode_H