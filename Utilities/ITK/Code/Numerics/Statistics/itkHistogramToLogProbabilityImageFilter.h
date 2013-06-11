/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkHistogramToLogProbabilityImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2009-04-06 11:51:06 $
  Version:   $Revision: 1.11 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkHistogramToLogProbabilityImageFilter_h
#define __itkHistogramToLogProbabilityImageFilter_h

#include "itkHistogramToImageFilter.h"

namespace itk
{
  
/** \class HistogramToLogProbabilityImageFilter
 * \brief The class takes a histogram as an input and gives the log probability
 * image as the output. A pixel, at position I,  in the output image is given by
 *  
 * \f[
 * f(I) = \log_2( \frac{q_I}{\sum_{i \in I} q_I} )
 * \f]
 *  where \f$q_I\f$ is the frequency of measurement vector, I.
 *
 * This is the log of the  frequency of a measurement vector by the sum of all 
 * frequencies. 
 * 
 * The output image is of type double.
 *
 * This is useful in plotting the joint histograms during registration.
 * 
 *  \sa HistogramToImageFilter, HistogramToProbabilityImageFilter,
 *  HistogramToIntensityImageFilter, HistogramToEntropyImageFilter
 * 
 */

namespace Function {  
template< class TInput, class TOutput=double >
class HistogramLogProbabilityFunction
{
public:
  
  //Probability function = Number of occurances in each bin /
  //   Total Number of occurances. 
  //
  // Returns pixels of float.. 
  typedef  TOutput  OutputPixelType;
  
  
  HistogramLogProbabilityFunction(): 
      m_TotalFrequency(1) {}
    
  ~HistogramLogProbabilityFunction() {};
  
  inline OutputPixelType operator()( const TInput & A ) const
    {
    if( A )
      {
      return static_cast<OutputPixelType>(vcl_log( static_cast<OutputPixelType>(A) / 
        static_cast<OutputPixelType>(m_TotalFrequency)) / vcl_log(2.0) );
      }
    else
      { // Check for Log 0. Always assume that the frequency is atleast 1.
      return static_cast<OutputPixelType>(vcl_log( static_cast<OutputPixelType>(A+1) / 
        static_cast<OutputPixelType>(m_TotalFrequency)) / vcl_log(2.0) );
      }
    }

  void SetTotalFrequency( unsigned long n ) 
    {
    m_TotalFrequency = n;
    }
  
  unsigned long GetTotalFrequency( ) const 
    {
    return m_TotalFrequency;
    }

private:
  unsigned long  m_TotalFrequency;
}; 
}

template <class THistogram, class TOutputPixel=double >
class ITK_EXPORT HistogramToLogProbabilityImageFilter :
  public HistogramToImageFilter< THistogram, 
  Function::HistogramLogProbabilityFunction< unsigned long, TOutputPixel > > 
{
public:
  
  /** Standard class typedefs. */
  typedef HistogramToLogProbabilityImageFilter Self;

  /** Standard "Superclass" typedef. */
  typedef HistogramToImageFilter< THistogram, 
    Function::HistogramLogProbabilityFunction< unsigned long, TOutputPixel > >
                                               Superclass;

  typedef SmartPointer<Self>                   Pointer;
  typedef SmartPointer<const Self>             ConstPointer;
  
  /** Run-time type information (and related methods).   */
  itkTypeMacro( HistogramToLogProbabilityImageFilter, HistogramToImageFilter );

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

protected:
  HistogramToLogProbabilityImageFilter() {}
  virtual ~HistogramToLogProbabilityImageFilter() {}
  
private:
  HistogramToLogProbabilityImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

};

} // end namespace itk

#endif