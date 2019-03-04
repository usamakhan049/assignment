// Copyright eeGeo Ltd (2012-2015), All Rights Reserved

#include "MobileExampleApp.h"
#include "ApiKey.h"
#include "GlobeCameraTouchController.h"
#include "RenderCamera.h"
#include "GpsGlobeCameraController.h"
#include "GlobeCameraInterestPointProvider.h"
#include "CameraHelpers.h"
#include "LatLongAltitude.h"
#include "IWorldPinsService.h"
#include "ISearchRefreshService.h"
#include "GpsGlobeCameraControllerFactory.h"
#include "GlobeCameraControllerFactory.h"
#include "GlobeCameraTouchSettings.h"
#include "GlobeCameraController.h"
#include "GpsGlobeCameraComponentConfiguration.h"
#include "InteriorsCameraController.h"
#include "InteriorsCameraControllerFactory.h"
#include "InteriorsGpsCameraController.h"
#include "InteriorsGpsCameraControllerFactory.h"
#include "ITextureFileLoader.h"
#include "IWeatherMenuModule.h"
#include "SearchMenuModule.h"
#include "AutocompleteSuggestionQueryPerformer.h"
#include "CompassUpdateController.h"
#include "CameraTransitionController.h"
#include "WorldAreaLoaderModule.h"
#include "IInitialExperienceModel.h"
#include "IInitialExperienceController.h"
#include "TagSearchModule.h"
#include "AboutPageModule.h"
#include "RenderContext.h"
#include "ScreenProperties.h"
#include "TerrainModelModule.h"
#include "MapModule.h"
#include "CityThemesModule.h"
#include "RenderingModule.h"
#include "StreamingModule.h"
#include "EnvironmentCharacterSet.h"
#include "IPoiRingTouchController.h"
#include "MyPinCreationModule.h"
#include "SearchResultSectionModule.h"
#include "PoiRingModule.h"
#include "IPoiRingController.h"
#include "MyPinCreationDetailsModule.h"
#include "MyPinsModule.h"
#include "IWorldPinsVisibilityController.h"
#include "MyPinDetailsModule.h"
#include "QuadTreeCube.h"
#include "LodRefinementConfig.h"
#include "StreamingVolumeController.h"
#include "GpsMarkerModule.h"
#include "IGpsMarkerController.h"
#include "INetworkCapabilities.h"
#include "ISearchServiceModule.h"
#include "IMyPinsService.h"
#include "IEegeoErrorHandler.h"
#include "OptionsMenuOption.h"
#include "AboutPageMenuOption.h"
#include "ImagePathHelpers.h"
#include "WatermarkModule.h"
#include "InteriorsPresentationModule.h"
#include "InteriorsModelModule.h"
#include "InteriorsExplorerModule.h"
#include "PinsModule.h"
#include "MapModeModule.h"
#include "AppModeModel.h"
#include "ICompassViewModel.h"
#include "CombinedSearchServiceModule.h"
#include "SelectFirstResultSearchService.h"
#include "GeoNamesSearchServiceModule.h"
#include "EegeoSearchServiceModule.h"
#include "SearchVendorNames.h"
#include "YelpSearchConstants.h"
#include "YelpSearchServiceModule.h"
#include "AppCameraModule.h"
#include "AppCameraController.h"
#include "AppModeStatesFactory.h"
#include "AppGlobeCameraWrapper.h"
#include "AppInteriorCameraWrapper.h"
#include "EnvironmentRayCaster.h"
#include "NativeUIFactories.h"
#include "UserInteractionModule.h"
#include "UserInteractionModel.h"
#include "EnvironmentFlatteningService.h"
#include "SceneModelsModule.h"
#include "VisualMapModule.h"
#include "ConnectivityChangedObserver.h"
#include "SurveyModule.h"
#include "SurveyObserver.h"
#include "InteriorsResourceCounts.h"
#include "HttpAsyncTextureLoader.h"
#include "HttpAsyncCubeTextureLoader.h"
#include "InteriorsMaterialsModule.h"
#include "IInteriorsTextureResourceService.h"
#include "InteriorMaterialSemantics.h"
#include "IInteriorsMaterialDtoRepository.h"
#include "InteriorsMaterialDto.h"
#include "InteriorsMaterialParser.h"
#include "InteriorsMaterialDescriptorLoader.h"
#include "ModalityIgnoredReactionModel.h"
#include "ReactorIgnoredReactionModel.h"
#include "WorldPinIconMappingFactory.h"
#include "IWorldPinIconMapping.h"
#include "DoubleTapIndoorInteractionController.h"
#include "IDoubleTapIndoorInteractionController.h"
#include "AggregateCollisionBvhProvider.h"
#include "TagSearchModelFactory.h"
#include "SearchTagsFactory.h"
#include "ITagSearchRepository.h"
#include "Colors.h"
#include "HighlightColorMapper.h"
#include "DeepLinkModule.h"
#include "DeepLinkController.h"
#include "InteriorsStreamingModule.h"
#include "InteriorMetaDataModule.h"
#include "CoverageTreeModule.h"
#include "GlobalAppModeTransitionRules.h"
#include "ILabelOptionsModel.h"
#include "MapLayersModule.h"
#include "MarkersModule.h"
#include "CameraSplinePlaybackController.h"
#include "InitialLocationModel.h"
#include "AutomatedScreenshotController.h"
#include "TrafficSimulationController.h"
#include "IScreenshotService.h"
#include "InteriorHighlightsModule.h"
#include "IInteriorsHighlightService.h"
#include "MapsceneModule.h"
#include "PolylineShapesModule.h"
#include "AboutPageMenuModule.h"
#include "OptionsMenuModule.h"
#include "FeatureInteractionModule.h"
#include "CollisionModule.h"
#include "INavRoutingCustomLocationPicker.h"
#include "MapCameraDistanceBasedFovControl.h"
#include "MapCameraDistanceFromBoundsCalculator.h"
#include "ReactionHideOtherScreenControls.h"
#include "ReactionPushScreenControl.h"
#include "LocationProvider.h"
#include "WhitelistUrlHelpersNative.h"

namespace ExampleApp
{
    namespace
    {
        float PanToUnlockThreshold = 0.03f;

        Eegeo::Rendering::LoadingScreen* CreateLoadingScreen(const Eegeo::Rendering::ScreenProperties& screenProperties,
                                                             const Eegeo::Modules::Core::RenderingModule& renderingModule,
                                                             const Eegeo::Modules::IPlatformAbstractionModule& platformAbstractionModule)
        {
            const Eegeo::v4 bgColor = Eegeo::v4(240.0f/255.0f, 240.0f/255.f, 240.0f/255.f, 1.0f);
            Eegeo::Rendering::LoadingScreenConfig loadingScreenConfig;
            loadingScreenConfig.layout = Eegeo::Rendering::LoadingScreenLayout::Centred;
            loadingScreenConfig.backgroundColor = bgColor;
            loadingScreenConfig.loadingBarColor = Eegeo::v4(0.0f/255.0f, 113.0f/255.0f, 188.0f/255.0f, 1.0f);
            loadingScreenConfig.loadingBarBackgroundColor = bgColor;
            loadingScreenConfig.fadeOutDurationSeconds = 1.5f;
            loadingScreenConfig.screenWidth = screenProperties.GetScreenWidth();
            loadingScreenConfig.screenHeight = screenProperties.GetScreenHeight();
            loadingScreenConfig.screenOversampleScaling = screenProperties.GetOversampleScale();
            loadingScreenConfig.loadingBarOffset = Eegeo::v2(0.5f, 0.1f);
            Eegeo::Rendering::LoadingScreen* loadingScreen = Eegeo::Rendering::LoadingScreen::Create(
                                                                                                     Helpers::ImageHelpers::GetImageNameForDevice("SplashScreen", ".png"),
                                                                                                     loadingScreenConfig,
                                                                                                     renderingModule.GetShaderIdGenerator(),
                                                                                                     renderingModule.GetMaterialIdGenerator(),
                                                                                                     renderingModule.GetGlBufferPool(),
                                                                                                     renderingModule.GetVertexLayoutPool(),
                                                                                                     renderingModule.GetVertexBindingPool(),
                                                                                                     platformAbstractionModule.GetTextureFileLoader());
            return loadingScreen;
        }

        void AddTagSearchModels(
                                TagSearch::View::ITagSearchRepository& repository,
                                const ApplicationConfig::ApplicationConfiguration& applicationConfig,
                                Search::Yelp::SdkModel::YelpCategoryMapperUpdater& yelpCategoryMapperUpdater)
        {
            const auto& tagSearchModels = TagSearch::View::CreateTagSearchModelsFromConfig(
                                                                                         applicationConfig,
                                                                                         "outdoor_search_menu_items",
                                                                                         yelpCategoryMapperUpdater);
            if(repository.GetItemCount() == 0)
            {
                for (const auto& t : tagSearchModels)
                {
                    repository.AddItem(t);
                }
            }
            else
            {
                repository.ForceRefresh();
            }
        }
    }

