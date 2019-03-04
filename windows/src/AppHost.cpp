// Copyright eeGeo Ltd (2012-2015), All Rights Reserved

#include "AppHost.h"
#include "WindowsSharedGlContext.h"
#include "LatLongAltitude.h"
#include "EegeoWorld.h"
#include "RenderContext.h"
#include "GlobalLighting.h"
#include "GlobalFogging.h"
#include "AppInterface.h"
#include "JpegLoader.h"
#include "EffectHandler.h"
#include "SearchServiceCredentials.h"
#include "WindowsThreadHelper.h"
#include "GlobeCameraController.h"
#include "RenderCamera.h"
#include "CameraHelpers.h"
#include "LoadingScreen.h"
#include "PlatformConfig.h"
#include "WindowsPlatformConfigBuilder.h"
#include "WindowsUrlEncoder.h"
#include "WindowsFileIO.h"
#include "WindowsLocationService.h"
#include "FixedIndoorLocationService.h"
#include "EegeoWorld.h"
#include "EnvironmentFlatteningService.h"
#include "TtyHandler.h"
#include "MenuViewModule.h"
#include "ModalityModule.h"
#include "ModalBackgroundViewModule.h"
#include "ModalBackgroundNativeViewModule.h"
#include "MenuModel.h"
#include "MenuViewModel.h"
#include "MenuOptionsModel.h"
#include "SearchModule.h"
#include "SearchResultOnMapModule.h"
#include "WorldPinsModule.h"
#include "RegularTexturePageLayout.h"
#include "PinsModule.h"
#include "SearchResultRepository.h"
#include "LatLongAltitude.h"
#include "SearchResultPoiModule.h"
#include "WindowsPlatformAbstractionModule.h"
#include "FlattenButtonModule.h"
#include "FlattenButtonViewModule.h"
#include "SearchResultPoiViewModule.h"
#include "PlaceJumpsModule.h"
#include "IPlaceJumpController.h"
#include "SettingsMenuViewModule.h"
#include "SearchMenuViewModule.h"
#include "CompassViewModule.h"
#include "CompassModule.h"
#include "AboutPageViewModule.h"
#include "WindowsInitialExperienceModule.h"
#include "ViewControllerUpdaterModule.h"
#include "ViewControllerUpdaterModel.h"
#include "TagSearchModule.h"
#include "ScreenProperties.h"
#include "MyPinCreationViewModule.h"
#include "IMyPinCreationModule.h"
#include "IMyPinCreationDetailsModule.h"
#include "MyPinCreationDetailsViewModule.h"
#include "MyPinDetailsViewModule.h"
#include "IMyPinDetailsModule.h"
#include "InitialExperienceIntroViewModule.h"
#include "Logger.h"
#include "WindowsAppThreadAssertionMacros.h"
#include "SearchResultRepositoryObserver.h"
#include "IMyPinsModule.h"
#include "ApiKey.h"
#include "OptionsViewModule.h"
#include "OptionsView.h"
#include "WatermarkViewModule.h"
#include "WatermarkView.h"
#include "NetworkCapabilities.h"
#include "ApplicationConfigurationModule.h"
#include "IApplicationConfigurationService.h"
#include "SearchVendorNames.h"
#include "InteriorsExplorerViewModule.h"
#include "IInteriorsExplorerModule.h"
#include "WindowsPersistentSettingsModel.h"
#include "IHttpCache.h"
#include "HttpCache.h"
#include "SearchMenuViewModule.h"
#include "SettingsMenuViewModule.h"
#include "SearchResultSectionViewModule.h"
#include "SearchResultSectionModule.h"
#include "SurveyViewModule.h"
#include "SearchResultPoiView.h"
#include "WindowsMenuReactionModel.h"
#include "TagSearchViewModule.h"
#include "IMyPinCreationInitiationViewModel.h"
#include "WindowsApplicationConfigurationVersionProvider.h"
#include "IUserIdleService.h"
#include "CurrentLocationService.h"
#include "AttractModeOverlayView.h"
#include "WindowsProcessHelper.h"
#include "ImagePathHelpers.h"
#include "GpsMarkerTutorialViewModule.h"
#include "GpsMarkerModule.h"

using namespace Eegeo::Windows;
using namespace Eegeo::Windows::Input;

