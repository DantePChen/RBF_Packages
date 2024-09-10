#ifndef _uiInfoHandler
#define _uiInfoHandler

// ===========================================================================
// Copyright 2020 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================

#include "common/ufeExport.h"

#include "sceneItem.h"
#include "types.h"

#include <string>

UFE_NS_DEF {

//! \brief Structure for information to draw a text cell.
/*!

  Provides a method where editors that are displaying UFE data can query for
  cell information (such as font or text color) used when displaying an item.
*/
struct UFE_SDK_DECL CellInfo {

    CellInfo() = default;
    CellInfo(
        const std::string& argFontName,
        double             argFontPointSize,
        bool               argFontBold,
        bool               argFontItalics,
        bool               argFontStrikeout,
        const Color3f&     argTextFgColor,
        const Color3f&     argTextBgColor
    );
    CellInfo(const CellInfo&) = default;

    //! Font family name.
    std::string fontName;

    //! Font point size.
    double fontPointSize{-1.0};

    //! Use bold font?
    bool fontBold{false};

    //! Use italics font?
    bool fontItalics{false};

    //! Use strikeout font?
    bool fontStrikeout{false};

    //! Text foreground color
    Color3f textFgColor;

    //! Text background color
    Color3f textBgColor;
};

/*!
  This base class defines an interface that runtimes can implement to provide
  UI related information.
*/

class UFE_SDK_DECL UIInfoHandler
{
public:
    typedef std::shared_ptr<UIInfoHandler> Ptr;

    enum Quadrant {None, UpperLeft, UpperRight, LowerLeft, LowerRight};

    enum Mode
    {
        Normal,     /*!< Icon state when available (not active and not selected). */
        Disabled,   /*!< Icon state when not available. */
        Active,     /*!< Icon state when highlighted or focused, but not selected. */
        Selected    /*!< Icon state when selected. */
    };

    struct Icon {
        Icon() {}
        Icon(const std::string& baseIconArg)
            : baseIcon(baseIconArg) {}
        Icon(const std::string& baseIconArg, Mode modeArg)
            : baseIcon(baseIconArg), mode(modeArg) {}
        Icon(const std::string& baseIconArg, const std::string& badgeIconArg, Quadrant posArg)
            : baseIcon(baseIconArg), badgeIcon(badgeIconArg), pos(posArg) {}

        std::string baseIcon;
        std::string badgeIcon;
        Quadrant pos{None};
        Mode mode{Normal};
    };

    //! Convenience method to retrieve the UI Info Handler from the input runtime id.
    //! The handler interface will remain valid as long as the given runtime
    //! remains set in the runtime manager.
    static Ptr uiInfoHandler(Rtid rt);

    //! Constructor.
    UIInfoHandler() = default;
    //! Default copy constructor.
    UIInfoHandler(const UIInfoHandler&) = default;
    //! Destructor.
    virtual ~UIInfoHandler() = default;

    //! Get the cell information for the input scene item. This cell information
    //! can be used when displaying the item in a tree view editor.
    //! It is expected that the input cell info will contain the default tree drawing
    //! info and in the override method you can change any of these values.
    //! \param[in] item The scene item to get cell info for
    //! \param[in,out] info The cell info
    //! \return True, if any cell info values were changed, otherwise false.
    virtual bool treeViewCellInfo(const SceneItem::Ptr& item, CellInfo& info) const = 0;

    //! Get the icon name with optional badge icon (draw overtop) for the input
    //! scene item. This can be used to load an icon for display in a tree view editor.
    //! The implementation in this class returns a default Icon struct
    //! (empty strings) which signifies that no special icon exists for this item.
    //! Note: it is allowed to return just a base icon, just a badge icon or both.
    //! \return Icon struct
    virtual Icon treeViewIcon(const SceneItem::Ptr& item) const;

    //! Get an extra tooltip string for the input scene item. This extra tooltip
    //! will normally be displayed after any base tooltip provided by the tree
    //! view itself. The tooltip string can be formatted using HTML 4 markup.
    //! The implementation in this class returns an empty string.
    //! \return Tooltip extra string or empty string.
    virtual std::string treeViewTooltip(const SceneItem::Ptr& item) const;

    //! Get a long (more descriptive) label for the runtime. This label
    //! (possibly localized) can be displayed in the UI.
    virtual std::string getLongRunTimeLabel() const = 0;
};

}

#endif // _uiInfoHandler
