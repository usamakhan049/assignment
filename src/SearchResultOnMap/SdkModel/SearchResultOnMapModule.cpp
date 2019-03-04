// Copyright eeGeo Ltd (2012-2015), All Rights Reserved

#include "SearchResultOnMapModule.h"
#include "Search.h"
#include "SearchResultOnMap.h"
#include "SearchResultOnMapFactory.h"
#include "SearchResultOnMapModel.h"
#include "SearchResultMyPinsService.h"
#include "IIdentity.h"

namespace ExampleApp
{
    namespace SearchResultOnMap
    {
        namespace SdkModel
        {
            SearchResultOnMapModule::SearchResultOnMapModule(Search::SdkModel::ISearchResultRepository& searchResultRepository,
                                                             SearchResultPoi::View::ISearchResultPoiViewModel& searchResultPoiViewModel,
                                                             WorldPins::SdkModel::IWorldPinsService& worldPinsService,
                                                             MyPins::SdkModel::IMyPinsService& myPinsService,
                                                             TagSearch::ISearchResultIconKeyMapper& searchResultOnMapIconKeyMapper,
                                                             Search::SdkModel::MyPins::ISearchResultMyPinsService& searchResultMyPinsService,
                                                             ExampleAppMessaging::TMessageBus& messageBus,
                                                             Metrics::IMetricsService& metricsService,
                                                             const Menu::View::IMenuReactionModel& menuReaction,
                                                             InteriorsExplorer::SdkModel::Highlights::InteriorEntityHighlightController& entityHighlightController)
            {
                m_pSearchResultOnMapFactory = Eegeo_NEW(View::SearchResultOnMapFactory)(messageBus, metricsService, entityHighlightController);
                
                m_pSearchResultOnMapModel = Eegeo_NEW(SearchResultOnMapModel)(worldPinsService,
                                                                              myPinsService,
                                                                              *m_pSearchResultOnMapFactory,
                                                                              searchResultMyPinsService,
                                                                              searchResultOnMapIconKeyMapper,
                                                                              searchResultRepository);
                
                m_pSearchResultOnMapItemModelSelectedObserver = Eegeo_NEW(View::SearchResultOnMapItemModelSelectedObserver)(searchResultPoiViewModel,
                                                                                                                            messageBus,
                                                                                                                            menuReaction);
            }

            SearchResultOnMapModule::~SearchResultOnMapModule()
            {
                Eegeo_DELETE m_pSearchResultOnMapItemModelSelectedObserver;
                Eegeo_DELETE m_pSearchResultOnMapModel;
                Eegeo_DELETE m_pSearchResultOnMapFactory;
            }

            ISearchResultOnMapModel& SearchResultOnMapModule::GetSearchResultOnMapModel() const
            {
                return *m_pSearchResultOnMapModel;
            }
        }
    }
}