    MobileExampleApp::MobileExampleApp(
                                       const ExampleApp::ApplicationConfig::ApplicationConfiguration& applicationConfiguration,
                                       Eegeo::Modules::IPlatformAbstractionModule& platformAbstractions,
                                       Eegeo::Rendering::ScreenProperties& screenProperties,
                                       Eegeo::Location::ILocationService& platformLocationService,
                                       Eegeo::UI::NativeUIFactories& nativeUIFactories,
                                       const Eegeo::Config::PlatformConfig& platformConfig,
                                       Eegeo::Helpers::Jpeg::IJpegLoader& jpegLoader,
                                       ExampleApp::InitialExperience::SdkModel::IInitialExperienceModule& initialExperienceModule,
                                       ExampleApp::PersistentSettings::IPersistentSettingsModel& persistentSettings,
                                       ExampleAppMessaging::TMessageBus& messageBus,
                                       ExampleAppMessaging::TSdkModelDomainEventBus& sdkModelDomainEventBus,
                                       Net::SdkModel::INetworkCapabilities& networkCapabilities,
                                       ExampleApp::Metrics::IMetricsService& metricsService,
                                       Eegeo::IEegeoErrorHandler& errorHandler,
                                       Menu::View::IMenuReactionModel& menuReaction,
                                       Eegeo::Input::IUserIdleService& userIdleService,
                                       ExampleApp::Automation::IScreenshotService& screenshotService)
    : m_pGlobeCameraController(NULL)
    , m_pCameraTouchController(NULL)
    , m_pCurrentTouchController(NULL)
    , m_pNavigationService(NULL)
    , m_pWorld(NULL)
    , m_platformAbstractions(platformAbstractions, networkCapabilities)
    , m_pLoadingScreen(NULL)
    , m_pinDiameter(28.f)
    , m_initialisedApplicationViewState(false)
    , m_pCameraTransitionController(NULL)
    , m_pAboutPageMenuModule(NULL)
    , m_pOptionsMenuModule(NULL)
    , m_pSearchMenuModule(NULL)
    , m_pNavUIModule(NULL)
    , m_pSearchResultSectionModule(NULL)
    , m_pModalityModule(NULL)
    , m_pTagSearchModule(NULL)
    , m_pMapModeModule(NULL)
    , m_pSearchModule(NULL)
    , m_pPinsModule(NULL)
    , m_pWorldPinsModule(NULL)
    , m_pWorldPinsIconMapping(NULL)
    , m_pSearchResultOnMapModule(NULL)
    , m_pReactionModelModule(NULL)
    , m_pSearchResultPoiModule(NULL)
    , m_pPlaceJumpsModule(NULL)
    , m_pWeatherMenuModule(NULL)
    , m_pCompassModule(NULL)
    , m_pGpsMarkerModule(NULL)
    , m_pWorldAreaLoaderModule(NULL)
    , m_pAboutPageModule(NULL)
    , m_pMapsceneModule(NULL)
    , m_initialExperienceModule(initialExperienceModule)
    , m_messageBus(messageBus)
    , m_sdkDomainEventBus(sdkModelDomainEventBus)
    , m_persistentSettings(persistentSettings)
    , m_pMyPinCreationModule(NULL)
    , m_pPoiRingModule(NULL)
    , m_pMyPinCreationDetailsModule(NULL)
    , m_pMyPinsModule(NULL)
    , m_pMyPinDetailsModule(NULL)
    , m_pOptionsModule(NULL)
    , m_pWatermarkModule(NULL)
    , m_pInteriorsExplorerModule(NULL)
    , m_screenProperties(screenProperties)
    , m_networkCapabilities(networkCapabilities)
    , m_setMetricsLocation(false)
    , m_pSearchServiceModule(NULL)
    , m_metricsService(metricsService)
    , m_applicationConfiguration(applicationConfiguration)
    , m_pGlobeCameraWrapper(NULL)
    , m_pCameraSplinePlaybackController(NULL)
    , m_pVisualMapModule(NULL)
    , m_pSurveyModule(NULL)
    , m_pConnectivityChangedObserver(NULL)
    , m_menuReaction(menuReaction)
    , m_pModalityIgnoredReactionModel(NULL)
    , m_pReactorIgnoredReactionModel(NULL)
    , m_pRayCaster(NULL)
    , m_pInteriorsHighlightVisibilityController(NULL)
    , m_pInitialLocationModel(NULL)
    , m_pHighlightColorMapper(NULL)
    , m_pDeepLinkModule(NULL)
    , m_userIdleService(userIdleService)
    , m_pGlobalAppModeTransitionRules(NULL)
    , m_pAutomatedScreenshotController(NULL)
    , m_screenshotService(screenshotService)
	, m_onUiCreatedCallback(this, &MobileExampleApp::OnUiCreated)
    , m_platformLocationService(platformLocationService)
    , m_pCurrentLocationService(NULL)
    , m_pLocationProvider(NULL)
    {
        if (m_applicationConfiguration.IsInKioskMode())
        {
            PanToUnlockThreshold = 0.0001f;
        }

        m_metricsService.BeginSession(m_applicationConfiguration.FlurryAppKey(), EEGEO_PLATFORM_VERSION_NUMBER);

        m_pCurrentLocationService = Eegeo_NEW(Eegeo::Helpers::CurrentLocationService::CurrentLocationService)(m_platformLocationService);

        m_pWorld = Eegeo_NEW(Eegeo::EegeoWorld)(applicationConfiguration.EegeoApiKey(),
                                                m_platformAbstractions,
                                                jpegLoader,
                                                screenProperties,
                                                *m_pCurrentLocationService,
                                                nativeUIFactories,
                                                Eegeo::EnvironmentCharacterSet::UseFontModuleConfig,
                                                platformConfig,
                                                NULL,
                                                &errorHandler
                                                );
        
        m_pWorld->GetMapModule().GetLabelsModule().GetLabelOptionsModel().SetOcclusionMode(Eegeo::Labels::OcclusionResolverMode::Always);

        m_pLocationProvider = Eegeo_NEW(LocationProvider::LocationProvider)(m_platformLocationService, m_pWorld->GetMapModule());
        m_pCurrentLocationService->SetLocationService(*m_pLocationProvider);

        m_pConnectivityChangedObserver = Eegeo_NEW(Net::SdkModel::ConnectivityChangedObserver)(m_pWorld->GetWebConnectivityValidator(), messageBus);

        Eegeo::Modules::Map::Layers::TerrainModelModule& terrainModelModule = m_pWorld->GetTerrainModelModule();
        Eegeo::Modules::Map::MapModule& mapModule = m_pWorld->GetMapModule();

        m_pNavigationService = Eegeo_NEW(Eegeo::Location::NavigationService)(&m_pWorld->GetLocationService(),
                                                                             &terrainModelModule.GetTerrainHeightProvider());

        Eegeo::Camera::GlobeCamera::GpsGlobeCameraControllerFactory cameraControllerFactory(terrainModelModule.GetTerrainHeightProvider(),
                                                                                            mapModule.GetEnvironmentFlatteningService(),
                                                                                            mapModule.GetResourceCeilingProvider(),
                                                                                            *m_pNavigationService);

        m_pAppModeModel = Eegeo_NEW(AppModes::SdkModel::AppModeModel)(m_messageBus);

        const bool useLowSpecSettings = false;

        Eegeo::Camera::GlobeCamera::GpsGlobeCameraComponentConfiguration gpsGlobeCameraConfig = Eegeo::Camera::GlobeCamera::GpsGlobeCameraComponentConfiguration::CreateDefault();
        gpsGlobeCameraConfig.panToUnlockThreshold = PanToUnlockThreshold;
        gpsGlobeCameraConfig.compassCameraOffset = m_applicationConfiguration.CompassCameraOffset();
        gpsGlobeCameraConfig.compassCameraOffsetTopDown = m_applicationConfiguration.CompassCameraOffsetTopDown();
        gpsGlobeCameraConfig.compassCameraDampingEnabled = m_applicationConfiguration.CompassCameraDampingEnabled();

        if(m_applicationConfiguration.IsInKioskMode() && m_applicationConfiguration.IsFixedIndoorLocationEnabled())
        {
            gpsGlobeCameraConfig.defaultGpsBearing = m_applicationConfiguration.FixedIndoorLocation().GetOrientationDegrees();
        }

        Eegeo::Camera::GlobeCamera::GlobeCameraTouchControllerConfiguration touchControllerConfig = Eegeo::Camera::GlobeCamera::GlobeCameraTouchControllerConfiguration::CreateDefault();
        Eegeo::Camera::GlobeCamera::GlobeCameraControllerConfiguration globeCameraConfig = Eegeo::Camera::GlobeCamera::GlobeCameraControllerConfiguration::CreateDefault(useLowSpecSettings);

        m_pMapCameraDistanceBasedFovControl = Eegeo::Camera::MapCamera::MapCameraDistanceBasedFovControl::CreateFromGlobeCameraConfig(globeCameraConfig);
        m_pMapCameraDistanceFromBoundsCalculator = Eegeo_NEW(Eegeo::Camera::MapCamera::MapCameraDistanceFromBoundsCalculator)(
                *m_pMapCameraDistanceBasedFovControl,
                globeCameraConfig.zoomAltitudeLow,
                globeCameraConfig.maxAltitude
        );

        m_pGlobeCameraController = cameraControllerFactory.Create(gpsGlobeCameraConfig, touchControllerConfig, globeCameraConfig, m_screenProperties);

        m_pGlobeCameraWrapper = Eegeo_NEW(AppCamera::SdkModel::AppGlobeCameraWrapper)(*m_pGlobeCameraController);

        m_pCameraSplinePlaybackController = Eegeo_NEW(Eegeo::Camera::SplinePlayback::CameraSplinePlaybackController)(mapModule.GetResourceCeilingProvider());

        m_pCameraTouchController = &m_pGlobeCameraController->GetTouchController();

        Eegeo::Camera::GlobeCamera::GlobeCameraTouchSettings touchSettings = m_pGlobeCameraController->GetGlobeCameraController().GetTouchSettings();
        touchSettings.TiltEnabled = true;
        m_pGlobeCameraController->GetGlobeCameraController().SetTouchSettings(touchSettings);

        m_pInitialLocationModel = Eegeo_NEW(ExampleApp::InitialLocation::SdkModel::InitialLocationModel)(m_persistentSettings, m_applicationConfiguration.InterestLocation());

        const Eegeo::Space::LatLongAltitude& location = m_applicationConfiguration.TryStartAtGpsLocation()
            ? m_pInitialLocationModel->GetInterestLocation()
            : m_applicationConfiguration.InterestLocation();

        float cameraControllerOrientationDegrees = m_applicationConfiguration.OrientationDegrees();
        float cameraControllerDistanceFromInterestPointMeters = m_applicationConfiguration.DistanceToInterestMetres();

        Eegeo::Space::EcefTangentBasis cameraInterestBasis;
        Eegeo::Camera::CameraHelpers::EcefTangentBasisFromPointAndHeading(location.ToECEF(), cameraControllerOrientationDegrees, cameraInterestBasis);

        m_pGlobeCameraController->SetView(cameraInterestBasis, cameraControllerDistanceFromInterestPointMeters);

        m_pRayCaster = Eegeo_NEW(Eegeo::Collision::EnvironmentRayCaster)(mapModule.GetAggregateCollisionBvhProvider(),mapModule.GetEnvironmentFlatteningService());

        Eegeo::Modules::Map::Layers::InteriorsPresentationModule& interiorsPresentationModule = mapModule.GetInteriorsPresentationModule();

        m_pCameraTransitionService = Eegeo_NEW(ExampleApp::CameraTransitions::SdkModel::CameraTransitionService)();


        m_pStreamingVolume = Eegeo_NEW(Eegeo::Streaming::CameraFrustumStreamingVolume)(mapModule.GetResourceCeilingProvider(),
                                                                                       Eegeo::Config::LodRefinementConfig::GetLodRefinementAltitudesForDeviceSpec(platformConfig.PerformanceConfig.DeviceSpecification),
                                                                                       Eegeo::Streaming::QuadTreeCube::MAX_DEPTH_TO_VISIT,
                                                                                       mapModule.GetEnvironmentFlatteningService());

        CreateApplicationModelModules(nativeUIFactories, platformConfig.OptionsConfig.InteriorsAffectedByFlattening, platformConfig.MapLayersConfig.BlueSphereConfig.CreateViews);
        
        Eegeo::Modules::Map::Layers::InteriorsModelModule& interiorsModelModule = mapModule.GetInteriorsModelModule();

        m_pCameraTransitionController = Eegeo_NEW(ExampleApp::CameraTransitions::SdkModel::CameraTransitionController)(*m_pGlobeCameraController,
                                                                                                                       m_pInteriorsExplorerModule->GetInteriorsCameraController(),
                                                                                                                       *m_pNavigationService,
                                                                                                                       terrainModelModule.GetTerrainHeightProvider(),
                                                                                                                       *m_pAppModeModel,
                                                                                                                       m_pAppCameraModule->GetController(),
                                                                                                                       interiorsPresentationModule.GetInteriorSelectionModel(),
                                                                                                                       interiorsPresentationModule.GetInteriorInteractionModel(),
                                                                                                                       interiorsPresentationModule.GetInteriorTransitionModel(),
                                                                                                                       m_pInteriorsExplorerModule->GetInteriorsExplorerModel(),
                                                                                                                       interiorsModelModule.GetInteriorsModelRepository(),
                                                                                                                       m_messageBus);
        m_pCameraTransitionService->SetTransitionController(*m_pCameraTransitionController);

        m_pDoubleTapIndoorInteractionController = Eegeo_NEW(ExampleApp::DoubleTapIndoorInteraction::SdkModel::DoubleTapIndoorInteractionController)(m_pInteriorsExplorerModule->GetInteriorsCameraController(),*m_pCameraTransitionController,interiorsPresentationModule.GetInteriorInteractionModel(),*m_pAppModeModel,interiorsPresentationModule.GetInteriorTransitionModel(),m_pWorld->GetTerrainModelModule(),m_pAppCameraModule->GetController());

        m_pLoadingScreen = CreateLoadingScreen(screenProperties, m_pWorld->GetRenderingModule(), m_pWorld->GetPlatformAbstractionModule());

        if (!m_pInitialLocationModel->HasPersistedInterestLocation())
        {
            if (m_applicationConfiguration.TryStartAtGpsLocation() && !m_applicationConfiguration.IsAttractModeEnabled())
            {
                m_pNavigationService->SetGpsMode(Eegeo::Location::NavigationService::GpsModeFollow);
            }
        }

        m_pGlobalAppModeTransitionRules = Eegeo_NEW(AppModes::GlobalAppModeTransitionRules)(m_pAppCameraModule->GetController(),
                                                                                            m_pInteriorsExplorerModule->GetInteriorsExplorerModel(),
                                                                                            interiorsPresentationModule.GetInteriorSelectionModel(),
                                                                                            GetAppModeModel(),
                                                                                            *m_pGlobeCameraWrapper,
                                                                                            *m_pInteriorCameraWrapper,
                                                                                            m_userIdleService,
                                                                                            m_applicationConfiguration.IsAttractModeEnabled(),
                                                                                            m_applicationConfiguration.AttractModeTimeoutMs(),
                                                                                            m_pMyPinCreationModule->GetMyPinCreationModel(),
                                                                                            m_pMyPinsModule->GetMyPinsService());
        InitialiseAppState(nativeUIFactories);

        m_pUserInteractionModule = Eegeo_NEW(UserInteraction::SdkModel::UserInteractionModule)(m_pAppCameraModule->GetController(), *m_pCameraTransitionService, m_pInteriorsExplorerModule->GetInteriorsExplorerUserInteractionModel(), m_messageBus);
        
        if (!applicationConfiguration.TryStartAtGpsLocation())
        {
            const float heading = Eegeo::Math::Deg2Rad(applicationConfiguration.OrientationDegrees());
            m_pCameraTransitionController->StartTransitionTo(location.ToECEF(), m_applicationConfiguration.DistanceToInterestMetres(), heading, m_applicationConfiguration.IndoorId(), applicationConfiguration.FloorIndex());
        }
       
        m_pMapsceneModule = Eegeo_NEW(Mapscene::SdkModel::MapsceneModule)(*m_pCameraTransitionController,
                                                                          m_platformAbstractions.GetWebLoadRequestFactory(),
                                                                          m_pWorld->GetNativeUIFactories().AlertBoxFactory(),
                                                                          m_applicationConfiguration,
                                                                          m_pWorld->GetMapModule().GetCoverageTreeModule().GetCoverageTreeLoader(),
                                                                          m_pWorld->GetMapModule().GetCoverageTreeModule().CoverageTreeManifestNotifier(),
                                                                          m_pWorld->GetMapModule().GetCityThemesModule().GetCityThemeLoader(),
                                                                          m_pWorld->GetMapModule().GetCityThemesModule().GetCityThemesService(),
                                                                          m_pSearchModule->GetInteriorMenuObserver(),
                                                                          m_pSearchModule->GetSearchQueryPerformer(),
                                                                          m_pAboutPageModule->GetAboutPageViewModel(),
                                                                          *m_pNavigationService,
                                                                          m_pWorld->GetApiTokenService(),
                                                                          interiorsPresentationModule.GetInteriorSelectionModel(),
                                                                          *m_pAppModeModel,
                                                                          mapModule,
                                                                          *m_pLocationProvider);

        m_pDeepLinkModule = Eegeo_NEW(DeepLink::SdkModel::DeepLinkModule)(
            *m_pCameraTransitionController,
            m_pWorld->GetNativeUIFactories().AlertBoxFactory(),
            *m_pSelectFirstResultSearchService,
            m_pMapsceneModule->GetMapsceneLoader());

        if (applicationConfiguration.HasMapScene())
        {
            // TODO: This is a bit smelly & needs refactoring because the MapScene responsibility is tightly coupled
            // to the DeepLink domain. Building up a fake deep link for now....
            const std::string mapScenePath = "/" + applicationConfiguration.MapSceneId();
            AppInterface::UrlData data;
            data.host = "mapscene";
            data.path = mapScenePath.c_str();
            m_pDeepLinkModule->GetDeepLinkController().HandleDeepLinkOpen(data);
        }
    }
    
