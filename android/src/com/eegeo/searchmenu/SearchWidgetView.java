// Copyright WRLD Ltd (2018-), All Rights Reserved

package com.eegeo.searchmenu;

import com.eegeo.entrypointinfrastructure.MainActivity;
import com.eegeo.mobileexampleapp.R;
import com.eegeo.searchproviders.MyTestSearchProvider;
import com.eegeo.searchproviders.QueryContext;

import android.app.SearchManager;
import android.app.SearchableInfo;
import android.content.Context;
import android.util.Log;
import android.view.View;
import android.widget.RelativeLayout;
import android.widget.TextView;

import com.eegeo.searchproviders.SearchResultNavigationHandler;
import com.eegeo.tags.TagResources;
import com.eegeo.ui.IAnimatedView;
import com.eegeo.ui.IViewAnimator;
import com.eegeo.ui.ViewAnimator;
import com.google.android.gms.maps.model.LatLng;
import com.wrld.widgets.search.WrldSearchWidget;
import com.wrld.widgets.search.model.MenuChild;
import com.wrld.widgets.search.model.MenuGroup;
import com.wrld.widgets.search.model.MenuOption;
import com.wrld.widgets.search.model.OnMenuOptionSelectedCallback;
import com.wrld.widgets.search.model.SearchQuery;
import com.wrld.widgets.search.model.SearchProviderQueryResult;
import com.wrld.widgets.search.model.SearchQueryModelListener;
import com.wrld.widgets.search.model.SearchResult;
import com.wrld.widgets.search.model.SearchResultsListener;
import com.wrld.widgets.search.model.TextChangedListener;
import com.wrld.widgets.search.view.MenuViewListener;
import com.wrld.widgets.search.view.SearchResultsViewListener;


import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.IOException;
import java.io.InputStream;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Hashtable;
import java.util.List;
import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.StringRequest;
import com.android.volley.toolbox.Volley;