namespace
{
    void SetFixedImageScaleAndSuffix(Eegeo::Config::PlatformConfig& platformConfiguration, Eegeo::Rendering::ScreenProperties& screenProperties, bool isInKioskMode)
    {
        const float FixedKioskScaling = 2.0f;
        const const char *const FixedKioskDensitySuffix = "@2x";

        const float FixedDesktopScaling = 1.5f;
        const const char *const FixedDesktopDensitySuffix = "@1.5x";

        const float fixedScaling = isInKioskMode ? FixedKioskScaling : FixedDesktopScaling;
        platformConfiguration.GraphicsConfig.ImageResolutionScale = fixedScaling * screenProperties.GetOversampleScale();
        platformConfiguration.GraphicsConfig.ImageResolutionSuffix = isInKioskMode ? FixedKioskDensitySuffix : FixedDesktopDensitySuffix;
        platformConfiguration.MapLayersConfig.LabelsModuleConfig.CustomTextScale = fixedScaling;
    }
}

AppHost::AppHost(
    WindowsNativeState& nativeState,
    Eegeo::Rendering::ScreenProperties screenProperties,
    EGLDisplay display,
    EGLSurface shareSurface,
    EGLContext resourceBuildShareContext,
    bool hasNativeTouchInput,
    int maxDeviceTouchCount
)
    :m_isPaused(false)
    , m_pJpegLoader(NULL)
    , m_pWindowsLocationService(NULL)
    , m_pFixedIndoorLocationService(NULL)
    , m_pCurrentLocationService(NULL)
    , m_nativeState(nativeState)
    , m_WindowsInputBoxFactory(&nativeState)
    , m_WindowsKeyboardInputFactory(&nativeState, m_inputHandler)
    , m_WindowsAlertBoxFactory(&nativeState)
    , m_WindowsNativeUIFactories(m_WindowsAlertBoxFactory, m_WindowsInputBoxFactory, m_WindowsKeyboardInputFactory)
    , m_pInputProcessor(NULL)
    , m_pWindowsPlatformAbstractionModule(NULL)
    , m_pSettingsMenuViewModule(NULL)
    , m_pSearchMenuViewModule(NULL)
    , m_pSearchResultSectionViewModule(NULL)
    , m_pModalBackgroundViewModule(NULL)
    , m_pFlattenButtonViewModule(NULL)
    , m_pGpsMarkerTutorialViewModule(NULL)
    , m_pMyPinCreationViewModule(NULL)
    , m_pMyPinCreationDetailsViewModule(NULL)
    , m_pMyPinDetailsViewModule(NULL)
    , m_pSearchResultPoiViewModule(NULL)
    , m_pCompassViewModule(NULL)
    , m_pApp(NULL)
    , m_pWindowsPersistentSettingsModel(NULL)
    , m_createdUIModules(false)
    , m_requestedApplicationInitialiseViewState(false)
    , m_uiCreatedMessageReceivedOnNativeThread(false)
    , m_pViewControllerUpdaterModule(NULL)
    , m_pWindowsFlurryMetricsService(NULL)
    , m_pInitialExperienceIntroViewModule(NULL)
    , m_pSurverysViewModule(NULL)
    , m_pInteriorsExplorerViewModule(NULL)
    , m_failAlertHandler(this, &AppHost::HandleStartupFailure)
    , m_pMenuReaction(NULL)
    , m_shouldStartFullscreen(false)
    , m_maxDeviceTouchCount(maxDeviceTouchCount)
    , m_pTagSearchViewModule(NULL)
    , m_pUserIdleService(NULL)
    , m_pVirtualKeyboardView(NULL)
    , m_pAttractModeOverlayView(NULL)
    , m_screenProperties(screenProperties)
{
    ASSERT_NATIVE_THREAD
         
    Eegeo_ASSERT(resourceBuildShareContext != EGL_NO_CONTEXT);

    Eegeo::TtyHandler::TtyEnabled = true;
    Eegeo::AssertHandler::BreakOnAssert = true;


    m_pJpegLoader = Eegeo_NEW(Eegeo::Helpers::Jpeg::JpegLoader)();

    const ExampleApp::ApplicationConfig::ApplicationConfiguration& applicationConfiguration = ExampleApp::ApplicationConfig::SdkModel::LoadAppConfig(
        WindowsFileIO(&nativeState),
        ExampleApp::ApplicationConfig::SdkModel::WindowsApplicationConfigurationVersionProvider(),
        ExampleApp::ApplicationConfigurationPath);

    m_pCustomKeyboardLayout = applicationConfiguration.CustomKeyboardLayout();

    m_pWindowsLocationService = Eegeo_NEW(Eegeo::Windows::WindowsLocationService)(&nativeState);
    m_pCurrentLocationService = Eegeo_NEW(Eegeo::Helpers::CurrentLocationService::CurrentLocationService)(*m_pWindowsLocationService);

    m_shouldStartFullscreen = applicationConfiguration.ShouldStartFullscreen();

    m_pWindowsPlatformAbstractionModule = Eegeo_NEW(Eegeo::Windows::WindowsPlatformAbstractionModule)(
        nativeState,
        *m_pJpegLoader,
        display,
        resourceBuildShareContext,
        shareSurface,
        applicationConfiguration.EegeoApiKey());

    Eegeo::EffectHandler::Initialise();

    Eegeo::Windows::WindowsPlatformConfigBuilder windowsPlatformConfigBuilder(nativeState.GetDeviceModel());
    Eegeo::Config::PlatformConfig& platformConfiguration = ExampleApp::ApplicationConfig::SdkModel::BuildPlatformConfig(windowsPlatformConfigBuilder, applicationConfiguration);
    SetFixedImageScaleAndSuffix(platformConfiguration, screenProperties, applicationConfiguration.IsInKioskMode());

    bool enableTouchControls =  hasNativeTouchInput ? applicationConfiguration.IsKioskTouchInputEnabled() : false;

    if (enableTouchControls && applicationConfiguration.ShouldStartFullscreen())
    {
        Eegeo::Helpers::ProcessHelpers::SetEdgeTouchGesturesEnabled(false);
    }

    const Eegeo::Windows::Input::WindowsInputProcessorConfig& windowsInputProcessorConfig = Eegeo::Windows::Input::WindowsInputProcessor::DefaultConfig();
    m_pInputProcessor = Eegeo_NEW(Eegeo::Windows::Input::WindowsInputProcessor)(&m_inputHandler, m_nativeState.GetWindow(), screenProperties.GetScreenWidth(), screenProperties.GetScreenHeight(), windowsInputProcessorConfig, enableTouchControls, m_maxDeviceTouchCount);
    m_pUserIdleService = m_pInputProcessor;

    m_pWindowsPersistentSettingsModel = Eegeo_NEW(ExampleApp::PersistentSettings::WindowsPersistentSettingsModel)(m_nativeState);

    m_pInitialExperienceModule = Eegeo_NEW(ExampleApp::InitialExperience::SdkModel::WindowsInitialExperienceModule)(
        m_nativeState,
        *m_pWindowsPersistentSettingsModel,
        m_messageBus
        );

    m_pNetworkCapabilities = Eegeo_NEW(ExampleApp::Net::SdkModel::NetworkCapabilities)(
        m_pWindowsPlatformAbstractionModule->GetConnectivityService(),
        m_pWindowsPlatformAbstractionModule->GetHttpCache(),
        *m_pWindowsPersistentSettingsModel);

    m_pWindowsFlurryMetricsService = Eegeo_NEW(ExampleApp::Metrics::WindowsFlurryMetricsService)(&m_nativeState);

    m_pMenuReaction = Eegeo_NEW(ExampleApp::Menu::View::WindowsMenuReactionModel)(false, false);


    m_pApp = Eegeo_NEW(ExampleApp::MobileExampleApp)(
        applicationConfiguration,
        *m_pWindowsPlatformAbstractionModule,
        screenProperties,
        *m_pCurrentLocationService,
        m_WindowsNativeUIFactories,
        platformConfiguration,
        *m_pJpegLoader,
        *m_pInitialExperienceModule,
        *m_pWindowsPersistentSettingsModel,
        m_messageBus,
        m_sdkDomainEventBus,
        *m_pNetworkCapabilities,
        *m_pWindowsFlurryMetricsService,        
        *this,
        *m_pMenuReaction,
        *m_pUserIdleService,
        m_screenshotService);

    if (applicationConfiguration.IsFixedIndoorLocationEnabled())
    {
        const ExampleApp::ApplicationConfig::SdkModel::ApplicationFixedIndoorLocation& fixedIndoorLocation = applicationConfiguration.FixedIndoorLocation();
        const Eegeo::Modules::Map::MapModule& mapModule = m_pApp->World().GetMapModule();
        m_pFixedIndoorLocationService = Eegeo_NEW(Eegeo::FixedLocation::FixedIndoorLocationService)(
            fixedIndoorLocation.GetLocation(),
            fixedIndoorLocation.GetInteriorId(),
            fixedIndoorLocation.GetBuildingFloorIndex(),
            fixedIndoorLocation.GetOrientationDegrees(),
            mapModule.GetEnvironmentFlatteningService(),
            mapModule.GetInteriorsPresentationModule().GetInteriorInteractionModel());
        m_pCurrentLocationService->SetLocationService(*m_pFixedIndoorLocationService);

        m_pInteriorsLocationServiceController = Eegeo_NEW(ExampleApp::InteriorsPosition::SdkModel::InteriorsLocationServiceController)(*m_pCurrentLocationService,
                                                                                                                                       mapModule.GetInteriorsPresentationModule().GetInteriorInteractionModel(),
                                                                                                                                       m_pApp->CameraTransitionController(),
                                                                                                                                       m_pApp->CompassModule().GetCompassModel(),
                                                                                                                                       applicationConfiguration.FixedIndoorLocation().GetLocationDistance());
    }

    m_pModalBackgroundNativeViewModule = Eegeo_NEW(ExampleApp::ModalBackground::SdkModel::ModalBackgroundNativeViewModule)(
        m_pApp->World().GetRenderingModule(),
        m_messageBus);

    m_pSurverysViewModule = Eegeo_NEW(ExampleApp::Surveys::View::SurveyViewModule)(m_messageBus, *m_pWindowsFlurryMetricsService);

    m_pAppInputDelegate = Eegeo_NEW(AppInputDelegate)(*m_pApp);
    m_inputHandler.AddDelegateInputHandler(m_pAppInputDelegate);
}