    MobileExampleApp::~MobileExampleApp()
    {
        OnPause();

        m_messageBus.UnsubscribeNative(m_onUiCreatedCallback);

        m_pAppModeModel->DestroyStateMachine();

        Eegeo_DELETE m_pDeepLinkModule;
        
        Eegeo_DELETE m_pMapsceneModule;

        Eegeo_DELETE m_pUserInteractionModule;

        Eegeo_DELETE m_pGlobalAppModeTransitionRules;

        Eegeo_DELETE m_pStreamingVolume;

        Eegeo_DELETE m_pInteriorsHighlightVisibilityController;
        Eegeo_DELETE m_pHighlightColorMapper;
        DestroyApplicationModelModules();
        Eegeo_DELETE m_pRayCaster;
        Eegeo_DELETE m_pCameraTransitionService;
        Eegeo_DELETE m_pCameraTransitionController;
        Eegeo_DELETE m_pDoubleTapIndoorInteractionController;
        Eegeo_DELETE m_pNavigationService;
        Eegeo_DELETE m_pInitialLocationModel;
        Eegeo_DELETE m_pCameraSplinePlaybackController;
        Eegeo_DELETE m_pGlobeCameraWrapper;
        Eegeo_DELETE m_pGlobeCameraController;
        Eegeo_DELETE m_pMapCameraDistanceBasedFovControl;
        Eegeo_DELETE m_pMapCameraDistanceFromBoundsCalculator;
        Eegeo_DELETE m_pLoadingScreen;

        Eegeo_DELETE m_pAppModeModel;

        Eegeo_DELETE m_pConnectivityChangedObserver;

        m_pCurrentLocationService->SetLocationService(m_platformLocationService);

        Eegeo_DELETE m_pLocationProvider;

        Eegeo_DELETE m_pWorld;

        Eegeo_DELETE m_pCurrentLocationService;
    }

