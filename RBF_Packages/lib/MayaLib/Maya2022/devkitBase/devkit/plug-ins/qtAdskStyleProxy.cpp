// ===========================================================================
// Copyright 2017 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================

#include <qtAdskStyleProxy.h>
#include <QtWidgets/QStyleFactory>
#include <QtWidgets/QStyleOption>
#include <QtWidgets/qdrawutil.h>
#include <QtGui/QPalette>
#include <maya/MGlobal.h>
#include <maya/MQtUtil.h>

////////////////////////////////////////////////////////////////////////////////
//
//	qtAdskStyleProxyPlugin class
//
////////////////////////////////////////////////////////////////////////////////

QStyle *qtAdskStyleProxyPlugin::create(const QString &key)
{
	// This is the name (of the style) used to create our style proxy which we can use to
	// override much of Maya UI drawn from the style.
	if (key.toLower() == "qtadskstyleproxy")
		return new qtAdskStyleProxy();

	return nullptr;
}

////////////////////////////////////////////////////////////////////////////////
//
//	qtAdskStyleProxy class
//
////////////////////////////////////////////////////////////////////////////////

qtAdskStyleProxy::qtAdskStyleProxy()
	// Our style wraps the Maya style 'adskdarkflatui' so we can override
	// painting or other specific style behavior. The Maya style is available
	// in the plugin '.../plugins/styles/AdskStyle'.
	: QProxyStyle(QStyleFactory::create(QLatin1String("adskdarkflatui")))
{
}

QPalette qtAdskStyleProxy::standardPalette() const
{
	// First call our base class, which will call into the internal
	// adskdarkflatui style class.
	QPalette pal = QProxyStyle::standardPalette();

	// Then we can customize the palette as needed.

	// Make the general background a bit darker than normal Maya.
	pal.setBrush(QPalette::Disabled, QPalette::Window,     QColor(45,45,45));
	pal.setBrush(QPalette::Active,   QPalette::Window,     QColor(45,45,45));
	pal.setBrush(QPalette::Inactive, QPalette::Window,     QColor(45,45,45));

	pal.setBrush(QPalette::Active,   QPalette::WindowText, QColor(133,133,133));
	pal.setBrush(QPalette::Inactive, QPalette::WindowText, QColor(133,133,133));

	pal.setBrush(QPalette::Active,   QPalette::ButtonText, QColor(133,133,133));
	pal.setBrush(QPalette::Inactive, QPalette::ButtonText, QColor(133,133,133));

	pal.setBrush(QPalette::Active,   QPalette::Text,       QColor(133,133,133));
	pal.setBrush(QPalette::Inactive, QPalette::Text,       QColor(133,133,133));

	return pal;
}

//
// You can override any of the virtual methods from the base class 'QProxyStyle'.
// This enables you to customize much of the look/drawing of Maya.
//

void qtAdskStyleProxy::drawControl(ControlElement element, const QStyleOption *option,
									QPainter *painter, const QWidget *widget) const
{
	return QProxyStyle::drawControl(element, option, painter, widget);
}

int qtAdskStyleProxy::pixelMetric(PixelMetric metric, const QStyleOption *option, const QWidget *widget) const
{
	int ret = -1;
	switch (metric)
	{
	case PM_MenuTearoffHeight:
		// Maya sets this value to 16 - let's make them a bit larger
		ret = MQtUtil::dpiScale(20);
		break;

	default:
		break;
	}

	return (ret != -1) ? ret : QProxyStyle::pixelMetric(metric, option, widget);
}