AppHost::~AppHost()
{
    ASSERT_NATIVE_THREAD

    m_inputHandler.RemoveDelegateInputHandler(m_pAppInputDelegate);

    if(m_pApp->GetApplicationConfiguration().IsFixedIndoorLocationEnabled())
    {
        Eegeo_DELETE m_pInteriorsLocationServiceController;
        m_pInteriorsLocationServiceController = NULL;
    }

    Eegeo_DELETE m_pAppInputDelegate;
    m_pAppInputDelegate = NULL;

    Eegeo_DELETE m_pSurverysViewModule;
    m_pSurverysViewModule = NULL;

    Eegeo_DELETE m_pApp;
    m_pApp = NULL;

    Eegeo_DELETE m_pWindowsFlurryMetricsService;
    m_pWindowsFlurryMetricsService = NULL;

    Eegeo_DELETE m_pNetworkCapabilities;
    m_pNetworkCapabilities = NULL;

    Eegeo_DELETE m_pInitialExperienceModule;
    m_pInitialExperienceModule = NULL;

    Eegeo_DELETE m_pWindowsPersistentSettingsModel;
    m_pWindowsPersistentSettingsModel = NULL;

    Eegeo_DELETE m_pInputProcessor;
    m_pInputProcessor = NULL;

    Eegeo::EffectHandler::Reset();
    Eegeo::EffectHandler::Shutdown();

    Eegeo_DELETE m_pWindowsPlatformAbstractionModule;
    m_pWindowsPlatformAbstractionModule = NULL;

    Eegeo_DELETE m_pJpegLoader;
    m_pJpegLoader = NULL;

    Eegeo_DELETE(m_pCurrentLocationService);
    m_pCurrentLocationService = NULL;
    Eegeo_DELETE(m_pWindowsLocationService);
    m_pWindowsLocationService = NULL;
    Eegeo_DELETE(m_pFixedIndoorLocationService);
    m_pFixedIndoorLocationService = NULL;
}

