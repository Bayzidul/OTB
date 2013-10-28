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

#include "otbMacro.h"

#include "otbPostGISConnectionImplementation.h"

int otbPostGISConnectionImplementationNew(int argc, char * argv[])
{

  typedef otb::PostGISConnectionImplementation GISConnectionType;

  //Instantiation
  GISConnectionType::Pointer data = GISConnectionType::New();

  std::cout << data << std::endl;

  return EXIT_SUCCESS;
}