public class SearchWidgetView implements OnMenuOptionSelectedCallback,
                                        SearchResultsListener,
                                        IAnimatedView,
                                        SearchResultsViewListener,
                                        View.OnFocusChangeListener,
                                        MenuViewListener,
                                        SearchQueryModelListener,
                                        SearchResultNavigationHandler,
                                        TextChangedListener
{
    protected MainActivity m_activity;
    protected MyTestSearchProvider m_searchProvider;
    protected long m_nativeCallerPointer;
    protected View m_view;
    private IViewAnimator m_viewAnimator;
    protected WrldSearchWidget m_searchWidget;

    private boolean m_searchResultsAreVisible = true;
    private boolean m_menuIsOpen = false;
    private boolean m_searchTextboxIsInFocus = false;
    private boolean m_hasSearchResults = false;
    private boolean m_searchInProgress = false;

    private Hashtable<String, MenuOption> m_menuOptions
            = new Hashtable<String, MenuOption>();

    private Hashtable<String, MenuGroup> m_menuGroups
            = new Hashtable<String, MenuGroup>();

    private boolean m_hasPopulatedData = false;

    private final long m_stateChangeAnimationTimeMilliseconds = 200;

    protected View m_weatherView;

    HashMap<String, LatLng> m_latLongData = new HashMap<>();
    private static final String WEATHER_API_BASE_URL = "http://api.openweathermap.org/data/2.5/weather?";
    private TextView m_minTempTextView, m_maxTempTextView, m_humidityTextView, m_cityTextView, m_weatherTextView;
    private String m_weatherString = "", m_minTempString = "", m_maxTempString = "", m_humidityString, m_cityString;




    public SearchWidgetView(MainActivity activity, long nativeCallerPointer,
                            MyTestSearchProvider searchProvider) {
        m_activity = activity;
        m_searchProvider = searchProvider;
        m_nativeCallerPointer = nativeCallerPointer;

        createView();
    }

    protected void createView()
    {
        final RelativeLayout uiRoot = (RelativeLayout) m_activity.findViewById(R.id.ui_container);
        m_view = m_activity.getLayoutInflater().inflate(R.layout.search_widget_view_layout, uiRoot, false);
        uiRoot.addView(m_view);

        m_viewAnimator = new ViewAnimator(this, m_view);
        m_viewAnimator.registerLayoutChangeListener();

        m_searchWidget = (WrldSearchWidget) m_activity.getFragmentManager().findFragmentById(R.id.search_widget);
        m_searchWidget.addSearchProvider(m_searchProvider);
        m_searchWidget.addSuggestionProvider(m_searchProvider);

        m_searchProvider.addNavigationRequestCallback(this);

        m_searchWidget.getSearchResultsModel().addResultListener(this);

        m_searchWidget.getMenuViewObserver().addMenuListener(this);
        m_searchWidget.getSearchViewFocusObserver().addListener(this);
        m_searchWidget.getSearchResultsViewObserver().addListener(this);
        m_searchWidget.getSearchQueryModel().addListener(this);
        m_searchWidget.getTextChangedObserver().addTextChangedListener(this);

        SearchManager searchManager = (SearchManager) m_activity.getSystemService(Context.SEARCH_SERVICE);
        SearchableInfo searchableInfo = searchManager.getSearchableInfo(m_activity.getComponentName());
        m_searchWidget.setSearchableInfo(searchableInfo);

        // adding weather view
        m_weatherView = m_activity.getLayoutInflater().inflate(R.layout.weather_view, uiRoot, false);
        uiRoot.addView(m_weatherView);
        m_weatherView.setVisibility(View.INVISIBLE);

        m_minTempTextView = (TextView) m_activity.findViewById(R.id.tvMinTemp);
        m_maxTempTextView = (TextView) m_activity.findViewById(R.id.tvMaxTemp);
        m_humidityTextView = (TextView) m_activity.findViewById(R.id.tvHumidity);
        m_cityTextView = (TextView) m_activity.findViewById(R.id.tvPlace);
        m_weatherTextView = (TextView) m_activity.findViewById(R.id.tvWeather);
        readPlacesFromJsonFile();

    }

    public void onSearchResultsReceived(SearchQuery searchQuery, List<SearchProviderQueryResult> list) {
        setHasSearchResults(true);
    }

    public void onSearchResultsCleared() {
        setHasSearchResults(false);

        SearchWidgetViewJniMethods.OnSearchResultsCleared(m_nativeCallerPointer);
    }

    public void clearSearchResults()
    {
        m_searchWidget.clearSearch();

        m_searchWidget.showResultsView(); // state to "shown", for the next time a search is done
    }

    public void onSearchResultsSelected(SearchResult searchResult) {
        SearchWidgetResult widgetResult = (SearchWidgetResult) searchResult;

        SearchWidgetViewJniMethods.OnSearchResultSelected(
                m_nativeCallerPointer,
                widgetResult.getIndex());
    }

    @Override
    public void textChanged(String newtext) {
        SearchWidgetViewJniMethods.OnSearchbarTextChanged(
                m_nativeCallerPointer,
                newtext
        );
    }

    @Override
    public void navigateTo(SearchResult navigateTo) {
        SearchWidgetResult widgetResult = (SearchWidgetResult) navigateTo;

        SearchWidgetViewJniMethods.OnSearchResultNavigationRequest(
                m_nativeCallerPointer,
                widgetResult.getIndex());
    }

    public void onSearchPerformed(String queryText,
                                  boolean clearPreviousResults,
                                  boolean isTag, String tagText,
                                  boolean tryInterior,
                                  boolean usesLocation,
                                  double latitude, double longitude, double altitude,
                                  boolean usesRadius,
                                  float radius)
    {
        QueryContext context =
            usesLocation ?
                new QueryContext(clearPreviousResults,
                                 isTag, tagText, tryInterior,
                                 latitude, longitude, altitude, radius) :
            usesRadius ?
                new QueryContext(clearPreviousResults,
                                 isTag, tagText, tryInterior,
                                 radius) :
                new QueryContext(clearPreviousResults,
                        isTag, tagText, tryInterior);

        m_searchWidget.getSearchResults(queryText, context);
    }

    public boolean onMenuOptionSelected(final String text, final Object context) {
        MenuIndexPath indexPath = (MenuIndexPath) context;

        SearchWidgetViewJniMethods.SelectedItem(m_nativeCallerPointer, text, indexPath.m_section, indexPath.m_item);
        try {
            LatLng latLng = m_latLongData.get(text);
            fetchWeatherInfoFromApi(latLng.latitude, latLng.longitude);
        } catch (Exception e) {
            Log.e("error", e.toString());
        }
        return true;
    }



    public void populateData(
            final long nativeCallerPointer,
            final String[] optionNames,
            final int[] optionSizes,
            final String[] childJsons) {

        if(m_hasPopulatedData==true){
            updateData(optionNames,optionSizes,childJsons);
            return;
        }

        m_hasPopulatedData = true;

        m_searchWidget.clearMenu();

        MenuGroup menuGroup = null;

        List<String> options = Arrays.asList(optionNames);
        int jsonChildIndex = 0;

        for (int optionIndex = 0; optionIndex < options.size(); optionIndex++)
        {
            String optionStartsGroup = getFromJson(optionNames[optionIndex], "groupStart");
            String optionName = getFromJson(optionNames[optionIndex], "name");

            if (optionIndex == 0 || optionStartsGroup != null && optionStartsGroup.equals("Y")) {
                menuGroup = new MenuGroup();
                m_searchWidget.addMenuGroup(menuGroup);
                m_menuGroups.put(optionName,menuGroup);
            }

            int optionSizeWithoutHeader = optionSizes[optionIndex]-1;

            MenuIndexPath optionIndexPath = new MenuIndexPath(optionIndex, 0);

            MenuOption menuOption = new MenuOption(optionName, optionIndexPath, this);
            m_menuOptions.put(optionName,menuOption);

            menuGroup.addOption(menuOption);

            jsonChildIndex++;

            for (int childIndex = 0; childIndex < optionSizeWithoutHeader; ++childIndex) {
                String childJson = childJsons[jsonChildIndex];
                addChildToMenuOption(childIndex,optionIndex,childJson,menuOption);
                jsonChildIndex++;
            }
        }
    }

    public void updateData(
            final String[] optionNames,
            final int[] optionSizes,
            final String[] childJsons){

        List<String> options = Arrays.asList(optionNames);
        int jsonChildIndex = 0;

        for (int optionIndex = 0; optionIndex < options.size(); optionIndex++) {

            int optionSizeWithoutHeader = optionSizes[optionIndex]-1;

            String optionName = getFromJson(optionNames[optionIndex], "name");
            MenuIndexPath optionIndexPath = new MenuIndexPath(optionIndex, 0);

            MenuOption menuOption = m_menuOptions.get(optionName);

            jsonChildIndex++;

            if(menuOption == null) {
                menuOption = new MenuOption(optionName, optionIndexPath, this);
                m_menuOptions.put(optionName,menuOption);
            }

            menuOption.removeAllChildren();
            for (int childIndex = 0; childIndex < optionSizeWithoutHeader; ++childIndex) {
                String childJson = childJsons[jsonChildIndex];
                addChildToMenuOption(childIndex,optionIndex,childJson,menuOption);
                jsonChildIndex++;
            }

        }
    }

    private void addChildToMenuOption(int childIndex, int optionIndex,String childJson, MenuOption menuOption){
        MenuIndexPath indexPath = new MenuIndexPath(optionIndex, childIndex);
        String name = getFromJson(childJson, "name");
        String iconName = getFromJson(childJson, "icon");
        int iconNumber = TagResources.getIconForResourceName(m_activity, iconName);
        MenuChild child = new MenuChild(name, iconNumber, indexPath, this);
        menuOption.addChild(child);
    }


    private String getFromJson(String jsonString, String tag) {
        try {
            JSONObject json = new JSONObject(jsonString);
            String name = json.getString(tag);
            return name;
        } catch (Exception e) {
            Log.e("SearchWidgetView", String.format("Unable to parse %s from group JSON: %s", tag, e.toString()));
            return null;
        }
    }

    public void ViewLayoutChanged() {
        final RelativeLayout uiRoot = (RelativeLayout) m_activity.findViewById(R.id.ui_container);

        final float viewHeight = m_view.getHeight();

        float y = m_view.getY();

        float inactiveY = -viewHeight;

        m_viewAnimator.setActivePos(y);
        m_viewAnimator.setInactivePos(inactiveY);

        m_view.setY(inactiveY);
    }

    public void animateOnScreen() {
        m_viewAnimator.animateToActive(m_stateChangeAnimationTimeMilliseconds);
    }

    public void animateOffScreen() {
        m_searchWidget.hideResultsView();
        m_searchWidget.closeMenu();
        m_viewAnimator.animateToInactive(m_stateChangeAnimationTimeMilliseconds);
    }

    public void animateToIntermediateOnScreenState(final float onScreenState) {
        m_searchWidget.hideResultsView();
        m_searchWidget.closeMenu();
        m_viewAnimator.animateToOnScreenState(onScreenState);
    }

    public void closeMenu() {
        m_searchWidget.hideResultsView();
        m_searchWidget.closeMenu();
        m_view.clearFocus();
    }

    private void pushControlsOfScreenIfNeeded(){

        boolean hasVisibleSearchResults = m_searchResultsAreVisible && (m_hasSearchResults || m_searchInProgress);

        boolean shouldTakeFocus = m_searchTextboxIsInFocus ||hasVisibleSearchResults || m_menuIsOpen;

        if( shouldTakeFocus )
        {
            SearchWidgetViewJniMethods.ViewPushesControlsOffscreen(m_nativeCallerPointer);
        }
        else
        {
            SearchWidgetViewJniMethods.ViewAllowsControlsOnscreen(m_nativeCallerPointer);
        }
    }

    private void setHasSearchResults(boolean hasResults){
        m_hasSearchResults = hasResults;
        pushControlsOfScreenIfNeeded();
    }

    @Override
    public void onFocusChange(View view, boolean inFocus) {
        if(inFocus && !m_searchTextboxIsInFocus && !(m_hasSearchResults || m_searchInProgress))
        {
            return;
        }

        m_searchTextboxIsInFocus = inFocus;
        pushControlsOfScreenIfNeeded();
    }

    //SearchResultViewListener:

    @Override
    public void onSearchResultsShown() {
        m_searchResultsAreVisible = true;
        pushControlsOfScreenIfNeeded();
    }

    @Override
    public void onSearchResultsHidden() {
        m_searchResultsAreVisible = false;
        pushControlsOfScreenIfNeeded();
    }

    //MenuViewListener:

    @Override
    public void onMenuClosed() {
        m_menuIsOpen = false;
        pushControlsOfScreenIfNeeded();
        m_weatherView.setVisibility(View.VISIBLE);

    }

    @Override
    public void onMenuOpened() {
        m_menuIsOpen = true;
        pushControlsOfScreenIfNeeded();
        m_weatherView.setVisibility(View.INVISIBLE);

    }

    @Override
    public void onMenuChildSelected(MenuChild menuChild) {

    }

    @Override
    public void onMenuOptionExpanded(MenuOption menuOption) {

    }

    @Override
    public void onMenuOptionCollapsed(MenuOption menuOption) {

    }

    @Override
    public void onMenuOptionSelected(MenuOption menuOption) {

    }

    //SearchQueryModelListener:
    @Override
    public void onSearchQueryStarted(SearchQuery query)
    {
        m_searchInProgress = true;
        pushControlsOfScreenIfNeeded();

        m_searchProvider.showNavButtons(true);
    }

    @Override
    public void onSearchQueryCompleted(SearchQuery query, List<SearchProviderQueryResult> results)
    {

        m_searchInProgress = false;
        pushControlsOfScreenIfNeeded();
    }

    @Override
    public void onSearchQueryCancelled(SearchQuery query)
    {
        m_searchInProgress = false;
        pushControlsOfScreenIfNeeded();
    }

    private void fetchWeatherInfoFromApi(double lat, double lng) {

        String weatherUnit = "metric";
        RequestQueue queue = Volley.newRequestQueue(m_activity);
        String url = String.format("%slat=%s&lon=%s&APPID=%s&units=%s", WEATHER_API_BASE_URL, lat, lng, m_activity.getString(R.string.weather_apikey), weatherUnit);

        // Request a string response from the provided URL.
        StringRequest stringRequest = new StringRequest(Request.Method.GET, url,
                new Response.Listener<String>() {
                    @Override
                    public void onResponse(String response) {

                        parseJson(response);

                    }
                }, new Response.ErrorListener() {
            @Override
            public void onErrorResponse(VolleyError error) {
                showErrorMessage();
            }
        });

        // Add the request to the RequestQueue.
        queue.add(stringRequest);
    }

    private void readPlacesFromJsonFile() {
        String json = null;
        JSONArray jsonArray = null;
        try {
            InputStream is = m_activity.getAssets().open("placejumps.json");
            int size = is.available();
            byte[] buffer = new byte[size];
            is.read(buffer);
            is.close();
            json = new String(buffer, "UTF-8");
        } catch (IOException ex) {
            ex.printStackTrace();
        }
        try {
            jsonArray = new JSONArray(json);

            for (int i = 0; i < jsonArray.length(); i++) {
                JSONObject jsonObject = jsonArray.getJSONObject(i);
                double lat = jsonObject.getDouble("Latitude");
                double lng = jsonObject.getDouble("Longitude");
                String name = jsonObject.getString("Name");
                m_latLongData.put(name, new LatLng(lat, lng));
            }


        } catch (JSONException e) {
            e.printStackTrace();
        }

    }

    private void showErrorMessage() {
        m_minTempTextView.setText(R.string.error_msg);
        m_maxTempTextView.setText("");
        m_humidityTextView.setText("");
        m_cityTextView.setText("");
        m_weatherTextView.setText("");
        SearchWidgetViewJniMethods.SelectedItem(m_nativeCallerPointer, m_weatherString, 4, 4);

    }


    private void parseJson(String responce) {

        int index = 4;
        try {
            JSONObject jsonObject = new JSONObject(responce);

            JSONArray weatherArray = jsonObject.getJSONArray(m_activity.getString(R.string.WEATHER_KEY));

            JSONObject weatherObj = weatherArray.getJSONObject(0);

            m_weatherString = weatherObj.optString(m_activity.getString(R.string.MAIN_KEY), "");

            if ("Clouds".equals(m_weatherString)) {
                index = 5;
                m_weatherString = "Overcast";

            } else if ("Mist".equals(m_weatherString) || "Haze".equals(m_weatherString)) {
                index = 6;
                m_weatherString = "foggy";

            } else if ("Drizzle".equals(m_weatherString) || "Rain".equals(m_weatherString)) {
                index = 7;
                m_weatherString = "rain";

            } else if ("Snow".equals(m_weatherString)) {
                index = 8;
                m_weatherString = "Snow";

            }

            JSONObject mMain = jsonObject.getJSONObject(m_activity.getString(R.string.MAIN_KEY));

            m_maxTempString = m_activity.getString(R.string.maxlbl) + mMain.optString( m_activity.getString(R.string.MAX_KEY), "");
            m_minTempString = m_activity.getString(R.string.minlbl) + mMain.optString( m_activity.getString(R.string.TEMP_MIN_KEY), "");
            m_humidityString = m_activity.getString(R.string.humiditylbl) + mMain.optString(m_activity.getString(R.string.TEMP_HUMIDITY_KEY), "");
            m_cityString = jsonObject.optString(m_activity.getString(R.string.NAME_KEY), "");

            m_minTempTextView.setText(m_minTempString);
            m_maxTempTextView.setText(m_maxTempString);
            m_humidityTextView.setText(m_humidityString);
            m_cityTextView.setText(m_cityString);
            m_weatherTextView.setText(m_weatherString);

            SearchWidgetViewJniMethods.SelectedItem(m_nativeCallerPointer, m_weatherString, 4, index);

        } catch (JSONException e) {
            e.printStackTrace();
        }

    }
}