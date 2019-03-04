// Copyright eeGeo Ltd (2012-2015), All Rights Reserved

#pragma once

#include <string>
#include <map>
#include "ApplicationConfig.h"
#include "LatLongAltitude.h"
#include "ApplicationInteriorTrackingInfo.h"
#include "ApplicationFixedIndoorLocation.h"
#include "ApplicationMenuItemTagSearchConfig.h"
#include "document.h"

namespace ExampleApp
{
    namespace ApplicationConfig
    {
        class ApplicationConfiguration
        {
            std::string m_name;
            std::string m_eegeoApiKey;
            std::string m_productVersion;
            std::string m_buildNumber;
            std::string m_combinedVersionString;
            std::string m_coverageTreeManifestURL;
            std::string m_themeManifestURL;
            std::string m_embeddedThemeTexturePath;

            Eegeo::Space::LatLongAltitude m_interestLocation;
            float m_distanceToInterestMetres;
            float m_orientationDegrees;
            std::string m_indoorId;
            int m_floorIndex;
            bool m_tryStartAtGpsLocation;
            bool m_shouldStartFullscreen;
            bool m_shouldPerformStartUpSearch;
            std::string m_startUpSearchTag;
            std::string m_googleAnalyticsReferrerToken;
            std::string m_yelpApiKey;
            std::string m_flurryAppKey;
            std::string m_geoNamesUserName;
            std::string m_eegeoSearchServiceUrl;
            std::string m_myPinsWebServiceUrl;
            std::string m_myPinsWebServiceAuthToken;
            std::string m_myPinsPoiSetId;
            std::string m_javascriptWhitelistUrl;
            bool m_isKioskTouchInputEnabled;
            bool m_isInKioskMode;
            bool m_useJapaneseFont;
            bool m_useChineseFont;

            SdkModel::ApplicationFixedIndoorLocation m_fixedIndoorLocation;
            
            std::map<std::string, SdkModel::ApplicationInteriorTrackingInfo> m_interiorTrackingInfo;

            std::vector<Eegeo::Space::LatLongAltitude> m_attractModeTargetSplinePoints;
            std::vector<Eegeo::Space::LatLongAltitude> m_attractModePositionSplinePoints;

            long long m_attractModeTimeoutMs;
            float m_attractModePlaybackSpeed;

            std::string m_optionsAdminPassword;

			float m_compassCameraOffset;
			float m_compassCameraOffsetTopDown;
			bool m_compassCameraDampingEnabled;
            
            long long m_surveyRequirementTimeSec;
            std::string m_timerSurveyUrl;
            
            std::string m_hockeyAppId;

            bool m_hasMapScene;
            std::string m_mapSceneId;

			std::vector<std::vector<std::string>> m_customKeyboardLayout;
			std::vector<SdkModel::ApplicationMenuItemTagSearchConfig> m_outdoorSearchMenuItems;
            bool m_overrideIndoorSearchMenuItems;

			bool m_navigationEnabled;

        public:
            
