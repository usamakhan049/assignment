// Copyright eeGeo Ltd (2012-2015), All Rights Reserved

#pragma once

#include <string>

namespace ExampleApp
{
    namespace NavRouting
    {
        class NavRoutingShowRerouteDialogMessage
        {
        private:
            std::string m_message;

        public:
            NavRoutingShowRerouteDialogMessage(const std::string& message);

            const std::string& GetMessage() const;
        };
    }
}
