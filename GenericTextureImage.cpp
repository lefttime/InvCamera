#include "GenericTextureImage.hpp"

class GenericTextureImage::GenericTextureImagePrivate
{
public:

  GenericTextureImagePrivate( GenericTextureImage* me, const QImage& image )
    : m_self( me ), m_image( image ) {

  }

  QImage generateTextureImage( const QVector2D& depthRange,
                               const QSize& imageSize ) {
    return m_image;

  }

  GenericTextureImage*         m_self;
  QImage                       m_image;
};

GenericTextureImage::GenericTextureImage( const QImage& image, QObject* parent )
  : AbstractTextureImage( parent ),
  _pd( new GenericTextureImagePrivate( this, image ) )
{

}

GenericTextureImage::~GenericTextureImage()
{

}

QImage GenericTextureImage::generateTextureImage( const QVector2D& depthRange,
                                                  const QSize& imageSize )
{
  return _pd->generateTextureImage( depthRange, imageSize );

}
