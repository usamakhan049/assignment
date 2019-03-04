// Copyright eeGeo Ltd (2012-2015), All Rights Reserved

#pragma once

#include <string>
#import <UIKit/UIKit.h>
#include "WatermarkViewIncludes.h"
#include "IAppModeModel.h"

@class WatermarkView;

@interface WatermarkView : UIView
{
    ExampleApp::Watermark::View::WatermarkViewInterop* m_pInterop;
    UIViewController* m_pController;
    
    float m_width;
    float m_height;
    
    float m_screenWidth;
    float m_screenHeight;
    float m_pixelScale;
    
    float m_yPosActive;
    float m_yPosInactive;
    
    float m_stateChangeAnimationTimeSeconds;
    
    std::string m_imageAssetName;
    std::string m_popupTitle;
    std::string m_popupBody;
    std::string m_webUrl;
    bool m_shouldShowShadow;
    
    bool m_alignAlongBottom;
    bool m_alignBelowFloorDisplay;
}

- (ExampleApp::Watermark::View::WatermarkViewInterop*) getInterop;

- (id) initWithScreenDimensions:(float)width
                               :(float)height
                               :(float)pixelScale
                               :(const ExampleApp::Watermark::View::WatermarkData&) watermarkData;

- (void) setFullyOnScreen;

- (void) setFullyOffScreen;

- (void) setOnScreenStateToIntermediateValue:(float)openState;

- (void) animateToY:(float)y;

- (void) updateWatermarkData: (const ExampleApp::Watermark::View::WatermarkData&) watermarkData;

- (void) setWatermarkAlignmentState: (bool) alignAlongBottom
                                   : (bool) alignBelowFloorDisplay;

- (void) setInteriorStylingState: (bool) shouldUseInteriorStyle;

@property(nonatomic, retain) UIButton* pButton;

@end