			ApplicationConfiguration(
				const std::string& name,
				const std::string& eegeoApiKey,
				const std::string& productVersion,
				const std::string& buildNumber,
				const std::string& combinedVersionString,
				const std::string& coverageTreeManifestURL,
				const std::string& themeManifestURL,
				const std::string& embeddedThemeTexturePath,
				const Eegeo::Space::LatLongAltitude& interestLocation,
				float distanceToInterestMetres,
				float orientationDegrees,
                const std::string& indoorId,
                const int floorIndex,
				bool tryStartAtGpsLocation,
				bool startFullscreen,
				bool performStartUpSearch,
				const std::string& startUpSearchTag,
				const std::string& googleAnalyticsReferrerToken,
				const std::string& yelpApiKey,
				const std::string& flurryAppKey,
				const std::string& geoNamesUserName,
				const std::string& eegeoSearchServiceUrl,
				const std::string& myPinsWebServiceUrl,
				const std::string& myPinsWebServiceAuthToken,
				const std::string& myPinsPoiSetId,
                const std::string& javascriptWhitelistUrl,
                const bool isKioskTouchInputEnabled,
				const bool isInKioskMode,
				bool useJapaneseFont,
                bool useChineseFont,
				const std::map<std::string, SdkModel::ApplicationInteriorTrackingInfo>& interiorTrackingInfo,
				const SdkModel::ApplicationFixedIndoorLocation& fixedIndoorLocation,
				const std::vector<Eegeo::Space::LatLongAltitude>& attractModeTargetSplinePoints,
				const std::vector<Eegeo::Space::LatLongAltitude>& attractModePositionSplinePoints,
				const long long attractModeTimeoutMs,
				const float attractModePlaybackSpeed,
				const std::string& optionsAdminPassword,
				const float compassCameraOffset,
				const float compassCameraOffsetTopDown,
				const bool compassCameraDampingEnabled,
				const long long& surveyRequirementTimeSec,
				const std::string& timerSurveyUrl,
				const std::string& hockeyAppId,
				bool hasMapScene,
				const std::string& mapSceneId,
				const std::vector<std::vector<std::string>>& customKeyboardLayout,
                const std::vector<SdkModel::ApplicationMenuItemTagSearchConfig>& outdoorSearchMenuItems,
                bool overrideIndoorSearchMenuItems,
				bool navigationEnabled
            );
            
            std::string Name() const;

            std::string EegeoApiKey() const;

            std::string ProductVersion() const;

            std::string BuildNumber() const;

            std::string CombinedVersionString() const;

            std::string ThemeManifestURL() const;

            std::string CoverageTreeManifestURL() const;

            std::string EmbeddedThemeTexturePath() const;
            
            Eegeo::Space::LatLongAltitude InterestLocation() const;
            
            float DistanceToInterestMetres() const;
            
            float OrientationDegrees() const;
            
            std::string IndoorId() const;
            
            int FloorIndex() const;
            
            bool TryStartAtGpsLocation() const;

            bool ShouldStartFullscreen() const;
            
            bool ShouldPerformStartUpSearch() const;
            
            std::string StartUpSearchTag() const;
            
            std::string GoogleAnalyticsReferrerToken() const;

            std::string FlurryAppKey() const;

            std::string YelpApiKey() const;

            std::string GeoNamesUserName() const;

            std::string EegeoSearchServiceUrl() const;

            std::string MyPinsWebServiceUrl() const;

            std::string MyPinsWebServiceAuthToken() const;

            std::string MyPinsPoiSetId() const;
            
            std::string JavascriptWhitelistUrl() const;

            bool IsKioskTouchInputEnabled() const;

            bool IsInKioskMode() const;
            
            bool UseJapaneseFont() const;

            bool UseChineseFont() const;
            
            const std::map<std::string, SdkModel::ApplicationInteriorTrackingInfo>& InteriorTrackingInfo() const;

            bool IsFixedIndoorLocationEnabled() const;
            const SdkModel::ApplicationFixedIndoorLocation& FixedIndoorLocation() const;

            const std::vector<Eegeo::Space::LatLongAltitude>& AttractModeTargetSplinePoints() const;
            const std::vector<Eegeo::Space::LatLongAltitude>& AttractModePositionSplinePoints() const;
            const long long AttractModeTimeoutMs() const;
            const float AttractModePlaybackSpeed() const;
            const bool IsAttractModeEnabled() const;

            std::string OptionsAdminPassword() const;

			const float CompassCameraOffset() const;
			const float CompassCameraOffsetTopDown() const;
			const bool CompassCameraDampingEnabled() const;
            
            const long long SurveyRequirementTimeSec() const;
            const std::string TimerSurveyUrl() const;
            
            const std::string HockeyAppId() const;

            bool HasMapScene() const;
            const std::string MapSceneId() const;

			const std::vector<std::vector<std::string>> CustomKeyboardLayout() const;
            const std::vector<SdkModel::ApplicationMenuItemTagSearchConfig> OutdoorSearchMenuItems() const;
            bool OverrideIndoorSearchMenuItems() const;

			bool NavigationEnabled() const;
        };
    }
}
