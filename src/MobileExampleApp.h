// Copyright eeGeo Ltd (2012-2015), All Rights Reserved

#pragma once

#include <map>
#include <string>
#include <vector>
#include "GlobeCamera.h"
#include "EegeoWorld.h"
#include "AppInterface.h"
#include "WorldPins.h"
#include "Search.h"
#include "SearchResultOnMap.h"
#include "NavigationService.h"
#include "IPlatformAbstractionModule.h"
#include "SearchResultSection.h"
#include "SearchMenu.h"
#include "Modality.h"
#include "Search.h"
#include "WorldPins.h"
#include "SearchResultOnMap.h"
#include "Pins.h"
#include "Reaction.h"
#include "PlaceJumps.h"
#include "IIdentity.h"
#include "SearchResultPoi.h"
#include "WeatherMenu.h"
#include "CameraTransitions.h"
#include "ModalityModule.h"
#include "MenuModel.h"
#include "MenuViewModel.h"
#include "MenuOptionsModel.h"
#include "AboutPageModule.h"
#include "SearchModule.h"
#include "SearchResultOnMapModule.h"
#include "IAutocompleteSuggestionQueryPerformer.h"
#include "WorldPinsModule.h"
#include "RegularTexturePageLayout.h"
#include "PinsModule.h"
#include "SearchResultRepository.h"
#include "LatLongAltitude.h"
#include "ReactionModelModule.h"
#include "SearchResultPoiModule.h"
#include "PlaceJumpsModule.h"
#include "IPlaceJumpController.h"
#include "WeatherMenuModule.h"
#include "CompassModule.h"
#include "TagSearch.h"
#include "WorldAreaLoader.h"
#include "InitialExperience.h"
#include "IInitialExperienceModule.h"
#include "MyPinCreation.h"
#include "MyPinCreationDetails.h"
#include "MyPins.h"
#include "MyPinDetails.h"
#include "PersistentSettings.h"
#include "BidirectionalBus.h"
#include "CameraFrustumStreamingVolume.h"
#include "ScreenProperties.h"
#include "PlatformConfig.h"
#include "OptionsModule.h"
#include "PlatformAbstractionsFacade.h"
#include "NetIncludes.h"
#include "GpsMarker.h"
#include "SdkModelDomainEventBus.h"
#include "IMetricsService.h"
#include "Watermark.h"
#include "IWatermarkModule.h"
#include "ApplicationConfiguration.h"
#include "InteriorsExplorer.h"
#include "MapMode.h"
#include "AppModes.h"
#include "IAppCameraModule.h"
#include "CameraTransitionService.h"
#include "UserInteraction.h"
#include "VisualMap.h"
#include "Surveys.h"
#include "IMenuReactionModel.h"
#include "IMenuIgnoredReactionModel.h"
#include "DoubleTapIndoorInteraction.h"		
#include "IRayCaster.h"
#include "DeepLink.h"
#include "YelpCategoryMapperUpdater.h"
#include "IUserIdleService.h"
#include "GlobalAppModeTransitionRules.h"
#include "CameraSplinePlaybackController.h"
#include "SurveyTimer.h"
#include "InitialLocation.h"
#include "AutomatedScreenshotController.h"
#include "SelectFirstResultSearchService.h"
#include "MapsceneModule.h"
#include "NavUIModule.h"
#include "NavRoutingModule.h"
#include "IAboutPageMenuModule.h"
#include "IOptionsMenuModule.h"
#include "InteriorEntityHighlightController.h"
#include "ICallback.h"
#include "UiCreatedMessage.h"
#include "MapCamera.h"
#include "ILocationProvider.h"

namespace ExampleApp
{
    class MobileExampleApp : private Eegeo::NonCopyable
    {
    private:
        Eegeo::Camera::GlobeCamera::GpsGlobeCameraController* m_pGlobeCameraController;
        AppCamera::SdkModel::AppGlobeCameraWrapper* m_pGlobeCameraWrapper;
        Eegeo::Camera::SplinePlayback::CameraSplinePlaybackController* m_pCameraSplinePlaybackController;
        AppCamera::SdkModel::AppInteriorCameraWrapper* m_pInteriorCameraWrapper;
        Eegeo::ITouchController* m_pCameraTouchController;
        Eegeo::ITouchController* m_pCurrentTouchController;
        Eegeo::EegeoWorld* m_pWorld;
        Eegeo::Location::NavigationService* m_pNavigationService;
        PlatformAbstractionsFacade m_platformAbstractions;
        Eegeo::Rendering::LoadingScreen* m_pLoadingScreen;
        Eegeo::Rendering::ScreenProperties m_screenProperties;
        bool m_initialisedApplicationViewState;
        bool m_setMetricsLocation;
        float m_pinDiameter;