    void MobileExampleApp::CreateApplicationModelModules(Eegeo::UI::NativeUIFactories& nativeUIFactories,
                                                         const bool interiorsAffectedByFlattening,
                                                         const bool createBlueSphereViews)
    {
        Eegeo::EegeoWorld& world = *m_pWorld;

        m_pReactorIgnoredReactionModel = Eegeo_NEW(Menu::View::ReactorIgnoredReactionModel)();
        m_pModalityIgnoredReactionModel = Eegeo_NEW(Menu::View::ModalityIgnoredReactionModel)();

        m_pAboutPageModule = Eegeo_NEW(ExampleApp::AboutPage::View::AboutPageModule)(m_identityProvider,
                                                                                     m_applicationConfiguration.ProductVersion(),
                                                                                     m_applicationConfiguration.Name());

        m_pOptionsModule = Eegeo_NEW(ExampleApp::Options::OptionsModule)(m_identityProvider,
                                                                         m_messageBus,
                                                                         m_networkCapabilities);
        
        Eegeo::Modules::Map::MapModule& mapModule = world.GetMapModule();
        Eegeo::Modules::Map::Layers::InteriorsPresentationModule& interiorsPresentationModule = mapModule.GetInteriorsPresentationModule();
        
        m_pAppCameraModule = Eegeo_NEW(AppCamera::SdkModel::AppCameraModule)(
                                                                             interiorsPresentationModule.GetInteriorInteractionModel(),
                                                                             interiorsPresentationModule.GetInteriorTransitionModel(),
                                                                             m_pWorld->GetFeatureInteractionModule().GetCollisionModule().GetFeatureRayCastingService());

        const bool useGeoName = true;
        if(useGeoName)
        {
            m_searchServiceModules[Search::GeoNamesVendorName] = Eegeo_NEW(Search::GeoNames::SdkModel::GeoNamesSearchServiceModule)(m_platformAbstractions.GetWebLoadRequestFactory(),
                                                                                                                                    m_platformAbstractions.GetUrlEncoder(),
                                                                                                                                    m_networkCapabilities,
                                                                                                                                    m_applicationConfiguration.GeoNamesUserName());
        }

        const auto& searchTags = Search::SdkModel::CreateSearchTagsFromFile(
                                                                            m_platformAbstractions.GetFileIO(), "search_tags.json");

        Search::EegeoPois::SdkModel::EegeoSearchServiceModule* eegeoSearchServiceModule = Eegeo_NEW(Search::EegeoPois::SdkModel::EegeoSearchServiceModule)(
                m_platformAbstractions.GetWebLoadRequestFactory(),
                m_platformAbstractions.GetUrlEncoder(),
                m_networkCapabilities,
                searchTags,
                m_applicationConfiguration.EegeoSearchServiceUrl(),
                m_pWorld->GetApiTokenModel(),
                mapModule.GetInteriorsPresentationModule().GetInteriorInteractionModel()
        );

        const bool useEegeoPois = true;
        if(useEegeoPois)
        {
            m_searchServiceModules[Search::EegeoVendorName] = eegeoSearchServiceModule;
        }

        const bool useYelpSearch = true;
        if (useYelpSearch)
        {
            m_searchServiceModules[ExampleApp::Search::YelpVendorName] = Eegeo_NEW(ExampleApp::Search::Yelp::YelpSearchServiceModule)(
                                                                                                                                      m_platformAbstractions.GetWebLoadRequestFactory(),
                                                                                                                                      m_networkCapabilities,
                                                                                                                                      m_platformAbstractions.GetUrlEncoder(),
                                                                                                                                      searchTags,
                                                                                                                                      m_applicationConfiguration.YelpApiKey(),
                                                                                                                                      m_platformAbstractions.GetFileIO(),
                                                                                                                                      m_yelpCategoryMapperUpdater,
                                                                                                                                      m_pAppCameraModule->GetController()
                                                                                                                                      );
        }

        m_pSearchServiceModule = Eegeo_NEW(Search::Combined::SdkModel::CombinedSearchServiceModule)(m_searchServiceModules, mapModule.GetInteriorsPresentationModule().GetInteriorInteractionModel());

        

        Eegeo::Modules::Map::CityThemesModule& cityThemesModule = world.GetCityThemesModule();



        m_pVisualMapModule = Eegeo_NEW(VisualMap::SdkModel::VisualMapModule)(cityThemesModule.GetCityThemesService(),
                                                                             cityThemesModule.GetCityThemesUpdater(),
                                                                             mapModule.GetEnvironmentFlatteningService());



        m_pGpsMarkerModule = Eegeo_NEW(ExampleApp::GpsMarker::SdkModel::GpsMarkerModule)(m_pWorld->GetLocationService(),
                                                                                         m_pWorld->GetTerrainModelModule(),
                                                                                         m_pWorld->GetMapModule(),
                                                                                         m_pWorld->GetRenderingModule(),
                                                                                         interiorsPresentationModule.GetInteriorInteractionModel(),
                                                                                         m_pVisualMapModule->GetVisualMapService(),
                                                                                         createBlueSphereViews,
                                                                                         m_messageBus);

        m_pSurveyModule = Eegeo_NEW(Surveys::SdkModel::SurveyModule)(m_messageBus,
                                                                     m_persistentSettings);
        
        m_pSurveyTimer = Eegeo_NEW(Surveys::SdkModel::SurveyTimer)(m_messageBus, m_applicationConfiguration.SurveyRequirementTimeSec());

        m_pSurveyModule->GetSurveyObserver().OnStartup();

        m_pWeatherMenuModule = Eegeo_NEW(ExampleApp::WeatherMenu::SdkModel::WeatherMenuModule)(m_platformAbstractions.GetFileIO(),
                                                                                               m_pVisualMapModule->GetVisualMapService(),
                                                                                               m_messageBus,
                                                                                               m_metricsService,
                                                                                               *m_pAppModeModel,
                                                                                               cityThemesModule.GetCityThemesService(),
                                                                                               cityThemesModule.GetCityThemesRepository());

        auto defaultFindMenuItems = TagSearch::View::CreateTagSearchModelsFromConfig(m_applicationConfiguration,
                                                                                     "outdoor_search_menu_items",
                                                                                     m_yelpCategoryMapperUpdater);

        m_pSearchModule = Eegeo_NEW(Search::SdkModel::SearchModule)(m_pSearchServiceModule->GetSearchService(),
                                                                    m_pAppCameraModule->GetController(),
                                                                    *m_pCameraTransitionService,
                                                                    m_pWorld->GetMapModule().GetInteriorsPresentationModule().GetInteriorInteractionModel(),
                                                                    m_messageBus,
                                                                    m_sdkDomainEventBus,
                                                                    m_metricsService,
                                                                    m_pWorld->GetMapModule().GetInteriorsPresentationModule().GetInteriorSelectionModel(),
                                                                    mapModule.GetInteriorMetaDataModule().GetInteriorMetaDataRepository(),
                                                                    m_yelpCategoryMapperUpdater,
                                                                    defaultFindMenuItems,
                                                                    searchTags.defaultIconKey);

        m_pSuggestionsQueryPerformer  = Eegeo_NEW(ExampleApp::Search::SdkModel::AutocompleteSuggestionQueryPerformer)(m_pAppCameraModule->GetController(),
                                                                                                                      m_platformAbstractions.GetWebLoadRequestFactory(),
                                                                                                                      eegeoSearchServiceModule->GetEegeoParser(),
                                                                                                                      m_applicationConfiguration.EegeoSearchServiceUrl(),
                                                                                                                      m_pWorld->GetApiTokenModel(),
                                                                                                                      m_platformAbstractions.GetUrlEncoder(),
                                                                                                                      m_messageBus);

        m_pSearchMenuModule = Eegeo_NEW(ExampleApp::SearchMenu::SdkModel::SearchMenuModule)(m_identityProvider,
                                                                                            m_pSearchModule->GetSearchQueryPerformer(),
                                                                                            *m_pSuggestionsQueryPerformer,
                                                                                            m_messageBus,
                                                                                            m_metricsService);

        m_pAboutPageMenuModule = Eegeo_NEW(AboutPage::SdkModel::AboutPageMenuModule)(m_pSearchMenuModule->GetSearchMenuViewModel(),
                                                                                     m_pAboutPageModule->GetAboutPageViewModel());

        m_pOptionsMenuModule = Eegeo_NEW(Options::SdkModel::OptionsMenuModule)(m_pSearchMenuModule->GetSearchMenuViewModel(),
                                                                                 m_pOptionsModule->GetOptionsViewModel());

        m_pPlaceJumpsModule = Eegeo_NEW(PlaceJumps::SdkModel::PlaceJumpsModule)(m_platformAbstractions.GetFileIO(),
                                                                                CameraTransitionController(),
                                                                                m_pSearchMenuModule->GetSearchMenuViewModel(),
                                                                                m_messageBus,
                                                                                m_metricsService,
                                                                                m_menuReaction);
        
        m_pTagSearchModule = &m_pSearchModule->GetTagSearchModule();

        m_pMapModeModule = Eegeo_NEW(MapMode::SdkModel::MapModeModule)(m_pVisualMapModule->GetVisualMapService());

        InitialisePinsModules(mapModule, world, interiorsAffectedByFlattening, m_screenProperties.GetOversampleScale());

        m_pMyPinsModule = Eegeo_NEW(ExampleApp::MyPins::SdkModel::MyPinsModule)(m_pWorldPinsModule->GetWorldPinsService(),
                                                                                m_platformAbstractions,
                                                                                m_persistentSettings,
                                                                                m_pSearchMenuModule->GetSearchMenuViewModel(),
                                                                                m_messageBus,
                                                                                m_sdkDomainEventBus,
                                                                                *m_pCameraTransitionService,
                                                                                m_pSearchModule->GetMyPinsSearchResultRefreshService(),
                                                                                m_metricsService,
                                                                                m_applicationConfiguration.MyPinsWebServiceUrl(),
                                                                                m_applicationConfiguration.MyPinsWebServiceAuthToken(),
                                                                                m_applicationConfiguration.MyPinsPoiSetId(),
                                                                                m_menuReaction,
                                                                                *m_pModalityIgnoredReactionModel);

        m_pSearchResultPoiModule =
        Eegeo_NEW(ExampleApp::SearchResultPoi::View::SearchResultPoiModule)(m_identityProvider,
                                                                            m_pMyPinsModule->GetMyPinsService(),
                                                                            m_pSearchModule->GetSearchResultMyPinsService(),
                                                                            m_pTagSearchModule->GetSearchResultIconKeyMapper(),
                                                                            world.GetPlatformAbstractionModule().GetWebLoadRequestFactory(),
                                                                            m_messageBus);
        
        Eegeo::Modules::Map::Layers::InteriorsModelModule& interiorsModelModule = mapModule.GetInteriorsModelModule();

        m_pNavUIModule = Eegeo_NEW(ExampleApp::NavRouting::View::NavUIModule)(m_identityProvider,
                                                                              *m_pModalityIgnoredReactionModel);

        m_pSearchResultSectionModule = Eegeo_NEW(SearchResultSection::SdkModel::SearchResultSectionModule)(m_pSearchMenuModule->GetSearchMenuViewModel(),
                                                                                                           m_pSearchModule->GetSearchResultRepository(),
                                                                                                           m_pSearchModule->GetSearchQueryPerformer(),
                                                                                                           *m_pCameraTransitionService,
                                                                                                           interiorsPresentationModule.GetInteriorInteractionModel(),
                                                                                                           interiorsModelModule.GetInteriorMarkerModelRepository(),
                                                                                                           m_pAppCameraModule->GetController(),
                                                                                                           m_pSearchModule->GetSearchRefreshService(),
                                                                                                           m_messageBus);

        namespace IntHighlights = InteriorsExplorer::SdkModel::Highlights;
        
        m_pHighlightColorMapper = Eegeo_NEW(IntHighlights::HighlightColorMapper)(Eegeo::v4(0.0, 1.0, 0.0, 1.0));
        
        Eegeo::Resources::Interiors::Highlights::IInteriorsHighlightService& highlightService =
        m_pWorld->GetInteriorHighlightsModule().GetHighlightService();
        
        m_pInteriorsHighlightVisibilityController = Eegeo_NEW(IntHighlights::InteriorEntityHighlightController)(
                                                                                                                mapModule.GetInteriorsPresentationModule().GetInteriorInteractionModel(),
                                                                                                                                          mapModule.GetInteriorsModelModule().GetInteriorsCellResourceObserver(),
                                                                                                                                          m_searchServiceModules[Search::EegeoVendorName]->GetSearchService(),
                                                                                                                                          m_pSearchModule->GetSearchQueryPerformer(),
                                                                                                                                          mapModule.GetInteriorsPresentationModule().GetInteriorsLabelsController(),
                                                                                                                                          mapModule.GetLabelsModule().GetLabelHiddenFilterModel(),
                                                                                                                                          mapModule.GetInteriorsStreamingModule().GetLabelLayerId(),
                                                                                                                                          m_messageBus,
                                                                                                                                          *m_pHighlightColorMapper,
                                                                                                                                          highlightService);
        
        m_pSearchResultOnMapModule = Eegeo_NEW(SearchResultOnMap::SdkModel::SearchResultOnMapModule)(m_pSearchModule->GetSearchResultRepository(),
                                                                                                     m_pSearchResultPoiModule->GetSearchResultPoiViewModel(),
                                                                                                     m_pWorldPinsModule->GetWorldPinsService(),
                                                                                                     m_pMyPinsModule->GetMyPinsService(),
                                                                                                     m_pTagSearchModule->GetSearchResultIconKeyMapper(),
                                                                                                     m_pSearchModule->GetSearchResultMyPinsService(),
                                                                                                     m_messageBus,
                                                                                                     m_metricsService,
                                                                                                     m_menuReaction,
                                                                                                     *m_pInteriorsHighlightVisibilityController);

        Eegeo::Camera::GlobeCamera::GpsGlobeCameraControllerFactory gpsCameraControllerFactory(m_pWorld->GetTerrainModelModule().GetTerrainHeightProvider(),
                                                                                               mapModule.GetEnvironmentFlatteningService(),
                                                                                               mapModule.GetResourceCeilingProvider(),
                                                                                               *m_pNavigationService);

        Eegeo::Camera::GlobeCamera::GlobeCameraControllerFactory cameraControllerFactory(m_pWorld->GetTerrainModelModule().GetTerrainHeightProvider(),
                                                                                         mapModule.GetEnvironmentFlatteningService(),
                                                                                         mapModule.GetResourceCeilingProvider());

        const Eegeo::Resources::Interiors::InteriorsCameraConfiguration& interiorsCameraConfig(Eegeo::Resources::Interiors::InteriorsCameraController::CreateDefaultConfig());
        const Eegeo::Camera::GlobeCamera::GlobeCameraControllerConfiguration& globeCameraConfig = Eegeo::Resources::Interiors::InteriorsCameraControllerFactory::DefaultGlobeCameraControllerConfiguration();
        const Eegeo::Camera::GlobeCamera::GlobeCameraTouchControllerConfiguration& globeCameraTouchConfig = Eegeo::Resources::Interiors::InteriorsCameraControllerFactory::DefaultGlobeCameraTouchControllerConfiguration();
        Eegeo::Camera::GlobeCamera::GpsGlobeCameraComponentConfiguration gpsGlobeCameraComponentConfig = Eegeo::Resources::Interiors::InteriorsGpsCameraControllerFactory::DefaultGpsGlobeCameraComponentConfiguration();
        gpsGlobeCameraComponentConfig.defaultGpsDistanceToInterest = ExampleApp::InteriorsExplorer::DefaultInteriorSearchResultTransitionInterestDistance;
        gpsGlobeCameraComponentConfig.panToUnlockThreshold = PanToUnlockThreshold;
        gpsGlobeCameraComponentConfig.compassCameraOffset = m_applicationConfiguration.CompassCameraOffset();
        gpsGlobeCameraComponentConfig.compassCameraOffsetTopDown = m_applicationConfiguration.CompassCameraOffsetTopDown();
        gpsGlobeCameraComponentConfig.compassCameraDampingEnabled = m_applicationConfiguration.CompassCameraDampingEnabled();

        if(m_applicationConfiguration.IsInKioskMode() && m_applicationConfiguration.IsFixedIndoorLocationEnabled())
        {
            gpsGlobeCameraComponentConfig.defaultGpsBearing = m_applicationConfiguration.FixedIndoorLocation().GetOrientationDegrees();
        }

        const Eegeo::Resources::Interiors::InteriorsCameraControllerFactory interiorsCameraControllerFactory(
                                                                                                             interiorsCameraConfig,
                                                                                                             globeCameraConfig,
                                                                                                             globeCameraTouchConfig,
                                                                                                             cameraControllerFactory,
                                                                                                             m_screenProperties,
                                                                                                             interiorsPresentationModule.GetInteriorInteractionModel(),
                                                                                                             interiorsPresentationModule.GetInteriorViewModel(),
                                                                                                             mapModule.GetEnvironmentFlatteningService(),
                                                                                                             interiorsAffectedByFlattening );

        const Eegeo::Resources::Interiors::InteriorsGpsCameraControllerFactory interiorsGpsCameraControllerFactory(gpsGlobeCameraComponentConfig,
                                                                                                                   globeCameraConfig,
                                                                                                                   gpsCameraControllerFactory,
                                                                                                                   m_screenProperties,
                                                                                                                   interiorsPresentationModule.GetInteriorViewModel());


        Eegeo::Modules::Map::StreamingModule& streamingModule = world.GetStreamingModule();
        m_pWorldAreaLoaderModule = Eegeo_NEW(WorldAreaLoader::SdkModel::WorldAreaLoaderModule)(streamingModule.GetPrecachingService());

        m_initialExperienceModule.InitialiseWithApplicationModels(m_pWorldAreaLoaderModule->GetWorldAreaLoaderModel());

        const InitialExperience::SdkModel::IInitialExperienceModel& initialExperienceModel = m_initialExperienceModule.GetInitialExperienceModel();


        m_pInteriorsExplorerModule = Eegeo_NEW(InteriorsExplorer::SdkModel::InteriorsExplorerModule)(interiorsPresentationModule.GetInteriorInteractionModel(),
                                                                                                     interiorsPresentationModule.GetInteriorSelectionModel(),
                                                                                                     interiorsPresentationModule.GetInteriorTransitionModel(),
                                                                                                     interiorsModelModule.GetInteriorMarkerModelRepository(),
                                                                                                     m_pWorldPinsModule->GetWorldPinsService(),
                                                                                                     *m_pWorldPinsIconMapping,
                                                                                                     mapModule.GetEnvironmentFlatteningService(),
                                                                                                     m_pVisualMapModule->GetVisualMapService(),
                                                                                                     interiorsCameraControllerFactory,
                                                                                                     interiorsGpsCameraControllerFactory,
                                                                                                     m_screenProperties,
                                                                                                     m_identityProvider,
                                                                                                     m_messageBus,
                                                                                                     m_metricsService,
                                                                                                     initialExperienceModel,
                                                                                                     interiorsAffectedByFlattening,
                                                                                                     m_persistentSettings,
                                                                                                     *m_pNavigationService,
                                                                                                     mapModule.GetInteriorMetaDataModule().GetInteriorMetaDataRepository(),
                                                                                                     m_pTagSearchModule->GetTagSearchRepository(),
                                                                                                     m_pWorld->GetNativeUIFactories().AlertBoxFactory());

        m_pWatermarkModule = Eegeo_NEW(ExampleApp::Watermark::SdkModel::WatermarkModule)(m_identityProvider,
                                                                                         m_applicationConfiguration.Name(),
                                                                                         m_applicationConfiguration.GoogleAnalyticsReferrerToken(),
                                                                                         world.GetMapModule().GetInteriorsPresentationModule().GetInteriorSelectionModel(),
                                                                                         world.GetMapModule().GetInteriorsModelModule().GetInteriorsModelRepository(),
                                                                                         m_pInteriorsExplorerModule->GetInteriorsExplorerModel(),
                                                                                         m_messageBus);

        m_pMyPinCreationModule = Eegeo_NEW(ExampleApp::MyPinCreation::SdkModel::MyPinCreationModule)(m_pMyPinsModule->GetMyPinsService(),
                                                                                                     m_identityProvider,
                                                                                                     m_pSearchMenuModule->GetSearchMenuViewModel(),
                                                                                                     m_pSearchModule->GetSearchQueryPerformer(),
                                                                                                     m_pSearchModule->GetSearchRefreshService(),
                                                                                                     m_pInteriorsExplorerModule->GetScreenControlViewModel(),
                                                                                                     mapModule.GetLabelsModule().GetDebugLabelLayerFilterModel(),
                                                                                                     mapModule.GetInteriorsStreamingModule().GetInteriorMarkerLabelLayerId(),
                                                                                                     m_messageBus,
                                                                                                     m_pWatermarkModule->GetWatermarkViewModel());

        m_pPoiRingModule = Eegeo_NEW(ExampleApp::MyPinCreation::PoiRing::SdkModel::PoiRingModule)(m_pMyPinCreationModule->GetMyPinCreationModel(),
                                                                                                  m_platformAbstractions,
                                                                                                  m_pWorld->GetRenderingModule(),
                                                                                                  m_pWorld->GetAsyncLoadersModule(),
                                                                                                  m_pWorld->GetLightingModule(),
                                                                                                  m_pWorld->GetTerrainModelModule(),
                                                                                                  m_pWorld->GetMapModule(),
                                                                                                  *m_pAppModeModel,
                                                                                                  m_screenProperties,
                                                                                                  interiorsAffectedByFlattening,
                                                                                                  m_applicationConfiguration.IsInKioskMode());

        m_pMyPinCreationDetailsModule = Eegeo_NEW(ExampleApp::MyPinCreationDetails::View::MyPinCreationDetailsModule)(m_identityProvider);

        m_pMyPinDetailsModule = Eegeo_NEW(ExampleApp::MyPinDetails::SdkModel::MyPinDetailsModule)(m_identityProvider,
                                                                                                  m_pMyPinsModule->GetMyPinsService(),
                                                                                                  m_pSearchResultPoiModule->GetSearchResultPoiViewModel(),
                                                                                                  m_messageBus,
                                                                                                  m_menuReaction);

        m_pCompassModule = Eegeo_NEW(ExampleApp::Compass::SdkModel::CompassModule)(*m_pNavigationService,
                                                                                   interiorsPresentationModule.GetInteriorInteractionModel(),
                                                                                   world.GetLocationService(),
                                                                                   m_pAppCameraModule->GetController(),
                                                                                   m_identityProvider,
                                                                                   m_messageBus,
                                                                                   m_metricsService,
                                                                                   m_pInteriorsExplorerModule->GetInteriorsExplorerModel(),
                                                                                   *m_pAppModeModel,
                                                                                   m_pWorld->GetNativeUIFactories().AlertBoxFactory(),
                                                                                   m_applicationConfiguration.IsInKioskMode());

        auto& polylineShapesModule = world.GetShapesModule().GetPolylineShapesModule();

        m_pNavRoutingModule = Eegeo_NEW(ExampleApp::NavRouting::SdkModel::NavRoutingModule)(polylineShapesModule.GetShapeService(),
                                                                                            world.GetRoutesModule().GetRoutingWebservice(),
                                                                                            world.GetLocationService(),
                                                                                            *m_pNavigationService,
                                                                                            m_pWorld->GetNativeUIFactories().AlertBoxFactory(),
                                                                                            *m_pCameraTransitionService,
                                                                                            *m_pMapCameraDistanceFromBoundsCalculator,
                                                                                            m_pCompassModule->GetCompassModel(),
                                                                                            m_messageBus,
                                                                                            interiorsModelModule.GetInteriorsModelRepository(),
                                                                                            interiorsModelModule.GetInteriorInteractionModel(),
                                                                                            mapModule.GetMarkersModule().GetMarkerService(),
                                                                                            m_pWorldPinsModule->GetWorldPinsService(),
                                                                                            m_pGpsMarkerModule->GetGpsMarkerModel(),
                                                                                            m_pWorldPinsModule->GetWorldPinsVisibilityController(),
                                                                                            m_pAppCameraModule->GetLocationPicker(),
                                                                                            m_pSearchModule->GetSearchQueryPerformer(),
                                                                                            *m_pHighlightColorMapper,
                                                                                            highlightService);
        
        m_pInteriorCameraWrapper = Eegeo_NEW(AppCamera::SdkModel::AppInteriorCameraWrapper)(m_pInteriorsExplorerModule->GetInteriorsGpsCameraController(),
                                                                                            m_pInteriorsExplorerModule->GetInteriorsCameraController());



        std::vector<Reaction::View::IReaction*> reactions(GetReactions());
        std::vector<ExampleApp::OpenableControl::View::IOpenableControlViewModel*> openables(GetOpenableControls());

        m_pModalityModule = Eegeo_NEW(Modality::View::ModalityModule)(m_messageBus, openables, *m_pModalityIgnoredReactionModel);

        m_pReactionModelModule = Eegeo_NEW(Reaction::View::ReactionModelModule)(openables,
                                                                                reactions,
                                                                                *m_pReactorIgnoredReactionModel);

        m_pSearchMenuModule->SetSearchSection("Search Results", m_pSearchResultSectionModule->GetSearchResultSectionModel());
        m_pSearchMenuModule->AddMenuSection("Find", m_pTagSearchModule->GetTagSearchMenuModel(), true);
        m_pSearchMenuModule->AddMenuSection("Locations", m_pPlaceJumpsModule->GetPlaceJumpsMenuModel(), true);
        
        if(!m_applicationConfiguration.IsInKioskMode())
        {
            m_pSearchMenuModule->AddMenuSection("Drop Pin", m_pMyPinCreationModule->GetMyPinCreationMenuModel(), false);
            m_pSearchMenuModule->AddMenuSection("My Pins", m_pMyPinsModule->GetMyPinsMenuModel(), true);
        }

        m_pSearchMenuModule->AddMenuSection("Weather", m_pWeatherMenuModule->GetWeatherMenuModel(), true);

        if(m_applicationConfiguration.NavigationEnabled())
        {
            m_pSearchMenuModule->AddMenuSection("Directions", m_pNavRoutingModule->GetNavMenuModel(), false);
        }

        m_pSearchMenuModule->AddMenuSection("Options", m_pOptionsMenuModule->GetOptionsMenuModel(), false);
        m_pSearchMenuModule->AddMenuSection("About",  m_pAboutPageMenuModule->GetAboutPageMenuModel(), false);

        m_pSelectFirstResultSearchService = Eegeo_NEW(Search::SelectFirstResult::SdkModel::SelectFirstResultSearchService)(m_pSearchModule->GetSearchQueryPerformer());

#ifdef AUTOMATED_SCREENSHOTS
        const bool instantiateAutomatedScreenshotController = true;
#else
        const bool instantiateAutomatedScreenshotController = false;
#endif
        if (instantiateAutomatedScreenshotController)
        {
            Eegeo::Traffic::PlaneSimulation& planeSimulation = world.GetTrafficModule().GetTrafficSimulationController().GetPlaneSimulation();

            m_pAutomatedScreenshotController = Eegeo_NEW(Automation::AutomatedScreenshotController)(m_applicationConfiguration,
                                                                                                    *m_pCameraTransitionService,
                                                                                                    m_pGlobeCameraController->GetGlobeCameraController(),
                                                                                                    planeSimulation,
                                                                                                    m_pPlaceJumpsModule->GetPlaceJumpController(),
                                                                                                    m_pWeatherMenuModule->GetWeatherController(),
                                                                                                    m_pSearchModule->GetSearchQueryPerformer(),
                                                                                                    m_pSearchResultPoiModule->GetSearchResultPoiViewModel(),
                                                                                                    m_pWorld->GetMapModule().GetInteriorsPresentationModule().GetInteriorSelectionModel(),
                                                                                                    m_pInteriorsExplorerModule->GetInteriorsCameraController(),
                                                                                                    m_pInteriorsExplorerModule->GetInteriorsExplorerModel(),
                                                                                                    streamingModule.GetStreamingController(),
                                                                                                    m_screenshotService,
                                                                                                    world,
                                                                                                    m_messageBus,
                                                                                                    *m_pAppCameraModule,
                                                                                                    m_screenProperties,
                                                                                                    *m_pWorldPinsModule);
        }
    }

