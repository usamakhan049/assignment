﻿using System;
using System.ComponentModel;
using System.Diagnostics;
using System.IO;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Controls.Primitives;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Media.Imaging;

namespace ExampleAppWPF
{
    public abstract class SearchResultPoiViewBase : Control, INotifyPropertyChanged
    {
        protected IntPtr m_nativeCallerPointer;
        protected MainWindow m_currentWindow;
        protected FrameworkElement m_mainContainer;
        protected Button m_closeButton;

        protected ScrollViewer m_contentContainer;

        protected Image m_headerFade;
        protected Image m_footerFade;

        protected Storyboard m_scrollFadeInAnim;
        protected Storyboard m_scrollFadeOutAnim;

        protected RepeatButton m_scrollDownButton;
        protected RepeatButton m_scrollUpButton;

        protected bool m_closing;
        protected bool m_isPinned;

        protected bool m_isOpen;

        private ControlClickHandler m_closeButtonClickHandler = null;

        private const double m_animationTimeMilliseconds = 250;

        public event PropertyChangedEventHandler PropertyChanged;

        private static bool m_isAnyPOIOpen = false;

        private double m_scrollSpeed;

        protected bool m_isInKioskMode;

        public bool IsPinned
        {
            get
            {
                return m_isPinned;
            }

            set
            {
                if (HandleTogglePinnedClicked(ref m_isPinned, !m_isPinned))
                {
                    OnPropertyChanged("IsPinned");
                }
            }
        }

        public static bool IsAnyPOIOpen()
        {
            return m_isAnyPOIOpen;
        }

        public SearchResultPoiViewBase(IntPtr nativeCallerPointer, bool isInKioskMode)
        {
            m_nativeCallerPointer = nativeCallerPointer;
            m_isInKioskMode = isInKioskMode;

            m_currentWindow = (MainWindow)Application.Current.MainWindow;
            m_currentWindow.MainGrid.Children.Add(this);

            Application.Current.MainWindow.SizeChanged += OnWindowResized;

            Visibility = Visibility.Hidden;

            m_isOpen = false;

            Panel.SetZIndex(this, 100);
        }

        public void Destroy()
        {
            m_mainContainer.MouseDown -= OnContainerMouseDown;

            m_closeButton.Click -= HandleCloseButtonClicked;

            TouchMove -= OnTouchMove;

            if (m_contentContainer != null)
            {
                m_contentContainer.ManipulationBoundaryFeedback -= OnBoundaryFeedback;
                m_contentContainer.ScrollChanged -= OnSearchResultsScrolled;
            }

            if (m_scrollUpButton != null)
            {
                m_scrollUpButton.Click -= HandleScrollUpButtonClicked;
            }

            if (m_scrollDownButton != null)
            {
                m_scrollDownButton.Click -= HandleScrollDownButtonClicked;
            }
        }

        private void OnWindowResized(object sender, SizeChangedEventArgs e)
        {
            var mainGrid = m_currentWindow.MainGrid;
            var screenWidth = mainGrid.ActualWidth;

            if (m_isOpen)
            {
                m_mainContainer.RenderTransform = new TranslateTransform((screenWidth / 2) - (m_mainContainer.ActualWidth / 2), 0);
            }
            else
            {
                m_mainContainer.RenderTransform = new TranslateTransform((screenWidth / 2) + (m_mainContainer.ActualWidth / 2), 0);
            }
        }

        protected virtual void OnPropertyChanged(string propertyName)
        {
            if (PropertyChanged != null)
                PropertyChanged(this, new PropertyChangedEventArgs(propertyName));
        }

        public override void OnApplyTemplate()
        {
            m_closeButton = GetTemplateChild("CloseButton") as Button;
            Debug.Assert(m_closeButton != null);

            m_mainContainer.MouseDown += OnContainerMouseDown;

            m_closeButton.Click += HandleCloseButtonClicked;

            TouchMove += OnTouchMove;
            TouchUp += OnTouchUp;

            m_contentContainer = (ScrollViewer)GetTemplateChild("ContentContainer");
            if (m_contentContainer != null)
            {
                m_contentContainer.ManipulationBoundaryFeedback += OnBoundaryFeedback;
                m_contentContainer.ScrollChanged += OnSearchResultsScrolled;
            }

            m_headerFade = (Image)GetTemplateChild("HeaderFade");
            m_footerFade = (Image)GetTemplateChild("FooterFade");

            m_scrollUpButton = (RepeatButton)GetTemplateChild("POIViewScrollUpButton");
            if (m_scrollUpButton != null)
            {
                m_scrollUpButton.Click += HandleScrollUpButtonClicked;
            }

            m_scrollDownButton = (RepeatButton)GetTemplateChild("POIViewScrollDownButton");
            if (m_scrollDownButton != null)
            {
                m_scrollDownButton.Click += HandleScrollDownButtonClicked;
            }

            Storyboard fadeInAnim = (Storyboard)Template.Resources["ScrollFadeIn"];
            if (fadeInAnim != null)
            {
                m_scrollFadeInAnim = fadeInAnim.Clone();
            }

            Storyboard fadeOutAnim = (Storyboard)Template.Resources["ScrollFadeOut"];
            if (fadeOutAnim != null)
            {
                m_scrollFadeOutAnim = fadeOutAnim.Clone();
            }

            m_scrollSpeed = (double)Application.Current.Resources["ScrollViewButtonScrollSpeed"];
        }