        CameraTransitions::SdkModel::ICameraTransitionController* m_pCameraTransitionController;
        CameraTransitions::SdkModel::CameraTransitionService* m_pCameraTransitionService;

        ExampleApp::PersistentSettings::IPersistentSettingsModel& m_persistentSettings;
        ExampleApp::Metrics::IMetricsService& m_metricsService;
        
        Eegeo::Helpers::IdentityProvider m_identityProvider;
        AboutPage::SdkModel::IAboutPageMenuModule* m_pAboutPageMenuModule;
        Options::SdkModel:: IOptionsMenuModule* m_pOptionsMenuModule;
        ExampleApp::SearchMenu::SdkModel::ISearchMenuModule* m_pSearchMenuModule;
        ExampleApp::NavRouting::SdkModel::NavRoutingModule* m_pNavRoutingModule;
        ExampleApp::NavRouting::View::NavUIModule* m_pNavUIModule;
        ExampleApp::SearchResultSection::SdkModel::ISearchResultSectionModule* m_pSearchResultSectionModule;
        ExampleApp::Modality::View::IModalityModule* m_pModalityModule;
        ExampleApp::TagSearch::SdkModel::ITagSearchModule* m_pTagSearchModule;
        ExampleApp::MapMode::SdkModel::IMapModeModule* m_pMapModeModule;
        Search::SdkModel::ISearchModule* m_pSearchModule;
        Eegeo::Pins::PinsModule* m_pPinsModule;
        ExampleApp::WorldPins::SdkModel::IWorldPinIconMapping* m_pWorldPinsIconMapping;
        ExampleApp::WorldPins::SdkModel::IWorldPinsModule* m_pWorldPinsModule;
        ExampleApp::WorldPins::SdkModel::IWorldPinsFocusModule* m_pWorldPinsFocusModule;
        SearchResultOnMap::SdkModel::ISearchResultOnMapModule* m_pSearchResultOnMapModule;
        ExampleApp::Reaction::View::IReactionModelModule* m_pReactionModelModule;
        ExampleApp::SearchResultPoi::View::ISearchResultPoiModule* m_pSearchResultPoiModule;
        ExampleApp::PlaceJumps::SdkModel::IPlaceJumpsModule* m_pPlaceJumpsModule;
        ExampleApp::WeatherMenu::SdkModel::IWeatherMenuModule* m_pWeatherMenuModule;
        ExampleApp::Compass::SdkModel::ICompassModule* m_pCompassModule;
        ExampleApp::GpsMarker::SdkModel::IGpsMarkerModule* m_pGpsMarkerModule;
        ExampleApp::WorldAreaLoader::SdkModel::IWorldAreaLoaderModule* m_pWorldAreaLoaderModule;
        ExampleApp::AboutPage::View::IAboutPageModule* m_pAboutPageModule;
        ExampleApp::InitialExperience::SdkModel::IInitialExperienceModule& m_initialExperienceModule;
        ExampleApp::MyPinCreation::PoiRing::SdkModel::IPoiRingModule* m_pPoiRingModule;
        ExampleApp::MyPinCreation::SdkModel::IMyPinCreationModule* m_pMyPinCreationModule;
        ExampleApp::MyPinCreationDetails::View::IMyPinCreationDetailsModule* m_pMyPinCreationDetailsModule;
        ExampleApp::MyPins::SdkModel::IMyPinsModule* m_pMyPinsModule;
        ExampleApp::MyPinDetails::SdkModel::IMyPinDetailsModule* m_pMyPinDetailsModule;
        ExampleApp::Options::IOptionsModule* m_pOptionsModule;
        Watermark::SdkModel::IWatermarkModule* m_pWatermarkModule;
        ExampleApp::ApplicationConfig::ApplicationConfiguration m_applicationConfiguration;
        Eegeo::Streaming::CameraFrustumStreamingVolume* m_pStreamingVolume;
        ExampleAppMessaging::TMessageBus& m_messageBus;
        ExampleAppMessaging::TSdkModelDomainEventBus& m_sdkDomainEventBus;
        Net::SdkModel::INetworkCapabilities& m_networkCapabilities;
        std::map<std::string,ExampleApp::Search::SdkModel::ISearchServiceModule*> m_searchServiceModules;
        Search::SdkModel::ISearchServiceModule* m_pSearchServiceModule;
        Search::SelectFirstResult::SdkModel::SelectFirstResultSearchService* m_pSelectFirstResultSearchService;
        Search::SdkModel::IAutocompleteSuggestionQueryPerformer* m_pSuggestionsQueryPerformer;
        InteriorsExplorer::SdkModel::IInteriorsExplorerModule* m_pInteriorsExplorerModule;
        UserInteraction::SdkModel::UserInteractionModule* m_pUserInteractionModule;
        VisualMap::SdkModel::IVisualMapModule* m_pVisualMapModule;
        Surveys::SdkModel::ISurveyModule* m_pSurveyModule;
        Surveys::SdkModel::SurveyTimer* m_pSurveyTimer;
        DeepLink::SdkModel::DeepLinkModule* m_pDeepLinkModule;
        Mapscene::SdkModel::MapsceneModule* m_pMapsceneModule;
        InteriorsExplorer::SdkModel::Highlights::InteriorEntityHighlightController* m_pInteriorsHighlightVisibilityController;
        InteriorsExplorer::SdkModel::Highlights::IHighlightColorMapper* m_pHighlightColorMapper;
        ExampleApp::InitialLocation::SdkModel::IInitialLocationModel* m_pInitialLocationModel;
        
