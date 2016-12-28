package com.drone.dronev3.activity;

import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

import com.drone.dronev3.R;
import com.parrot.arsdk.ardiscovery.ARDiscoveryDeviceService;

/**
 * Created by dre on 02-04-2016.
 */
public class MenuActivity extends AppCompatActivity {

    private String TAG = "MenuActivity";

    private Button bttController;
    private Button bttMavLinkOp;
    private Button bttGPSOp;
    private Button bttStreamOp;

    private Intent navIntent;

    private ARDiscoveryDeviceService serviceDevice;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_menu);

        // init Buttons
        bttController = (Button) findViewById(R.id.bttController);
        bttMavLinkOp = (Button) findViewById(R.id.bttMavLinkOp);
        bttGPSOp = (Button) findViewById(R.id.bttGPSOp);
        bttStreamOp = (Button) findViewById(R.id.bttStreamOp);

        // Test if exist and get ServiceDevice from DeviceListActivity
        testAndInitService();

        // Buttons Listeners
        bttController.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                // Transfer the service from DeviceListActivity to BebopActivity
                navIntent = new Intent(MenuActivity.this, BebopActivity.class);
                navIntent.putExtra(
                        DeviceListActivity.EXTRA_DEVICE_SERVICE,
                        serviceDevice);

                Log.d(TAG, "Go To Controller, with service");
                if (navIntent != null) startActivity(navIntent);
            }
        });
        bttMavLinkOp.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                // Transfer the service from DeviceListActivity to MavlinkActivity
                navIntent = new Intent(MenuActivity.this, MavlinkActivity.class);
                navIntent.putExtra(
                        DeviceListActivity.EXTRA_DEVICE_SERVICE,
                        serviceDevice);

                Log.d(TAG, "Go To Mavlink Options, , with service");
                if (navIntent != null) startActivity(navIntent);
            }
        });
        bttGPSOp.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                // Transfer the service from DeviceListActivity to MavlinkActivity
                navIntent = new Intent(MenuActivity.this, GPSActivity.class);
                navIntent.putExtra(
                        DeviceListActivity.EXTRA_DEVICE_SERVICE,
                        serviceDevice);

                Log.d(TAG, "Go To GPS Options, , with service");
                if (navIntent != null) startActivity(navIntent);
            }
        });
        bttStreamOp.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                // Transfer the service from DeviceListActivity to MavlinkActivity
                navIntent = new Intent(MenuActivity.this, StreamActivity.class);
                navIntent.putExtra(
                        DeviceListActivity.EXTRA_DEVICE_SERVICE,
                        serviceDevice);

                Log.d(TAG, "Go To Stream Options, , with service");
                if (navIntent != null) startActivity(navIntent);
            }
        });
    }

    /**      Test if exist and get ServiceDevice from DeviceListActivity
     * */
    public void testAndInitService(){

        Intent intentWithService = getIntent();
        if ( intentWithService.hasExtra( DeviceListActivity.EXTRA_DEVICE_SERVICE )){
            serviceDevice = intentWithService.getParcelableExtra( DeviceListActivity.EXTRA_DEVICE_SERVICE );

        } else {

            Log.e(TAG, "sem serivce, descobrir o Drone primeiro");
            Toast.makeText(getApplicationContext(),
                    "No service found, run discovery frist", Toast.LENGTH_LONG).show();
            bttController.setEnabled(false);
            bttMavLinkOp.setEnabled(false);
        }

    }
}
