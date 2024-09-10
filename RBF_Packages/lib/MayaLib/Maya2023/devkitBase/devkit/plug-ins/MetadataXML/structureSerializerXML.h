#ifndef structureSerializerXML_h
#define structureSerializerXML_h

#include <maya/adskDataStructureSerializer.h>	// for base class
#include <maya/adskCommon.h>

namespace adsk {
  namespace Data {
	class Structure;
  }
}

// ****************************************************************************
/*!
	\class structureSerializerXML
 	\brief Class handling the data structure format type "XML"

	The XML format is an example of a plug-in that creates a new Structure
	serialization type. The initializePlugin method creates a serializer
	information object which automatically registers it so that it
	becomes available anywhere the serialization type is referenced (by name).

	The "XML" format is a structure format using XML syntax. Its format
	is explicitly defined in the accompanying file structureSchema.xsd but
	here's a quick summary of what it contains.

		<?xml version='1.0' encoding='UTF-8'?>
		<structure>
			<name>THE_STRUCTURE_NAME</name>
			<member>							   <-- one or more of these
				<name>THE_MEMBER_NAME</name>
				<dim>3</dim>
				<type>float</type>
			</member>
		</structure>
	
	"dim" is the dimension of the member, e.g. the above is a float[3]
	Default value if unspecified is 1.

	The "type" value must be one of the legal structure member types:
			bool double dbl4x4 float flt4x4 int8 int16 int32 int64
			string uint8 uint16 uint32 uint64
*/
using namespace adsk::Data;
class StructureSerializerXML : public StructureSerializer
{
	DeclareSerializerFormat(StructureSerializerXML, StructureSerializer);
public:
	~StructureSerializerXML() override;

	// Mandatory implementation overrides
	Structure*	read	(std::istream&		cSrc,
								 std::string&		errors)		const override;
	int			write	(const Structure&	dataToWrite,
								 std::ostream&		cDst)		const override;
	void	getFormatDescription(std::ostream&	info)		const override;

private:
	StructureSerializerXML();		//! Use theXMLFormat() to create.
};

//-
// ==================================================================
// Copyright 2015 Autodesk, Inc.  All rights reserved.
// 
// This computer source code  and related  instructions and comments are
// the unpublished confidential and proprietary information of Autodesk,
// Inc. and are  protected  under applicable  copyright and trade secret
// law. They may not  be disclosed to, copied or used by any third party
// without the prior written consent of Autodesk, Inc.
// ==================================================================
//+
#endif // structureSerializerXML_h
