#ifndef ABSTRACTTEXTUREIMAGE_HPP
#define ABSTRACTTEXTUREIMAGE_HPP

#include <QObject>

#include <QImage>
#include <QVector2D>

class AbstractTextureImage : public QObject
{
  Q_OBJECT;

public:

  AbstractTextureImage( QObject* parent=0 );
  virtual ~AbstractTextureImage();

  virtual QImage generateTextureImage( const QVector2D& depthRange,
                                       const QSize& imageSize ) = 0;
private:

  class AbstractTextureImagePrivate;
  QScopedPointer<AbstractTextureImagePrivate>         _pd;

};

#endif // ABSTRACTTEXTUREIMAGE_HPP