bool AppHost::ShouldStartFullscreen()
{
    return m_shouldStartFullscreen;
}

bool AppHost::IsInKioskMode()
{
    return m_pApp->GetApplicationConfiguration().IsInKioskMode();
}

void AppHost::OnResume()
{
    ASSERT_NATIVE_THREAD

        m_pApp->OnResume();
    m_isPaused = false;
}

void AppHost::OnPause()
{
    ASSERT_NATIVE_THREAD

        m_isPaused = true;
    m_pApp->OnPause();
    m_pCurrentLocationService->StopListening();
}

void AppHost::NotifyScreenPropertiesChanged(const Eegeo::Rendering::ScreenProperties& screenProperties)
{
    m_screenProperties = screenProperties;
    m_pApp->NotifyScreenPropertiesChanged(screenProperties);
}

void AppHost::SetSharedSurface(EGLSurface sharedSurface)
{
    ASSERT_NATIVE_THREAD

        m_pWindowsPlatformAbstractionModule->UpdateSurface(sharedSurface);
}

void AppHost::SetViewportOffset(float x, float y)
{
    ASSERT_NATIVE_THREAD

        m_inputHandler.SetViewportOffset(x, y);
}

void AppHost::HandleMousePreviewInputEvent(const Eegeo::Windows::Input::MouseInputEvent& event)
{
    ASSERT_NATIVE_THREAD

    m_pInputProcessor->HandleMousePreviewInput(event);
}

