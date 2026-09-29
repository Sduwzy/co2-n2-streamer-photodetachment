/* chombo-discharge
 * Copyright © 2021 SINTEF Energy Research.
 * Please refer to Copyright.txt and LICENSE in the chombo-discharge root directory.
 */

/*!
  @file   CD_EBHelmholtzLarsenDomainBCFactory.cpp
  @brief  Implementation of CD_EBHelmholtzLarsenDomainBCFactory.H
  @author Robert Marskar
*/

// Our includes
#include <CD_EBHelmholtzLarsenDomainBCFactory.H>
#include <CD_EBHelmholtzLarsenDomainBC.H>
#include <CD_NamespaceHeader.H>

EBHelmholtzLarsenDomainBCFactory::EBHelmholtzLarsenDomainBCFactory(const RefCountedPtr<RtSpecies>& a_species,
                                                                   const Real                      a_r1,
                                                                   const Real                      a_r2,
                                                                   const SourceFunction            a_source)
{
  m_species = a_species;
  m_r1      = a_r1;
  m_r2      = a_r2;
  m_source  = a_source;
  m_kappaFunction = [species = this->m_species](const RealVect& a_position) {
    return species->getAbsorptionCoefficient(a_position);
  };
}

EBHelmholtzLarsenDomainBCFactory::EBHelmholtzLarsenDomainBCFactory(const RefCountedPtr<RtSpecies>& a_species,
                                                                   const Real                      a_r1,
                                                                   const Real                      a_r2,
                                                                   const SourceFunction            a_source,
                                                                   const KappaFunction             a_kappaFunction)
{
  m_species       = a_species;
  m_r1            = a_r1;
  m_r2            = a_r2;
  m_source        = a_source;
  m_kappaFunction = a_kappaFunction;
  if (!m_kappaFunction) {
    m_kappaFunction = [species = this->m_species](const RealVect& a_position) {
      return species->getAbsorptionCoefficient(a_position);
    };
  }
}

EBHelmholtzLarsenDomainBCFactory::EBHelmholtzLarsenDomainBCFactory(const RefCountedPtr<RtSpecies>& a_species,
                                                                   const Real                      a_r1,
                                                                   const Real                      a_r2)
{
  m_species = a_species;
  m_r1      = a_r1;
  m_r2      = a_r2;

  m_source = [](const RealVect& a_position) {
    return 0.0;
  };
  m_kappaFunction = [species = this->m_species](const RealVect& a_position) {
    return species->getAbsorptionCoefficient(a_position);
  };
}

EBHelmholtzLarsenDomainBCFactory::~EBHelmholtzLarsenDomainBCFactory()
{}

RefCountedPtr<EBHelmholtzDomainBC>
EBHelmholtzLarsenDomainBCFactory::create() const
{
  return RefCountedPtr<EBHelmholtzLarsenDomainBC>(
    new EBHelmholtzLarsenDomainBC(m_species, m_r1, m_r2, m_source, m_kappaFunction));
}

#include <CD_NamespaceFooter.H>
