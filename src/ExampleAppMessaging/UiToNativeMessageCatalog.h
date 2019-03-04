// Copyright eeGeo Ltd (2012-2015), All Rights Reserved

#pragma once

#include "Types.h"
#include "CatalogBinding.h"
#include "UiCreatedMessage.h"
#include "SearchMenuPerformedSearchMessage.h"
#include "SearchMenuSearchWithContextMessage.h"
#include "AutocompleteSuggestionsMessage.h"
#include "AutocompleteSuggestionsCancelledMessage.h"
#include "SearchResultSectionItemSelectedMessage.h"
#include "TagSearchSelectedMessage.h"
#include "WeatherSelectedMessage.h"
#include "PlaceJumpSelectedMessage.h"
#include "ModalityChangedMessage.h"
#include "CompassViewCycledMessage.h"
#include "SearchResultViewClearedMessage.h"
#include "MyPinCreationViewStateChangedMessage.h"
#include "MyPinCreationViewSavePinMessage.h"
#include "MyPinDetailsViewRemovePinMessage.h"
#include "MyPinSelectedMessage.h"
#include "WorldPinsVisibilityMessage.h"
#include "UpdateNativeModalBackgroundMessage.h"
#include "StreamOverWifiOnlyChangedMessage.h"
#include "CacheEnabledChangedMessage.h"
#include "ClearCacheMessage.h"
#include "GpsMarkerVisibilityMessage.h"
#include "InitialExperienceIntroDismissedMessage.h"
#include "SearchResultPoiPinToggledMessage.h"
#include "NavigateToMessage.h"
#include "SearchResultPoiViewOpenedMessage.h"
#include "SearchResultPoiViewClosedMessage.h"
#include "InteriorsExplorerSelectFloorMessage.h"
#include "InteriorsExplorerFloorSelectionDraggedMessage.h"
#include "InteriorsExplorerExitMessage.h"
#include "MenuDragStateChangedMessage.h"
#include "MenuSectionExpandedChangedMessage.h"
#include "WorldPinsSelectedFocussedMessage.h"
#include "SurveyTimeRequirementMetMessage.h"
#include "InteriorsLocationAuthorizationChangedMessage.h"
#include "InteriorsLocationChangedMessage.h"
#include "NavRoutingViewStartEndLocationSwappedMessage.h"
#include "NavRoutingSelectStartLocationClickedMessage.h"
#include "NavRoutingSelectEndLocationClickedMessage.h"
#include "NavRoutingEndLocationClearClickedMessage.h"
#include "NavRoutingStartLocationClearClickedMessage.h"
#include "NavRoutingSelectedDirectionChangedMessage.h"
#include "NavRoutingStartEndRoutingButtonClickedMessage.h"
#include "NavRoutingViewClosedMessage.h"
#include "NavRoutingViewOpenedMessage.h"
#include "NavRoutingRerouteDialogClosedMessage.h"
#include "NavRoutingStartLocationSetFromSearchMessage.h"
#include "NavRoutingEndLocationSetFromSearchMessage.h"
#include "NavRoutingSearchForLocationMessage.h"

