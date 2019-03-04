// Copyright eeGeo Ltd (2012-2015), All Rights Reserved

#pragma once

#import <UIKit/UIKit.h>
#include "AppRunner.h"
#include "ModalBackgroundViewIncludes.h"

@class ModalBackgroundView;

@interface ModalBackgroundView : UIView<UIGestureRecognizerDelegate>
{
    ExampleApp::ModalBackground::View::ModalBackgroundViewInterop* m_pInterop;
    float m_stateChangeAnimationTimeSeconds;
    bool m_isAnimating;
    
    UITapGestureRecognizer* m_tapGestureRecogniser;

    bool m_isDismissed;
}

- (id)initWithParams:(float)screenWidth :(float)screenHeight;

- (ExampleApp::ModalBackground::View::ModalBackgroundViewInterop*) getInterop;

- (void) setIsDismissed;

- (void) setFullyActive;

- (void) setFullyInactive;

- (void) setActiveStateToIntermediateValue:(float)activeState;

@end
