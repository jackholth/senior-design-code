package com.accdj.sheetsdata;

import android.os.Bundle;
import android.view.View;
import android.widget.Button;

import androidx.appcompat.app.AppCompatActivity;
import android.content.Intent;

public class PlantInfo extends AppCompatActivity {
    private Button plantButtonOne;
    private Button plantButtonTwo;
    private Button plantButtonThree;
    private Button plantButtonFour;


    private Button dispenseButtonOne;
    private Button dispenseButtonTwo;
    private Button dispenseButtonThree;
    private Button dispenseButtonFour;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_plant_info);

        plantButtonOne = findViewById(R.id.plantOneButton);
        plantButtonTwo = findViewById(R.id.plantTwoButton);
        plantButtonThree = findViewById(R.id.plantThreeButton);
        plantButtonFour = findViewById(R.id.plantFourButton);

        dispenseButtonOne = findViewById(R.id.dispenseOneButton);
        dispenseButtonTwo = findViewById(R.id.dispenseTwoButton);
        dispenseButtonThree = findViewById(R.id.dispenseThreeButton);
        dispenseButtonFour = findViewById(R.id.dispenseFourButton);

        plantButtonOne.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                openReadData();
            }
        });

        plantButtonTwo.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                openReadDataTwo();
            }
        });

        plantButtonThree.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                openReadDataThree();
            }
        });

        plantButtonFour.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                openReadDataFour();
            }
        });

        dispenseButtonOne.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                openPostData("one");
            }
        });

        dispenseButtonTwo.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                openPostData("two");
            }
        });

        dispenseButtonThree.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                openPostData("three");
            }
        });

        dispenseButtonFour.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                openPostData("four");
            }
        });
    }

    public void openReadData() {
        Intent intent = new Intent(this, ReadData.class);
        startActivity(intent);
    }

    public void openReadDataTwo() {
        Intent intent = new Intent(this, ReadDataTwo.class);
        startActivity(intent);
    }

    public void openReadDataThree() {
        Intent intent = new Intent(this, ReadDataThree.class);
        startActivity(intent);
    }

    public void openReadDataFour() {
        Intent intent = new Intent(this, ReadDataFour.class);
        startActivity(intent);
    }

    public void openPostData(String plant) {
        Intent intent = new Intent(this, PostData.class);
        intent.putExtra("plant", plant);
        startActivity(intent);
    }
}

