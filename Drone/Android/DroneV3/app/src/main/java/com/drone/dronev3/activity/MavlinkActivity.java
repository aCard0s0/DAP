package com.drone.dronev3.activity;

import android.app.ProgressDialog;
import android.content.Intent;
import android.os.AsyncTask;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;


import com.drone.dronev3.R;
import com.drone.dronev3.drones.BebopDrone;
import com.drone.dronev3.mavlink.Maps;
import com.drone.dronev3.mavlink.MavLinkFile;
import com.drone.dronev3.utility.FileOperations;
import com.parrot.arsdk.arcommands.ARCOMMANDS_ARDRONE3_MEDIARECORDEVENT_PICTUREEVENTCHANGED_ERROR_ENUM;
import com.parrot.arsdk.arcommands.ARCOMMANDS_ARDRONE3_PILOTINGSTATE_FLYINGSTATECHANGED_STATE_ENUM;
import com.parrot.arsdk.arcommands.ARCOMMANDS_COMMON_MAVLINK_START_TYPE_ENUM;
import com.parrot.arsdk.arcontroller.ARCONTROLLER_DEVICE_STATE_ENUM;
import com.parrot.arsdk.arcontroller.ARControllerCodec;
import com.parrot.arsdk.arcontroller.ARFrame;
import com.parrot.arsdk.ardiscovery.ARDiscoveryDeviceService;

import java.io.File;

import it.sauronsoftware.ftp4j.FTPDataTransferListener;


/**
 * Created by dre on 28-03-2016.
 */
public class MavlinkActivity extends AppCompatActivity {

    private String TAG = "MavlinkActivity";

    private Button bttCreateMavlink;
    private Button bttUploadMavlink;
    private Button bttStartMavlink;
    private Button bttPauseMavlink;
    private Button bttStopMavlink;
    private TextView txtSrcFile;
    private TextView txtFeedBack;

    private ProgressDialog mConnectionProgressDialog;

    private MavLinkFile mavfile;

    private BebopDrone mBebopDrone;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_mavlink);

        // init UI objects and listeners
        initView();

        // Start the Mavlink object with operations
        mavfile = new MavLinkFile( getApplicationContext() );

        // Test if serviceDevice, that comes from MenuActivity is available &&
        // create serviceDevice and mBebopDrone
        initServiceAndDrone();

    }

    public void initView(){

        bttCreateMavlink = (Button) findViewById(R.id.bttCreateMavlink);
        txtSrcFile = (TextView) findViewById(R.id.txtSrcFile);
        bttUploadMavlink = (Button) findViewById(R.id.bttUploadMavlink);
        bttStartMavlink = (Button) findViewById(R.id.bttStartMavlink);
        bttPauseMavlink = (Button) findViewById(R.id.bttPauseMavlink);
        bttStopMavlink = (Button) findViewById(R.id.bttStopMavlink);
        txtFeedBack = (TextView) findViewById(R.id.txtFeedBack);

        bttCreateMavlink.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                /** TODO: completar mavllink file
                 *
                 * mavfile.fileGenerator();
                 * FileOperations fop = new FileOperations( getApplicationContext() );
                 * fop.writeFile( Maps.mavLinkPathTest );
                 */
            }
        });
        bttUploadMavlink.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                // execute background threat for FTP connection with Drone
                new DoUploadToDrone().execute();
            }
        });
        bttStartMavlink.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                mBebopDrone.startMavLinkFile(
                        "flightPlan.mavlink",
                        ARCOMMANDS_COMMON_MAVLINK_START_TYPE_ENUM.ARCOMMANDS_COMMON_MAVLINK_START_TYPE_FLIGHTPLAN);
                txtFeedBack.setText(txtFeedBack.getText() + "\n" + "Autonomous Flight has started");
            }
        });
        bttPauseMavlink.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                mBebopDrone.pauseMavLinkFile();
                txtFeedBack.setText(txtFeedBack.getText() + "\n" + "Autonomous Flight has pause");
            }
        });
        bttStopMavlink.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                mBebopDrone.stopMavLinkFile();
                txtFeedBack.setText(txtFeedBack.getText() + "\n" + "Autonomous Flight has stop");
                //mBebopDrone.returnHome();
                //txtFeedBack.setText(txtFeedBack.getText() + "\n" + "Drone returning home");
            }
        });
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
            txtFeedBack.setText(txtFeedBack.getText() + "\n" + "Service found, Drone Controller created");
        } else {

            Log.e(TAG, "No serviceDevice found, run discovery first");
            Toast.makeText(getApplicationContext(),
                    "No serviceDevice found, run discovery first", Toast.LENGTH_LONG).show();
            txtFeedBack.setText(txtFeedBack.getText() + "\n" + "No serviceDevice found, run discovery first");
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

        }
        // nothing to do here
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
            // nothing to do here
        }
    };

    /**      Background threat to Upload the file Mavlink to Drone by FTP
     *  TODO: implement process bar that implement FTPDataTransferListener, then pass it on client.upload()
     * */
    public class DoUploadToDrone extends AsyncTask<Void, Void, Boolean> implements FTPDataTransferListener {

        @Override
        protected Boolean doInBackground(Void... params) {

            // object to save file in private internal store
            FileOperations fop = new FileOperations( getApplicationContext() );

            mavfile.uploadMavlinkToDrone(
                    new File(fop.writeFile(Maps.mavLinkPathTest) + "/" + Maps.mavLinkNameFileTest),
                    mBebopDrone.getServiceDevice() );

            return true;
        }

        @Override
        protected void onPostExecute(Boolean result) {
            if (result) {
                Toast.makeText(getApplicationContext(), "Upload Complete!", Toast.LENGTH_LONG).show();
                bttStartMavlink.setEnabled(true);
            }
        }

        /** ********************************************
         *      FTPDataTransferListener methods
         * */

        @Override
        public void started() {

            txtFeedBack.setText(txtFeedBack.getText() + "\n" + "Upload Started ...");
            Toast.makeText(getBaseContext(), " Upload Started ...", Toast.LENGTH_SHORT).show();
        }

        @Override
        public void transferred(int length) {

            // Yet other length bytes has been transferred since the last time this
            // method was called
            txtFeedBack.setText(txtFeedBack.getText() + "\n" + "transferred ..." + length);
            Toast.makeText(getBaseContext(), " transferred ..." + length, Toast.LENGTH_SHORT).show();
        }

        @Override
        public void completed() {

            txtFeedBack.setText(txtFeedBack.getText() + "\n" + "completed ...");
            Toast.makeText(getBaseContext(), " completed ...", Toast.LENGTH_SHORT).show();
        }

        @Override
        public void aborted() {

            txtFeedBack.setText(txtFeedBack.getText() + "\n" + "transfer aborted , please try again...");
            Toast.makeText(getBaseContext()," transfer aborted , please try again...", Toast.LENGTH_SHORT).show();
        }

        @Override
        public void failed() {

            txtFeedBack.setText(txtFeedBack.getText() + "\n" + "failed...");
            Toast.makeText(getBaseContext()," failed...", Toast.LENGTH_SHORT).show();
        }

    }

}
