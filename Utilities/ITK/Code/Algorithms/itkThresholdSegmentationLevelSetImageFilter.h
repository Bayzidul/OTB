/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkThresholdSegmentationLevelSetImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2009-01-27 19:30:16 $
  Version:   $Revision: 1.20 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkThresholdSegmentationLevelSetImageFilter_h
#define __itkThresholdSegmentationLevelSetImageFilter_h

#include "itkSegmentationLevelSetImageFilter.h"
#include "itkThresholdSegmentationLevelSetFunction.h"

namespace itk {

/** \class ThresholdSegmentationLevelSetImageFilter
 *    \brief Segments structures in images based on intensity values.
 *
 *   \par IMPORTANT
 *   The SegmentationLevelSetImageFilter class and the
 *   ThresholdSegmentationLevelSetFunction class contain additional information necessary
 *   to the full understanding of how to use this filter.
 *
 *    \par OVERVIEW
 *    This class is a level set method segmentation filter.  It constructs a
 *    speed function which is close to zero at the upper and lower bounds of an
 *    intensity window, effectively locking the propagating front onto those
 *    edges.  Elsewhere, the front will propagate quickly.
 *
 *    \par INPUTS
 *    This filter requires two inputs.  The first input is a seed
 *    image.  This seed image must contain an isosurface that you want to use as the
 *    seed for your segmentation.  It can be a binary, graylevel, or floating
 *    point image.  The only requirement is that it contain a closed isosurface
 *    that you will identify as the seed by setting the IsosurfaceValue parameter
 *    of the filter.  For a binary image you will want to set your isosurface
 *    value halfway between your on and off values (i.e. for 0's and 1's, use an
 *    isosurface value of 0.5).
 *
 *    \par
 *    The second input is the feature image.  This is the image from which the
 *    speed function will be calculated.  For most applications, this is the
 *    image that you want to segment. The desired isosurface in your seed image
 *    should lie within the region of your feature image that you are trying to
 *    segment. Note that this filter does no preprocessing of the feature image
 *    before thresholding.
 *
 *    \par
 *    See SegmentationLevelSetImageFilter for more information on Inputs.
 *
 *    \par OUTPUTS
 *    The filter outputs a single, scalar, real-valued image.
 *    Positive values in the output image are inside the segmentated region
 *    and negative values in the image are outside of the inside region.  The
 *    zero crossings of the image correspond to the position of the level set
 *    front.
 *
 *   \par
 *   See SparseFieldLevelSetImageFilter and
 *   SegmentationLevelSetImageFilter for more information.
 *
 *   \par PARAMETERS
 *   In addition to parameters described in SegmentationLevelSetImageFilter,
 *   this filter adds the UpperThreshold and LowerThreshold.  See
 *   ThresholdSegmentationLevelSetFunction for a description of how these values
 *   affect the segmentation.
 *
 *   \sa SegmentationLevelSetImageFilter
 *   \sa ThresholdSegmentationLevelSetFunction,
 *   \sa SparseFieldLevelSetImageFilter */
template <class TInputImage,
          class TFeatureImage,
          class TOutputPixelType = float >
class ITK_EXPORT ThresholdSegmentationLevelSetImageFilter
  : public SegmentationLevelSetImageFilter<TInputImage, TFeatureImage, TOutputPixelType >
{
public:
  /** Standard class typedefs */
  typedef ThresholdSegmentationLevelSetImageFilter Self;
  typedef  SegmentationLevelSetImageFilter<TInputImage, TFeatureImage, TOutputPixelType >
                                                   Superclass;
  typedef SmartPointer<Self>                       Pointer;
  typedef SmartPointer<const Self>                 ConstPointer;

  /** Inherited typedef from the superclass. */
  typedef typename Superclass::ValueType        ValueType;
  typedef typename Superclass::OutputImageType  OutputImageType;
  typedef typename Superclass::FeatureImageType FeatureImageType;
  
  /** Type of the segmentation function */
  typedef ThresholdSegmentationLevelSetFunction<OutputImageType,
                                                FeatureImageType> ThresholdFunctionType;
  typedef typename ThresholdFunctionType::Pointer ThresholdFunctionPointer;
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(ThresholdSegmentationLevelSetImageFilter, SegmentationLevelSetImageFilter);

  /** Method for creation through the object factory */
  itkNewMacro(Self);
  
  /** Get/Set the threshold values that will be used to calculate the speed function. */
  void SetUpperThreshold(ValueType v)
    {
    this->m_ThresholdFunction->SetUpperThreshold(v);
    this->Modified();
    }
  void SetLowerThreshold(ValueType v)
    {
    this->m_ThresholdFunction->SetLowerThreshold(v);
    this->Modified();
    }
  ValueType GetUpperThreshold() const
    {
    return m_ThresholdFunction->GetUpperThreshold();
    }
  ValueType GetLowerThreshold() const
    {
    return m_ThresholdFunction->GetLowerThreshold();
    }

  /** Set/Get the weight applied to the edge (Laplacian) attractor in the speed
   *  term function. Zero will turn this term off. */
  void SetEdgeWeight(ValueType v)
    {
    this->m_ThresholdFunction->SetEdgeWeight(v);
    this->Modified();
    }
  ValueType GetEdgeWeight() const
    {
    return m_ThresholdFunction->GetEdgeWeight();
    }

  /** Anisotropic diffusion is applied to the FeatureImage before calculating
   * the Laplacian (edge) term. This method sets/gets the number of diffusion
   * iterations. */
  void SetSmoothingIterations(int v)
    {
    this->m_ThresholdFunction->SetSmoothingIterations(v);
    this->Modified();
    }
  int GetSmoothingIterations() const
    {
    return m_ThresholdFunction->GetSmoothingIterations();
    }

  /** Anisotropic diffusion is applied to the FeatureImage before calculating
   * the Laplacian (edge) term. This method sets/gets the diffusion time
   * step. */
  void SetSmoothingTimeStep(ValueType v)
    {
    this->m_ThresholdFunction->SetSmoothingTimeStep(v);
    this->Modified();
    }
  ValueType GetSmoothingTimeStep() const
    {
    return m_ThresholdFunction->GetSmoothingTimeStep();
    }

  /** Anisotropic diffusion is applied to the FeatureImage before calculatign
   * the Laplacian (edge) term. This method sets/gets the smoothing
   * conductance. */
  void SetSmoothingConductance(ValueType v)
    {
    this->m_ThresholdFunction->SetSmoothingConductance(v);
    this->Modified();
    }
  ValueType GetSmoothingConductance() const
    {
    return m_ThresholdFunction->GetSmoothingConductance();
    } 
  
protected:
  ~ThresholdSegmentationLevelSetImageFilter() {}
  ThresholdSegmentationLevelSetImageFilter();

  virtual void PrintSelf(std::ostream &os, Indent indent) const; 

  ThresholdSegmentationLevelSetImageFilter(const Self &); // purposely not impl.
  void operator=(const Self&); //purposely not implemented
private:
  ThresholdFunctionPointer m_ThresholdFunction;
};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkThresholdSegmentationLevelSetImageFilter.txx"
#endif

#endif