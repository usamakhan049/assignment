// Copyright eeGeo Ltd (2012-2015), All Rights Reserved

#pragma once

#include "IScreenControlViewModel.h"

namespace ExampleApp
{
    namespace MyPinCreation
    {
        namespace View
        {
            class IMyPinCreationConfirmationViewModel : public ScreenControl::View::IScreenControlViewModel
            {
            public:

                virtual ~IMyPinCreationConfirmationViewModel() { }
                virtual bool TryOpen() = 0;
                virtual void Close() = 0;

                virtual void Open() = 0;
                virtual void InsertOpenCallback(Eegeo::Helpers::ICallback0& callback) = 0;
                virtual void RemoveOpenCallback(Eegeo::Helpers::ICallback0& callback) = 0;
            };
        }
    }
}
