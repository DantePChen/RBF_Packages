// ==================================================================
//
// Copyright (c) 2014 Autodesk, Inc.
// All rights reserved.
//
// This computer source code and related instructions and comments are the
// unpublished confidential and proprietary information of Autodesk, Inc.
// and are protected under applicable copyright and trade secret law.
// They may not be disclosed to, copied or used by any third party without
// the prior written consent of Autodesk, Inc.
// ==================================================================
//
//!
//! \file synColor/roi.h
//!
//! \brief Collection of classes for defining image regions.
//!

#ifndef _SYNCOLOR_ROI_H
#define _SYNCOLOR_ROI_H

#include <synColor/synExport.h>
#include <synColor/synColorNamespace.h>

SYN_VISIBILITY_PUSH

namespace SYNCOLOR {

  //--------------------------------------------------------------------------
  //!
  //! \brief Region of interest class.
  //!
  //!  The ROI class describes the region of interest within an image onto
  //!  which to apply a color transformation.  It defines a rectangle with
  //!  bottom left at (x,y) of size (width, height) within a rectangle with
  //!  bottom left at (0,0) of size (imageWidth, imageHeight).
  //!
  class SYN_EXPORT ROI
  {
  public:
    //! \brief Detailed ROI constructor.
	//!
	//! This constructor assumes that x+width <= imageWidth and
	//! y+height <= imageHeight.  No validation is performed.
    //!
	// Note: The default copy constructor and the assignment operator are allowed.
    //
    ROI(unsigned x,           //!< Bottom left corner x offset.
        unsigned y,           //!< Bottom left corner y offset.
        unsigned width,       //!< ROI width.
        unsigned height,      //!< ROI height.
        unsigned imageWidth,  //!< Image width.
        unsigned imageHeight  //!< Image height.
        );

    //! \brief Constructor of an ROI that corresponds to the entire image.
    //!
    //! This constructor sets x=0, y=0, width = imageWidth and
    //! height = imageHeight.
    //!
    ROI(unsigned imageWidth,       //!< ROI and image width.
        unsigned imageHeight       //!< ROI and image height.
        );

    //! Destructor.
    ~ROI();

    //! \brief Number of pixels in the ROI.
    //! \return the number of pixels within the region of interest.
    inline unsigned getNumROIPixels() const
    { return _width * _height; }

    //! \brief Bottom left corner x offset.
    //! \return the x offset.
    inline unsigned getX() const
    { return _x; }

    //! \brief Bottom left corner y offset.
    //! \return the y offset.
    inline unsigned getY() const
    { return _y; }

    //! \brief ROI width.
    //! \return the ROI width.
    inline unsigned getActiveWidth() const
    { return _width; }
    
    //! \brief ROI height.
    //! \return the ROI height.
    inline unsigned getActiveHeight() const
    { return _height; }
    
    //! \brief Image width.
    //! \return the width of the image containing the ROI.
    inline unsigned getImageWidth() const
    { return _imageWidth; }
    
    //! \brief Image height.
    //! \return the height of the image containing the ROI.
    inline unsigned getImageHeight() const
    { return _imageHeight; }

  private:
    unsigned _x;           //!< Bottom left corner x offset.
    unsigned _y;           //!< Bottom left corner y offset
    unsigned _width;       //!< ROI width.
    unsigned _height;      //!< ROI height.
    unsigned _imageWidth;  //!< Width of the image containing the ROI.
    unsigned _imageHeight; //!< Height of the image containing the ROI.
  };

  //--------------------------------------------------------------------------
  //!
  //! \brief Rectangle class
  //!
  //!  This rectangle class is use to define an image.  The image origin (x,y)
  //!  is assumed to be at the bottom left as is typical in graphical
  //!  coordinate systems.
  //!
  //  Note: The default copy constructor and the assignment operator are allowed.
  //
  class SYN_EXPORT Rect
  {
  public:
    //! Constructor  
    Rect(float x,       //!< Origin x coordinate.
         float y,       //!< Origin y coordinate.
         float width,   //!< Rectangle width.
         float height   //!< Rectangle height.
         );

    //! Destructor
    ~Rect();

    //! \brief Origin x coordinate.
    //! \return the origin's x coordinate.
    inline float getX() const
    { return _x; }

    //! \brief Origin y coordinate.
    //! \return the origin's y coordinate.
    inline float getY() const
    { return _y; }

    //! \brief Rectangle width.
    //! \return the rectangle width.
    inline float getWidth() const
    { return _width; }
    
    //! \brief Rectangle height.
    //! \return the rectangle height.
    inline float getHeight() const
    { return _height; }
    
  private:
    float _x;        //!< Origin x coordinate.
    float _y;        //!< Origin y coordinate.
    float _width;    //!< Rectangle width.
    float _height;   //!< Rectangle height.
  };

  //----------------------------------------------------------------------------
  //
  //! \brief Collection of 4 vertices describing a rectangle.
  //
  //! The ordering of vertices is counterclockwise starting from the bottom left.
  //!
  //  Note: The default copy constructor and the assignment operator are allowed.
  //
  class SYN_EXPORT Vertices
  {
  public:
    //! Constructor using vertex coordinates.
    Vertices(float x1, //!< Bottom left x.
             float y1, //!< Bottom left y.
             float x2, //!< Top left x.
             float y2, //!< Top left y.
             float x3, //!< Top right x.
             float y3, //!< Top right y.
             float x4, //!< Bottom right x.
             float y4  //!< Bottom right y.
             );

    //! Constructor using a SYNCOLOR::Rect.
    Vertices(const Rect& rect //!< Rect to convert to Vertices.
             );

    //! Destructor.
    ~Vertices();

    // @{
    //! \name Vertex coordinate accessor methods.
    inline float getX1() const { return _x1; }
    inline float getX2() const { return _x2; }
    inline float getX3() const { return _x3; }
    inline float getX4() const { return _x4; }

    inline float getY1() const { return _y1; }
    inline float getY2() const { return _y2; }
    inline float getY3() const { return _y3; }
    inline float getY4() const { return _y4; }
    // @}

  private:
    float _x1; //!< Bottom left x.
    float _y1; //!< Bottom left y.
    float _x2; //!< Top left x.
    float _y2; //!< Top left y.
    float _x3; //!< Top right x.
    float _y3; //!< Top right y.
    float _x4; //!< Bottom right x.
    float _y4; //!< Bottom right y.
  };
};

SYN_VISIBILITY_POP

#endif
