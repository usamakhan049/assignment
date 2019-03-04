// Copyright eeGeo Ltd (2012-2015), All Rights Reserved

#include <string>
#include <algorithm>
#include "SearchResultPoiView.h"
#include "MathFunc.h"
#include "UIColors.h"
#include "ImageHelpers.h"
#include "IconResources.h"
#include "SizeHelpers.h"
#include "StringHelpers.h"
#include "ExampleTourSearchResultPoiView.h"
#include "UIHelpers.h"
#include "ExampleSearchJsonParser.h"

#import "UIButton+DefaultStates.h"
#import "UIView+TouchExclusivity.h"

@interface ExampleTourSearchResultPoiView()<UIGestureRecognizerDelegate>
{
}
@end

@implementation ExampleTourSearchResultPoiView

- (id)initWithInterop:(ExampleApp::SearchResultPoi::View::SearchResultPoiViewInterop*)pInterop
{
    self = [super init];
    
    if(self)
    {
        m_pInterop = pInterop;
        self.alpha = 0.f;
        m_stateChangeAnimationTimeSeconds = 0.2f;
        
        self.pControlContainer = [[[UIView alloc] initWithFrame:CGRectMake(0, 0, 0, 0)] autorelease];
        self.pControlContainer.backgroundColor = ExampleApp::Helpers::ColorPalette::UiBackgroundColor;
        [self addSubview: self.pControlContainer];
        
        self.pCloseButtonContainer = [[[UIView alloc] initWithFrame:CGRectMake(0, 0, 0, 0)] autorelease];
        self.pCloseButtonContainer.backgroundColor = ExampleApp::Helpers::ColorPalette::UiBorderColor;
        [self.pControlContainer addSubview: self.pCloseButtonContainer];
        
        self.pCloseButton = [[[UIButton alloc] initWithFrame:CGRectMake(0, 0, 0, 0)] autorelease];
        [self.pCloseButton setDefaultStatesWithImageNames:@"button_close_off" :@"button_close_on"];
        [self.pCloseButton addTarget:self action:@selector(handleClosedButtonSelected) forControlEvents:UIControlEventTouchUpInside];
        [self.pCloseButtonContainer addSubview: self.pCloseButton];
        
        self.pContentContainer = [[[UIView alloc] initWithFrame:CGRectMake(0, 0, 0, 0)] autorelease];
        self.pContentContainer.backgroundColor = ExampleApp::Helpers::ColorPalette::UiBackgroundColor;
        [self.pControlContainer addSubview: self.pContentContainer];
        
        self.pLabelsContainer = [[[UIScrollView alloc] initWithFrame:CGRectMake(0, 0, 0, 0)] autorelease];
        self.pLabelsContainer.backgroundColor = ExampleApp::Helpers::ColorPalette::UiBackgroundColor;
        [self.pContentContainer addSubview: self.pLabelsContainer];
        
        self.pHeadlineContainer = [[[UIView alloc] initWithFrame:CGRectMake(0, 0, 0, 0)] autorelease];
        self.pHeadlineContainer.backgroundColor = ExampleApp::Helpers::ColorPalette::UiBackgroundColor;
        [self.pControlContainer addSubview: self.pHeadlineContainer];
        
        self.pTagIconContainer = [[[UIView alloc] initWithFrame:CGRectMake(0, 0, 0, 0)] autorelease];
        [self.pHeadlineContainer addSubview: self.pTagIconContainer];
        
        self.pTitleLabel = [self createLabel :ExampleApp::Helpers::ColorPalette::UiTextTitleColor :ExampleApp::Helpers::ColorPalette::UiBackgroundColor];
        [self.pHeadlineContainer addSubview: self.pTitleLabel];
        
        self.pTourInfoHeaderContainer = [[[UIView alloc] initWithFrame:CGRectMake(0, 0, 0, 0)] autorelease];
        self.pTourInfoHeaderContainer.backgroundColor = ExampleApp::Helpers::ColorPalette::UiBorderColor;
        [self.pLabelsContainer addSubview: self.pTourInfoHeaderContainer];
        
        self.pTourInfoHeaderLabel = [self createLabel :ExampleApp::Helpers::ColorPalette::UiTextHeaderColor :ExampleApp::Helpers::ColorPalette::UiBorderColor];
        [self.pTourInfoHeaderContainer addSubview: self.pTourInfoHeaderLabel];
        
        self.pTourInfoContent = [self createLabel :ExampleApp::Helpers::ColorPalette::UiTextCopyColor :ExampleApp::Helpers::ColorPalette::UiBackgroundColor];
        [self.pLabelsContainer addSubview: self.pTourInfoContent];
        
        self.pPreviewImage = [[[UIImageView alloc] initWithFrame:CGRectMake(0, 0, 0, 0)] autorelease];
        [self.pLabelsContainer addSubview: self.pPreviewImage];
        
        [self setTouchExclusivity: self];
        
        self.alpha = 0.f;
    }
    
    return self;
}