    void MobileExampleApp::InitialiseAppState(Eegeo::UI::NativeUIFactories& nativeUIFactories)
    {
        Eegeo::Modules::Map::MapModule& mapModule = m_pWorld->GetMapModule();
        Eegeo::Modules::Map::Layers::InteriorsPresentationModule& interiorsPresentationModule = mapModule.GetInteriorsPresentationModule();

        AppModes::States::SdkModel::AppModeStatesFactory appModeStatesFactory(m_pAppCameraModule->GetController(),
                                                                              *m_pCameraSplinePlaybackController,
                                                                              *m_pGlobeCameraWrapper,
                                                                              *m_pInteriorCameraWrapper,
                                                                              *m_pStreamingVolume,
                                                                              m_pInteriorsExplorerModule->GetInteriorVisibilityUpdater(),
                                                                              m_pInteriorsExplorerModule->GetInteriorsExplorerModel(),
                                                                              m_pInteriorsExplorerModule->GetInteriorsExplorerUserInteractionModel(),
                                                                              *m_pAppModeModel,
                                                                              interiorsPresentationModule.GetInteriorSelectionModel(),
                                                                              interiorsPresentationModule.GetInteriorInteractionModel(),
                                                                              nativeUIFactories,
                                                                              m_pVisualMapModule->GetVisualMapService(),
                                                                              m_pWorld->GetLocationService(),
                                                                              m_userIdleService,
                                                                              m_applicationConfiguration.IsAttractModeEnabled(),
                                                                              m_applicationConfiguration.AttractModeTargetSplinePoints(),
                                                                              m_applicationConfiguration.AttractModePositionSplinePoints(),
                                                                              m_applicationConfiguration.AttractModePlaybackSpeed(),
                                                                              m_screenProperties,
                                                                              m_messageBus,
                                                                              *m_pNavigationService,
                                                                              m_pSearchModule->GetSearchQueryPerformer());

        m_pAppModeModel->InitialiseStateMachine(appModeStatesFactory.CreateStateMachineStates(*m_pGlobalAppModeTransitionRules), AppModes::SdkModel::WorldMode, m_pGlobalAppModeTransitionRules);

		m_messageBus.SubscribeNative(m_onUiCreatedCallback);
    }