        Search::Yelp::SdkModel::YelpCategoryMapperUpdater m_yelpCategoryMapperUpdater;
        
        AppModes::SdkModel::IAppModeModel* m_pAppModeModel;
        Net::SdkModel::ConnectivityChangedObserver* m_pConnectivityChangedObserver;
        
        AppCamera::SdkModel::IAppCameraModule* m_pAppCameraModule;
        Eegeo::Camera::MapCamera::MapCameraDistanceBasedFovControl* m_pMapCameraDistanceBasedFovControl;
        Eegeo::Camera::MapCamera::MapCameraDistanceFromBoundsCalculator* m_pMapCameraDistanceFromBoundsCalculator;
        ExampleApp::DoubleTapIndoorInteraction::SdkModel::IDoubleTapIndoorInteractionController* m_pDoubleTapIndoorInteractionController;
        Eegeo::Collision::IRayCaster* m_pRayCaster;

        Menu::View::IMenuReactionModel& m_menuReaction;
        Menu::View::IMenuIgnoredReactionModel* m_pModalityIgnoredReactionModel;
        Menu::View::IMenuIgnoredReactionModel* m_pReactorIgnoredReactionModel;

        Eegeo::Input::IUserIdleService& m_userIdleService;
        AppModes::GlobalAppModeTransitionRules* m_pGlobalAppModeTransitionRules;

        Automation::AutomatedScreenshotController* m_pAutomatedScreenshotController;

        ExampleApp::Automation::IScreenshotService& m_screenshotService;

		Eegeo::Helpers::TCallback1<MobileExampleApp, const UiCreatedMessage&> m_onUiCreatedCallback;

        Eegeo::Location::ILocationService& m_platformLocationService;
        Eegeo::Helpers::CurrentLocationService::CurrentLocationService* m_pCurrentLocationService;
        LocationProvider::ILocationProvider * m_pLocationProvider;

        void CreateApplicationModelModules(Eegeo::UI::NativeUIFactories& nativeUIFactories,
                                           const bool interiorsAffectedByFlattening,
                                           const bool createBlueSphereViews);

        void DestroyApplicationModelModules();

        void UpdateLoadingScreen(float dt);

        std::vector<ExampleApp::OpenableControl::View::IOpenableControlViewModel*> GetOpenableControls() const;

        std::vector<ExampleApp::ScreenControl::View::IScreenControlViewModel*> GetReactorControls() const;
        std::vector<ExampleApp::Reaction::View::IReaction*> GetReactions() const;
        
        Eegeo::Pins::PinsModule* CreatePlatformPinsModuleInstance(Eegeo::EegeoWorld& world,
            const Eegeo::Helpers::GLHelpers::TextureInfo& pinTextureInfo,
            const Eegeo::Rendering::AtlasTexturePageLayout& atlasTexturePageLayou);

        void InitialisePinsModules(Eegeo::Modules::Map::MapModule& mapModule,
                                   Eegeo::EegeoWorld& world,
                                   const bool interiorsAffectedByFlattening,
                                   const float screenOversampleScale);
        
        void InitialiseAppState(Eegeo::UI::NativeUIFactories& nativeUIFactories);

        bool CanAcceptTouch() const;

		void OnUiCreated(const UiCreatedMessage& message);

