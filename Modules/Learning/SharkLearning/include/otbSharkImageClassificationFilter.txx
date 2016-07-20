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
#ifndef otbSharkImageClassificationFilter_txx
#define otbSharkImageClassificationFilter_txx

#include "otbSharkImageClassificationFilter.h"
#include "itkImageRegionIterator.h"
#include "itkProgressReporter.h"

namespace otb
{
/**
 * Constructor
 */
template <class TInputImage, class TOutputImage, class TMaskImage>
SharkImageClassificationFilter<TInputImage, TOutputImage, TMaskImage>
::SharkImageClassificationFilter()
{
  this->SetNumberOfIndexedInputs(2);
  this->SetNumberOfRequiredInputs(1);
  m_DefaultLabel = itk::NumericTraits<LabelType>::ZeroValue();

  this->SetNumberOfRequiredOutputs(2);
  this->SetNthOutput(0,TOutputImage::New());
  this->SetNthOutput(1,ConfidenceImageType::New());
  m_UseConfidenceMap = false;
  m_Batch = false;
}

template <class TInputImage, class TOutputImage, class TMaskImage>
void
SharkImageClassificationFilter<TInputImage, TOutputImage, TMaskImage>
::SetInputMask(const MaskImageType * mask)
{
  this->itk::ProcessObject::SetNthInput(1, const_cast<MaskImageType *>(mask));
}

template <class TInputImage, class TOutputImage, class TMaskImage>
const typename SharkImageClassificationFilter<TInputImage, TOutputImage, TMaskImage>
::MaskImageType *
SharkImageClassificationFilter<TInputImage, TOutputImage, TMaskImage>
::GetInputMask()
{
  if (this->GetNumberOfInputs() < 2)
    {
    return ITK_NULLPTR;
    }
  return static_cast<const MaskImageType *>(this->itk::ProcessObject::GetInput(1));
}

template <class TInputImage, class TOutputImage, class TMaskImage>
typename SharkImageClassificationFilter<TInputImage, TOutputImage, TMaskImage>
::ConfidenceImageType *
SharkImageClassificationFilter<TInputImage, TOutputImage, TMaskImage>
::GetOutputConfidence()
{
  if (this->GetNumberOfOutputs() < 2)
    {
    return ITK_NULLPTR;
    }
  return static_cast<ConfidenceImageType *>(this->itk::ProcessObject::GetOutput(1));
}

template <class TInputImage, class TOutputImage, class TMaskImage>
void
SharkImageClassificationFilter<TInputImage, TOutputImage, TMaskImage>
::BeforeThreadedGenerateData()
{
  if (!m_Model)
    {
    itkGenericExceptionMacro(<< "No model for classification");
    }
  if(m_Batch)
    {
    // The calls to m_Model->SetInputListSample() and
    // m_Model->SetTargetListSample() make the model non thread safe.
    // OpenMP allows to speed things up (configure with
    // CMAKE_CXX_FLAGS:STRING=-fopenmp)
    this->SetNumberOfThreads(1);
    }
}

template <class TInputImage, class TOutputImage, class TMaskImage>
void
SharkImageClassificationFilter<TInputImage, TOutputImage, TMaskImage>
::ClassicThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId)
{
  // Get the input pointers
  InputImageConstPointerType inputPtr     = this->GetInput();
  MaskImageConstPointerType  inputMaskPtr  = this->GetInputMask();
  OutputImagePointerType     outputPtr    = this->GetOutput();
  ConfidenceImagePointerType confidencePtr = this->GetOutputConfidence();

  // Progress reporting
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  // Define iterators
  typedef itk::ImageRegionConstIterator<InputImageType> InputIteratorType;
  typedef itk::ImageRegionConstIterator<MaskImageType>  MaskIteratorType;
  typedef itk::ImageRegionIterator<OutputImageType>     OutputIteratorType;
  typedef itk::ImageRegionIterator<ConfidenceImageType> ConfidenceMapIteratorType;

  InputIteratorType inIt(inputPtr, outputRegionForThread);
  OutputIteratorType outIt(outputPtr, outputRegionForThread);

  // Eventually iterate on masks
  MaskIteratorType maskIt;
  if (inputMaskPtr)
    {
    maskIt = MaskIteratorType(inputMaskPtr, outputRegionForThread);
    maskIt.GoToBegin();
    }

  // setup iterator for confidence map
  bool computeConfidenceMap(m_UseConfidenceMap && m_Model->HasConfidenceIndex() && !m_Model->GetRegressionMode());
  ConfidenceMapIteratorType confidenceIt;
  if (computeConfidenceMap)
    {
    confidenceIt = ConfidenceMapIteratorType(confidencePtr,outputRegionForThread);
    confidenceIt.GoToBegin();
    }

  bool validPoint = true;
  double confidenceIndex = 0.0;

  // Walk the part of the image
  for (inIt.GoToBegin(), outIt.GoToBegin(); !inIt.IsAtEnd() && !outIt.IsAtEnd(); ++inIt, ++outIt)
    {
    // Check pixel validity
    if (inputMaskPtr)
      {
      validPoint = maskIt.Get() > 0;
      ++maskIt;
      }
    // If point is valid
    if (validPoint)
      {
      // Classifify
      if (computeConfidenceMap)
        {
        outIt.Set(m_Model->Predict(inIt.Get(),&confidenceIndex)[0]);
        }
      else
        {
        outIt.Set(m_Model->Predict(inIt.Get())[0]);
        }
      }
    else
      {
      // else, set default value
      outIt.Set(m_DefaultLabel);
      confidenceIndex = 0.0;
      }
    if (computeConfidenceMap)
      {
      confidenceIt.Set(confidenceIndex);
      ++confidenceIt;
      }
    progress.CompletedPixel();
    }

}

