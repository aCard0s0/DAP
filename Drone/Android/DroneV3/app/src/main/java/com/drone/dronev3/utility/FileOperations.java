package com.drone.dronev3.utility;

import android.content.Context;
import android.util.Log;
import android.widget.Toast;


import com.drone.dronev3.mavlink.Maps;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;

/**
 * Created by dre on 29-03-2016.
 */
public class FileOperations {

    private final String TAG = "FileOperations";

    private Context appContext;

    public FileOperations(Context ctx){
        appContext = ctx;
    }

    public String writeFile( String data ){

        FileOutputStream outputStream;

        try {
            outputStream = appContext.openFileOutput(Maps.mavLinkNameFileTest, Context.MODE_PRIVATE);
            outputStream.write( data.getBytes() );
            outputStream.close();

        } catch (Exception e) {
            e.printStackTrace();
            Log.e(TAG, "erro ao escrever ficheiro");
            Toast.makeText(appContext, "erro ao escrever ficheiro", Toast.LENGTH_SHORT).show();
            return null;
        }

        return appContext.getFilesDir().toString();
    }

    public String readFile(String name){
        FileInputStream fis = null;
        StringBuilder sb = null;
        try {
            fis = appContext.openFileInput(name);

            InputStreamReader isr = new InputStreamReader(fis);
            BufferedReader bufferedReader = new BufferedReader(isr);
            sb = new StringBuilder();
            String line;

            while ((line = bufferedReader.readLine()) != null) {
                sb.append(line);
            }

            bufferedReader.close();
            isr.close();
            fis.close();

        } catch (FileNotFoundException e) {
            e.printStackTrace();
            Toast.makeText(appContext, "ficheiro nao encontrado", Toast.LENGTH_SHORT).show();

        } catch (IOException e) {
            e.printStackTrace();
            Toast.makeText(appContext, "erro ao ler ficheiro", Toast.LENGTH_SHORT).show();
        }

        return sb.toString();
    }
}
