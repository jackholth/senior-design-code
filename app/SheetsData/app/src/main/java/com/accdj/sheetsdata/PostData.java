package com.accdj.sheetsdata;

import androidx.appcompat.app.AppCompatActivity;

import android.app.ProgressDialog;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

import com.android.volley.DefaultRetryPolicy;
import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.RetryPolicy;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.StringRequest;
import com.android.volley.toolbox.Volley;

import java.util.HashMap;
import java.util.Map;

public class PostData extends AppCompatActivity {
    Button btnSmall;
    Button btnMedium;
    Button btnLarge;
    ProgressDialog progressDialog;
    String value;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Bundle extras = getIntent().getExtras();
        if (extras != null) {
            value = extras.getString("plant");
        }
        System.out.println(value);
        setContentView(R.layout.activity_post_data);

        btnSmall = findViewById(R.id.btn_small_id);
        btnMedium = findViewById(R.id.btn_medium_id);
        btnLarge = findViewById(R.id.btn_large_id);


        progressDialog = new ProgressDialog(this);
        progressDialog.setMessage("Loading...");

        btnSmall.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                postData("small");
                progressDialog.show();
            }
        });
        btnMedium.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                postData("medium");
                progressDialog.show();
            }
        });
        btnLarge.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                postData("large");
                progressDialog.show();
            }
        });
    }

    public void postData(String size) {

        StringRequest stringRequest = new StringRequest(Request.Method.POST, "https://script.google.com/macros/s/AKfycbx6iq-tczFTbGb3ddgn-uy9ihIHFXTG21i4Wk9aJ61q7GazfrxSAx7MycWCfSI9sQahIw/exec", new Response.Listener<String>() {
            @Override
            public void onResponse(String response) {
                progressDialog.hide();
            }
        }, new Response.ErrorListener() {
            public void onErrorResponse(VolleyError error) {

            }
        }){
            @Override
            protected Map<String, String> getParams() {
                Map<String, String> params = new HashMap<>();
                params.put("client", "app");
                params.put("vWaterAmount", size);
                params.put("plant", value);

                return params;
            }
        };

        int socketTimeOut = 50000;
        RetryPolicy retryPolicy = new DefaultRetryPolicy(socketTimeOut, 0, DefaultRetryPolicy.DEFAULT_BACKOFF_MULT);
        stringRequest.setRetryPolicy(retryPolicy);

        RequestQueue requestQueue = Volley.newRequestQueue(this);
        requestQueue.add(stringRequest);
    }
}