        protected virtual void OnSearchResultsScrolled(object sender, RoutedEventArgs e)
        {
            if (m_contentContainer != null
                && m_scrollFadeInAnim != null
                && m_scrollFadeOutAnim != null)
            {
                bool canScroll = m_contentContainer.ExtentHeight > m_contentContainer.ActualHeight;
                if (m_contentContainer.VerticalOffset == m_contentContainer.ScrollableHeight)
                {
                    if (canScroll && m_headerFade.Opacity < 1)
                    {
                        m_scrollFadeInAnim.Begin(m_headerFade);
                        m_scrollFadeInAnim.Begin(m_scrollUpButton);
                    }

                    if (m_footerFade.Opacity > 0)
                    {
                        m_scrollFadeOutAnim.Begin(m_footerFade);
                        m_scrollFadeOutAnim.Begin(m_scrollDownButton);
                    }
                }
                else if (m_contentContainer.VerticalOffset == 0)
                {
                    if (m_headerFade.Opacity > 0)
                    {
                        m_scrollFadeOutAnim.Begin(m_headerFade);
                        m_scrollFadeOutAnim.Begin(m_scrollUpButton);
                    }

                    if (canScroll && m_footerFade.Opacity < 1)
                    {
                        m_scrollFadeInAnim.Begin(m_footerFade);
                        m_scrollFadeInAnim.Begin(m_scrollDownButton);
                    }
                }
                else if (canScroll)
                {
                    if (m_headerFade.Opacity < 1)
                    {
                        m_scrollFadeInAnim.Begin(m_headerFade);
                        m_scrollFadeInAnim.Begin(m_scrollUpButton);
                    }

                    if (m_footerFade.Opacity < 1)
                    {
                        m_scrollFadeInAnim.Begin(m_footerFade);
                        m_scrollFadeInAnim.Begin(m_scrollDownButton);
                    }
                }
            }
        }

        private void OnTouchMove(object sender, TouchEventArgs e)
        {
            e.Handled = true;
        }

        private void OnTouchUp(object sender, TouchEventArgs e)
        {
            e.Handled = true;
        }

        private void OnContainerMouseDown(object sender, MouseButtonEventArgs e)
        {
            e.Handled = true;
        }

        protected void HideAll()
        {
            var mainGrid = m_currentWindow.MainGrid;
            var screenWidth = mainGrid.ActualWidth;

            var db = new DoubleAnimation((screenWidth / 2) + (m_mainContainer.ActualWidth / 2), TimeSpan.FromMilliseconds(m_animationTimeMilliseconds));
            db.From = (screenWidth / 2) - (m_mainContainer.ActualWidth / 2);
            db.Completed += OnHideAnimationCompleted;

            var easingFunction = new CubicEase();
            easingFunction.EasingMode = EasingMode.EaseInOut;
            db.EasingFunction = easingFunction;

            m_mainContainer.RenderTransform = new TranslateTransform();
            m_mainContainer.RenderTransform.BeginAnimation(TranslateTransform.XProperty, db);

            m_isOpen = false;
            m_isAnyPOIOpen = false;
        }

        protected void ShowAll()
        {
            if(m_isOpen)
            {
                return;
            }

            Visibility = Visibility.Visible;

            var mainGrid = m_currentWindow.MainGrid;
            var screenWidth = mainGrid.ActualWidth;

            var db = new DoubleAnimation((screenWidth / 2) - (m_mainContainer.ActualWidth / 2), TimeSpan.FromMilliseconds(m_animationTimeMilliseconds));
            db.From = (screenWidth / 2) + (m_mainContainer.ActualWidth / 2);

            var easingFunction = new CubicEase();
            easingFunction.EasingMode = EasingMode.EaseInOut;
            db.EasingFunction = easingFunction;

            m_mainContainer.RenderTransform = new TranslateTransform();
            m_mainContainer.RenderTransform.BeginAnimation(TranslateTransform.XProperty, db);

            m_isOpen = true;
            m_isAnyPOIOpen = true;
        }

