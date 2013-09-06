#ifndef INVSHAPESCENENODE_HPP
#define INVSHAPESCENENODE_HPP

#include <Inventor/nodes/SoSeparator.h>

#include <QScopedPointer>

class SoAction;

class InvShapeSceneNode : public SoSeparator
{
public:

  InvShapeSceneNode();
  ~InvShapeSceneNode();

protected:

  virtual void processCallback( SoAction* );

private:

  class InvShapeSceneNodePrivate;
  QScopedPointer<InvShapeSceneNodePrivate>         _pd;

};

#endif // INVSHAPESCENENODE_HPP