- (void)dealloc
{
    [self.pPreviewImage removeFromSuperview];
    [self.pPreviewImage release];
    
    [self.pCloseButton removeFromSuperview];
    [self.pCloseButton release];
    
    [self.pCloseButtonContainer removeFromSuperview];
    [self.pCloseButtonContainer release];
    
    [self.pControlContainer removeFromSuperview];
    [self.pControlContainer release];
    
    [self.pHeadlineContainer removeFromSuperview];
    [self.pHeadlineContainer release];
    
    [self.pLabelsContainer removeFromSuperview];
    [self.pLabelsContainer release];
    
    [self.pContentContainer removeFromSuperview];
    [self.pContentContainer release];
    
    [self.pTagIconContainer removeFromSuperview];
    [self.pTagIconContainer release];
    
    [self.pTitleLabel removeFromSuperview];
    [self.pTitleLabel release];
    
    [self.pTourInfoHeaderLabel removeFromSuperview];
    [self.pTourInfoHeaderLabel release];
    
    [self.pTourInfoHeaderContainer removeFromSuperview];
    [self.pTourInfoHeaderContainer release];
    
    [self.pTourInfoContent removeFromSuperview];
    [self.pTourInfoContent release];
    
    [self removeFromSuperview];
    [super dealloc];
}

- (void) layoutSubviews
{
    const float boundsWidth = static_cast<float>(self.superview.bounds.size.width);
    const float boundsHeight = static_cast<float>(self.superview.bounds.size.height);
    const float boundsOccupyMultiplier = 0.9f;
    const float mainWindowWidth = ExampleApp::Helpers::UIHelpers::UsePhoneLayout() ? std::min(boundsWidth, 348.f) : 450.f;
    const float mainWindowHeight = (boundsHeight * boundsOccupyMultiplier);
    const float mainWindowX = (boundsWidth * 0.5f) - (mainWindowWidth * 0.5f);
    const float mainWindowY = ((boundsHeight * 0.5f) - (mainWindowHeight * 0.5f));
    
    self.frame = CGRectMake(mainWindowX,
                            mainWindowY,
                            mainWindowWidth,
                            mainWindowHeight);
    
    self.pControlContainer.frame = CGRectMake(0.f,
                                              0.f,
                                              mainWindowWidth,
                                              mainWindowHeight);
    
    const float headlineHeight = 50.f;
    const float closeButtonSectionHeight = 64.f;
    const float closeButtonSectionOffsetY = mainWindowHeight - closeButtonSectionHeight;
    const float contentSectionHeight = mainWindowHeight - (closeButtonSectionHeight + headlineHeight);
    
    self.pHeadlineContainer.frame = CGRectMake(0.f,
                                               0.f,
                                               mainWindowWidth,
                                               headlineHeight);
    
    
    self.pContentContainer.frame = CGRectMake(0.f,
                                              headlineHeight,
                                              mainWindowWidth,
                                              contentSectionHeight);
    
    const float labelsSectionOffsetX = 0.f;
    m_labelsSectionWidth = mainWindowWidth - (2.f * labelsSectionOffsetX);
    
    self.pLabelsContainer.frame = CGRectMake(labelsSectionOffsetX,
                                             0.f,
                                             m_labelsSectionWidth,
                                             contentSectionHeight);
    
    
    self.pCloseButtonContainer.frame = CGRectMake(0.f,
                                                  closeButtonSectionOffsetY,
                                                  mainWindowWidth,
                                                  closeButtonSectionHeight);
    
    self.pCloseButton.frame = CGRectMake(mainWindowWidth - closeButtonSectionHeight,
                                         0.f,
                                         closeButtonSectionHeight,
                                         closeButtonSectionHeight);
    
    
    self.pTagIconContainer.frame = CGRectMake(0.f, 0.f, headlineHeight, headlineHeight);
    const float titlePadding = 10.0f;
    self.pTitleLabel.frame = CGRectMake(headlineHeight + titlePadding,
                                        0.f,
                                        mainWindowWidth - (headlineHeight + titlePadding),
                                        headlineHeight);
    self.pTitleLabel.font = [UIFont systemFontOfSize:24.0f];
}

