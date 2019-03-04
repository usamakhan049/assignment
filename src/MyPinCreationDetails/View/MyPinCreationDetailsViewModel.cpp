// Copyright eeGeo Ltd (2012-2015), All Rights Reserved

#include "MyPinCreationDetailsViewModel.h"

namespace ExampleApp
{
    namespace MyPinCreationDetails
    {
        namespace View
        {
            MyPinCreationDetailsViewModel::MyPinCreationDetailsViewModel(Eegeo::Helpers::TIdentity identity)
                : m_openable(identity)
            {

            }

            MyPinCreationDetailsViewModel::~MyPinCreationDetailsViewModel()
            {

            }


            bool MyPinCreationDetailsViewModel::IsOpen() const
            {
                return m_openable.IsOpen();
            }

            void MyPinCreationDetailsViewModel::Open()
            {
                if(!IsOpen())
                {
                    m_openable.Open();
                    m_openedCallbacks.ExecuteCallbacks();
                }
            }

            void MyPinCreationDetailsViewModel::Close()
            {
                if(IsOpen())
                {
                    m_openable.Close();
                    m_closedCallbacks.ExecuteCallbacks();
                }
            }

            OpenableControl::View::IOpenableControlViewModel& MyPinCreationDetailsViewModel::GetOpenableControl()
            {
                return m_openable;
            }

            void MyPinCreationDetailsViewModel::InsertOpenedCallback(Eegeo::Helpers::ICallback0& openedCallback)
            {
                m_openedCallbacks.AddCallback(openedCallback);
            }

            void MyPinCreationDetailsViewModel::RemoveOpenedCallback(Eegeo::Helpers::ICallback0& openedCallback)
            {
                m_openedCallbacks.RemoveCallback(openedCallback);
            }

            void MyPinCreationDetailsViewModel::InsertClosedCallback(Eegeo::Helpers::ICallback0& closedCallback)
            {
                m_closedCallbacks.AddCallback(closedCallback);
            }

            void MyPinCreationDetailsViewModel::RemoveClosedCallback(Eegeo::Helpers::ICallback0& closedCallback)
            {
                m_closedCallbacks.RemoveCallback(closedCallback);
            }
        }
    }
}
