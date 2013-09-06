#ifndef GENERICTEXTUREIMAGE_HPP
#define GENERICTEXTUREIMAGE_HPP

#include "AbstractTextureImage.hpp"

class GenericTextureImage : public AbstractTextureImage
{
  Q_OBJECT;

public:

  GenericTextureImage( const QImage& image, QObject* parent=0 );
  ~GenericTextureImage();

  QImage generateTextureImage( const QVector2D& depthRange,
                               const QSize& imageSize );

private:

  class GenericTextureImagePrivate;
  QScopedPointer<GenericTextureImagePrivate>         _pd;

};

#endif // GENERICTEXTUREIMAGE_HPP