- (void) performDynamicContentLayout:(CGFloat)imageWidth :(CGFloat)imageHeight
{
    const float headerLabelHeight = 20.f;
    const float labelYSpacing = 8.f;
    const float headerTextPadding = 3.0f;
    
    float currentLabelY = 0.f;
    
    std::string imageUrl = "";
    
    if(ExampleApp::Search::Example::SdkModel::TryParseImageDetails(m_model, imageUrl))
    {
        UIImage *image = [UIImage imageNamed: [NSString stringWithUTF8String:imageUrl.c_str()]];
        [self.pPreviewImage setImage:image];
        
        Eegeo::v2 maxImageSize;
        maxImageSize.x = self.frame.size.width;
        maxImageSize.y = 0.7*self.frame.size.height;
        Eegeo::v2 imageSize(image.size.width, image.size.height);
        Eegeo::v2 fitMaintainSize = ExampleApp::Helpers::SizeHelpers::CalculateFitMaintain(maxImageSize, imageSize);
        CGSize fitMaintainImageSize;
        fitMaintainImageSize.width = fitMaintainSize.x;
        fitMaintainImageSize.height = fitMaintainSize.y;
        
        CGRect frame = self.pPreviewImage.frame;
        const CGFloat initialFrameHeight = frame.size.height;
        frame.size = fitMaintainImageSize;
        frame.origin.x = self.frame.size.width * 0.5f - frame.size.width * 0.5f;
        self.pPreviewImage.frame = frame;
        self.pPreviewImage.hidden = false;
        
        const CGFloat imageContentHeightDifference = (fitMaintainImageSize.height - initialFrameHeight);
        const CGFloat newContentHeight = self.pLabelsContainer.contentSize.height + imageContentHeightDifference;
        [self.pLabelsContainer setContentSize:CGSizeMake(self.pLabelsContainer.contentSize.width, newContentHeight)];
        
        currentLabelY += (fitMaintainImageSize.height);
    }
    
    currentLabelY += 8.f;
    
    if(!m_model.GetSubtitle().empty())
    {
        self.pTourInfoHeaderContainer.frame = CGRectMake(0.f, currentLabelY, m_labelsSectionWidth, headerLabelHeight + 2 * headerTextPadding);
        self.pTourInfoHeaderContainer.hidden = false;
        
        self.pTourInfoHeaderLabel.frame = CGRectMake(headerTextPadding, headerTextPadding, m_labelsSectionWidth - headerTextPadding, headerLabelHeight);
        self.pTourInfoHeaderLabel.text = @"Tour Info";
        self.pTourInfoHeaderLabel.hidden = false;
        currentLabelY += labelYSpacing + self.pTourInfoHeaderLabel.frame.size.height;
        
        self.pTourInfoContent.frame = CGRectMake(headerTextPadding, currentLabelY, m_labelsSectionWidth - headerTextPadding, 85.f);
        self.pTourInfoContent.text = @"";
        self.pTourInfoContent.numberOfLines = 0;
        self.pTourInfoContent.adjustsFontSizeToFitWidth = NO;
        self.pTourInfoContent.lineBreakMode = NSLineBreakByTruncatingTail;
        
        std::string tourInfoText = m_model.GetSubtitle();
        self.pTourInfoContent.text = [NSString stringWithUTF8String:tourInfoText.c_str()];
        self.pTourInfoContent.hidden = false;
        [self.pTourInfoContent sizeToFit];
        
        currentLabelY += labelYSpacing + self.pTourInfoContent.frame.size.height;
    }
    
    [self.pLabelsContainer setContentSize:CGSizeMake(m_labelsSectionWidth, currentLabelY)];
}

- (void) setContent:(const ExampleApp::Search::SdkModel::SearchResultModel*)pModel :(bool)isPinned
{
    Eegeo_ASSERT(pModel != NULL);
    
    m_model = *pModel;
    m_isPinned = isPinned;
    
    self.pTitleLabel.text = [NSString stringWithUTF8String:pModel->GetTitle().c_str()];
    
    [self.pTagIconContainer.subviews makeObjectsPerformSelector: @selector(removeFromSuperview)];
    std::string tagIcon = ExampleApp::Helpers::IconResources::GetSmallIconForTag(pModel->GetIconKey());
    ExampleApp::Helpers::ImageHelpers::AddPngImageToParentView(self.pTagIconContainer, tagIcon, ExampleApp::Helpers::ImageHelpers::Centre);
    
    self.pTourInfoHeaderContainer.hidden = true;
    self.pTourInfoContent.hidden = true;
    
    const CGFloat previewImagePlaceholderSize = 64.f;
    [self performDynamicContentLayout :previewImagePlaceholderSize :previewImagePlaceholderSize];
    
    [self.pLabelsContainer setContentOffset:CGPointMake(0,0) animated:NO];
}

- (void) updateImage:(const std::string&)url :(bool)success bytes:(const std::vector<Byte>*)bytes;
{
    
}

- (void) setFullyActive
{
    if(self.alpha == 1.f)
    {
        return;
    }
    
    [self animateToAlpha:1.f];
}

- (void) setFullyInactive
{
    if(self.alpha == 0.f)
    {
        return;
    }
    
    [self animateToAlpha:0.f];
}

- (void) setActiveStateToIntermediateValue:(float)openState
{
    self.alpha = openState;
}

- (ExampleApp::SearchResultPoi::View::SearchResultPoiViewInterop*)getInterop
{
    return m_pInterop;
}

- (BOOL)consumesTouch:(UITouch *)touch
{
    return self.alpha > 0.f;
}

- (void) animateToAlpha:(float)alpha
{
    [UIView animateWithDuration:m_stateChangeAnimationTimeSeconds
                     animations:^
     {
         self.alpha = alpha;
     }];
}

- (UILabel*) createLabel:(UIColor*)textColor :(UIColor*)backgroundColor
{
    UILabel* pLabel = [[[UILabel alloc] initWithFrame:CGRectMake(0, 0, 0, 0)] autorelease];
    pLabel.textColor = textColor;
    pLabel.backgroundColor = backgroundColor;
    pLabel.textAlignment = NSTextAlignmentLeft;
    return pLabel;
}

- (void) handleClosedButtonSelected
{
    m_pInterop->HandleCloseClicked();
}



@end
