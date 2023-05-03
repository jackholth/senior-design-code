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

public class ReadDataThree extends AppCompatActivity {

    String sheetsID = "1RcYlmxQAznj83vKuDPzCUz2ENzumkxtC3WUML4Wr3Mg";
    String apiKEY = "API KEY GOES HERE";
    String strWater;
    String strMoisture;
    String strDate;

    JSONArray jsonArray;
    ListView listView;

    CustomAdapter customAdapter;
    ArrayList<String> soilMoisture = new ArrayList<String>();
    ArrayList<String> date = new ArrayList<String>();



    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_read_data);

        listView = findViewById(R.id.listview_id);

        String urls = "https://sheets.googleapis.com/v4/spreadsheets/"+sheetsID+"/values/deviceToApp?key="+apiKEY;


        RequestQueue queue = Volley.newRequestQueue(ReadDataThree.this);
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
                                strMoisture = json.getString(2);   //I don't know why I can't use soilMoisture for the ArrayList, check it out.
                                strDate = json.getString(5);

                                soilMoisture.add(0, strMoisture);
                                date.add(0, strDate);
                                customAdapter = new CustomAdapter(getApplicationContext(), null, soilMoisture, date, true);
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