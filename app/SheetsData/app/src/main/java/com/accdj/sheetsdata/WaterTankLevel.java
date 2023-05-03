package com.accdj.sheetsdata;

import android.os.Bundle;
import android.widget.ListView;

import androidx.appcompat.app.AppCompatActivity;

import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.JsonObjectRequest;
import com.android.volley.toolbox.Volley;

import org.json.JSONArray;
import org.json.JSONObject;

import java.util.ArrayList;
import java.util.stream.IntStream;

public class WaterTankLevel extends AppCompatActivity {

    String sheetsID = "1RcYlmxQAznj83vKuDPzCUz2ENzumkxtC3WUML4Wr3Mg";
    String apiKEY = "API KEY GOES HERE";
    String strWater;
    String strDate;
    JSONArray jsonArray;
    ListView listView;

    CustomAdapter customAdapter;

    ArrayList<String> waterLevel = new ArrayList<String>();
    ArrayList<String> soilMoisture = new ArrayList<String>();
    ArrayList<String> date = new ArrayList<String>();



    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_read_water_data);

        listView = findViewById(R.id.listTwoView_id);

        String urls = "https://sheets.googleapis.com/v4/spreadsheets/"+sheetsID+"/values/deviceToApp?key="+apiKEY;


        RequestQueue queue = Volley.newRequestQueue(WaterTankLevel.this);
        JsonObjectRequest jsonObjectRequest = new JsonObjectRequest(Request.Method.GET, urls, null, new Response.Listener<JSONObject>() {
            @Override
            public void onResponse(JSONObject response) {
                try {
                    jsonArray = response.getJSONArray("values");
                } catch (Exception e) {
                }
                IntStream.range(1, jsonArray.length())
                        .forEach(i -> {
                            try {
                                JSONArray json = jsonArray.getJSONArray(i);
                                strWater = json.getString(4);
                                strDate = json.getString(5);

                                waterLevel.add(0, strWater);
                                date.add(0, strDate);

                                customAdapter = new CustomAdapter(getApplicationContext(), waterLevel, null, date, false);
                                listView.setAdapter(customAdapter);
                            } catch (Exception e) {
                            }
                        });
            }
        }, new Response.ErrorListener() {
            @Override
            public void onErrorResponse(VolleyError error) {

            }
        });
        queue.add(jsonObjectRequest);
    }
}