	void MobileExampleApp::OnUiCreated(const UiCreatedMessage& message)
	{
		AddTagSearchModels(m_pTagSearchModule->GetTagSearchRepository(), m_applicationConfiguration,
						   m_yelpCategoryMapperUpdater);

		if (m_applicationConfiguration.ShouldPerformStartUpSearch())
		{
            bool isTag = true;
            bool tryInteriorSearch = false;
            bool selectFirstResult = false;
            m_pSearchModule->GetSearchQueryPerformer().AskForSearchQuery(m_applicationConfiguration.StartUpSearchTag(),
                                                                         isTag, tryInteriorSearch, selectFirstResult);
		}

        m_pDeepLinkModule->GetDeepLinkController().OnUiStarted();
	}

    void MobileExampleApp::DestroyApplicationModelModules()
    {
        m_initialExperienceModule.TearDown();

        Eegeo_DELETE m_pModalityIgnoredReactionModel;

        Eegeo_DELETE m_pReactorIgnoredReactionModel;

        Eegeo_DELETE m_pWorldAreaLoaderModule;

        Eegeo_DELETE m_pReactionModelModule;

        Eegeo_DELETE m_pModalityModule;
        
        Eegeo_DELETE m_pNavRoutingModule;

        Eegeo_DELETE m_pCompassModule;

        Eegeo_DELETE m_pMyPinDetailsModule;

        Eegeo_DELETE m_pMyPinCreationDetailsModule;

        Eegeo_DELETE m_pPoiRingModule;

        Eegeo_DELETE m_pMyPinCreationModule;
        
        Eegeo_DELETE m_pSearchMenuModule;
        
        Eegeo_DELETE m_pNavUIModule;

        Eegeo_DELETE m_pSearchResultSectionModule;

        Eegeo_DELETE m_pSearchResultOnMapModule;

        Eegeo_DELETE m_pSearchResultPoiModule;

        Eegeo_DELETE m_pPlaceJumpsModule;

        Eegeo_DELETE m_pMapModeModule;

        Eegeo_DELETE m_pTagSearchModule;

        Eegeo_DELETE m_pOptionsMenuModule;

        Eegeo_DELETE m_pAboutPageMenuModule;

        Eegeo_DELETE m_pSurveyModule;

        Eegeo_DELETE m_pVisualMapModule;

        Eegeo_DELETE m_pWeatherMenuModule;

        Eegeo_DELETE m_pGpsMarkerModule;

        Eegeo_DELETE m_pInteriorCameraWrapper;

        Eegeo_DELETE m_pWatermarkModule;

        Eegeo_DELETE m_pInteriorsExplorerModule;

        Eegeo_DELETE m_pSearchModule;

        for(std::map<std::string, ExampleApp::Search::SdkModel::ISearchServiceModule*>::iterator it = m_searchServiceModules.begin(); it != m_searchServiceModules.end(); ++it)
        {
            Eegeo_DELETE (*it).second;
        }
        m_searchServiceModules.clear();

        Eegeo_DELETE m_pMyPinsModule;

        Eegeo_DELETE m_pWorldPinsModule;

        Eegeo_DELETE m_pPinsModule;

        Eegeo_DELETE m_pWorldPinsIconMapping;

        Eegeo_DELETE m_pAppCameraModule;

        Eegeo_DELETE m_pOptionsModule;

        Eegeo_DELETE m_pAboutPageModule;
    }

    std::vector<ExampleApp::OpenableControl::View::IOpenableControlViewModel*> MobileExampleApp::GetOpenableControls() const
    {
        std::vector<ExampleApp::OpenableControl::View::IOpenableControlViewModel*> openables;
        openables.push_back(&SearchMenuModule().GetSearchMenuViewModel());
        openables.push_back(&SearchResultPoiModule().GetObservableOpenableControl());
        openables.push_back(&AboutPageModule().GetObservableOpenableControl());
        openables.push_back(&MyPinCreationDetailsModule().GetObservableOpenableControl());
        openables.push_back(&MyPinDetailsModule().GetObservableOpenableControl());
        openables.push_back(&MyPinCreationModule().GetObservableOpenableControl());
        openables.push_back(&OptionsModule().GetObservableOpenableControl());
        openables.push_back(&NavUIModule().GetObservableOpenableControl());
        return openables;
    }

    std::vector<ExampleApp::ScreenControl::View::IScreenControlViewModel*> MobileExampleApp::GetReactorControls() const
    {
        std::vector<ExampleApp::ScreenControl::View::IScreenControlViewModel*> reactors;
        reactors.push_back(&SearchMenuModule().GetSearchMenuViewModel());
        reactors.push_back(&CompassModule().GetScreenControlViewModel());
        reactors.push_back(&MyPinCreationModule().GetInitiationScreenControlViewModel());
        reactors.push_back(&WatermarkModule().GetScreenControlViewModel());
        reactors.push_back(&InteriorsExplorerModule().GetScreenControlViewModel());
        return reactors;
    }

