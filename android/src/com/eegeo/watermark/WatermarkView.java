// Copyright eeGeo Ltd (2012-2017), All Rights Reserved

package com.eegeo.watermark;

import android.animation.Animator;
import android.animation.Animator.AnimatorListener;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.graphics.drawable.Drawable;
import android.net.Uri;
import android.support.v4.content.ContextCompat;
import android.view.View;
import android.widget.RelativeLayout;

import com.eegeo.entrypointinfrastructure.MainActivity;
import com.eegeo.mobileexampleapp.R;

public class WatermarkView implements View.OnClickListener
{
    private static AlertDialog m_options = null;
    private final long m_stateChangeAnimationTimeMilliseconds = 300;
    private final float InteriorStylingDisabledAlpha = 0.8f;
    private final float InteriorStylingEnabledAlpha = 0.5f;
    private MainActivity m_activity = null;
    private long m_nativeCallerPointer;
    private View m_view = null;
    private View m_gradientView = null;
    private String m_imageAssetUrl;
    private String m_popupTitle;
    private String m_popupBody;
    private String m_webUrl;
    private boolean m_shouldShowShadow;
    private boolean m_alignAlongBottom;
    private boolean m_alignBelowFloorDisplay;
    private float m_yPosActive;
    private float m_yPosInactive;
    private boolean m_shouldShow;
    private AnimatorListener m_transitionOffListener;
    private Drawable m_currentBackgroundDrawable;

    public WatermarkView(MainActivity activity,
                         long nativeCallerPointer,
                         String imageAssetUrl,
                         String popupTitle,
                         String popupBody,
                         String webUrl,
                         boolean shouldShowShadow)
    {
        m_activity = activity;
        m_nativeCallerPointer = nativeCallerPointer;
        m_transitionOffListener = new AnimatorListener()
        {
            @Override
            public void onAnimationCancel(Animator animation)
            {
                refreshImage();
                animation.removeAllListeners();
            }

            @Override
            public void onAnimationEnd(Animator animator)
            {
                refreshImage();

                m_view.setY(m_yPosInactive);
                animateToActive();

                float gradientAlpha = m_shouldShowShadow ? 1.0f : 0.0f;
                m_gradientView.animate().alpha(gradientAlpha).setDuration(m_stateChangeAnimationTimeMilliseconds);
            }

            @Override
            public void onAnimationRepeat(Animator animation)
            {
            }

            @Override
            public void onAnimationStart(Animator animation)
            {
            }
        };

        final RelativeLayout uiRoot = (RelativeLayout) m_activity.findViewById(R.id.ui_container);
        m_gradientView = m_activity.getLayoutInflater().inflate(R.layout.watermark_background_layout, uiRoot, false);
        m_view = m_activity.getLayoutInflater().inflate(R.layout.watermark_layout, uiRoot, false);
        //m_view.setOnClickListener(this);

        m_view.addOnLayoutChangeListener(new View.OnLayoutChangeListener()
        {
            @Override
            public void onLayoutChange(View v, int left, int top, int right,
                                       int bottom, int oldLeft, int oldTop, int oldRight,
                                       int oldBottom)
            {
                refreshPositions();

                m_view.setY(m_yPosInactive);
                m_view.removeOnLayoutChangeListener(this);
            }
        });

        updateWatermarkData(imageAssetUrl, popupTitle, popupBody, webUrl, shouldShowShadow);
        m_shouldShow = false;
        refreshPositions();
        m_view.setY(m_yPosInactive);
        m_view.setAlpha(InteriorStylingDisabledAlpha);
        m_gradientView.setAlpha(0.0f);
        m_gradientView.setY(uiRoot.getHeight() - m_activity.dipAsPx(52.0f));
        uiRoot.addView(m_gradientView);
        uiRoot.addView(m_view);
    }

    public void destroy()
    {
        if (m_options != null)
        {
            m_options.dismiss();
            m_options = null;
        }

        final RelativeLayout uiRoot = (RelativeLayout) m_activity.findViewById(R.id.ui_container);
        uiRoot.removeView(m_view);
        uiRoot.removeView(m_gradientView);
        m_view = null;
        m_gradientView = null;
    }

    @Override
    public void onClick(View view)
    {
        AlertDialog.Builder builder = new AlertDialog.Builder(m_activity);

        builder.setTitle(m_popupTitle);
        builder.setMessage(m_popupBody);
        builder.setPositiveButton("Find Out More", createClickListener(true));
        builder.setNegativeButton("Later", createClickListener(false));
        builder.setCancelable(false);

        m_options = builder.show();
        WatermarkViewJniMethods.OnSelected(m_nativeCallerPointer);
    }

