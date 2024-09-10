#ifndef qtAdskStyleProxy_h
#define qtAdskStyleProxy_h

// ===========================================================================
// Copyright 2017 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================

#include <QtWidgets/QStylePlugin>
#include <QtWidgets/QProxyStyle>


// Note: this example is a Qt style plugin and not a Maya plugin.
class qtAdskStyleProxyPlugin : public QStylePlugin
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QStyleFactoryInterface" FILE "qtAdskStyleProxy.json")

public:
	QStyle *create(const QString &key) override;
};


// Create a Qt style by wrapping another style allowing us to dynamically
// override QStyle elements.
class qtAdskStyleProxy : public QProxyStyle
{
	Q_OBJECT

public:
	qtAdskStyleProxy();

	QPalette standardPalette() const override;

	void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const override;
	int pixelMetric(PixelMetric metric, const QStyleOption *option = 0, const QWidget *widget = 0) const override;
};


#endif // qtAdskStyleProxy_h