        private void HandleCloseButtonClicked(object sender, RoutedEventArgs e)
        {
            if (!m_closing)
            {
                m_closing = true;
                ExampleApp.SearchResultPoiViewCLI.CloseButtonClicked(m_nativeCallerPointer);
            }
        }

        private void OnBoundaryFeedback(object sender, ManipulationBoundaryFeedbackEventArgs e)
        {
            e.Handled = true;
        }

        protected virtual void DisplayCustomPoiInfo(Object modelObject)
        {
            if (m_headerFade != null)
            {
                m_headerFade.Opacity = 0;
            }

            if (m_scrollUpButton != null)
            {
                m_scrollUpButton.Opacity = 0;
            }

            if (m_footerFade != null)
            {
                m_footerFade.Opacity = 0;
            }

            if (m_scrollDownButton != null)
            {
                m_scrollDownButton.Opacity = 0;
            }
        }

        public void DisplayPoiInfo(Object modelObject, bool isPinned)
        {
            // set the pinned state from the native model without feeding back into the native model
            if (m_isPinned != isPinned)
            {
                m_isPinned = isPinned;
                OnPropertyChanged("IsPinned");
            }

            DisplayCustomPoiInfo(modelObject);
        }

        public virtual void DismissPoiInfo()
        {
            HideAll();
        }

        public abstract void UpdateImageData(string url, bool hasImage, byte[] imgData);

        private void HandleCloseClicked(object sender, System.Windows.Input.MouseButtonEventArgs e)
        {
            ExampleApp.SearchResultPoiViewCLI.CloseButtonClicked(m_nativeCallerPointer);
        }

        private bool HandleTogglePinnedClicked(ref bool oldValue, bool newValue)
        {
            if (oldValue != newValue)
            {
                if (!newValue)
                {
                    if (ShowRemovePinDialog() == true)
                    {
                        ExampleApp.SearchResultPoiViewCLI.TogglePinnedButtonClicked(m_nativeCallerPointer);
                        oldValue = newValue;

                        return true;
                    }
                }
                else
                {
                    ExampleApp.SearchResultPoiViewCLI.TogglePinnedButtonClicked(m_nativeCallerPointer);
                    oldValue = newValue;

                    return true;
                }
            }

            return false;
        }

        private void HandleScrollUpButtonClicked(object sender, RoutedEventArgs e)
        {
            if (m_contentContainer != null)
            {
                m_contentContainer.ScrollToVerticalOffset(m_contentContainer.VerticalOffset - m_scrollSpeed);
            }
        }

        private void HandleScrollDownButtonClicked(object sender, RoutedEventArgs e)
        {
            if (m_contentContainer != null)
            {
                m_contentContainer.ScrollToVerticalOffset(m_contentContainer.VerticalOffset + m_scrollSpeed);
            }
        }

        private bool ShowRemovePinDialog()
        {
            DialogBox dialogBox = new DialogBox("Remove Pin", "Are you sure you want to remove this pin?", "Yes", "No");
            dialogBox.Owner = m_currentWindow;

            m_currentWindow.SetOpacity(MainWindow.OpacityOnPopup);

            bool? result = dialogBox.ShowDialog();

            m_currentWindow.SetOpacity(1.0f);

            if (result == null)
            {
                return false;
            }

            return (bool)result;
        }

        protected static BitmapImage LoadImageFromByteArray(byte[] imageData)
        {
            if (imageData == null || imageData.Length == 0)
            {
                return null;
            }

            var image = new BitmapImage();

            try
            {
                using (var mem = new MemoryStream(imageData))
                {
                    mem.Position = 0;
                    image.BeginInit();
                    image.CreateOptions = BitmapCreateOptions.PreservePixelFormat;
                    image.CacheOption = BitmapCacheOption.OnLoad;
                    image.UriSource = null;
                    image.StreamSource = mem;
                    image.EndInit();
                }
            }
            catch (NotSupportedException)
            {
                // the byte stream did not point at parseable image data (can happen if user sets something other than
                // an image to be the image url for a given poi).
                return null;
            }

            image.Freeze();

            return image;
        }

        protected virtual void OnHideAnimationCompleted(object s, EventArgs e)
        {
        }
    }
}
