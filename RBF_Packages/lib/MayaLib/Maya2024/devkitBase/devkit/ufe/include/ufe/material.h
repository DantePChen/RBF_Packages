#ifndef _ufeMaterial
#define _ufeMaterial
// ===========================================================================
// Copyright 2022 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================

#include "sceneItem.h"

#include <vector>

UFE_NS_DEF {

//! \brief Abstract base class for Material interface.
/*!

  This base class defines the interface that runtimes can implement to
  provide basic material behavior, e.g. querying the materials assigned
  to a SceneItem.

  To avoid the memory-consuming "one proxy object per scene object" approach,
  Material interface objects should be obtained and used within a local
  scope, and not stored.  Material interfaces should be considered
  stateless, and can be bound to new selection items.
*/

class UFE_SDK_DECL Material
{
public:
    typedef std::shared_ptr<Material> Ptr;
    
    /*!
        Convenience method that calls the material method on the 
        Material handler for the item.  Returns a null pointer if the
        argument is null, or has an empty path.
        \param item SceneItem for which Material interface is desired.
        \return Material interface of the given SceneItem.
    */
    static Ptr material(const SceneItem::Ptr& item);

    //! Constructor.
    Material();
    //! Default copy constructor.
    Material(const Material&) = default;
    //! Destructor.
    virtual ~Material();

    //! Returns all the materials assigned to this scene item.
    virtual std::vector<SceneItem::Ptr> getMaterials() const = 0;
};

}

#endif /* _ufeMaterial */
