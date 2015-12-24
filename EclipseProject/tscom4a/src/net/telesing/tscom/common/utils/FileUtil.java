package net.telesing.tscom.common.utils;

import android.os.Environment;
import android.util.Log;
import net.telesing.tscom.common.contants.ConstantValues;

import java.io.*;

public class FileUtil {
    public static File updateDir = null;
    public static File updateFile = null;


    /**
     * 创建文件
     */
    public static void createFile(String name, String type) {
        if (Environment.MEDIA_MOUNTED.equals(Environment.getExternalStorageState())) {
            updateDir = new File(Environment.getExternalStorageDirectory() + "/" + ConstantValues.DOWNLOAD_DIR);
            updateFile = new File(updateDir + "/" + name + type);
            if (!updateDir.exists()) {
                updateDir.mkdirs();
            }
            if (!updateFile.exists()) {
                try {
                    updateFile.createNewFile();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }

        }
    }



    public static void writeTxt(String content, String name) {
        try {
            if (Environment.MEDIA_MOUNTED.equals(Environment.getExternalStorageState())) {
                updateDir = new File(Environment.getExternalStorageDirectory() + "/" + ConstantValues.DOWNLOAD_DIR + "/log");
                File files = new File(updateDir + "/" + name);
                if (!updateDir.exists()) {
                    updateDir.mkdirs();
                }
                FileWriter fileWrite = new FileWriter(files, true);
                BufferedWriter writer = new BufferedWriter(fileWrite);

                writer.write(content);

                writer.close();

            }
        } catch (Exception e) {
            Log.e("exce", e.getMessage());
            e.getLocalizedMessage();
        }
    }


}
