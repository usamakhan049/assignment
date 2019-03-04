// Copyright eeGeo Ltd (2012-2015), All Rights Reserved

#pragma once

#include "IWatermarkView.h"
#include "Watermark.h"
#include "WatermarkView.h"
#include "CallbackCollection.h"
#include "IScreenControlView.h"

namespace ExampleApp
{
    namespace Watermark
    {
        namespace View
        {
            class WatermarkViewInterop : public IWatermarkView
            {
            private:
                WatermarkView* m_pView;
                Eegeo::Helpers::CallbackCollection0 m_callbacks;

            public:
                WatermarkViewInterop(WatermarkView* pView)
                    : m_pView(pView)
                {
                }

                void SetOnScreenStateToIntermediateValue(float value);

                void SetOnScreen();

                void SetOffScreen();
                
                void OnSelected();

                void InsertSelectedCallback(Eegeo::Helpers::ICallback0& callback);

                void RemoveSelectedCallback(Eegeo::Helpers::ICallback0& callback);
                
                void UpdateWatermarkData(const WatermarkData& watermarkData);
                
                void  SetWatermarkAlignmentState(bool alignAlongBottom, bool alignBelowFloorDisplay);
                
                void SetInteriorStylingState(bool shouldUseInteriorStyle);
            };
        }
    }
}