    public:
        MobileExampleApp(const ExampleApp::ApplicationConfig::ApplicationConfiguration& applicationConfiguration,
                         Eegeo::Modules::IPlatformAbstractionModule& platformAbstractions,
                         Eegeo::Rendering::ScreenProperties& screenProperties,
                         Eegeo::Location::ILocationService& nativeLocationService,
                         Eegeo::UI::NativeUIFactories& nativeUIFactories,
                         const Eegeo::Config::PlatformConfig& platformConfig,
                         Eegeo::Helpers::Jpeg::IJpegLoader& jpegLoader,
                         ExampleApp::InitialExperience::SdkModel::IInitialExperienceModule& initialExperienceModule,
                         ExampleApp::PersistentSettings::IPersistentSettingsModel& persistentSettings,
                         ExampleAppMessaging::TMessageBus& messageBus,
                         ExampleAppMessaging::TSdkModelDomainEventBus& sdkModelDomainEventBus,
                         ExampleApp::Net::SdkModel::INetworkCapabilities& networkCapabilities,
                         ExampleApp::Metrics::IMetricsService& metricsService,                         
                         Eegeo::IEegeoErrorHandler& errorHandler,
                         Menu::View::IMenuReactionModel& menuReaction,
                         Eegeo::Input::IUserIdleService& userIdleService,
                         ExampleApp::Automation::IScreenshotService& screenshotService);

        ~MobileExampleApp();

        Eegeo::EegeoWorld& World() const
        {
            return * m_pWorld;
        }

        const ExampleApp::ApplicationConfig::ApplicationConfiguration& GetApplicationConfiguration() const
        {
            return m_applicationConfiguration;
        }

        float PinDiameter() const
        {
            return m_pinDiameter;
        }
        
        CameraTransitions::SdkModel::ICameraTransitionController& CameraTransitionController() const
        {
            return *m_pCameraTransitionService;
        }

        Menu::View::IMenuIgnoredReactionModel& ModalityIgnoredReactionModel() const
        {
            return *m_pModalityIgnoredReactionModel;
        }

        Menu::View::IMenuIgnoredReactionModel& ReactorIgnoredReactionModel() const
        {
            return *m_pReactorIgnoredReactionModel;
        }
        
        const SearchMenu::SdkModel::ISearchMenuModule& SearchMenuModule() const
        {
            return *m_pSearchMenuModule;
        }
        
        const ExampleApp::NavRouting::View::NavUIModule& NavUIModule() const
        {
            return *m_pNavUIModule;
        }
        
        const SearchResultSection::SdkModel::ISearchResultSectionModule& SearchResultSectionModule() const
        {
            return *m_pSearchResultSectionModule;
        }

        const ExampleApp::Modality::View::IModalityModule& ModalityModule() const
        {
            return *m_pModalityModule;
        }

        const Search::SdkModel::ISearchModule& SearchModule() const
        {
            return *m_pSearchModule;
        }

        const Eegeo::Pins::PinsModule& PinsModule() const
        {
            return *m_pPinsModule;
        }

        const ExampleApp::WorldPins::SdkModel::IWorldPinsModule& WorldPinsModule() const
        {
            return *m_pWorldPinsModule;
        }
        
        const ExampleApp::WorldPins::SdkModel::IWorldPinsFocusModule* WorldPinsFocusModule() const
        {
            return m_pWorldPinsFocusModule;
        }
        
        const SearchResultOnMap::SdkModel::ISearchResultOnMapModule& SearchResultOnMapModule() const
        {
            return *m_pSearchResultOnMapModule;
        }

        const Reaction::View::IReactionModelModule& ReactionModelModule() const
        {
            return *m_pReactionModelModule;
        }

        const ExampleApp::SearchResultPoi::View::ISearchResultPoiModule& SearchResultPoiModule() const
        {
            return *m_pSearchResultPoiModule;
        }

        const ExampleApp::PlaceJumps::SdkModel::IPlaceJumpsModule& PlaceJumpsModule() const
        {
            return *m_pPlaceJumpsModule;
        }

        const ExampleApp::WeatherMenu::SdkModel::IWeatherMenuModule& WeatherMenuModule() const
        {
            return *m_pWeatherMenuModule;
        }

        const ExampleApp::Compass::SdkModel::ICompassModule& CompassModule() const
        {
            return *m_pCompassModule;
        }

        const ExampleApp::AboutPage::View::IAboutPageModule& AboutPageModule() const
        {
            return *m_pAboutPageModule;
        }

        const ExampleApp::TagSearch::SdkModel::ITagSearchModule& TagSearchModule() const
        {
            return *m_pTagSearchModule;
        }

