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
#ifndef otbRCC8InEdgeIterator_txx
#define otbRCC8InEdgeIterator_txx

#include "otbRCC8InEdgeIterator.h"

namespace otb
{
/** Constructor */
template <class TGraph>
RCC8InEdgeIterator<TGraph>
::RCC8InEdgeIterator()
{
  m_Graph = GraphType::New();
  boost::tuples::tie(m_Iter, m_End) = in_edges(0, *m_Graph->GetGraph());
}
/**
 * Copy operator.
 */
template <class TGraph>
RCC8InEdgeIterator<TGraph>
::RCC8InEdgeIterator(const Self& iter)
{
  m_Iter = iter.m_Iter;
  m_Graph = iter.m_Graph;
  m_VertexIndex = iter.m_VertexIndex;
  m_End = iter.m_End;
}
/**
 *Constructor with input graph
 */
template <class TGraph>
RCC8InEdgeIterator<TGraph>
::RCC8InEdgeIterator(VertexDescriptorType vertex, GraphType * graph)
{
  m_Graph = graph;
  m_VertexIndex = vertex;
  boost::tuples::tie(m_Iter, m_End) = in_edges(vertex, *m_Graph->GetGraph());
}
/**
 * Get the current edge value.
 * \return The value of the current edge pointed by the iterator.
 */
template <class TGraph>
typename RCC8InEdgeIterator<TGraph>
::RCC8ValueType
RCC8InEdgeIterator<TGraph>
::GetValue(void)
{
  return (*m_Graph->GetGraph())[*m_Iter]->GetValue();
}
/**
 * Return the source vertex of the current edge.
 * \return The source edge.
 */
template <class TGraph>
typename RCC8InEdgeIterator<TGraph>
::VertexPointerType
RCC8InEdgeIterator<TGraph>
::GetSourceVertex(void)
{
  return (*m_Graph->GetGraph())[source(*m_Iter, (*m_Graph->GetGraph()))];
}
/**
 * Return the source vertex index of the current edge.
 * \return The souce vertex index.
 */
template <class TGraph>
typename RCC8InEdgeIterator<TGraph>
::VertexDescriptorType
RCC8InEdgeIterator<TGraph>
::GetSourceIndex(void)
{
  IndexMapType index = get(boost::vertex_index, (*m_Graph->GetGraph()));
  return index[source(*m_Iter, (*m_Graph->GetGraph()))];
}
/**
 * Return true if the iterator is at the end.
 * \return True if the iterator is at the end.
 */
template <class TGraph>
bool
RCC8InEdgeIterator<TGraph>
::IsAtEnd(void)
{
  return (m_Iter == m_End);
}
/**
 * Go to the beginning.
 */
template <class TGraph>
void
RCC8InEdgeIterator<TGraph>
::GoToBegin(void)
{
  boost::tuples::tie(m_Iter, m_End) = in_edges(m_VertexIndex, *m_Graph->GetGraph());
}
/**
 * Increment.
 */
template <class TGraph>
typename RCC8InEdgeIterator<TGraph>
::Self&
RCC8InEdgeIterator<TGraph>
::operator++ ()
  {
  ++m_Iter;
  return *this;
  }
/**
 * Decrement.
 */
template <class TGraph>
typename RCC8InEdgeIterator<TGraph>
::Self&
RCC8InEdgeIterator<TGraph>
::operator-- ()
  {
  --m_Iter;
  return *this;
  }
/**
 * Add
 */
template <class TGraph>
typename RCC8InEdgeIterator<TGraph>
::Self&
RCC8InEdgeIterator<TGraph>
::operator + (int i)
  {
  m_Iter = m_Iter + i;
  return *this;
  }
/**
 * Remove
 */
template <class TGraph>
typename RCC8InEdgeIterator<TGraph>
::Self&
RCC8InEdgeIterator<TGraph>
::operator - (int i)
  {
  m_Iter = m_Iter - i;
  return *this;
  }
/**
 * Difference comparison operator.
 */
template <class TGraph>
bool
RCC8InEdgeIterator<TGraph>
::operator != (const Self &iter)
  {
  return (m_Iter != iter.m_Iter);
  }
/**
 * Equality comparison operator.
 */
template <class TGraph>
bool
RCC8InEdgeIterator<TGraph>
::operator == (const Self &iter)
  {
  return (m_Iter == iter.m_Iter);
  }
/**
 * Instantiation operator.
 */
template <class TGraph>
typename RCC8InEdgeIterator<TGraph>
::Self&
RCC8InEdgeIterator<TGraph>
::operator = (const Self &iter)
  {
  m_Iter = iter.m_Iter;
  return *this;
  }
} // End namespace otb
#endif
