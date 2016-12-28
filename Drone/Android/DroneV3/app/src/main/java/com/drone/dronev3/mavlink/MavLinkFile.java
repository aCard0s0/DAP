package com.drone.dronev3.mavlink;

import android.content.Context;
import android.content.ContextWrapper;
import android.util.Log;
import android.widget.Toast;

import com.parrot.arsdk.ardiscovery.ARDiscoveryDeviceNetService;
import com.parrot.arsdk.ardiscovery.ARDiscoveryDeviceService;
import com.parrot.arsdk.armavlink.ARMavlinkException;
import com.parrot.arsdk.armavlink.ARMavlinkFileGenerator;
import com.parrot.arsdk.armavlink.ARMavlinkMissionItem;

import java.io.File;

import it.sauronsoftware.ftp4j.FTPClient;


/**
 *      FTP Client here
 *
 *      TODO: este objecto nao pode ser criado sem entrar o ficheiro, valido ou nao
 *
 * Created by dre on 27-03-2016.
 */
public class MavLinkFile {

    private final String TAG = "MavLinkFile";

    static final String FTP_HOST = "192.168.42.51";
    static final String FTP_DIR = "";
    static final int FTP_PORT = 61;
    static final String FTP_USER = "anonymous";
    static final String FTP_PASS  = "";

    private Context appContext;

    public MavLinkFile(Context ctx){
        appContext = ctx;
    }

    public void fileGenerator(){

        // TODO: Rever toda a esta funcionalidade

        ContextWrapper conwrap = new ContextWrapper(appContext);            // explorar esta Class
        String filepath = conwrap.getFilesDir().toString()+" teste "+".csv";
        Log.d(TAG,"Mavlink Filepath: "+ filepath);

        try {
            ARMavlinkFileGenerator filegen = new ARMavlinkFileGenerator();
            // Take Off
            filegen.addMissionItem(
                    ARMavlinkMissionItem.CreateMavlinkTakeoffMissionItem(0 , 0, 0, 0, 0));
                            //(float) position.latitude, (float) position.longitude, altitude, yaw, 0));
            // Create MavLink Way Point
            filegen.addMissionItem(
                    ARMavlinkMissionItem.CreateMavlinkNavWaypointMissionItem(0 , 0, 0, 0));
                            //(float) position.latitude, (float) position.longitude, altitude, yaw));
            // Land
            filegen.addMissionItem(
                    ARMavlinkMissionItem.CreateMavlinkLandMissionItem(0 , 0, 0, 0));
                            //(float) position.latitude,(float) position.longitude, altitude, yaw));

            filegen.CreateMavlinkFile(filepath);
            Log.d(TAG, "Created Mavlink File");


        } catch (ARMavlinkException e) {
            e.printStackTrace();
        }

        //deviceController.getFeatureCommon().sendMavlinkStart(filepath,ARCOMMANDS_COMMON_MAVLINK_START_TYPE_ENUM.ARCOMMANDS_COMMON_MAVLINK_START_TYPE_FLIGHTPLAN);
        //Log.d(TAG,"sending goToPos Mavlink file");
    }


    public void uploadMavlinkToDrone(File mavfile, ARDiscoveryDeviceService service) {

        if (!mavfile.exists()) {
            Toast.makeText(appContext, "MavLink File doesn't exist", Toast.LENGTH_SHORT).show();
            return ;
        }

        String productIP = ((ARDiscoveryDeviceNetService) (service.getDevice())).getIp();

        FTPClient client = new FTPClient();

        try {
            client.setAutoNoopTimeout(10000);
            client.connect(productIP, FTP_PORT);
            client.login(FTP_USER, FTP_PASS);
            client.setType(FTPClient.TYPE_BINARY);

            //client.changeDirectory( FTP_DIR );

            client.upload( mavfile ); // litener

        } catch (Exception e) {
            e.printStackTrace();
            Log.e(TAG, "erro no upload do ficheiro para o drone");
        }
        try {
            client.disconnect(true);
        } catch (Exception e2) {
            e2.printStackTrace();
            Log.e(TAG, "erro em disconnect do drone");
        }
    }
}
