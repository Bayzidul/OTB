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
#ifndef otbGDALImageIO_h
#define otbGDALImageIO_h


/* C++ Libraries */
#include <string>

/* ITK Libraries */
#include "otbImageIOBase.h"

namespace otb
{
class GDALDatasetWrapper;
class GDALDataTypeWrapper;

/** \class GDALImageIO
 *
 * \brief ImageIO object for reading and writing images with GDAL
 *
 * This ImageIO uses GDAL interface to read/write images. The
 * origin and spacing are translated from/to GDAL geotransform
 * matrix (even in the case of a sensor image). It means that
 * extracts from sensor images are well supported. Typical
 * sensor images in OTB have a spacing of [1,1] and an origin
 * at [0.5,0.5] (when there is no GDAL geotransform, GDAL
 * physical space is identical to GDAL index space).
 *
 * Note that the geotransform matrix supports any rotated physical
 * space whereas OTB doesn't.
 *
 * \em Warning : the index coordinate system used in GDAL is attached
 * to the corner of the top left pixel, whereas in OTB, the index
 * coordinate system is attached to the centre of the top-left
 * pixel. It means that the origin coefficents read from the
 * GDAL geotransform are the location of the top-left pixel
 * corner. This is why this location has to be shifted by
 * half a pixel to be used as an OTB origin. In a nutshell,
 * OTB images read/written by this ImageIO have the \em same
 * physical space as GDAL physical space : a given point of
 * image has the same physical location in OTB and in GDAL.
 *
 * The streaming read is implemented.
 *
 * \ingroup IOFilters
 *
 *
 * \ingroup OTBIOGDAL
 */
class ITK_EXPORT GDALImageIO : public otb::ImageIOBase
{
public:

  typedef unsigned char InputPixelType;

  /** Standard class typedefs. */
  typedef GDALImageIO             Self;
  typedef otb::ImageIOBase        Superclass;
  typedef itk::SmartPointer<Self> Pointer;

  typedef std::vector<std::string> GDALCreationOptionsType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(GDALImageIO, otb::ImageIOBase);

  /** Set/Get the level of compression for the output images.
   *  0-9; 0 = none, 9 = maximum. */
  itkSetMacro(CompressionLevel, int);
  itkGetMacro(CompressionLevel, int);

  /** Set/Get whether the pixel type (otb side) is complex */
  itkSetMacro(IsComplex, bool);
  itkGetMacro(IsComplex, bool);

  /** Set/Get whether the pixel type (otb side) is Vector or Scalar */
  itkSetMacro(IsVectorImage, bool);
  itkGetMacro(IsVectorImage, bool);

  /** Set/get wether the driver will write RPC tags to TIFF */
  itkSetMacro(WriteRPCTags,bool);
  itkGetMacro(WriteRPCTags,bool);

  
  /** Set/Get the options */
  void SetOptions(const GDALCreationOptionsType& opts)
  {
    m_CreationOptions = opts;
  }

  GDALCreationOptionsType GetOptions(void)
  {
    return m_CreationOptions;
  }
  
  /** Provide hist about the output container to deal with complex pixel
   *  type */ 
  void SetOutputImagePixelType( bool isComplexInternalPixelType, 
                                        bool isVectorImage) ITK_OVERRIDE
  {
    this->SetIsComplex(isComplexInternalPixelType);
    this->SetIsVectorImage(isVectorImage);
  }

  /*-------- This part of the interface deals with reading data. ------ */

  /** Determine the file type. Returns true if this ImageIO can read the
   * file specified. */
  bool CanReadFile(const char*) ITK_OVERRIDE;

  /** Determine the file type. Returns true if the ImageIO can stream read the specified file */
  bool CanStreamRead() ITK_OVERRIDE
  {
    return true;
  }

  /** Set the spacing and dimention information for the set filename. */
  void ReadImageInformation() ITK_OVERRIDE;

  /** Reads the data from disk into the memory buffer provided. */
  void Read(void* buffer) ITK_OVERRIDE;

  /** Reads 3D data from multiple files assuming one slice per file. */
  virtual void ReadVolume(void* buffer);

  /** Get Info about all subDataset in hdf file */
  bool GetSubDatasetInfo(std::vector<std::string>& names, std::vector<std::string>& desc);

  /** Get if the pixel type in the file is complex or not (GDAL side)*/
  bool GDALPixelTypeIsComplex();

  /*-------- This part of the interfaces deals with writing data. ----- */