void AppHost::HandleMouseInputEvent(const Eegeo::Windows::Input::MouseInputEvent& event)
{
    ASSERT_NATIVE_THREAD

    m_pInputProcessor->HandleMouseInput(event, static_cast<float>(m_nativeState.GetWidth()), static_cast<float>(m_nativeState.GetHeight()));
}

void AppHost::HandleKeyboardInputEvent(const Eegeo::Windows::Input::KeyboardInputEvent& event)
{
    m_pInputProcessor->HandleMouseInput(event);
}

void AppHost::HandleTouchScreenInputEvent(const Eegeo::Windows::Input::TouchScreenInputEvent& event)
{
    m_pInputProcessor->HandleTouchScreenInput(event, static_cast<float>(m_nativeState.GetWidth()), static_cast<float>(m_nativeState.GetHeight()));
}

void AppHost::SetAllInputEventsToPointerUp(int x, int y)
{
    m_pInputProcessor->SetAllInputEventsToPointerUp(x, y);
}

void AppHost::SetTouchInputEventToPointerUp(int touchId)
{
    m_pInputProcessor->SetTouchInputEventToPointerUp(touchId);
}

void AppHost::PopAllTouchEvents()
{
    m_pInputProcessor->PopAllTouchEvents();
}

void AppHost::SaveInputTime()
{
    m_pInputProcessor->SaveInputTime();
}

void AppHost::Update(float dt)
{
    ASSERT_NATIVE_THREAD

        if (m_isPaused)
        {
            return;
        }

    m_messageBus.FlushToNative();

    m_pApp->Update(dt);

    m_pModalBackgroundNativeViewModule->Update(dt);

    if(m_pApp->GetApplicationConfiguration().IsFixedIndoorLocationEnabled() && m_pApp->IsLoadingScreenComplete())
    {
        m_pInteriorsLocationServiceController->Update();
    }

    if (m_pApp->IsLoadingScreenComplete() && !m_requestedApplicationInitialiseViewState)
    {
        m_requestedApplicationInitialiseViewState = true;
        DispatchRevealUiMessageToUiThreadFromNativeThread();
    }
}

void AppHost::Draw(float dt)
{
    ASSERT_NATIVE_THREAD

        if (m_isPaused)
        {
            return;
        }

    m_pApp->Draw(dt);
    m_pInputProcessor->Update(dt);
}

void AppHost::HandleApplicationUiCreatedOnNativeThread()
{
    ASSERT_NATIVE_THREAD

        m_uiCreatedMessageReceivedOnNativeThread = true;
}

void AppHost::DispatchRevealUiMessageToUiThreadFromNativeThread()
{
    ASSERT_NATIVE_THREAD

    RevealUiFromUiThread();
}

void AppHost::DispatchUiCreatedMessageToNativeThreadFromUiThread()
{
    ASSERT_UI_THREAD

    HandleApplicationUiCreatedOnNativeThread();
}

void AppHost::RevealUiFromUiThread()
{
    ASSERT_UI_THREAD

    m_pApp->InitialiseApplicationViewState();
}

void AppHost::CreateUiFromUiThread()
{
    ASSERT_UI_THREAD

        Eegeo_ASSERT(!m_createdUIModules);
    CreateApplicationViewModulesFromUiThread();
    DispatchUiCreatedMessageToNativeThreadFromUiThread();
}

