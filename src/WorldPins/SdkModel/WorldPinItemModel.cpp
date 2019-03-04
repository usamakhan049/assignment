// Copyright eeGeo Ltd (2012-2015), All Rights Reserved

#include "WorldPinItemModel.h"
#include "Types.h"

namespace ExampleApp
{
    namespace WorldPins
    {
        namespace SdkModel
        {
            namespace
            {
                const float TransitionSeconds = 0.25f;
                const float TransitionDeltaPerSeconds = 1.f / TransitionSeconds;
            }

            WorldPinItemModel::WorldPinItemModel(Eegeo::Markers::IMarker* pMarker,
                                                 IWorldPinSelectionHandler* pSelectionHandler,
                                                 IWorldPinVisibilityStateChangedHandler* pVisibilityStateChangedHandler,
                                                 const WorldPinFocusData& worldPinFocusData,
                                                 bool interior,
                                                 const WorldPinInteriorData& worldPinInteriorData,
                                                 int visibilityMask,
                                                 std::string identifier)
                : m_pMarker(pMarker)
                , m_pSelectionHandler(pSelectionHandler)
                , m_pVisibilityStateChangedHandler(pVisibilityStateChangedHandler)
                , m_focusModel(m_pMarker->GetId(),
                               worldPinFocusData.title,
                               worldPinFocusData.subtitle,
                               worldPinFocusData.vendor,
                               worldPinFocusData.json,
                               worldPinFocusData.ratingsImage,
                               worldPinFocusData.reviewCount,
                               worldPinFocusData.priorityOrder)
                , m_transitionState(StableHidden)
                , m_transitionStateValue(0.f)
                , m_focusable(true)
                , m_interior(interior)
                , m_worldPinInteriorData(worldPinInteriorData)
                , m_visibilityMask(visibilityMask)
                , m_identifier(identifier)
            {
                Eegeo_ASSERT(m_pSelectionHandler != NULL, "WorldPinItemModel must be provided with a non-null selection handler.")
            }

            WorldPinItemModel::~WorldPinItemModel()
            {
            }
            
            bool WorldPinItemModel::IsFocusable() const
            {
                return m_focusable;
            }
            
            void WorldPinItemModel::SetFocusable(bool focusable)
            {
                m_focusable = focusable;
            }
            
            const WorldPinItemModel::WorldPinItemModelId WorldPinItemModel::Id() const
            {
                return m_pMarker->GetId();
            }
            
            void WorldPinItemModel::SetMarker(Eegeo::Markers::IMarker* pMarker)
            {
                m_pMarker = pMarker;
                m_focusModel.SetPinId(m_pMarker->GetId());
            }

            void WorldPinItemModel::Select()
            {
                m_pSelectionHandler->SelectPin();
            }

            bool WorldPinItemModel::IsHidden() const
            {
                return m_transitionState == StableHidden;
            }

            bool WorldPinItemModel::IsVisible() const
            {
                return m_transitionState == StableVisible;
            }

            bool WorldPinItemModel::IsTransitioning() const
            {
                return !IsHidden() && !IsVisible();
            }

            void WorldPinItemModel::Hide()
            {
                if(m_transitionState != StableHidden)
                {
                    SetTransitionState(TransitionToHidden);
                }
                
                UpdateMarkerState();
            }

            void WorldPinItemModel::Show()
            {
                if(m_transitionState != StableVisible && m_transitionState != TransitionToVisible)
                {
                    SetTransitionState(TransitionToVisible);
                }
                
                UpdateMarkerState();
            }

            float WorldPinItemModel::TransitionStateValue() const
            {
                return m_transitionStateValue;
            }
            
            void WorldPinItemModel::Refresh(const std::string& title, const std::string& description, const std::string& ratingsImage, const int reviewCount)
            {
                m_focusModel.Refresh(title, description, ratingsImage, reviewCount);
            }

            void WorldPinItemModel::Update(float deltaSeconds)
            {
                if(m_transitionState == TransitionToHidden)
                {
                    m_transitionStateValue -= (deltaSeconds * TransitionDeltaPerSeconds);

                    if(m_transitionStateValue <= 0.f)
                    {
                        if(m_transitionState != StableHidden && m_pVisibilityStateChangedHandler != NULL)
                        {
                            m_pVisibilityStateChangedHandler->HandlePinBecameFullyInvisible();
                        }
                        
                        m_transitionStateValue = 0.f;
                        SetTransitionState(StableHidden);
                    }
                }
                else if(m_transitionState == TransitionToVisible)
                {
                    m_transitionStateValue += (deltaSeconds * TransitionDeltaPerSeconds);

                    if(m_transitionStateValue >= 1.f)
                    {
                        if(m_transitionState != StableVisible && m_pVisibilityStateChangedHandler != NULL)
                        {
                            m_pVisibilityStateChangedHandler->HandlePinBecameFullyVisible();
                        }
                        
                        m_transitionStateValue = 1.f;
                        SetTransitionState(StableVisible);
                    }
                }
            }
            
            void WorldPinItemModel::SetTransitionState(TransitionState transitionState)
            {
                if (m_transitionState != transitionState)
                {
                    m_transitionState = transitionState;
                }
                
                UpdateMarkerState();
            }
            
            void WorldPinItemModel::UpdateMarkerState()
            {
                if(!IsTransitioning())
                {
                    if (m_pMarker->IsHidden() != IsHidden())
                    {
                        m_pMarker->SetHidden(IsHidden());
                    }
                }
            }

            const IWorldPinsInFocusModel& WorldPinItemModel::GetInFocusModel() const
            {
                return m_focusModel;
            }
            
            bool WorldPinItemModel::IsInterior() const
            {
                return m_interior;
            }
            
            const WorldPinInteriorData& WorldPinItemModel::GetInteriorData() const
            {
                return m_worldPinInteriorData;
            }
            
            int WorldPinItemModel::VisibilityMask() const
            {
                return m_visibilityMask;
            }
            
            void WorldPinItemModel::SetVisibilityMask(int visibilityMask)
            {
                m_visibilityMask = visibilityMask;
            }
            
            std::string WorldPinItemModel::GetIdentifier() const
            {
                return m_identifier;
            }
        }
    }
}