  /** Determine the file type. Returns true if this ImageIO can read the
   * file specified. */
  bool CanWriteFile(const char*) ITK_OVERRIDE;

  /** Determine the file type. Returns true if the ImageIO can stream write the specified file */
  bool CanStreamWrite() ITK_OVERRIDE;

  /** Writes the spacing and dimentions of the image.
   * Assumes SetFileName has been called with a valid file name. */
  void WriteImageInformation() ITK_OVERRIDE;

  /** Writes the data to disk from the memory buffer provided. Make sure
   * that the IORegion has been set properly. */
  void Write(const void* buffer) ITK_OVERRIDE;

  /** Get all resolutions possible from the file dimensions */
  bool GetAvailableResolutions(std::vector<unsigned int>& res);

  /** Get Info about all resolution possible from the file dimensions  */
  bool GetResolutionInfo(std::vector<unsigned int>& res, std::vector<std::string>& desc);
  
  /** Get number of available overviews in the file
   *  Return 0 if no overviews available 
   *  Currently this overview count is only based on the first band
   *  If no pre-computed overviews are available we provide the overview
   *  count based on size division by 2*/
  unsigned int GetOverviewsCount() ITK_OVERRIDE;

  /** Get description about overviews available into the file specified */
  std::vector<std::string> GetOverviewsInfo() ITK_OVERRIDE;

  /** Returns gdal pixel type as string */
  std::string GetGdalPixelTypeAsString() const;

protected:
  /**
   * Constructor.
   * Spacing is set to [1,1] and origin to [0.5,0.5] as it would correspond
   * to an image without geotransform
   */
  GDALImageIO();
  /** Destructor.*/
  ~GDALImageIO() ITK_OVERRIDE;

  void PrintSelf(std::ostream& os, itk::Indent indent) const ITK_OVERRIDE;
  /** Read all information on the image*/
  void InternalReadImageInformation();
  /** Write all information on the image*/
  void InternalWriteImageInformation(const void* buffer);
  /** Number of bands of the image*/
  int m_NbBands;
  /** Buffer*/
  //float **pafimas;

  /** Determines the level of compression for written files.
   *  Range 0-9; 0 = none, 9 = maximum , default = 4 */
  int         m_CompressionLevel;

  bool        m_IsIndexed;

  /** Dataset index to extract (starting at 0)*/
  unsigned int m_DatasetNumber;

private:
  GDALImageIO(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  /** Determine real file name to write the image */
  std::string GetGdalWriteImageFileName(const std::string& gdalDriverShortName, const std::string& filename) const;

  std::string FilenameToGdalDriverShortName(const std::string& name) const;

  /** Parse a GML box from a Jpeg2000 file and get the origin */
  bool GetOriginFromGMLBox(std::vector<double> &origin);
  
  /** Test whether m_CreationOptions has an option
   *  \param partialOption The beginning of a creation option (for example "QUALITY=")
   */
  bool CreationOptionContains(std::string partialOption) const;

  /** GDAL parameters. */
  typedef itk::SmartPointer<GDALDatasetWrapper> GDALDatasetWrapperPointer;
  GDALDatasetWrapperPointer m_Dataset;

  GDALDataTypeWrapper*    m_PxType;
  /** Nombre d'octets par pixel */
  int m_BytePerPixel;

  bool GDALInfoReportCorner(const char * corner_name, double x, double y,
                            double& dfGeoX, double& dfGeoY) const;

  bool m_FlagWriteImageInformation;
  bool m_CanStreamWrite;

  /** Whether the pixel type (otb side, not gdal side) is complex
   * this information has to be provided by the reader */
  bool m_IsComplex;

  /** Whether the pixel type (otb side, not gdal side) is Vector
   * this information has to be provided by the reader */
  bool m_IsVectorImage;

  /**
   *  Creation Options */
  GDALCreationOptionsType m_CreationOptions;

  /**
   * Number of Overviews in the file */
  unsigned int m_NumberOfOverviews;

  /**
   * Size of the different overviews of the file */
  std::vector<std::pair<unsigned int, unsigned int> > m_OverviewsSize;

  /** Resolution factor
   */
  unsigned int m_ResolutionFactor;

  /**
   * Original dimension of the input image
   */
  std::vector<unsigned int> m_OriginalDimensions;

  /**
   * True if RPC tags should be exported
   */
  bool m_WriteRPCTags;
  
};

} // end namespace otb

#endif // otbGDALImageIO_h