void AppHost::UpdateUiViewsFromUiThread(float dt)
{
    ASSERT_UI_THREAD

        m_messageBus.FlushToUi();

    if (m_createdUIModules)
    {
        m_pViewControllerUpdaterModule->GetViewControllerUpdaterModel().UpdateObjectsUiThread(dt);

        if(m_pApp->GetApplicationConfiguration().IsInKioskMode())
        {
            const Eegeo::Camera::RenderCamera& renderCamera = m_pApp->GetAppModeModel().GetAppMode() == ExampleApp::AppModes::SdkModel::InteriorMode
                                                              ? m_pApp->InteriorsExplorerModule().GetInteriorsCameraController().GetRenderCamera()
                                                              : m_pApp->GetCameraController().GetRenderCamera();
            m_pGpsMarkerTutorialViewModule->GetController().UpdateScreenLocation(renderCamera,
                                                                                 m_screenProperties.GetOversampleScale());
        }
    }
    else
    {
        CreateUiFromUiThread();
    }
}

void AppHost::DestroyUiFromUiThread()
{
    ASSERT_UI_THREAD

        DestroyApplicationViewModulesFromUiThread();
}

void AppHost::CreateApplicationViewModulesFromUiThread()
{
    ASSERT_UI_THREAD

        m_createdUIModules = true;
    ExampleApp::MobileExampleApp& app = *m_pApp;

    m_pWatermarkViewModule = Eegeo_NEW(ExampleApp::Watermark::View::WatermarkViewModule)(
        m_nativeState,
        app.WatermarkModule().GetWatermarkViewModel(),
        app.WatermarkModule().GetWatermarkDataRepository(),
        m_messageBus,
        *m_pWindowsFlurryMetricsService
        );

    // 3d map view layer.

    // HUD behind modal background layer.
    m_pFlattenButtonViewModule = Eegeo_NEW(ExampleApp::FlattenButton::View::FlattenButtonViewModule)(
        m_nativeState,
        app.FlattenButtonModule().GetFlattenButtonViewModel(),
        m_messageBus,
        *m_pWindowsFlurryMetricsService
        );

    m_pCompassViewModule = Eegeo_NEW(ExampleApp::Compass::View::CompassViewModule)(
        m_nativeState,
        app.CompassModule().GetCompassViewModel(),
        m_messageBus,
        app.GetApplicationConfiguration().IsInKioskMode()
        );

    m_pMyPinCreationViewModule = Eegeo_NEW(ExampleApp::MyPinCreation::View::MyPinCreationViewModule)(
        m_nativeState,
        app.MyPinCreationModule().GetMyPinCreationInitiationViewModel(),
        app.MyPinCreationModule().GetMyPinCreationConfirmationViewModel(),
        app.MyPinCreationDetailsModule().GetMyPinCreationDetailsViewModel(),
        m_messageBus,
        *m_pWindowsFlurryMetricsService,
        app.GetApplicationConfiguration().IsInKioskMode()
        );

    // Modal background layer.
    m_pModalBackgroundViewModule = Eegeo_NEW(ExampleApp::ModalBackground::View::ModalBackgroundViewModule)(
        m_nativeState,
        app.ModalityModule().GetModalityModel(),
        m_messageBus
        );

    m_pSearchMenuViewModule = Eegeo_NEW(ExampleApp::SearchMenu::View::SearchMenuViewModule)(
        "ExampleAppWPF.SearchMenuView",
        m_nativeState,
        app.SearchMenuModule().GetSearchMenuModel(),
        app.SearchMenuModule().GetSearchMenuViewModel(),
        app.SearchMenuModule().GetSearchSectionViewModel(),
        app.TagSearchModule().GetTagSearchRepository(),
        m_pModalBackgroundViewModule->GetView(),
        app.ModalityModule().GetModalityController(),
        m_messageBus,
        app.ReactionModelModule().GetReactionModel(),
        app.GetApplicationConfiguration().IsInKioskMode()
        );

    m_pTagSearchViewModule = ExampleApp::TagSearch::View::TagSearchViewModule::Create(
        app.TagSearchModule().GetTagSearchMenuOptionsModel(),
        app.SettingsMenuModule().GetSettingsMenuViewModel(),
        m_messageBus,
        *m_pMenuReaction);

    m_pSettingsMenuViewModule = Eegeo_NEW(ExampleApp::SettingsMenu::View::SettingsMenuViewModule)(
        "ExampleAppWPF.SettingsMenuView",
        m_nativeState,
        app.SettingsMenuModule().GetSettingsMenuModel(),
        app.SettingsMenuModule().GetSettingsMenuViewModel(),
        m_pModalBackgroundViewModule->GetView(),
        m_pSearchMenuViewModule->GetMenuView(),
        m_messageBus,
        app.GetApplicationConfiguration().IsInKioskMode()
        );
    
    m_pSearchResultSectionViewModule = Eegeo_NEW(ExampleApp::SearchResultSection::View::SearchResultSectionViewModule)(
        app.SearchMenuModule().GetSearchMenuViewModel(),
        app.SearchResultSectionModule().GetSearchResultSectionOptionsModel(),
        app.SearchResultSectionModule().GetSearchResultSectionOrder(),
        m_messageBus,
        *m_pMenuReaction,
        app.SearchResultPoiModule().GetSearchResultPoiViewModel()
        );

    if(app.GetApplicationConfiguration().IsInKioskMode())
    {
        m_pGpsMarkerTutorialViewModule = Eegeo_NEW(ExampleApp::GpsMarkerTutorial::View::GpsMarkerTutorialViewModule)(m_nativeState,
            m_messageBus,
            app.GpsMarkerModule().GetGpsMarkerModel(),
            app.World().GetMapModule().GetEnvironmentFlatteningService());
    }

    // Pop-up layer.
    m_pSearchResultPoiViewModule = Eegeo_NEW(ExampleApp::SearchResultPoi::View::SearchResultPoiViewModule)(
        m_nativeState,
        app.SearchResultPoiModule().GetSearchResultPoiViewModel(),
        m_messageBus,
        *m_pWindowsFlurryMetricsService,
        m_pMyPinCreationViewModule->GetMyPinCreationInitiationView(),
        app.World().GetMapModule().GetInteriorsPresentationModule().GetInteriorSelectionModel(),
        app.MyPinDetailsModule().GetMyPinDetailsViewModel(),
        app.GetApplicationConfiguration().IsInKioskMode()
        );

    m_pAboutPageViewModule = Eegeo_NEW(ExampleApp::AboutPage::View::AboutPageViewModule)(
        m_nativeState,
        app.AboutPageModule().GetAboutPageViewModel(),
        *m_pWindowsFlurryMetricsService,
        m_messageBus
        );

    m_pMyPinCreationDetailsViewModule = Eegeo_NEW(ExampleApp::MyPinCreationDetails::View::MyPinCreationDetailsViewModule)(
        m_nativeState,
        app.MyPinCreationDetailsModule().GetMyPinCreationDetailsViewModel(),
        m_messageBus,
        *m_pWindowsFlurryMetricsService,
        app.GetApplicationConfiguration().IsInKioskMode()
        );

    m_pMyPinDetailsViewModule = Eegeo_NEW(ExampleApp::MyPinDetails::View::MyPinDetailsViewModule)(
        m_nativeState,
        app.MyPinDetailsModule().GetMyPinDetailsViewModel(),
        m_messageBus
        );

    // Initial UX layer
    m_pInitialExperienceIntroViewModule = Eegeo_NEW(ExampleApp::InitialExperience::View::InitialExperienceIntroViewModule)(
        m_nativeState,
        m_messageBus,
        app.GetApplicationConfiguration().IsInKioskMode(),
        app.CameraTransitionController()
        );

    m_pInteriorsExplorerViewModule = Eegeo_NEW(ExampleApp::InteriorsExplorer::View::InteriorsExplorerViewModule)(
        app.InteriorsExplorerModule().GetInteriorsExplorerModel(),
        app.InteriorsExplorerModule().GetInteriorsExplorerViewModel(),
        m_messageBus,
        app.GetNavigationService());

    m_pOptionsViewModule = Eegeo_NEW(ExampleApp::Options::View::OptionsViewModule)(
        m_nativeState,
        app.OptionsModule().GetOptionsViewModel(),
        m_pWindowsPlatformAbstractionModule->GetWindowsHttpCache(),
        m_messageBus,
        m_pInteriorsExplorerViewModule->GetController(),
        m_pInitialExperienceIntroViewModule->GetController(),
        app.GetApplicationConfiguration().OptionsAdminPassword(),
        app.GetApplicationConfiguration().IsInKioskMode());

    m_pViewControllerUpdaterModule = Eegeo_NEW(ExampleApp::ViewControllerUpdater::View::ViewControllerUpdaterModule);

    if (IsInKioskMode())
    {
        m_pVirtualKeyboardView = Eegeo_NEW(ExampleApp::VirtualKeyboard::View::VirtualKeyboardView)(m_nativeState, m_messageBus);
        m_pVirtualKeyboardView->AddCustomKeyboardLayout(m_pCustomKeyboardLayout);
    }

    if (m_pApp->GetApplicationConfiguration().IsAttractModeEnabled())
    {
        m_pAttractModeOverlayView = Eegeo_NEW(ExampleApp::AttractModeOverlay::View::AttractModeOverlayView)(m_nativeState,
                                                                                                            m_pVirtualKeyboardView,
                                                                                                            app.MyPinCreationDetailsModule().GetMyPinCreationDetailsViewModel(),
                                                                                                            m_messageBus);
    }

    ExampleApp::ViewControllerUpdater::View::IViewControllerUpdaterModel& viewControllerUpdaterModel = m_pViewControllerUpdaterModule->GetViewControllerUpdaterModel();

    viewControllerUpdaterModel.AddUpdateableObject(m_pSettingsMenuViewModule->GetMenuController());
    viewControllerUpdaterModel.AddUpdateableObject(m_pSearchMenuViewModule->GetMenuController());

    Eegeo::Helpers::TIdentity settingsMenuIdentity = app.SearchResultPoiModule().GetObservableOpenableControl().GetIdentity();
    Eegeo::Helpers::TIdentity searchMenuIdentity= app.SearchMenuModule().GetSearchMenuViewModel().GetIdentity();
    
    app.ModalityIgnoredReactionModel().AddIgnoredMenuIdentity(settingsMenuIdentity);
    app.ReactorIgnoredReactionModel().AddIgnoredMenuIdentity(settingsMenuIdentity);
    
    app.ModalityIgnoredReactionModel().AddIgnoredMenuIdentity(searchMenuIdentity);
    app.ReactorIgnoredReactionModel().AddIgnoredMenuIdentity(searchMenuIdentity);
}

