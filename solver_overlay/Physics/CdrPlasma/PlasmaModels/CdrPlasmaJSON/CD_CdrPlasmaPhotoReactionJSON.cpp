/* chombo-discharge
 * Copyright © 2022 SINTEF Energy Research.
 * Please refer to Copyright.txt and LICENSE in the chombo-discharge root directory.
 */

/*!
  @file   CD_CdrPlasmaPhotoReactionJSON.cpp
  @brief  Implementation of CD_CdrPlasmaPhotoReactionJSON.H
  @author Robert Marskar
*/

// Our includes
#include <CD_CdrPlasmaPhotoReactionJSON.H>
#include <CD_NamespaceHeader.H>

using namespace Physics::CdrPlasma;

CdrPlasmaPhotoReactionJSON::CdrPlasmaPhotoReactionJSON(const std::list<int> a_plasmaReactants,
                                                       const std::list<int> a_neutralReactants,
                                                       const std::list<int> a_photonReactants,
                                                       const std::list<int> a_plasmaProducts,
                                                       const std::list<int> a_neutralProducts,
                                                       const bool           a_hasPlasmaAbsorption,
                                                       const int            a_absorbingPlasmaSpecies,
                                                       const Real           a_absorptionCrossSection)
{
  m_plasmaReactants        = a_plasmaReactants;
  m_neutralReactants       = a_neutralReactants;
  m_photonReactants        = a_photonReactants;
  m_plasmaProducts         = a_plasmaProducts;
  m_neutralProducts        = a_neutralProducts;
  m_hasPlasmaAbsorption    = a_hasPlasmaAbsorption;
  m_absorbingPlasmaSpecies = a_absorbingPlasmaSpecies;
  m_absorptionCrossSection = a_absorptionCrossSection;
}

CdrPlasmaPhotoReactionJSON::~CdrPlasmaPhotoReactionJSON()
{}

const std::list<int>&
CdrPlasmaPhotoReactionJSON::getPlasmaReactants() const
{
  return m_plasmaReactants;
}

const std::list<int>&
CdrPlasmaPhotoReactionJSON::getNeutralReactants() const
{
  return m_neutralReactants;
}

const std::list<int>&
CdrPlasmaPhotoReactionJSON::getPhotonReactants() const
{
  return m_photonReactants;
}

const std::list<int>&
CdrPlasmaPhotoReactionJSON::getPlasmaProducts() const
{
  return m_plasmaProducts;
}

const std::list<int>&
CdrPlasmaPhotoReactionJSON::getNeutralProducts() const
{
  return m_neutralProducts;
}

bool
CdrPlasmaPhotoReactionJSON::hasPlasmaAbsorption() const
{
  return m_hasPlasmaAbsorption;
}

int
CdrPlasmaPhotoReactionJSON::getAbsorbingPlasmaSpecies() const
{
  return m_absorbingPlasmaSpecies;
}

Real
CdrPlasmaPhotoReactionJSON::getAbsorptionCrossSection() const
{
  return m_absorptionCrossSection;
}

#include <CD_NamespaceFooter.H>
