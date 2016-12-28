package com.drone.dronev3.activity;

import android.app.ProgressDialog;
import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import com.drone.dronev3.R;
import com.drone.dronev3.drones.BebopDrone;
import com.parrot.arsdk.arcommands.ARCOMMANDS_ARDRONE3_MEDIARECORDEVENT_PICTUREEVENTCHANGED_ERROR_ENUM;
import com.parrot.arsdk.arcommands.ARCOMMANDS_ARDRONE3_PILOTINGSTATE_FLYINGSTATECHANGED_STATE_ENUM;
import com.parrot.arsdk.arcontroller.ARCONTROLLER_DEVICE_STATE_ENUM;
import com.parrot.arsdk.arcontroller.ARControllerCodec;
import com.parrot.arsdk.arcontroller.ARFrame;
import com.parrot.arsdk.ardiscovery.ARDiscoveryDeviceService;

/**
 * Created by dre on 03-04-2016.
 */
public class GPSActivity extends AppCompatActivity {

    private String TAG = "GPSActivity";

    private Button bttGPSstart;
    private Button bttGPSstop;
    private TextView txtGPScoor;

    private ProgressDialog mConnectionProgressDialog;

    private BebopDrone mBebopDrone;

    private ARDiscoveryDeviceService serviceDevice;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_gps);

        // init UI objects and listeners
        initView();

        // Test if serviceDevice, that comes from MenuActivity is available &&
        // create serviceDevice and mBebopDrone
        initServiceAndDrone();
    }

    public void initView(){

        bttGPSstart = (Button) findViewById(R.id.bttGPSstart);
        bttGPSstop = (Button) findViewById(R.id.bttGPSstop);
        txtGPScoor = (TextView) findViewById(R.id.txtGPScoor);

        // Buttons listeners
        bttGPSstart.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                // TODO: GPS options
                mBebopDrone.startGPSreadings();
            }
        });
        bttGPSstop.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                mBebopDrone.stopGPSreadings();
            }
        });
    }

    /**     Service come from MenuActivity, that comes from DeviceListActivity (Main Class)
     *  If the service exist BebopDrone and the serviceDevice are init.
     * */
    public void initServiceAndDrone(){

        Intent intentWithService = getIntent();
        if ( intentWithService.hasExtra( DeviceListActivity.EXTRA_DEVICE_SERVICE )){

            serviceDevice = intentWithService.getParcelableExtra( DeviceListActivity.EXTRA_DEVICE_SERVICE );
            mBebopDrone = new BebopDrone(this, serviceDevice);
            mBebopDrone.addListener(mBebopListener);

            Log.e(TAG, "Service found, Drone Controller created");
            Toast.makeText(getApplicationContext(),
                    "Service found, Drone Controller created", Toast.LENGTH_LONG).show();

        } else {

            Log.e(TAG, "No serviceDevice found, run discovery first");
            Toast.makeText(getApplicationContext(),
                    "No serviceDevice found, run discovery first", Toast.LENGTH_LONG).show();
        }
    }

    /**     Connect the Drone on Start this Activity
     * */
    @Override
    protected void onStart() {
        super.onStart();

        // show a loading view while the bebop drone is connecting
        if ((mBebopDrone != null) && !(ARCONTROLLER_DEVICE_STATE_ENUM.ARCONTROLLER_DEVICE_STATE_RUNNING.equals(mBebopDrone.getConnectionState())))
        {
            mConnectionProgressDialog = new ProgressDialog(this, R.style.AppCompatAlertDialogStyle);
            mConnectionProgressDialog.setIndeterminate(true);
            mConnectionProgressDialog.setMessage("Connecting ...");
            mConnectionProgressDialog.setCancelable(false);
            mConnectionProgressDialog.show();

            // if the connection to the Bebop fails, finish the activity
            if (!mBebopDrone.connect()) {
                finish();
            }
        }
    }

    /**     Disconnect the Drone and Finish this Activity
     * */
    @Override
    public void onBackPressed() {
        if (mBebopDrone != null)
        {
            mConnectionProgressDialog = new ProgressDialog(this, R.style.AppCompatAlertDialogStyle);
            mConnectionProgressDialog.setIndeterminate(true);
            mConnectionProgressDialog.setMessage("Disconnecting ...");
            mConnectionProgressDialog.setCancelable(false);
            mConnectionProgressDialog.show();

            if (!mBebopDrone.disconnect()) {
                finish();
            }
        }
    }

    /**
     *      We need to add the Drone Listener to know when:
     *      - The Drone change the connections status "onDroneConnectionChanged()",
     *          and do the proper connection and disconnection.
     **/
    private final BebopDrone.Listener mBebopListener = new BebopDrone.Listener() {
        @Override
        public void onDroneConnectionChanged(ARCONTROLLER_DEVICE_STATE_ENUM state) {
            switch (state)
            {
                case ARCONTROLLER_DEVICE_STATE_RUNNING:
                    mConnectionProgressDialog.dismiss();
                    break;

                case ARCONTROLLER_DEVICE_STATE_STOPPED:
                    // if the deviceController is stopped, go back to the previous activity
                    mConnectionProgressDialog.dismiss();
                    finish();
                    break;

                default:
                    break;
            }
        }

        @Override
        public void onBatteryChargeChanged(int batteryPercentage) {
            // nothing to do here
        }

        @Override
        public void onPilotingStateChanged(ARCOMMANDS_ARDRONE3_PILOTINGSTATE_FLYINGSTATECHANGED_STATE_ENUM state) {
            // nothing to do here
        }

        @Override
        public void onPictureTaken(ARCOMMANDS_ARDRONE3_MEDIARECORDEVENT_PICTUREEVENTCHANGED_ERROR_ENUM error) {
            // nothing to do here
        }

        @Override
        public void configureDecoder(ARControllerCodec codec) {
            // nothing to do here
        }

        @Override
        public void onFrameReceived(ARFrame frame) {
            // nothing to do here
        }

        @Override
        public void onMatchingMediasFound(int nbMedias) {
            // nothing to do here
        }

        @Override
        public void onDownloadProgressed(String mediaName, int progress) {
            // nothing to do here
        }

        @Override
        public void onDownloadComplete(String mediaName) {
            // nothing to do here
        }

        @Override
        public void onGPSReadings(double latitude, double longitude, double altitude) {

            String str = txtGPScoor.getText() +"\n"
                    + "Latitude: "+ latitude +", Longitude: "+ longitude +", Altitude: "+ altitude;
            txtGPScoor.setText( str );
        }
    };
}
