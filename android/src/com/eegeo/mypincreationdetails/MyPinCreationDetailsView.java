// Copyright eeGeo Ltd (2012-2015), All Rights Reserved

package com.eegeo.mypincreationdetails;

import android.Manifest;
import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.database.Cursor;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Matrix;
import android.media.ExifInterface;
import android.net.Uri;
import android.provider.MediaStore.Images;
import android.support.v4.app.ActivityCompat;
import android.text.Editable;
import android.text.Html;
import android.text.TextWatcher;
import android.text.method.LinkMovementMethod;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.View;
import android.view.ViewGroup;
import android.view.ViewGroup.MarginLayoutParams;
import android.widget.CompoundButton;
import android.widget.CompoundButton.OnCheckedChangeListener;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
import android.widget.ScrollView;
import android.widget.TextView;
import android.widget.ToggleButton;

import com.eegeo.entrypointinfrastructure.MainActivity;
import com.eegeo.helpers.IActivityIntentResultHandler;
import com.eegeo.helpers.IRuntimePermissionResultHandler;
import com.eegeo.mobileexampleapp.R;
import com.eegeo.photos.PhotoIntentDispatcher;
import com.eegeo.runtimepermissions.RuntimePermissionDispatcher;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class MyPinCreationDetailsView implements View.OnClickListener, IActivityIntentResultHandler, IRuntimePermissionResultHandler
{
    protected MainActivity m_activity = null;
    protected long m_nativeCallerPointer;
    protected LinearLayout m_view = null;
    protected View m_closeButton = null;
    protected View m_takePhotoButton = null;
    protected View m_selectFromGalleryButton = null;
    protected View m_submitButton = null;
    protected ImageView m_poiImage = null;
    protected EditText m_title = null;
    protected EditText m_description = null;
    protected ToggleButton m_shouldShareButton = null;
    protected TextView m_termsAndConditionsLink = null;
    protected ScrollView m_scrollSection = null;

    private byte[] m_imageBuffer = null;
    private boolean m_awaitingIntentResponse;
    private boolean m_hasNetworkConnectivity = false;
    private boolean m_showingNoNetworkDialog = false;

    private final int JPEG_QUALITY = 90;
    private final String TERMS_AND_CONDITIONS_LINK = "http://wrld3d.com/terms-of-service";

    private class ScreenDimensions
    {
        DisplayMetrics m_metrics;

        ScreenDimensions(Activity activity)
        {
            m_metrics = activity.getResources().getDisplayMetrics();
        }

        float getWidth ()   { return m_metrics.widthPixels;  }
        float getHeight()   { return m_metrics.heightPixels; }
    }

    public MyPinCreationDetailsView(MainActivity activity, long nativeCallerPointer)
    {
        m_activity = activity;
        m_nativeCallerPointer = nativeCallerPointer;

        createView();
    }

    public void destroy()
    {
        final IActivityIntentResultHandler thisHandler = this;

        final RelativeLayout uiRoot = (RelativeLayout)m_activity.findViewById(R.id.ui_container);
        uiRoot.removeView(m_view);
        m_view = null;

        m_activity.getPhotoIntentDispatcher().removeActivityIntentResultHandler(thisHandler);
        m_activity.getRuntimePermissionDispatcher().removeIRuntimePermissionResultHandler(this);
    }

    private void createView()
    {
        final RelativeLayout uiRoot = (RelativeLayout)m_activity.findViewById(R.id.ui_container);

        m_view = (LinearLayout)m_activity.getLayoutInflater().inflate(R.layout.poi_creation_details_layout, uiRoot, false);
        ViewGroup mainGroup = (ViewGroup)m_view.findViewById(R.id.poi_creation_details_main);

        m_closeButton = (View)m_view.findViewById(R.id.poi_creation_details_button_close);
        m_closeButton.setOnClickListener(this);
        m_submitButton = (View)m_view.findViewById(R.id.poi_creation_details_button_submit);
        m_submitButton.setOnClickListener(this);
        m_takePhotoButton = (View)m_view.findViewById(R.id.poi_creation_details_button_photo);
        m_takePhotoButton.setOnClickListener(this);
        m_selectFromGalleryButton = (View)m_view.findViewById(R.id.poi_creation_details_button_gallery);
        m_selectFromGalleryButton.setOnClickListener(this);
        m_poiImage = (ImageView)m_view.findViewById(R.id.poi_creation_details_image);
        m_title = (EditText)m_view.findViewById(R.id.poi_creation_details_title_edit_text);
        m_description = (EditText)m_view.findViewById(R.id.poi_creation_details_description);
        m_shouldShareButton = (ToggleButton)m_view.findViewById(R.id.poi_creation_details_share_togglebutton);

        addStringRemovalListener(m_title, Pattern.compile("[￼\n\r]+"));
        addStringRemovalListener(m_description, Pattern.compile("[￼]+"));

        ScreenDimensions dims = new ScreenDimensions(m_activity);
        MarginLayoutParams margins = (MarginLayoutParams)mainGroup.getLayoutParams();
        margins.leftMargin = margins.rightMargin  = (int)Math.round(dims.getWidth()  * 0.05);
        margins.topMargin  = margins.bottomMargin = (int)Math.round(dims.getHeight() * 0.05);

        m_shouldShareButton.setOnCheckedChangeListener(new OnCheckedChangeListener()
        {
            @Override
            public void onCheckedChanged(CompoundButton arg0, boolean arg1)
            {
                verifyShareSettingsValid();
            }
        });

        m_termsAndConditionsLink = (TextView)m_view.findViewById(R.id.poi_creation_details_terms_conditions_link);
        m_scrollSection = (ScrollView)m_view.findViewById(R.id.poi_creation_details_scroll_section);

        m_view.setVisibility(View.GONE);

        m_termsAndConditionsLink.setClickable(true);
        m_termsAndConditionsLink.setMovementMethod(LinkMovementMethod.getInstance());
        String linkText = "<a href='" + TERMS_AND_CONDITIONS_LINK + "'>(Terms & Conditions)</a>";
        m_termsAndConditionsLink.setText(Html.fromHtml(linkText));

        m_activity.getPhotoIntentDispatcher().addActivityIntentResultHandler(this);
        m_activity.getRuntimePermissionDispatcher().addRuntimePermissionResultHandler(this);

        uiRoot.addView(m_view);
    }

    public void show()
    {
        m_view.setVisibility(View.VISIBLE);
        m_view.requestFocus();

        m_poiImage.setImageResource(R.drawable.image_blank);
        m_title.setText("");
        m_description.setText("");

        m_shouldShareButton.setChecked(m_hasNetworkConnectivity);

        m_imageBuffer = null;

        m_awaitingIntentResponse = false;

        m_scrollSection.setScrollY(0);
    }

    public void dismiss()
    {
        m_view.setVisibility(View.GONE);
    }

    public String getTitle()
    {
        String titleText = m_title.getText().toString();
        titleText = titleText.isEmpty() ? "Untitled Pin" : titleText;
        return titleText;
    }

    public String getDescription()
    {
        String descriptionText = m_description.getText().toString();
        return descriptionText;
    }

    public boolean getShouldShare()
    {
        return m_shouldShareButton.isChecked();
    }

    public byte[] getImageBuffer()
    {
        return m_imageBuffer;
    }

    public int getImageBufferSize()
    {
        return m_imageBuffer == null ? 0 : m_imageBuffer.length;
    }

    @Override
    public void onClick(final View view)
    {
        if(view == m_takePhotoButton)
        {
            if(m_awaitingIntentResponse)
            {
                return;
            }
            if(m_activity.getRuntimePermissionDispatcher().hasCameraAndStoragePermissions(this))
            {
                if (m_activity.getPhotoIntentDispatcher().takePhoto())
                {
                    m_awaitingIntentResponse = true;
                }
                else
                {
                    showCameraErrorDialog();
                }
            }
        }
        else if(view == m_selectFromGalleryButton)
        {
            if(m_awaitingIntentResponse)
            {
                return;
            }

            if (m_activity.getPhotoIntentDispatcher().selectPhotoFromGallery())
            {
                m_awaitingIntentResponse = true;
            }
            else
            {
                showGalleryErrorDialog();
            }
        }
        else if(view == m_closeButton)
        {
            MyPinCreationDetailsJniMethods.CloseButtonPressed(m_nativeCallerPointer);
        }
        else if(view == m_submitButton)
        {
            MyPinCreationDetailsJniMethods.SubmitButtonPressed(m_nativeCallerPointer);
        }
    }

    @Override
    public void onActivityResult(final int requestCode, final int resultCode, final Intent data)
    {
        if(requestCode == PhotoIntentDispatcher.REQUEST_IMAGE_CAPTURE && resultCode == MainActivity.RESULT_OK)
        {
            Intent mediaScanIntent = new Intent(Intent.ACTION_MEDIA_SCANNER_SCAN_FILE);
            Uri uri = m_activity.getPhotoIntentDispatcher().getCurrentPhotoPath();
            mediaScanIntent.setData(uri);
            m_activity.sendBroadcast(mediaScanIntent);

            setImageUri(uri);
        }
        else if(requestCode == PhotoIntentDispatcher.SELECT_PHOTO_FROM_GALLERY && resultCode == MainActivity.RESULT_OK)
        {
            Uri uri = data.getData();
            setImageUri(uri);
        }

        if(requestCode == PhotoIntentDispatcher.SELECT_PHOTO_FROM_GALLERY || requestCode == PhotoIntentDispatcher.REQUEST_IMAGE_CAPTURE)
        {
            m_awaitingIntentResponse = false;
        }
    }

    private void setImageUri(final Uri uri)
    {
        try
        {
            Bitmap bitmap = decodeImage(uri);
            if (bitmap != null)
            {
                ByteArrayOutputStream stream = new ByteArrayOutputStream();
                bitmap.compress(Bitmap.CompressFormat.JPEG, JPEG_QUALITY, stream);
                m_imageBuffer = stream.toByteArray();
                stream.close();

                m_poiImage.setImageBitmap(bitmap);
            }
        }
        catch (IOException e)
        {
            m_poiImage.setImageResource(R.drawable.image_blank);
            m_imageBuffer = null;
            e.printStackTrace();
        }
    }

    private Bitmap decodeImage(final Uri uri) throws IOException
    {
        final int idealSizePx = 512;

        InputStream is = m_activity.getContentResolver().openInputStream(uri);

        BitmapFactory.Options bmOptions = new BitmapFactory.Options();
        bmOptions.inJustDecodeBounds = true;
		bmOptions.inPreferredConfig = Bitmap.Config.RGB_565;

        Bitmap bitmap = BitmapFactory.decodeStream(is, null, bmOptions);
        is.close();
        int photoWidth = bmOptions.outWidth;
        int photoHeight = bmOptions.outHeight;

        int scaleFactor = Math.min(photoWidth/idealSizePx, photoHeight/idealSizePx);
        bmOptions.inJustDecodeBounds = false;
        bmOptions.inSampleSize = scaleFactor;
        bmOptions.inPurgeable = true;

        is = m_activity.getContentResolver().openInputStream(uri);
        bitmap = BitmapFactory.decodeStream(is, null, bmOptions);
        is.close();
        if (bitmap == null)
        {
            return null;
        }

        int finalWidth = bitmap.getWidth();
        int finalHeight = bitmap.getHeight();

        float rotation = getOrientationRotation(uri);
        Matrix mtx = new Matrix();
        mtx.postRotate(rotation);
        bitmap = Bitmap.createBitmap(bitmap, 0, 0, finalWidth, finalHeight, mtx, true);
        return bitmap;
    }

    private float getOrientationRotation(final Uri uri)
    {
        float photoRotation = 0;
        boolean hasRotation = false;
        String[] projection = { Images.ImageColumns.ORIENTATION };
        try
        {
            Cursor cursor = m_activity.getContentResolver().query(uri, projection, null, null, null);
            if(cursor.moveToFirst())
            {
                if (cursor.moveToFirst())
                {
                    int orientation = cursor.getInt(0);
                    photoRotation = orientation;
                    hasRotation = (orientation != 0);
                }
            }
        }
        catch (Exception e)
        {
            Log.d("EEGEO", "Failed to fetch orientation data for " + uri.toString());
        }

        if(!hasRotation)
        {
            ExifInterface exif = createExifInterface(uri);
            if(exif==null)
            {
                Log.d("EEGEO", "Failed to fetch exif interface for image " + uri.toString());
                return photoRotation;
            }

            int exifRotation = exif.getAttributeInt(ExifInterface.TAG_ORIENTATION, ExifInterface.ORIENTATION_UNDEFINED);

            switch(exifRotation)
            {
            case ExifInterface.ORIENTATION_ROTATE_90:
                photoRotation = 90.0f;
                break;
            case ExifInterface.ORIENTATION_ROTATE_180:
                photoRotation = 180.0f;
                break;
            case ExifInterface.ORIENTATION_ROTATE_270:
                photoRotation = 270.0f;
                break;
            default:
                break;

            }
        }

        return photoRotation;
    }

    private ExifInterface createExifInterface(final Uri uri)
    {
        try
        {
            if(android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.N)
            {
                InputStream is = m_activity.getContentResolver().openInputStream(uri);
                return (is == null)?null : new ExifInterface(is);
            }
            else
            {
                return new ExifInterface(uri.getPath());
            }
        }
        catch (IOException e)
        {
            return null;
        }
    }

    private void verifyShareSettingsValid()
    {
        if (m_shouldShareButton.isChecked() && !m_hasNetworkConnectivity)
        {
            if (!m_showingNoNetworkDialog)
            {
                AlertDialog.Builder builder = new AlertDialog.Builder(m_activity);
                builder.setTitle("No network connection")
                .setMessage("Pins cannot be shared when no network connection is available")
                .setCancelable(false)
                .setPositiveButton("Dismiss", new DialogInterface.OnClickListener()
                {
                    public void onClick(DialogInterface dialog, int id)
                    {
                        m_showingNoNetworkDialog = false;
                    }
                });
                AlertDialog alert = builder.create();
                alert.show();
                m_showingNoNetworkDialog = true;
            }

            m_shouldShareButton.setChecked(false);
        }
    }

    public void setHasNetworkConnectivity(boolean hasNetworkConnectivity, boolean shouldVerify)
    {
        m_hasNetworkConnectivity = hasNetworkConnectivity;

        if (shouldVerify)
        {
            verifyShareSettingsValid();
        }
        else
        {
            m_shouldShareButton.setChecked(m_hasNetworkConnectivity);
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults)
    {
        if (requestCode != RuntimePermissionDispatcher.CAMERA_PERMISSION_REQUEST_CODE)
            return;
        
        // If request is cancelled, the result arrays are empty.
        if (grantResults.length > 0 && grantResults[0] == PackageManager.PERMISSION_GRANTED
                && grantResults[1] == PackageManager.PERMISSION_GRANTED)
        {
        	if (m_activity.getPhotoIntentDispatcher().takePhoto())
            {
                m_awaitingIntentResponse = true;
            }
            else
            {
                showCameraErrorDialog();
            }
        }
        else
        {
            // If any of the permission is denied, we can't use the camera
            // properly, so we will show the dialog with agree or cancel dialog
            showPermissionRequiredDialog(m_activity);
        }
        return;
    }
    
    private void showCameraErrorDialog()
    {
    	AlertDialog.Builder builder = new AlertDialog.Builder(m_activity);
        builder.setTitle("Error");
        builder.setMessage("Error opening camera. Make sure device camera is not 'disabled'");
        builder.setNegativeButton("Ok", null);
        builder.setCancelable(false);
        builder.show();
    }
    
    private void showGalleryErrorDialog()
    {
    	AlertDialog.Builder builder = new AlertDialog.Builder(m_activity);
        builder.setTitle("Error");
        builder.setMessage("Error opening gallery. Make sure device gallery is not 'disabled'");
        builder.setNegativeButton("Ok", null);
        builder.setCancelable(false);
        builder.show();
    }

    private void showPermissionRequiredDialog(final Activity context)
    {
        DialogInterface.OnClickListener dialogClickListener = new DialogInterface.OnClickListener()
        {
            @Override
            public void onClick(DialogInterface dialog, int which)
            {
                dialog.dismiss();
                switch(which)
                {
                case DialogInterface.BUTTON_POSITIVE:
                    if (ActivityCompat.shouldShowRequestPermissionRationale(context, Manifest.permission.CAMERA)
                            || ActivityCompat.shouldShowRequestPermissionRationale(context,
                                    Manifest.permission.WRITE_EXTERNAL_STORAGE))
                    {
                        // Checking Permissions again
                        m_activity.getRuntimePermissionDispatcher().hasCameraAndStoragePermissions(MyPinCreationDetailsView.this);
                    }
                    else
                    {
                        // Open App Settings Page
                        m_activity.getRuntimePermissionDispatcher().startAppSettings(context);
                    }
                    break;
                }
            }
        };

        AlertDialog.Builder builder = new AlertDialog.Builder(context);
        builder.setMessage(context.getResources().getString(R.string.required_camera_permission_text))
                .setPositiveButton(context.getResources().getString(R.string.ok_text), dialogClickListener)
                .setNegativeButton(context.getResources().getString(R.string.cancel_text), dialogClickListener).show();
    }

    private void addStringRemovalListener(final EditText text, final Pattern pattern)
    {
        text.addTextChangedListener(new TextWatcher() {
            boolean reentryGuard = false;
            @Override
            public void beforeTextChanged(CharSequence s, int start, int count, int after) {

            }

            @Override
            public void onTextChanged(CharSequence s, int start, int before, int count) {

            }

            @Override
            public void afterTextChanged(Editable editable) {
                if (reentryGuard) {
                    return;
                }

                final String input = editable.toString();
                final Matcher matcher = pattern.matcher(input);
                if (matcher.find()) {
                    final String output = matcher.replaceAll("");
                    final int selectionStart = text.getSelectionStart();
                    final int charsRemovedCount = input.length() - output.length();

                    reentryGuard = true;
                    text.setText(output);
                    text.setSelection(selectionStart - charsRemovedCount);
                    reentryGuard = false;
                }
            }
        });
    }
}
