// Copyright eeGeo Ltd (2012-2016), All Rights Reserved

#include "DeepLinkModule.h"
#include "DeepLinkModel.h"
#include "DeepLinkController.h"
#include "ICameraTransitionController.h"
#include "IAlertBoxFactory.h"
#include "IWebLoadRequestFactory.h"
#include "DeepLinkLocationHandler.h"
#include "CoverageTrees.h"
#include "CityThemes.h"
#include "SelectFirstResultSearchService.h"

namespace {
    const char * const LOCATION_PATH = "location";
    const char * const SEARCH_PATH = "search";
    const char * const MYMAP_PATH = "mapscene";
    const bool CONFIG_DEEP_LINK_ENABLED = true;
}

namespace ExampleApp
{
    namespace DeepLink
    {
        namespace SdkModel
        {
            ExampleApp::DeepLink::SdkModel::DeepLinkModule::DeepLinkModule(CameraTransitions::SdkModel::ICameraTransitionController& cameraTransitionController,
                                                                           Eegeo::UI::NativeAlerts::IAlertBoxFactory& alertBoxFactory,
                                                                           Search::SelectFirstResult::SdkModel::SelectFirstResultSearchService& selectFirstResultSearchService,
                                                                           Mapscene::SdkModel::MapsceneLoader& mapsceneLoader)
            {
                m_pDeepLinkModel = Eegeo_NEW(DeepLinkModel)();
                m_pDeepLinkLocationHandler = Eegeo_NEW(DeepLinkLocationHandler)(cameraTransitionController, alertBoxFactory);
                m_pDeepLinkModel->AddRoute(LOCATION_PATH, m_pDeepLinkLocationHandler);

                if(CONFIG_DEEP_LINK_ENABLED)
                {
                    m_pDeepLinkConfigHandler = Eegeo_NEW(DeepLinkConfigHandler)(mapsceneLoader);
                    m_pDeepLinkModel->AddRoute(MYMAP_PATH, m_pDeepLinkConfigHandler);
                }
                m_pDeepLinkSearchHandler = Eegeo_NEW(DeepLinkSearchHandler)(selectFirstResultSearchService, alertBoxFactory, mapsceneLoader, cameraTransitionController);
                
                m_pDeepLinkModel->AddRoute(SEARCH_PATH, m_pDeepLinkSearchHandler);

                m_pDeepLinkController = Eegeo_NEW(DeepLinkController)(*m_pDeepLinkModel);
            }

            ExampleApp::DeepLink::SdkModel::DeepLinkModule::~DeepLinkModule()
            {
                if(m_pDeepLinkController != nullptr)
                {
                    Eegeo_DELETE(m_pDeepLinkController);
                }
                if(m_pDeepLinkSearchHandler != nullptr)
                {
                    Eegeo_DELETE(m_pDeepLinkSearchHandler);
                }
                if(m_pDeepLinkConfigHandler != nullptr)
                {
                    Eegeo_DELETE(m_pDeepLinkConfigHandler);
                }
                if(m_pDeepLinkLocationHandler != nullptr)
                {
                    Eegeo_DELETE(m_pDeepLinkLocationHandler);
                }
                if(m_pDeepLinkModel != nullptr)
                {
                    Eegeo_DELETE(m_pDeepLinkModel);
                }
            }

            ExampleApp::DeepLink::SdkModel::DeepLinkController& ExampleApp::DeepLink::SdkModel::DeepLinkModule::GetDeepLinkController()
            {
                return *m_pDeepLinkController;
            }
        }
    }
}
