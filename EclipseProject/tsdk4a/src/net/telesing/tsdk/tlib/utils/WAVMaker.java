package net.telesing.tsdk.tlib.utils;

import java.io.DataOutputStream;
import java.io.File;
import java.io.FileOutputStream;

/**
 * Created by FengJZ on 2014/8/12.
 */
public class WAVMaker {
    public static final short MAX_AMP = Short.MAX_VALUE;//防止爆音/2,防止察觉/1000
    public static void make(String path, double[] wave, int repeat) {
        byte[] byteWave = new byte[wave.length * 2];//16bit每个样本2个字�?
//        short[] ss = new short[wave.size];
        //获得�?大幅�?
        double max = 0;
        for (double aWave : wave) {
            double current = Math.abs(aWave);
            max = current > max ? current : max;
        }
        //转换为字�?
        for (int i = 0; i < wave.length; i++) {
            short s = (short) (MathEx.roundHalfUp(wave[i] / max * MAX_AMP, 0));
//            ss[i] = s;
            byteWave[i * 2] = (byte) (s & 0xFF);
            byteWave[i * 2 + 1] = (byte) ((s >> 8) & 0xFF);
        }

//        toCSV(ss);
        make(path, byteWave, repeat);
    }
    public static void makeRaw(String path, double[] wave, int repeat) {
        byte[] byteWave = new byte[wave.length * 2];//16bit每个样本2个字
        for (int i = 0; i < wave.length; i++) {
            short s = (short) (MathEx.roundHalfUp(wave[i], 0));
            byteWave[i * 2] = (byte) (s & 0xFF);
            byteWave[i * 2 + 1] = (byte) ((s >> 8) & 0xFF);
        }
        make(path, byteWave, repeat);
    }

    public static void makeRaw(String path,short[] waves,int repeat){
        byte[] byteWave = new byte[waves.length * 2];//16bit每个样本2个字
        for (int i = 0; i < waves.length; i++) {
            short s = waves[i];
            byteWave[i * 2] = (byte) (s & 0xFF);
            byteWave[i * 2 + 1] = (byte) ((s >> 8) & 0xFF);
        }
        make(path, byteWave, repeat);
    }

    public static void make(String path, byte[] wave, int repeat) {
        StringBuilder waveBuffer = new StringBuilder();
        StringBuilder fileBuffer = new StringBuilder();

        //-------------waveBuffer--------------------------------------
        for (int i = 0; i < repeat; i++) {
            for (byte aWave : wave) {
                waveBuffer.append((char) (aWave % 256));
            }
        }

        //-------------fileBuffer--------------------------------------
        //00H~03H
        fileBuffer.append("RIFF");

        //04H~07H
        int sizeT = waveBuffer.length() + 36;
        for (int i = 0; i < 4; i++, sizeT = sizeT / 256) {
            fileBuffer.append((char) (sizeT % 256));
        }

        //08H~0FH
        fileBuffer.append("WAVEfmt ");

        //10H~13H
        int flt = 16;
        for (int i = 0; i < 4; i++, flt = flt / 256) {
            fileBuffer.append((char) (flt % 256));
        }

        //14H~15H
        short fmt = 1;
        for (int i = 0; i < 2; i++, fmt = (short) (fmt / 256)) {
            fileBuffer.append((char) (fmt % 256));
        }

        //16H~17H
        short cn = 1;
        for (int i = 0; i < 2; i++, cn = (short) (cn / 256)) {
            fileBuffer.append((char) (cn % 256));
        }

        //18H~1BH
        int fs = 44100;
        for (int i = 0; i < 4; i++, fs = fs / 256) {
            fileBuffer.append((char) (fs % 256));
        }

        //1CH~1FH
        int tr = 88200;
        for (int i = 0; i < 4; i++, tr = tr / 256) {
            fileBuffer.append((char) (tr % 256));
        }

        //20H~21H
        short sizeData = 2;
        for (int i = 0; i < 2; i++, sizeData = (short) (sizeData / 256)) {
            fileBuffer.append((char) (sizeData % 256));
        }

        //22H~23H
        short pcm = 16;
        for (int i = 0; i < 2; i++, pcm = (short) (pcm / 256)) {
            fileBuffer.append((char) (pcm % 256));
        }

        fileBuffer.append("data");

        //1CH~1FH
        int data = waveBuffer.length();
        for (int i = 0; i < 4; i++, data = data / 256) {
            fileBuffer.append((char) (data % 256));
        }


        //waveBuffer
        fileBuffer.append(waveBuffer);


        //-------------写入文件--------------------------------------
        try {
            if (!new File(path).exists()) {
                new File(path).createNewFile();
            }
            DataOutputStream dos = new DataOutputStream(new FileOutputStream(new File(path)));
            dos.writeBytes(fileBuffer.toString());
            dos.flush();
            dos.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

}
