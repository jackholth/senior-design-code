package com.accdj.sheetsdata;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.content.Intent;
import android.view.View;
import android.widget.Button;

public class MainActivity extends AppCompatActivity {

    private Button plantButton;

    private Button tankButton;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        plantButton = findViewById(R.id.plantInfo);
        tankButton = findViewById(R.id.waterTankLevel);

        plantButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                openPlantInfo();
            }
        });

        tankButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                openWaterTankLevel();
            }
        });
    }

    public void openPlantInfo() {
        Intent intent = new Intent(this, PlantInfo.class);
        startActivity(intent);
    }

    public void openWaterTankLevel() {
        Intent intent = new Intent(this, ReadWaterData.class);
        startActivity(intent);
    }
}