    std::vector<ExampleApp::Reaction::View::IReaction*> MobileExampleApp::GetReactions() const
    {
        std::vector<ExampleApp::ScreenControl::View::IScreenControlViewModel*> allReactors(GetReactorControls());

        std::vector<Reaction::View::IReaction*> reactions;
        reactions.push_back(Eegeo_NEW(Reaction::View::ReactionHideOtherScreenControls)(SearchMenuModule().GetSearchMenuViewModel(), allReactors));
        reactions.push_back(Eegeo_NEW(Reaction::View::ReactionHideOtherScreenControls)(SearchResultPoiModule().GetObservableOpenableControl(), allReactors));
        reactions.push_back(Eegeo_NEW(Reaction::View::ReactionHideOtherScreenControls)(AboutPageModule().GetObservableOpenableControl(), allReactors));
        reactions.push_back(Eegeo_NEW(Reaction::View::ReactionHideOtherScreenControls)(MyPinCreationDetailsModule().GetObservableOpenableControl(), allReactors));
        reactions.push_back(Eegeo_NEW(Reaction::View::ReactionHideOtherScreenControls)(MyPinDetailsModule().GetObservableOpenableControl(), allReactors));
        reactions.push_back(Eegeo_NEW(Reaction::View::ReactionHideOtherScreenControls)(MyPinCreationModule().GetObservableOpenableControl(), allReactors));
        reactions.push_back(Eegeo_NEW(Reaction::View::ReactionHideOtherScreenControls)(NavUIModule().GetObservableOpenableControl(), SearchMenuModule().GetSearchMenuViewModel()));
        reactions.push_back(Eegeo_NEW(Reaction::View::ReactionHideOtherScreenControls)(OptionsModule().GetObservableOpenableControl(), allReactors));
        reactions.push_back(Eegeo_NEW(Reaction::View::ReactionPushScreenControl)
                                    (NavUIModule().GetObservableOpenableControl(),
                                     CompassModule().GetScreenControlViewModel(),
                                     NavUIModule().GetNavWidgetViewModel().GetCompassStateProvider()));
        reactions.push_back(Eegeo_NEW(Reaction::View::ReactionPushScreenControl)
                                    (NavUIModule().GetObservableOpenableControl(),
                                     InteriorsExplorerModule().GetScreenControlViewModel(),
                                     NavUIModule().GetNavWidgetViewModel().GetInteriorsStateProvider()));
        return reactions;
    }

    Eegeo::Pins::PinsModule* MobileExampleApp::CreatePlatformPinsModuleInstance(Eegeo::EegeoWorld& world,
                                                                                const Eegeo::Helpers::GLHelpers::TextureInfo& pinTextureInfo,
                                                                                const Eegeo::Rendering::AtlasTexturePageLayout& atlasTexturePageLayout)
    {
        Eegeo::Modules::Core::RenderingModule& renderingModule = world.GetRenderingModule();
        Eegeo::Modules::Map::MapModule& mapModule = world.GetMapModule();

        return Eegeo::Pins::PinsModule::CreateWithAtlas(renderingModule,
                                                        world.GetPlatformAbstractionModule(),
                                                        mapModule,
                                                        pinTextureInfo.textureId,
                                                        atlasTexturePageLayout,
                                                        Eegeo::Rendering::LayerIds::Pins,
                                                        m_screenProperties);
    }

    void MobileExampleApp::InitialisePinsModules(Eegeo::Modules::Map::MapModule& mapModule,
                                                 Eegeo::EegeoWorld& world,
                                                 const bool interiorsAffectedByFlattening,
                                                 const float screenOversampleScale)
    {
        ExampleApp::WorldPins::SdkModel::WorldPinIconMappingFactory worldPinIconMappingFactory(
                                                                                               m_platformAbstractions.GetFileIO(),
                                                                                               "SearchResultOnMap/pin_sheet.json",
                                                                                               m_platformAbstractions.GetTextureFileLoader());

        m_pWorldPinsIconMapping = worldPinIconMappingFactory.Create();

        m_pPinsModule = CreatePlatformPinsModuleInstance(world, m_pWorldPinsIconMapping->GetTextureInfo(), m_pWorldPinsIconMapping->GetTexturePageLayout());

        Eegeo::Modules::Map::Layers::InteriorsPresentationModule& interiorsPresentationModule = mapModule.GetInteriorsPresentationModule();

        m_pWorldPinsModule = Eegeo_NEW(ExampleApp::WorldPins::SdkModel::WorldPinsModule)(m_messageBus,
                                                                                         interiorsPresentationModule.GetInteriorInteractionModel(),
                                                                                         interiorsPresentationModule.GetInteriorTransitionModel(),
                                                                                         m_sdkDomainEventBus,
                                                                                         interiorsPresentationModule.GetInteriorMarkerPickingService(),
                                                                                         mapModule.GetMarkersModule().GetMarkerService(),
                                                                                         *m_pNavigationService,
                                                                                         m_pSearchModule->GetSearchResultMyPinsService(),
                                                                                         *m_pCameraTransitionService);
    }

    void MobileExampleApp::OnPause()
    {
        Eegeo::EegeoWorld& eegeoWorld = World();
        eegeoWorld.OnPause();

        const Eegeo::dv3& interestPoint = m_pGlobeCameraController->GetEcefInterestPoint();
        m_pInitialLocationModel->SetInterestLocation(Eegeo::Space::LatLongAltitude::FromECEF(interestPoint));

        m_pCurrentLocationService->StopListening();
    }

    void MobileExampleApp::OnResume()
    {
        Eegeo::EegeoWorld& eegeoWorld = World();
        eegeoWorld.OnResume();
    }

    void MobileExampleApp::Update(float dt)
    {
        Eegeo::EegeoWorld& eegeoWorld(World());

        m_pCurrentTouchController = &m_pAppCameraModule->GetController().GetTouchController();

        eegeoWorld.EarlyUpdate(dt);

        m_pNavigationService->Update(dt);

        m_pCameraTransitionService->Update(dt);
        m_pAppCameraModule->GetController().Update(dt);

        if (!eegeoWorld.Initialising() && m_initialisedApplicationViewState)
        {
            m_pAppModeModel->Update(dt);
        }

        m_pInteriorsExplorerModule->Update(dt);
        m_pNavRoutingModule->Update(dt);

        Eegeo::Camera::RenderCamera renderCamera = m_pAppCameraModule->GetController().GetRenderCamera();
        Eegeo::Camera::CameraState cameraState = m_pAppCameraModule->GetController().GetCameraState();

        Eegeo::dv3 ecefInterestPoint(cameraState.InterestPointEcef());

        m_pPoiRingModule->GetPoiRingController().Update(dt, renderCamera, ecefInterestPoint);

        Eegeo::EegeoUpdateParameters updateParameters(dt,
                                                      cameraState.LocationEcef(),
                                                      cameraState.InterestPointEcef(),
                                                      cameraState.ViewMatrix(),
                                                      cameraState.ProjectionMatrix(),
                                                      GetUpdatedStreamingVolume(cameraState, renderCamera),
                                                      m_screenProperties);

        eegeoWorld.Update(updateParameters);

        m_pSearchModule->GetSearchRefreshService().TryRefreshSearch(dt, ecefInterestPoint, cameraState.LocationEcef());

        m_pPinsModule->GetController().Update(dt, renderCamera);

        if(!eegeoWorld.Initialising() || (m_pLoadingScreen == NULL && eegeoWorld.Initialising()))
        {
            m_pWorldPinsModule->Update(dt);

            CompassModule().GetCompassUpdateController().Update(dt);
            m_pGpsMarkerModule->GetGpsMarkerController().Update(dt, renderCamera);

            InitialExperience::SdkModel::IInitialExperienceModel& initialExperienceModel = m_initialExperienceModule.GetInitialExperienceModel();
            if(!initialExperienceModel.HasCompletedInitialExperience() && IsLoadingScreenComplete())
            {
                InitialExperience::SdkModel::IInitialExperienceController& initialExperienceController = m_initialExperienceModule.GetInitialExperienceController();
                initialExperienceController.Update(dt);
            }

            if (!m_setMetricsLocation)
            {
                Eegeo::dv3 gpsLocation;
                if(m_pNavigationService->TryGetGpsLocationOnTerrain(gpsLocation))
                {
                    Eegeo::Space::LatLong ll = Eegeo::Space::LatLong::FromECEF(gpsLocation);
                    m_metricsService.SetPosition(ll.GetLatitudeInDegrees(), ll.GetLongitudeInDegrees(), 0.f, 0.f);
                    m_setMetricsLocation = true;
                }
            }

            if (nullptr != m_pAutomatedScreenshotController)
            {
                m_pAutomatedScreenshotController->Update(dt);
            }
        }

        UpdateLoadingScreen(dt);
        
        m_pSurveyTimer->Update();
    }

    void MobileExampleApp::Draw (float dt)
    {
        Eegeo::EegeoWorld& eegeoWorld = World();

        Eegeo::Camera::RenderCamera renderCamera = m_pAppCameraModule->GetController().GetRenderCamera();
        Eegeo::Camera::CameraState cameraState = m_pAppCameraModule->GetController().GetCameraState();

        Eegeo::dv3 ecefInterestPoint(cameraState.InterestPointEcef());

        Eegeo::EegeoDrawParameters drawParameters(cameraState.LocationEcef(),
                                                  cameraState.InterestPointEcef(),
                                                  cameraState.ViewMatrix(),
                                                  cameraState.ProjectionMatrix(),
                                                  m_screenProperties);

        eegeoWorld.Draw(drawParameters);

        if (m_pLoadingScreen != NULL)
        {
            m_pLoadingScreen->Draw();
        }
    }

    void MobileExampleApp::NotifyScreenPropertiesChanged(const Eegeo::Rendering::ScreenProperties& screenProperties)
    {
        m_screenProperties = screenProperties;

        if (m_pLoadingScreen != NULL)
        {
            m_pLoadingScreen->NotifyScreenDimensionsChanged(screenProperties.GetScreenWidth(), screenProperties.GetScreenHeight());
        }

        m_pPinsModule->UpdateScreenProperties(m_screenProperties);

        m_pGlobeCameraController->UpdateScreenProperties(m_screenProperties);

        m_pCameraSplinePlaybackController->UpdateScreenProperties(m_screenProperties);

        m_pInteriorsExplorerModule->GetInteriorsGpsCameraController().UpdateScreenProperties(m_screenProperties);
    }

    void MobileExampleApp::InitialiseApplicationViewState()
    {
        Eegeo_ASSERT(m_initialisedApplicationViewState == false, "Can only initialise application state once!\n");

        m_initialisedApplicationViewState = true;

        m_pSearchMenuModule->GetSearchMenuViewModel().AddToScreen();
        m_pCompassModule->GetScreenControlViewModel().AddToScreen();
        m_pMyPinCreationModule->GetInitiationScreenControlViewModel().AddToScreen();
        m_pWatermarkModule->GetWatermarkViewModel().AddToScreen();
    }