namespace ExampleApp
{
    namespace ExampleAppMessaging
    {
        class UiToNativeMessageCatalog
            : public Eegeo::Messaging::CatalogBinding<UiCreatedMessage>
            , public Eegeo::Messaging::CatalogBinding<SearchMenu::SearchMenuPerformedSearchMessage>
			, public Eegeo::Messaging::CatalogBinding<SearchMenu::SearchMenuSearchWithContextMessage>
            , public Eegeo::Messaging::CatalogBinding<SearchMenu::AutocompleteSuggestionsMessage>
            , public Eegeo::Messaging::CatalogBinding<SearchMenu::AutocompleteSuggestionsCancelledMessage>
            , public Eegeo::Messaging::CatalogBinding<SearchResultSection::SearchResultSectionItemSelectedMessage>
            , public Eegeo::Messaging::CatalogBinding<TagSearch::TagSearchSelectedMessage>
            , public Eegeo::Messaging::CatalogBinding<WeatherMenu::WeatherSelectedMessage>
            , public Eegeo::Messaging::CatalogBinding<PlaceJumps::PlaceJumpSelectedMessage>
            , public Eegeo::Messaging::CatalogBinding<Modality::ModalityChangedMessage>
            , public Eegeo::Messaging::CatalogBinding<Modality::UpdateNativeModalBackgroundMessage>
            , public Eegeo::Messaging::CatalogBinding<Compass::CompassViewCycledMessage>
            , public Eegeo::Messaging::CatalogBinding<SearchResultSection::SearchResultViewClearedMessage>
            , public Eegeo::Messaging::CatalogBinding<MyPinCreation::MyPinCreationViewStateChangedMessage>
            , public Eegeo::Messaging::CatalogBinding<MyPinCreation::MyPinCreationViewSavePinMessage>
            , public Eegeo::Messaging::CatalogBinding<MyPinDetails::MyPinDetailsViewRemovePinMessage>
            , public Eegeo::Messaging::CatalogBinding<MyPins::MyPinSelectedMessage>
            , public Eegeo::Messaging::CatalogBinding<WorldPins::WorldPinsVisibilityMessage>
            , public Eegeo::Messaging::CatalogBinding<Options::StreamOverWifiOnlyChangedMessage>
            , public Eegeo::Messaging::CatalogBinding<Options::CacheEnabledChangedMessage>
            , public Eegeo::Messaging::CatalogBinding<Options::ClearCacheMessage>
            , public Eegeo::Messaging::CatalogBinding<GpsMarker::GpsMarkerVisibilityMessage>
        	, public Eegeo::Messaging::CatalogBinding<InitialExperience::InitialExperienceIntroDismissedMessage>
            , public Eegeo::Messaging::CatalogBinding<SearchResultPoi::SearchResultPoiPinToggledMessage>
            , public Eegeo::Messaging::CatalogBinding<NavRouting::NavigateToMessage>
            , public Eegeo::Messaging::CatalogBinding<SearchResultPoi::SearchResultPoiViewOpenedMessage>
            , public Eegeo::Messaging::CatalogBinding<SearchResultPoi::SearchResultPoiViewClosedMessage>
            , public Eegeo::Messaging::CatalogBinding<InteriorsExplorer::InteriorsExplorerSelectFloorMessage>
            , public Eegeo::Messaging::CatalogBinding<InteriorsExplorer::InteriorsExplorerFloorSelectionDraggedMessage>
            , public Eegeo::Messaging::CatalogBinding<InteriorsExplorer::InteriorsExplorerExitMessage>
            , public Eegeo::Messaging::CatalogBinding<Menu::MenuDragStateChangedMessage>
            , public Eegeo::Messaging::CatalogBinding<Menu::MenuSectionExpandedChangedMessage>
            , public Eegeo::Messaging::CatalogBinding<WorldPins::WorldPinsSelectedFocussedMessage>
            , public Eegeo::Messaging::CatalogBinding<Surveys::SurveyTimeRequirementMetMessage>
            , public Eegeo::Messaging::CatalogBinding<InteriorsPosition::InteriorsLocationChangedMessage>
            , public Eegeo::Messaging::CatalogBinding<InteriorsPosition::InteriorsLocationAuthorizationChangedMessage>
            , public Eegeo::Messaging::CatalogBinding<NavRouting::NavRoutingViewStartEndLocationSwappedMessage>
            , public Eegeo::Messaging::CatalogBinding<NavRouting::NavRoutingSelectStartLocationClickedMessage>
            , public Eegeo::Messaging::CatalogBinding<NavRouting::NavRoutingSelectEndLocationClickedMessage>
            , public Eegeo::Messaging::CatalogBinding<NavRouting::NavRoutingStartLocationClearClickedMessage>
            , public Eegeo::Messaging::CatalogBinding<NavRouting::NavRoutingEndLocationClearClickedMessage>
            , public Eegeo::Messaging::CatalogBinding<NavRouting::NavRoutingSelectedDirectionChangedMessage>
            , public Eegeo::Messaging::CatalogBinding<NavRouting::NavRoutingStartEndRoutingButtonClickedMessage>
            , public Eegeo::Messaging::CatalogBinding<NavRouting::NavRoutingViewClosedMessage>
            , public Eegeo::Messaging::CatalogBinding<NavRouting::NavRoutingViewOpenedMessage>
            , public Eegeo::Messaging::CatalogBinding<NavRouting::NavRoutingRerouteDialogClosedMessage>
            , public Eegeo::Messaging::CatalogBinding<NavRouting::NavRoutingStartLocationSetFromSearchMessage>
            , public Eegeo::Messaging::CatalogBinding<NavRouting::NavRoutingEndLocationSetFromSearchMessage>
            , public Eegeo::Messaging::CatalogBinding<NavRouting::NavRoutingSearchForLocationMessage>
            , private Eegeo::NonCopyable
        {

        };
    }
}