void AppHost::DestroyApplicationViewModulesFromUiThread()
{
    ASSERT_UI_THREAD

        if (m_createdUIModules)
        {
            Eegeo_DELETE m_pAttractModeOverlayView;

            Eegeo_DELETE m_pVirtualKeyboardView;

            Eegeo_DELETE m_pMyPinDetailsViewModule;

            Eegeo_DELETE m_pViewControllerUpdaterModule;

            Eegeo_DELETE m_pMyPinCreationDetailsViewModule;

            Eegeo_DELETE m_pFlattenButtonViewModule;

            Eegeo_DELETE m_pMyPinCreationViewModule;

            Eegeo_DELETE m_pAboutPageViewModule;

            Eegeo_DELETE m_pSearchResultPoiViewModule;

            Eegeo_DELETE m_pModalBackgroundViewModule;

            Eegeo_DELETE m_pSearchResultSectionViewModule;

            Eegeo_DELETE m_pTagSearchViewModule;

            Eegeo_DELETE m_pSearchMenuViewModule;

            Eegeo_DELETE m_pSettingsMenuViewModule;

            Eegeo_DELETE m_pCompassViewModule;

            Eegeo_DELETE m_pInitialExperienceIntroViewModule;

            Eegeo_DELETE m_pOptionsViewModule;

            Eegeo_DELETE m_pInteriorsExplorerViewModule;

            Eegeo_DELETE m_pWatermarkViewModule;

            if(m_pApp->GetApplicationConfiguration().IsInKioskMode())
            {
                Eegeo_DELETE m_pGpsMarkerTutorialViewModule;
            }
        }
    m_createdUIModules = false;
}

void AppHost::HandleFailureToProvideWorkingApiKey()
{
    m_WindowsAlertBoxFactory.CreateSingleOptionAlertBox
        (
            "Bad API Key",
            "You must provide a valid API key to the constructor of EegeoWorld. See the readme file for details.",
            m_failAlertHandler
            );
}

void AppHost::HandleFailureToDownloadBootstrapResources()
{
    std::string message =
        m_pNetworkCapabilities->StreamOverWifiOnly()
        ? "Unable to download required data! Please ensure you have a Wi-fi connection the next time you attempt to run this application."
        : "Unable to download required data! Please ensure you have an Internet connection the next time you attempt to run this application.";

    m_WindowsAlertBoxFactory.CreateSingleOptionAlertBox("Error", message, m_failAlertHandler);
}

void AppHost::HandleNoConnectivityWarning()
{
}

void AppHost::HandleInvalidConnectivityError()
{
    m_WindowsAlertBoxFactory.CreateSingleOptionAlertBox
    (
     "Network error",
     "Unable to access web reliably. Please check your connection is valid & authenticated.",
     m_failAlertHandler
     );
}

void AppHost::HandleStartupFailure()
{
    exit(1);
}