    void MobileExampleApp::UpdateLoadingScreen(float dt)
    {
        if (m_pLoadingScreen == NULL)
        {
            return;
        }

        Eegeo::EegeoWorld& eegeoWorld = World();

        if (!eegeoWorld.Initialising() && !m_pLoadingScreen->IsDismissed())
        {
            m_pNavigationService->SetGpsMode(Eegeo::Location::NavigationService::GpsModeOff);
            m_pLoadingScreen->Dismiss();
        }

        m_pLoadingScreen->SetProgress(eegeoWorld.GetInitialisationProgress());
        m_pLoadingScreen->Update(dt);

        if (!m_pLoadingScreen->IsVisible())
        {
            Eegeo_DELETE m_pLoadingScreen;
            m_pLoadingScreen = NULL;

            // Note: we're doing this here in a hacky way, as the views aren't constructed when the MEA ctor runs
            // ... doing it a little later ensures the view will get the notifications when items are added.
            MyPinsModule().GetMyPinsService().LoadAllPinsFromDisk();

            if (m_applicationConfiguration.IsAttractModeEnabled())
            {
                m_pAppModeModel->SetAppMode(AppModes::SdkModel::AttractMode);
            }
        }
    }

    bool MobileExampleApp::IsRunning() const
    {
        return !m_pWorld->Initialising();
    }

    Eegeo::Streaming::IStreamingVolume& MobileExampleApp::GetUpdatedStreamingVolume(const Eegeo::Camera::CameraState& cameraState,
                                                                                    const Eegeo::Camera::RenderCamera& renderCamera)
    {
        std::vector<Eegeo::Geometry::Plane> frustumPlanes(Eegeo::Geometry::Frustum::PLANES_COUNT);
        BuildFrustumPlanesFromViewProjection(frustumPlanes, renderCamera.GetViewProjectionMatrix());
        const double d = renderCamera.GetAltitude() * Eegeo::Streaming::StreamingVolumeController::CAMERA_ALTITUDE_TO_FAR_PLANE_DISTANCE_MULTIPLIER;
        const double cameraFarPlaneD = fmin(fmax(d, Eegeo::Streaming::StreamingVolumeController::MIN_STREAMING_FAR_PLANE_DISTANCE),
                                            frustumPlanes[Eegeo::Geometry::Frustum::PLANE_FAR].d);
        frustumPlanes[Eegeo::Geometry::Frustum::PLANE_FAR].d = static_cast<float>(cameraFarPlaneD);

        m_pStreamingVolume->updateStreamingVolume(renderCamera.GetEcefLocation(), frustumPlanes, renderCamera.GetFOV());
        m_pStreamingVolume->ResetVolume(cameraState.InterestPointEcef());
        return *m_pStreamingVolume;
    }

    void MobileExampleApp::Event_TouchRotate(const AppInterface::RotateData& data)
    {
        if (!CanAcceptTouch())
        {
            return;
        }

        m_pCurrentTouchController->Event_TouchRotate(data);

    }

    void MobileExampleApp::Event_TouchRotate_Start(const AppInterface::RotateData& data)
    {
        if (!CanAcceptTouch())
        {
            return;
        }

        m_pCompassModule->GetTouchController().HandleRotateStart(data);
        m_pCurrentTouchController->Event_TouchRotate_Start(data);
    }

    void MobileExampleApp::Event_TouchRotate_End(const AppInterface::RotateData& data)
    {
        if (!CanAcceptTouch())
        {
            return;
        }

        m_pCompassModule->GetTouchController().HandleRotateEnd(data);
        m_pCurrentTouchController->Event_TouchRotate_End(data);
    }

    void MobileExampleApp::Event_TouchPinch(const AppInterface::PinchData& data)
    {
        if (!CanAcceptTouch())
        {
            return;
        }

        m_pCurrentTouchController->Event_TouchPinch(data);
    }

    void MobileExampleApp::Event_TouchPinch_Start(const AppInterface::PinchData& data)
    {
        if (!CanAcceptTouch())
        {
            return;
        }

        m_pCurrentTouchController->Event_TouchPinch_Start(data);
    }

    void MobileExampleApp::Event_TouchPinch_End(const AppInterface::PinchData& data)
    {
        if (!CanAcceptTouch())
        {
            return;
        }

        m_pCurrentTouchController->Event_TouchPinch_End(data);
    }

    void MobileExampleApp::Event_TouchPan(const AppInterface::PanData& data)
    {
        MyPinCreation::PoiRing::SdkModel::IPoiRingTouchController& poiRingTouchController = m_pPoiRingModule->GetPoiRingTouchController();
        if(!CanAcceptTouch() || poiRingTouchController.IsDragging())
        {
            return;
        }

        m_pCurrentTouchController->Event_TouchPan(data);
    }

    void MobileExampleApp::Event_TouchPan_Start(const AppInterface::PanData& data)
    {
        MyPinCreation::PoiRing::SdkModel::IPoiRingTouchController& poiRingTouchController = m_pPoiRingModule->GetPoiRingTouchController();
        if(!CanAcceptTouch() || poiRingTouchController.IsDragging())
        {
            return;
        }

        m_pCurrentTouchController->Event_TouchPan_Start(data);
    }

    void MobileExampleApp::Event_TouchPan_End(const AppInterface::PanData& data)
    {
        MyPinCreation::PoiRing::SdkModel::IPoiRingTouchController& poiRingTouchController = m_pPoiRingModule->GetPoiRingTouchController();
        if(!CanAcceptTouch() || poiRingTouchController.IsDragging())
        {
            return;
        }

        m_pCurrentTouchController->Event_TouchPan_End(data);
    }

    void MobileExampleApp::Event_TouchTap(const AppInterface::TapData& data)
    {
        if (!CanAcceptTouch())
        {
            return;
        }

        if (m_pWorldPinsModule->GetWorldPinsService().HandleTouchTap(data.point))
        {
            return;
        }

        if(m_pNavRoutingModule->GetCustomLocationPicker().HandleTouchTap(data.point.x, data.point.y))
        {
            return;
        }

        m_pCurrentTouchController->Event_TouchTap(data);
    }

    void MobileExampleApp::Event_TouchDoubleTap(const AppInterface::TapData& data)
    {
        if (!CanAcceptTouch())
        {
            return;
        }

        if (m_pWorldPinsModule->GetWorldPinsService().HandleTouchDoubleTap(data.point))
        {
            return;
        }
        if (m_pAppModeModel->GetAppMode() == AppModes::SdkModel::InteriorMode)
        {
            m_pDoubleTapIndoorInteractionController->OnDoubleTap(data);
        }
        m_pCurrentTouchController->Event_TouchDoubleTap(data);
    }

    void MobileExampleApp::Event_TouchDown(const AppInterface::TouchData& data)
    {
        if (!CanAcceptTouch())
        {
            return;
        }

        MyPinCreation::PoiRing::SdkModel::IPoiRingTouchController& poiRingTouchController = m_pPoiRingModule->GetPoiRingTouchController();
        if (!poiRingTouchController.HandleTouchDown(data, m_pAppCameraModule->GetController().GetRenderCamera(), m_pAppCameraModule->GetController().GetNonFlattenedCameraPosition()))
        {
            m_pCurrentTouchController->Event_TouchDown(data);
        }
    }

    void MobileExampleApp::Event_TouchMove(const AppInterface::TouchData& data)
    {
        if (!CanAcceptTouch())
        {
            return;
        }

        MyPinCreation::PoiRing::SdkModel::IPoiRingTouchController& poiRingTouchController = m_pPoiRingModule->GetPoiRingTouchController();
        if (!poiRingTouchController.HandleTouchMove(data, m_pAppCameraModule->GetController().GetRenderCamera(), m_pAppCameraModule->GetController().GetNonFlattenedCameraPosition()))
        {
            m_pCurrentTouchController->Event_TouchMove(data);
        }
    }

    void MobileExampleApp::Event_TouchUp(const AppInterface::TouchData& data)
    {
        if (!CanAcceptTouch())
        {
            return;
        }

        MyPinCreation::PoiRing::SdkModel::IPoiRingTouchController& poiRingTouchController = m_pPoiRingModule->GetPoiRingTouchController();
        if (!poiRingTouchController.HandleTouchUp(data))
        {
            m_pCurrentTouchController->Event_TouchUp(data);
        }
    }

    void MobileExampleApp::Event_Zoom(const AppInterface::ZoomData& data)
    {
        if (!CanAcceptTouch())
        {
            return;
        }

        m_pCurrentTouchController->Event_Zoom(data);
    }

    void MobileExampleApp::Event_Keyboard(const AppInterface::KeyboardData& data)
    {
        if (data.printable)
            Eegeo_TTY("Key Down: %c", data.keyCode);
        else
            Eegeo_TTY("Key Up: %c", data.keyCode);
    }

    void MobileExampleApp::Event_TiltStart(const AppInterface::TiltData& data)
    {
        if (!CanAcceptTouch())
        {
            return;
        }

        m_pCurrentTouchController->Event_TiltStart(data);
    }

    void MobileExampleApp::Event_TiltEnd(const AppInterface::TiltData& data)
    {
        if (!CanAcceptTouch())
        {
            return;
        }

        m_pCurrentTouchController->Event_TiltEnd(data);
    }

    void MobileExampleApp::Event_Tilt(const AppInterface::TiltData& data)
    {
        if (!CanAcceptTouch())
        {
            return;
        }

        m_pCurrentTouchController->Event_Tilt(data);
    }

    void MobileExampleApp::Event_OpenUrl(const AppInterface::UrlData& data)
    {
        m_pDeepLinkModule->GetDeepLinkController().HandleDeepLinkOpen(data);
    }

    bool MobileExampleApp::CanAcceptTouch() const
    {
        const bool worldIsInitialising = World().Initialising() && m_pLoadingScreen != NULL;
        const bool userInteractionEnabled = m_pUserInteractionModule->GetUserInteractionModel().IsEnabled();
        const bool cameraTransitioning = m_pCameraTransitionService->IsTransitioning();

        InitialExperience::SdkModel::IInitialExperienceModel& initialExperienceModel = m_initialExperienceModule.GetInitialExperienceModel();
        const bool lockedCameraStepsCompleted = initialExperienceModel.LockedCameraStepsCompleted();

        return !worldIsInitialising && lockedCameraStepsCompleted && userInteractionEnabled && !cameraTransitioning;
    }

    Automation::AutomatedScreenshotController* MobileExampleApp::AutomatedScreenshotController() const
    {
        return m_pAutomatedScreenshotController;
    }
}
