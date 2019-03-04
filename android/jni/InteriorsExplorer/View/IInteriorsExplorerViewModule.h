// Copyright eeGeo Ltd (2012-2015), All Rights Reserved

#pragma once

#include "Types.h"
#include "InteriorsExplorerView.h"


namespace ExampleApp
{
    namespace InteriorsExplorer
    {
        namespace View
        {
            class IInteriorsExplorerViewModule
            {
            public:
                virtual ~IInteriorsExplorerViewModule() {}
                
                virtual InteriorsExplorerController& GetController() const = 0;
                
                virtual InteriorsExplorerView& GetView() const = 0;
            };
        }
    }
}
