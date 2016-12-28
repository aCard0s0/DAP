package com.drone.dronev3.activity;

import android.app.ProgressDialog;
import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

import com.drone.dronev3.R;
import com.drone.dronev3.drones.BebopDrone;
import com.drone.dronev3.view.BebopVideoView;
import com.parrot.arsdk.arcommands.ARCOMMANDS_ARDRONE3_MEDIARECORDEVENT_PICTUREEVENTCHANGED_ERROR_ENUM;
import com.parrot.arsdk.arcommands.ARCOMMANDS_ARDRONE3_PILOTINGSTATE_FLYINGSTATECHANGED_STATE_ENUM;
import com.parrot.arsdk.arcontroller.ARCONTROLLER_DEVICE_STATE_ENUM;
import com.parrot.arsdk.arcontroller.ARControllerCodec;
import com.parrot.arsdk.arcontroller.ARFrame;
import com.parrot.arsdk.ardiscovery.ARDiscoveryDeviceService;

/**
 * Created by acardoso on 03-04-2016.
 */
public class StreamActivity extends AppCompatActivity {

    private String TAG = "StreamActivity";

    private BebopVideoView mVideoView;

    private Button bttTakePicture;
    private Button bttStartVideo;
    private Button bttStopVideo;
    private Button bttGoToMedia;

    private boolean recording = false;

    private ProgressDialog mConnectionProgressDialog;

    private BebopDrone mBebopDrone;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_stream);

        // init UI objects and listeners
        initView();

        // Test if serviceDevice, that comes from MenuActivity is available &&
        // create serviceDevice and mBebopDrone
        initServiceAndDrone();
    }

    public void initView(){

        mVideoView = (BebopVideoView) findViewById(R.id.videoView);
        bttTakePicture = (Button) findViewById(R.id.bttTakePicture);
        bttStartVideo = (Button) findViewById(R.id.bttStartVideo);
        bttStopVideo = (Button) findViewById(R.id.bttStopVideo);
        bttGoToMedia = (Button) findViewById(R.id.bttGoToMedia);

        bttTakePicture.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                mBebopDrone.takePicture();
            }
        });
        bttStartVideo.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                /** TODO: Pause video
                 */
                mBebopDrone.startVideo();

                if (recording)  bttStartVideo.setText("Pause Video");
                else    bttStartVideo.setText("");
            }
        });
        bttStopVideo.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                mBebopDrone.stopVideo();
            }
        });
        bttGoToMedia.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                /** TODO: go to media
                 *

                // Transfer the service from StreamActivity to MediaActivity
                Intent navIntent = new Intent(StreamActivity.this, MediaActivity.class);
                navIntent.putExtra(
                        DeviceListActivity.EXTRA_DEVICE_SERVICE,
                        serviceDevice);

                Log.d(TAG, "Go To Stream Options, , with service");
                if (navIntent != null) startActivity(navIntent);*/
            }
        });

    }

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

    /**     Service come from MenuActivity, that comes from DeviceListActivity (Main Class)
     *  If the service exist BebopDrone and the serviceDevice are init.
     * */
    public void initServiceAndDrone(){

        Intent intentWithService = getIntent();
        if ( intentWithService.hasExtra( DeviceListActivity.EXTRA_DEVICE_SERVICE )){

            ARDiscoveryDeviceService serviceDevice = intentWithService.getParcelableExtra( DeviceListActivity.EXTRA_DEVICE_SERVICE );
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

    /**
     *      We need to add the Drone Listener to know when he change the connections status
     *      and do the proper connection and disconnection.
     * */
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
            mVideoView.configureDecoder(codec);
        }

        @Override
        public void onFrameReceived(ARFrame frame) {
            mVideoView.displayFrame(frame);
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
            // nothing to do here
        }
    };
}
