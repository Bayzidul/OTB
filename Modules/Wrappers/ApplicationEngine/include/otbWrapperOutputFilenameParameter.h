/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef otbWrapperOutputFilenameParameter_h
#define otbWrapperOutputFilenameParameter_h

#include <string>
#include "otbWrapperStringParameter.h"

namespace otb
{
namespace Wrapper
{

/** \class OutputFilenameParameter
 *  \brief This class represent a string parameter for the wrapper framework
 *
 * \ingroup OTBApplicationEngine
 */
class ITK_ABI_EXPORT OutputFilenameParameter
  : public Parameter
{
public:
  /** Standard class typedef */
  typedef OutputFilenameParameter        Self;
  typedef StringParameter               Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Defining ::New() static method */
  itkNewMacro(Self);

  /** RTTI support */
  itkTypeMacro(OutputFilenameParameter, Parameter);


  bool HasValue() const ITK_OVERRIDE
  {
    return m_StringParam->HasValue();
  }

  // Get Value
  otbGetObjectMemberMacro(StringParam, Value , std::string);

  // Set Value
  virtual void SetValue(const std::string value)
    {
    itkDebugMacro("setting member m_StringParam to " << value);
    this->m_StringParam->SetValue(value);
    SetActive(true);
    this->Modified();
    }

  // Clear Value
  void ClearValue() ITK_OVERRIDE
  {
    m_StringParam->ClearValue();
  }

protected:
  /** Constructor */
  OutputFilenameParameter()
  {
    // Initialize the String Parameter
    m_StringParam= StringParameter::New();


  }

  /** Destructor */
  ~OutputFilenameParameter() ITK_OVERRIDE
  {}

private:
  OutputFilenameParameter(const OutputFilenameParameter &); //purposely not implemented
  void operator =(const OutputFilenameParameter&); //purposely not
                                                  //implemented

  StringParameter::Pointer m_StringParam;

}; // End class Parameter

} // End namespace Wrapper
} // End namespace otb

#endif
