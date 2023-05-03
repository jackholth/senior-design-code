package com.accdj.sheetsdata;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.TextView;

import java.util.ArrayList;

public class CustomAdapter extends BaseAdapter {

    Context context;
    LayoutInflater inflater;
    ArrayList<String> waterLevel;
    ArrayList<String> soilMoisture;
    ArrayList<String> date;
    boolean chooseAdapter;

    public CustomAdapter(Context context, ArrayList<String> waterLevel, ArrayList<String> soilMoisture, ArrayList<String> date, boolean chooseAdapter) {
        this.context = context;
        this.waterLevel = waterLevel;
        this.soilMoisture = soilMoisture;
        this.date = date;
        this.chooseAdapter = chooseAdapter;
        inflater = LayoutInflater.from(context);
    }


    @Override
    public int getCount() {
        if (chooseAdapter) {
            return soilMoisture.size();
        }
        else {
            return waterLevel.size();
        }
    }

    @Override
    public Object getItem(int i) {
        return null;
    }

    @Override
    public long getItemId(int pos) {
        return pos;
    }

    @Override
    public View getView(int position, View view, ViewGroup viewGroup) {

        if(chooseAdapter) {
            view = inflater.inflate(R.layout.custom_list_data, null);
            TextView tvSoilMoisture = view.findViewById(R.id.soil_moisture);
            TextView tvDate = view.findViewById(R.id.dateOne);
            System.out.println(date);
            System.out.println(soilMoisture);
            tvSoilMoisture.setText(soilMoisture.get(position));
            tvDate.setText(date.get(position));
            return view;
        } else if (!chooseAdapter) {
            view = inflater.inflate(R.layout.custom_list_water_data, null);
            TextView tvWaterLevel = view.findViewById(R.id.waterLevelTwo);
            TextView tvDate = view.findViewById(R.id.dateTwo);

            tvWaterLevel.setText(waterLevel.get(position));
            tvDate.setText(date.get(position));
            return view;
        } else {
            return view;
        }
    }
}