        const ExampleApp::MyPinCreation::SdkModel::IMyPinCreationModule& MyPinCreationModule() const
        {
            return *m_pMyPinCreationModule;
        }

        const ExampleApp::MyPinCreation::PoiRing::SdkModel::IPoiRingModule& PoiRingModule() const
        {
            return *m_pPoiRingModule;
        }

        const ExampleApp::MyPinCreationDetails::View::IMyPinCreationDetailsModule& MyPinCreationDetailsModule() const
        {
            return *m_pMyPinCreationDetailsModule;
        }

        const ExampleApp::MyPins::SdkModel::IMyPinsModule& MyPinsModule() const
        {
            return *m_pMyPinsModule;
        }

        const ExampleApp::MyPinDetails::SdkModel::IMyPinDetailsModule& MyPinDetailsModule() const
        {
            return *m_pMyPinDetailsModule;
        }
        
        const ExampleApp::Options::IOptionsModule& OptionsModule() const
        {
            return *m_pOptionsModule;
        }
        
        const ExampleApp::Watermark::SdkModel::IWatermarkModule& WatermarkModule() const
        {
            return *m_pWatermarkModule;
        }
        
        const InteriorsExplorer::SdkModel::IInteriorsExplorerModule& InteriorsExplorerModule() const
        {
            return *m_pInteriorsExplorerModule;
        }

        const GpsMarker::SdkModel::IGpsMarkerModule& GpsMarkerModule() const
        {
            return *m_pGpsMarkerModule;
        }
        
        const Mapscene::SdkModel::MapsceneModule& GetMapsceneModule() const
        {
            return *m_pMapsceneModule;
        }
        
        // Exposed to allow view model creation in iOS code.
        // Remove this.
        Eegeo::Helpers::IdentityProvider& GetIdentityProvider()
        {
            return m_identityProvider;
        }

        AppModes::SdkModel::IAppModeModel& GetAppModeModel() const
        {
            return *m_pAppModeModel;
        }

        Eegeo::Location::NavigationService& GetNavigationService() const
        {
            return *m_pNavigationService;
        }

        LocationProvider::ILocationProvider& GetLocationProvider() const
        {
            return *m_pLocationProvider;
        }

        void OnPause();

        void OnResume();

        void Update (float dt);

        void Draw (float dt);

        void NotifyScreenPropertiesChanged(const Eegeo::Rendering::ScreenProperties& screenProperties);


        Eegeo::Streaming::IStreamingVolume& GetUpdatedStreamingVolume(const Eegeo::Camera::CameraState& cameraState,
                const Eegeo::Camera::RenderCamera& renderCamera);

        bool IsLoadingScreenComplete() const
        {
            return m_pLoadingScreen == NULL;
        }

        bool IsRunning() const;

        bool IsApplicationViewStateInitialised() const
        {
            return m_initialisedApplicationViewState;
        }

        void InitialiseApplicationViewState();

        Eegeo::Camera::GlobeCamera::GpsGlobeCameraController& GetCameraController()
        {
            return *m_pGlobeCameraController;
        }

        void Event_TouchRotate 			(const AppInterface::RotateData& data);
        void Event_TouchRotate_Start	(const AppInterface::RotateData& data);
        void Event_TouchRotate_End 		(const AppInterface::RotateData& data);

        void Event_TouchPinch 			(const AppInterface::PinchData& data);
        void Event_TouchPinch_Start 	(const AppInterface::PinchData& data);
        void Event_TouchPinch_End 		(const AppInterface::PinchData& data);

        void Event_TouchPan				(const AppInterface::PanData& data);
        void Event_TouchPan_Start		(const AppInterface::PanData& data);
        void Event_TouchPan_End 		(const AppInterface::PanData& data);

        void Event_TouchTap 			(const AppInterface::TapData& data);
        void Event_TouchDoubleTap		(const AppInterface::TapData& data);

        void Event_TouchDown 			(const AppInterface::TouchData& data);
        void Event_TouchMove 			(const AppInterface::TouchData& data);
        void Event_TouchUp 				(const AppInterface::TouchData& data);

        void Event_Zoom(const AppInterface::ZoomData& data);
        void Event_Keyboard(const AppInterface::KeyboardData& data);

        void Event_TiltStart(const AppInterface::TiltData& data);
        void Event_TiltEnd(const AppInterface::TiltData& data);
        void Event_Tilt(const AppInterface::TiltData& data);
        void Event_OpenUrl(const AppInterface::UrlData& data);

        Automation::AutomatedScreenshotController* AutomatedScreenshotController() const;
    };
}
