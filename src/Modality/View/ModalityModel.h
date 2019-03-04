// Copyright eeGeo Ltd (2012-2015), All Rights Reserved

#pragma once

#include <vector>
#include "Types.h"
#include "IModalityModel.h"
#include "IMenuViewModel.h"
#include "IOpenableControlViewModel.h"
#include "CallbackCollection.h"

namespace ExampleApp
{
    namespace Modality
    {
        namespace View
        {
            class ModalityModel : public IModalityModel, private Eegeo::NonCopyable
            {
                ModalityValue m_modality;
                Eegeo::Helpers::CallbackCollection0 m_modalityChangedCallbacks;

            public:
                ModalityModel();

                ~ModalityModel();

                ModalityValue GetModality() const;

                void SetModality(ModalityValue modality);

                void InsertModalityChangedCallback(Eegeo::Helpers::ICallback0& modalityChangedCallback);

                void RemoveModalityChangedCallback(Eegeo::Helpers::ICallback0& modalityChangedCallback);
            };
        }
    }
}