    public void animateToActive()
    {
        m_shouldShow = true;
        animateViewToY((int) m_yPosActive);
    }

    public void animateToInactive()
    {
        m_shouldShow = false;
        animateViewToY((int) m_yPosInactive);
    }

    protected void animateViewToY(final int yAsPx)
    {
        m_view.animate()
                .y(yAsPx)
                .setListener(null)
                .setDuration(m_stateChangeAnimationTimeMilliseconds);
    }

    public void animateToIntermediateOnScreenState(final float onScreenState)
    {
        int viewYPx = (int) m_view.getY();
        int newYPx = (int) (m_yPosInactive + (int) (((m_yPosActive - m_yPosInactive) * onScreenState) + 0.5f));

        if (viewYPx != newYPx)
        {
            m_view.setY(newYPx);
        }
    }

    public void updateWatermarkData(final String imageAssetUrl,
                                    final String popupTitle,
                                    final String popupBody,
                                    final String webUrl,
                                    final boolean shouldShowShadow)
    {
        m_popupTitle = popupTitle;
        m_popupBody = popupBody;
        m_webUrl = webUrl;
        m_shouldShowShadow = shouldShowShadow;

        boolean shouldTransitionImage = imageAssetUrl != m_imageAssetUrl;

        if (shouldTransitionImage)
        {
            m_imageAssetUrl = imageAssetUrl;
            transitionToNewImage();
        }
    }

    private void transitionToNewImage()
    {
        if (m_shouldShow)
        {
            m_view.clearAnimation();
            m_view.animate()
                    .y(m_yPosInactive)
                    .setDuration(m_stateChangeAnimationTimeMilliseconds)
                    .setListener(m_transitionOffListener);
        }
        else
        {
            refreshImage();
            m_view.setY(m_yPosInactive);
        }
    }

    private DialogInterface.OnClickListener createClickListener(final boolean shouldPreload)
    {
        return new DialogInterface.OnClickListener()
        {
            @Override
            public void onClick(DialogInterface dialog, int which)
            {
                handleSelection(shouldPreload);
            }
        };
    }

    private void handleSelection(final boolean shouldOpenLink)
    {
        if (shouldOpenLink)
        {
            final Uri uri = Uri.parse(m_webUrl);
            final Intent browserIntent = new Intent(Intent.ACTION_VIEW, uri);
            m_activity.startActivity(browserIntent);
        }

        m_options = null;
    }

    public void setAlignmentState(final boolean alignAlongBottom, final boolean alignBelowFloorDisplay)
    {
        m_alignAlongBottom = alignAlongBottom;
        m_alignBelowFloorDisplay = alignBelowFloorDisplay;
        refreshPositions();
    }

    public void setInteriorStylingState(final boolean shouldUseInteriorStyling)
    {
        m_view.setAlpha(shouldUseInteriorStyling ? InteriorStylingEnabledAlpha : InteriorStylingDisabledAlpha);
    }

    private void refreshImage()
    {
        String uri = "drawable/" + m_imageAssetUrl;
        int imageResource = m_activity.getResources().getIdentifier(uri, null, m_activity.getPackageName());
        m_view.setBackgroundResource(imageResource);
        m_currentBackgroundDrawable = ContextCompat.getDrawable(m_activity, imageResource);
        refreshPositions();
    }

    private void refreshPositions()
    {
        final RelativeLayout uiRoot = (RelativeLayout) m_activity.findViewById(R.id.ui_container);

        final float screenWidth = uiRoot.getWidth();
        final float screenHeight = uiRoot.getHeight();
        final float watermarkHeight = m_currentBackgroundDrawable.getIntrinsicHeight();
        final float watermarkWidth  = m_currentBackgroundDrawable.getIntrinsicWidth();

        if (m_activity.getResources().getBoolean(R.bool.isPhone))
        {
            if (m_alignAlongBottom)
            {
                m_yPosActive = (screenHeight - watermarkHeight) - m_activity.dipAsPx(8);
                m_yPosInactive = (screenHeight + watermarkHeight);
            }
            else
            {
                m_yPosActive = m_alignBelowFloorDisplay
                  ? watermarkHeight + m_activity.dipAsPx(40)
                  : m_activity.dipAsPx(20);

                m_yPosInactive = -watermarkHeight;
            }
        }
        else
        {
            m_yPosActive = (screenHeight - watermarkHeight) - m_activity.dipAsPx(8);
            m_yPosInactive = (screenHeight + watermarkHeight);
        }

        m_view.setX(m_activity.getResources().getBoolean(R.bool.isPhone)
          ? screenWidth * 0.5f - watermarkWidth * 0.5f
          : m_activity.dipAsPx(20));
    }
}