template <class TInputImage, class TOutputImage, class TMaskImage>
void
SharkImageClassificationFilter<TInputImage, TOutputImage, TMaskImage>
::BatchThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId)
{

  //Simplified case without masks nor confidences.
  // Problems: the model is not thread safe when using predict all, because we set the samples. Two solutions : force one thread and use openmp so shark makes the parallel classification, or have one model per thread. Also, see if we can minimise the copies from pixels to samples with an adaptor.

  MaskImageConstPointerType  inputMaskPtr  = this->GetInputMask();
  bool computeConfidenceMap(m_UseConfidenceMap && m_Model->HasConfidenceIndex() 
                            && !m_Model->GetRegressionMode());
  if (inputMaskPtr || computeConfidenceMap)
    {
    std::cout << "Redirecting to classic mode\n";
    this->ClassicThreadedGenerateData(outputRegionForThread, threadId);
    }
  else
    {
    // Get the input pointers
    InputImageConstPointerType inputPtr     = this->GetInput();
    OutputImagePointerType     outputPtr    = this->GetOutput();
    
    // Progress reporting
    itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

    // Define iterators
    typedef itk::ImageRegionConstIterator<InputImageType> InputIteratorType;
    typedef itk::ImageRegionIterator<OutputImageType>     OutputIteratorType;

    InputIteratorType inIt(inputPtr, outputRegionForThread);
    OutputIteratorType outIt(outputPtr, outputRegionForThread);

    typedef typename ModelType::InputValueType       InputValueType;
    typedef typename ModelType::InputSampleType      InputSampleType;
    typedef typename ModelType::InputListSampleType  InputListSampleType;
    typedef typename ModelType::TargetValueType      TargetValueType;
    typedef typename ModelType::TargetSampleType     TargetSampleType;
    typedef typename ModelType::TargetListSampleType TargetListSampleType;

    typename InputListSampleType::Pointer samples = InputListSampleType::New();
    auto num_features = inputPtr->GetNumberOfComponentsPerPixel();
    samples->SetMeasurementVectorSize(num_features);
    InputSampleType sample(num_features);
    // Fill the samples
    for (inIt.GoToBegin(); !inIt.IsAtEnd(); ++inIt)
      {
      auto pix = inIt.Get();
      for(size_t feat=0; feat<num_features; ++feat)
        {
        sample[feat]=pix[feat];
        }
      samples->PushBack(sample);
      }
    //Make the batch prediction
    m_Model->SetInputListSample(samples);
    typename TargetListSampleType::Pointer labels = TargetListSampleType::New();
    m_Model->SetTargetListSample(labels);
    m_Model->PredictAll();
    // Set the output values
    auto labIt = labels->Begin();
    for (outIt.GoToBegin(); labIt!=labels->End() && !outIt.IsAtEnd(); ++labIt, ++outIt)
      {
      outIt.Set(labIt.GetMeasurementVector()[0]);
      }
    progress.CompletedPixel();
    }
}
template <class TInputImage, class TOutputImage, class TMaskImage>
void
SharkImageClassificationFilter<TInputImage, TOutputImage, TMaskImage>
::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId)
{
  if(m_Batch)
    {
    std::cout << "Batch mode\n";
    this->BatchThreadedGenerateData(outputRegionForThread, threadId);
    }
  else
    {
    std::cout << "Classic mode\n";
    this->ClassicThreadedGenerateData(outputRegionForThread, threadId);
    }

}
/**
 * PrintSelf Method
 */
template <class TInputImage, class TOutputImage, class TMaskImage>
void
SharkImageClassificationFilter<TInputImage, TOutputImage, TMaskImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} // End namespace otb
#endif
